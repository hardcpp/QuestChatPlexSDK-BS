#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexService.hpp"
#include "CP_SDK/CPConfig.hpp"

#include <UnityEngine/SystemInfo.hpp>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

namespace CP_SDK {

    std::atomic_bool                 ChatPlexService::m_ThreadCondition     = false;
    _u::il2cpp_aware_thread*         ChatPlexService::m_Thread              = nullptr;
    std::atomic<ChatPlexService::EState> ChatPlexService::m_State           = ChatPlexService::EState::Disconnected;
    _v::WebClientCore::Ptr           ChatPlexService::m_WebClientCore       = nullptr;
    _v::JsonRPCClient::Ptr           ChatPlexService::m_JsonRPCClient       = nullptr;
    std::u16string                   ChatPlexService::m_LinkRequestID       = u"";
    std::u16string                   ChatPlexService::m_LinkCode            = u"";
    std::u16string                   ChatPlexService::m_LastError           = u"";
    std::u16string                   ChatPlexService::m_ActiveSubscription  = u"";
    std::vector<std::u16string>      ChatPlexService::m_UnlockedFeatures;
    std::queue<_v::Action<>>         ChatPlexService::m_OnTokenReadyQueue;
    std::mutex                       ChatPlexService::m_OnTokenReadyQueueMutex;
    std::u16string                   ChatPlexService::m_DeviceName          = u"";
    std::mutex                       ChatPlexService::m_DataMutex;
    std::mutex                       ChatPlexService::m_ThreadWaitMutex;
    std::condition_variable          ChatPlexService::m_ThreadWaitCondition;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    const ChatPlexService::EState ChatPlexService::State()
    {
        return m_State.load();
    }
    const std::u16string_view ChatPlexService::Token()
    {
        thread_local std::u16string l_Snapshot;
        std::lock_guard l_Lock(m_DataMutex);
        l_Snapshot = CPConfig::Instance()->ChatPlexServiceToken;
        return l_Snapshot;
    }
    const std::u16string_view ChatPlexService::LinkCode()
    {
        thread_local std::u16string l_Snapshot;
        std::lock_guard l_Lock(m_DataMutex);
        l_Snapshot = m_LinkCode;
        return l_Snapshot;
    }
    const std::u16string_view ChatPlexService::LastError()
    {
        thread_local std::u16string l_Snapshot;
        std::lock_guard l_Lock(m_DataMutex);
        l_Snapshot = m_LastError;
        return l_Snapshot;
    }
    const std::u16string_view ChatPlexService::ActiveSubscription()
    {
        thread_local std::u16string l_Snapshot;
        std::lock_guard l_Lock(m_DataMutex);
        l_Snapshot = m_State.load() == EState::Connected ? m_ActiveSubscription : std::u16string();
        return l_Snapshot;
    }
    const std::vector<std::u16string>& ChatPlexService::UnlockedFeatures()
    {
        thread_local std::vector<std::u16string> l_Snapshot;
        std::lock_guard l_Lock(m_DataMutex);
        l_Snapshot = m_UnlockedFeatures;
        return l_Snapshot;
    }

