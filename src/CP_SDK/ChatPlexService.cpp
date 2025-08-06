#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexService.hpp"
#include "CP_SDK/CPConfig.hpp"

#include <UnityEngine/SystemInfo.hpp>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

static std::u16string s_EmptyU16String = u"";

namespace CP_SDK {

    bool                             ChatPlexService::m_ThreadCondition     = true;
    _u::il2cpp_aware_thread*         ChatPlexService::m_Thread              = nullptr;
    ChatPlexService::EState          ChatPlexService::m_State               = ChatPlexService::EState::Disconnected;
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

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    const ChatPlexService::EState               ChatPlexService::State()
    {
        return m_State;
    }
    const std::u16string_view                   ChatPlexService::Token()
    {
        return CPConfig::Instance()->ChatPlexServiceToken;
    }
    const std::u16string_view                   ChatPlexService::LinkCode()
    {
        return m_LinkCode;
    }
    const std::u16string_view                   ChatPlexService::LastError()
    {
        return m_LastError;
    }
    const std::u16string_view                   ChatPlexService::ActiveSubscription()
    {
        return m_State == EState::Connected ? m_ActiveSubscription : s_EmptyU16String;
    }
    const std::vector<const std::u16string>&    ChatPlexService::UnlockedFeatures()
    {
        return *reinterpret_cast<std::vector<const std::u16string>*>(&m_UnlockedFeatures);
    }

    _v::Event<ChatPlexService::EState, ChatPlexService::EState> ChatPlexService::StateChanged;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init the service
    void ChatPlexService::Init()
    {
        m_WebClientCore = _v::WebClientCore::Make(u"https://api.chatplex.org/", _u::TimeSpan::FromSeconds(10), false, true);
        m_JsonRPCClient = _v::JsonRPCClient::Create(m_WebClientCore);

        m_Thread = new _u::il2cpp_aware_thread(&ThreadRunner);

        m_DeviceName = _u::SystemInfo::GetDeviceName();
    }
    /// @brief Release the service
    void ChatPlexService::Release()
    {
        m_ThreadCondition = false;
        m_Thread->join();

        delete m_Thread;
        m_Thread = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Add a callback to be called when the token is ready (call immediatly if ready)
    /// @param action Callback to be caled
    void ChatPlexService::OnTokenReady(_v::CActionRef<> action)
    {
        if (m_State == EState::Connected)
            action();
        else
        {
            std::lock_guard<std::mutex> l_Lock(m_OnTokenReadyQueueMutex);
            m_OnTokenReadyQueue.push(action);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Start linking procedure
    void ChatPlexService::StartLinking()
    {
        if (m_State != EState::Disconnected)
            return;

        ChangeState(EState::LinkRequest);
    }
    /// @brief Stop linking procedure
    void ChatPlexService::StopLinking()
    {
        if (m_State != EState::LinkRequest && m_State != EState::LinkWait)
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
        CPConfig::Instance()->ChatPlexServiceToken = u"";
        CPConfig::Instance()->Save();

        ChangeState(EState::Disconnected);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Change state and notify listenners
    void ChatPlexService::ChangeState(const EState newState)
    {
        auto l_OldState = m_State;
        m_State = newState;

        Unity::MTThreadInvoker::EnqueueOnThread([=]() -> void { StateChanged(l_OldState, newState); });
    }
    /// @brief Fire on token ready actions
    void ChatPlexService::FireOnTokenReady()
    {
        m_OnTokenReadyQueueMutex.lock();
        while (!m_OnTokenReadyQueue.empty())
        {
            auto l_Action = m_OnTokenReadyQueue.front();
            m_OnTokenReadyQueue.pop();
            m_OnTokenReadyQueueMutex.unlock();
            try
            {
                l_Action();
            }
            catch (std::exception l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK][ChatPlexService.FireOnTokenReady] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
            m_OnTokenReadyQueueMutex.lock();
        }

        m_OnTokenReadyQueueMutex.unlock();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Thread function
    void ChatPlexService::ThreadRunner()
    {
        while (m_ThreadCondition)
        {
            if (m_State == EState::Disconnected && !CPConfig::Instance()->ChatPlexServiceToken.empty())
            {
                m_WebClientCore->RemoveHeader(u"Authorization");

                ChangeState(EState::Connecting);

                std::shared_ptr<_v::Json::U16Document> l_Content(new _v::Json::U16Document());
                l_Content->SetObject();
                l_Content->AddMember(u"ConnectedApplicationToken", _v::Json::U16Value(CPConfig::Instance()->ChatPlexServiceToken, l_Content->GetAllocator()), l_Content->GetAllocator());

                auto l_Result = m_JsonRPCClient->Request(
                    u"Account_AuthByConnectedApplicationToken",
                    l_Content
                );

                if (IsRPCSuccess(l_Result))
                    OnAuthed(l_Result);
                else
                {
                    if (l_Result->Result != nullptr && l_Result->Result->HasMember(u"Result") && (*l_Result->Result)[u"Result"].GetBool() == false)
                    {
                        CPConfig::Instance()->ChatPlexServiceToken = u"";
                        CPConfig::Instance()->Save();

                        ChangeState(EState::Disconnected);
                    }
                    else
                        OnError(l_Result);
                }
            }
            else if (m_State == EState::LinkRequest)
            {
                std::shared_ptr<_v::Json::U16Document> l_Content(new _v::Json::U16Document());
                l_Content->SetObject();
                l_Content->AddMember(u"ApplicationIdentifier", _v::Json::U16Value(ChatPlexSDK::ProductName().data(), l_Content->GetAllocator()), l_Content->GetAllocator());
                l_Content->AddMember(u"ApplicationDeviceName", _v::Json::U16Value(m_DeviceName, l_Content->GetAllocator()), l_Content->GetAllocator());

                auto l_Result = m_JsonRPCClient->Request(
                    u"ConnectedApplication_CreateLinkRequest",
                    l_Content
                );

                if (IsRPCSuccess(l_Result))
                {
                    auto& l_ResultR = *l_Result->Result.get();
                    m_LinkRequestID = l_ResultR[u"RequestID"].GetString();
                    m_LinkCode      = l_ResultR[u"Code"].GetString();

                    ChangeState(EState::LinkWait);
                }
                else
                    OnError(l_Result);
            }
            else if (m_State == EState::LinkWait)
            {
                std::shared_ptr<_v::Json::U16Document> l_Content(new _v::Json::U16Document());
                l_Content->SetObject();
                l_Content->AddMember(u"RequestID", _v::Json::U16Value(m_LinkRequestID, l_Content->GetAllocator()), l_Content->GetAllocator());

                auto l_Result = m_JsonRPCClient->Request(
                    u"ConnectedApplication_GetLinkRequestStatus",
                    l_Content
                );

                if (IsRPCSuccess(l_Result))
                {
                    auto& l_ResultR = *l_Result->Result.get();
                    if (l_ResultR[u"ResultToken"].IsString())
                    {
                        CPConfig::Instance()->ChatPlexServiceToken = l_ResultR[u"ResultToken"].GetString();
                        CPConfig::Instance()->Save();

                        ChangeState(EState::Disconnected);
                    }
                }
                else
                    OnError(l_Result);
            }

            if (m_State == EState::LinkWait)
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Is RPC call result a success result?
    /// @param rpcResult Result of the RPC command
    /// @return True if success
    bool ChatPlexService::IsRPCSuccess(_v::JsonRPCResult::Ptr& rpcResult)
    {
        if (rpcResult->Result == nullptr)
            return false;

        auto& l_ResultR = *rpcResult->Result.get();
        return l_ResultR.HasMember(u"Result") && l_ResultR[u"Result"].GetBool() == true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief When we are Authed
    /// @param rpcResult Result of the RPC command
    void ChatPlexService::OnAuthed(_v::JsonRPCResult::Ptr& rpcResult)
    {
        auto& l_ResultR = *rpcResult->Result.get();
        m_ActiveSubscription    = l_ResultR.HasMember(u"ActiveSubscription") ? l_ResultR[u"ActiveSubscription"].GetString() : u"";

        m_UnlockedFeatures.clear();
        if (l_ResultR.HasMember(u"UnlockedFeatures") && l_ResultR[u"UnlockedFeatures"].IsArray())
        {
            auto l_Array = l_ResultR[u"UnlockedFeatures"].GetArray();
            for (auto& l_CurrentValue : l_Array)
            {
                if (!l_CurrentValue.IsString())
                    continue;

                m_UnlockedFeatures.push_back(l_CurrentValue.GetString());
            }
        }

        m_WebClientCore->SetHeader(u"Authorization", std::u16string(u"ConnectedApplicationToken ") + CPConfig::Instance()->ChatPlexServiceToken);

        ChangeState(EState::Connected);
        FireOnTokenReady();
    }
    /// @brief On error received
    /// @param rpcResult Result of the RPC command
    void ChatPlexService::OnError(_v::JsonRPCResult::Ptr& rpcResult)
    {
        std::u16string l_Error = u"Unknow server error!";
        if (rpcResult->Result != nullptr && rpcResult->Result->HasMember(u"Error"))
        {
            auto& l_ResultR = *rpcResult->Result.get();
            l_Error = l_ResultR[u"Error"].GetString();
        }

        m_LastError = l_Error;
        ChangeState(EState::Error);
    }

}   ///< namespace CP_SDK