    _v::Event<ChatPlexService::EState, ChatPlexService::EState> ChatPlexService::StateChanged;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init the service
    void ChatPlexService::Init()
    {
        if (m_Thread)
            return;

        m_DeviceName = _u::SystemInfo::GetDeviceName();
        m_WebClientCore = _v::WebClientCore::Make(u"https://api.chatplex.org/", _u::TimeSpan::FromSeconds(10), false, true);
        m_JsonRPCClient = _v::JsonRPCClient::Create(m_WebClientCore);

        m_ThreadCondition.store(true);
        m_Thread = new _u::il2cpp_aware_thread(&ThreadRunner);
    }
    /// @brief Release the service
    void ChatPlexService::Release()
    {
        auto l_Thread = m_Thread;
        if (!l_Thread)
            return;

        m_ThreadCondition.store(false);
        m_ThreadWaitCondition.notify_all();
        if (m_WebClientCore)
            m_WebClientCore->CancelAllRequests();

        if (l_Thread->joinable())
            l_Thread->join();

        delete l_Thread;
        m_Thread = nullptr;
        m_JsonRPCClient.reset();
        m_WebClientCore.reset();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Add a callback to be called when the token is ready (call immediatly if ready)
    /// @param action Callback to be caled
    void ChatPlexService::OnTokenReady(_v::CActionRef<> action)
    {
        bool l_InvokeNow = false;
        {
            std::lock_guard<std::mutex> l_Lock(m_OnTokenReadyQueueMutex);
            if (m_State.load() == EState::Connected)
                l_InvokeNow = true;
            else
                m_OnTokenReadyQueue.push(action);
        }

        if (l_InvokeNow)
            action();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Start linking procedure
    void ChatPlexService::StartLinking()
    {
        if (m_State.load() != EState::Disconnected)
            return;

        ChangeState(EState::LinkRequest);
    }
    /// @brief Stop linking procedure
    void ChatPlexService::StopLinking()
    {
        const auto l_State = m_State.load();
        if (l_State != EState::LinkRequest && l_State != EState::LinkWait)
            return;

        ChangeState(EState::Disconnected);
    }
    /// @brief Refresh the session
    void ChatPlexService::Refresh()
    {
        ChangeState(EState::Disconnected);
    }
    /// @brief Disconnect and erase the saved connected application token
    void ChatPlexService::Disconnect()
    {
        {
            std::lock_guard l_Lock(m_DataMutex);
            CPConfig::Instance()->ChatPlexServiceToken = u"";
            CPConfig::Instance()->Save();
        }

        ChangeState(EState::Disconnected);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Change state and notify listenners
    void ChatPlexService::ChangeState(const EState newState)
    {
        auto l_OldState = m_State.exchange(newState);

        Unity::MTThreadInvoker::EnqueueOnThread([=]() -> void { StateChanged(l_OldState, newState); });
        m_ThreadWaitCondition.notify_all();
    }
    /// @brief Fire on token ready actions
    void ChatPlexService::FireOnTokenReady()
    {
        std::queue<_v::Action<>> l_Actions;
        {
            std::lock_guard l_Lock(m_OnTokenReadyQueueMutex);
            l_Actions.swap(m_OnTokenReadyQueue);
        }

        while (!l_Actions.empty())
        {
            auto l_Action = std::move(l_Actions.front());
            l_Actions.pop();
            try
            {
                l_Action();
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK][ChatPlexService.FireOnTokenReady] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
            catch (...)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK][ChatPlexService.FireOnTokenReady] Unknown error");
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Thread function
    void ChatPlexService::ThreadRunner()
    {
        while (m_ThreadCondition.load())
        {
            try
            {
                const auto l_State = m_State.load();
                auto l_Token = std::u16string();
                {
                    std::lock_guard l_Lock(m_DataMutex);
                    l_Token = CPConfig::Instance()->ChatPlexServiceToken;
                }

                if (l_State == EState::Disconnected && !l_Token.empty())
                {
                    m_WebClientCore->RemoveHeader(u"Authorization");

                    ChangeState(EState::Connecting);

                    auto l_Content = std::make_shared<_v::Json::U16Document>();
                    l_Content->SetObject();
                    l_Content->AddMember(u"ConnectedApplicationToken", _v::Json::U16Value(l_Token, l_Content->GetAllocator()), l_Content->GetAllocator());

                    auto l_Result = m_JsonRPCClient->Request(
                        u"Account_AuthByConnectedApplicationToken",
                        l_Content
                    );

                    if (IsRPCSuccess(l_Result))
                        OnAuthed(l_Result);
                    else if (l_Result && l_Result->Result && l_Result->Result->IsObject()
                             && l_Result->Result->HasMember(u"Result")
                             && (*l_Result->Result)[u"Result"].IsBool()
                             && !(*l_Result->Result)[u"Result"].GetBool())
                    {
                        {
                            std::lock_guard l_Lock(m_DataMutex);
                            CPConfig::Instance()->ChatPlexServiceToken = u"";
                            CPConfig::Instance()->Save();
                        }

                        ChangeState(EState::Disconnected);
                    }
                    else
                        OnError(l_Result);
                }
                else if (l_State == EState::LinkRequest)
                {
                    auto l_Content = std::make_shared<_v::Json::U16Document>();
                    l_Content->SetObject();
                    l_Content->AddMember(u"ApplicationIdentifier", _v::Json::U16Value(ChatPlexSDK::ProductName().data(), l_Content->GetAllocator()), l_Content->GetAllocator());
                    l_Content->AddMember(u"ApplicationDeviceName", _v::Json::U16Value(m_DeviceName, l_Content->GetAllocator()), l_Content->GetAllocator());

                    auto l_Result = m_JsonRPCClient->Request(
                        u"ConnectedApplication_CreateLinkRequest",
                        l_Content
                    );

                    if (IsRPCSuccess(l_Result))
                    {
                        auto& l_ResultR = *l_Result->Result;
                        if (!l_ResultR.HasMember(u"RequestID") || !l_ResultR[u"RequestID"].IsString()
                            || !l_ResultR.HasMember(u"Code") || !l_ResultR[u"Code"].IsString())
                        {
                            OnError(l_Result);
                        }
                        else
                        {
                            {
                                std::lock_guard l_Lock(m_DataMutex);
                                m_LinkRequestID = l_ResultR[u"RequestID"].GetString();
                                m_LinkCode      = l_ResultR[u"Code"].GetString();
                            }

                            ChangeState(EState::LinkWait);
                        }
                    }
                    else
                        OnError(l_Result);
                }
                else if (l_State == EState::LinkWait)
                {
                    auto l_LinkRequestID = std::u16string();
                    {
                        std::lock_guard l_Lock(m_DataMutex);
                        l_LinkRequestID = m_LinkRequestID;
                    }

                    auto l_Content = std::make_shared<_v::Json::U16Document>();
                    l_Content->SetObject();
                    l_Content->AddMember(u"RequestID", _v::Json::U16Value(l_LinkRequestID, l_Content->GetAllocator()), l_Content->GetAllocator());

                    auto l_Result = m_JsonRPCClient->Request(
                        u"ConnectedApplication_GetLinkRequestStatus",
                        l_Content
                    );

                    if (IsRPCSuccess(l_Result))
                    {
                        auto& l_ResultR = *l_Result->Result;
                        if (l_ResultR.HasMember(u"ResultToken") && l_ResultR[u"ResultToken"].IsString())
                        {
                            {
                                std::lock_guard l_Lock(m_DataMutex);
                                CPConfig::Instance()->ChatPlexServiceToken = l_ResultR[u"ResultToken"].GetString();
                                CPConfig::Instance()->Save();
                            }

                            ChangeState(EState::Disconnected);
                        }
                    }
                    else
                        OnError(l_Result);
                }
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK][ChatPlexService.ThreadRunner] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
                {
                    std::lock_guard l_Lock(m_DataMutex);
                    m_LastError = u"Internal ChatPlex service error";
                }
                ChangeState(EState::Error);
            }
            catch (...)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK][ChatPlexService.ThreadRunner] Unknown error");
                {
                    std::lock_guard l_Lock(m_DataMutex);
                    m_LastError = u"Internal ChatPlex service error";
                }
                ChangeState(EState::Error);
            }

            const auto l_Delay = m_State.load() == EState::LinkWait
                ? std::chrono::milliseconds(1500)
                : std::chrono::milliseconds(100);
            std::unique_lock l_Lock(m_ThreadWaitMutex);
            m_ThreadWaitCondition.wait_for(l_Lock, l_Delay);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Is RPC call result a success result?
    /// @param rpcResult Result of the RPC command
    /// @return True if success
    bool ChatPlexService::IsRPCSuccess(_v::JsonRPCResult::Ptr& rpcResult)
    {
        if (!rpcResult || !rpcResult->Result || !rpcResult->Result->IsObject())
            return false;

        auto& l_ResultR = *rpcResult->Result;
        return l_ResultR.HasMember(u"Result")
            && l_ResultR[u"Result"].IsBool()
            && l_ResultR[u"Result"].GetBool();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief When we are Authed
    /// @param rpcResult Result of the RPC command
    void ChatPlexService::OnAuthed(_v::JsonRPCResult::Ptr& rpcResult)
    {
        if (!rpcResult || !rpcResult->Result || !rpcResult->Result->IsObject())
        {
            OnError(rpcResult);
            return;
        }

        auto& l_ResultR = *rpcResult->Result;
        auto l_ActiveSubscription = std::u16string();
        if (l_ResultR.HasMember(u"ActiveSubscription") && l_ResultR[u"ActiveSubscription"].IsString())
            l_ActiveSubscription = l_ResultR[u"ActiveSubscription"].GetString();

        auto l_UnlockedFeatures = std::vector<std::u16string>();
        if (l_ResultR.HasMember(u"UnlockedFeatures") && l_ResultR[u"UnlockedFeatures"].IsArray())
        {
            auto l_Array = l_ResultR[u"UnlockedFeatures"].GetArray();
            for (auto& l_CurrentValue : l_Array)
            {
                if (!l_CurrentValue.IsString())
                    continue;

                l_UnlockedFeatures.push_back(l_CurrentValue.GetString());
            }
        }

        auto l_Token = std::u16string();
        {
            std::lock_guard l_Lock(m_DataMutex);
            m_ActiveSubscription = std::move(l_ActiveSubscription);
            m_UnlockedFeatures = std::move(l_UnlockedFeatures);
            l_Token = CPConfig::Instance()->ChatPlexServiceToken;
        }

        m_WebClientCore->SetHeader(u"Authorization", std::u16string(u"ConnectedApplicationToken ") + l_Token);

        ChangeState(EState::Connected);
        FireOnTokenReady();
    }
    /// @brief On error received
    /// @param rpcResult Result of the RPC command
    void ChatPlexService::OnError(_v::JsonRPCResult::Ptr& rpcResult)
    {
        std::u16string l_Error = u"Unknown server error!";
        if (rpcResult && rpcResult->Result && rpcResult->Result->IsObject()
            && rpcResult->Result->HasMember(u"Error") && (*rpcResult->Result)[u"Error"].IsString())
        {
            l_Error = (*rpcResult->Result)[u"Error"].GetString();
        }
        else if (rpcResult && rpcResult->Error && rpcResult->Error->IsObject()
                 && rpcResult->Error->HasMember(u"message") && (*rpcResult->Error)[u"message"].IsString())
        {
            l_Error = (*rpcResult->Error)[u"message"].GetString();
        }

        {
            std::lock_guard l_Lock(m_DataMutex);
            m_LastError = std::move(l_Error);
        }
        ChangeState(EState::Error);
    }

}   ///< namespace CP_SDK
