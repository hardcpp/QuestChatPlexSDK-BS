#include "CP_SDK/Network/WebClientUnity.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/Net/HttpStatusCode.hpp>
#include <UnityEngine/Networking/DownloadHandlerBuffer.hpp>
#include <UnityEngine/Networking/UploadHandler.hpp>
#include <UnityEngine/Networking/UploadHandlerRaw.hpp>
#include <UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp>
#include <UnityEngine/WaitForSecondsRealtime.hpp>

using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace UnityEngine;
using namespace UnityEngine::Networking;

namespace CP_SDK::Network {

    WebClientUnity::Ptr WebClientUnity::m_GlobalClient;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Global client instance
    WebClientUnity* WebClientUnity::GlobalClient()
    {
        if (!m_GlobalClient)
            m_GlobalClient = WebClientUnity::Make(u"", TimeSpan::FromSeconds(10));

        return m_GlobalClient.get();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param p_BaseAddress       Base address
    /// @param p_TimeOut           Requests timeout
    /// @param p_ForceCacheDiscard Should force server cache discard
    WebClientUnity::WebClientUnity(PRIVATE_TAG_ARG(), std::u16string_view p_BaseAddress, TimeSpan p_TimeOut, bool p_ForceCacheDiscard)
        : m_Headers({})
    {
        DownloadTimeout = 2 * 60;
        MaxRetry        = 2;
        RetryInterval   = 5;

        m_BaseAddress = p_BaseAddress;

        m_Timeout = (int)p_TimeOut.get_TotalSeconds();

        if (p_ForceCacheDiscard)
            m_Headers[u"Cache-Control"] = u"no-cache, must-revalidate, proxy-revalidate, max-age=0, s-maxage=0, max-stale=0";
    }

    /// @brief Constructor
    /// @param p_BaseAddress       Base address
    /// @param p_TimeOut           Requests timeout
    /// @param p_ForceCacheDiscard Should force server cache discard
    WebClientUnity::Ptr WebClientUnity::Make(std::u16string_view p_BaseAddress, TimeSpan p_TimeOut, bool p_ForceCacheDiscard)
    {
        return std::make_shared<WebClientUnity>(PRIVATE_TAG_VAL(), p_BaseAddress, p_TimeOut, p_ForceCacheDiscard);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get header
    /// @param p_Name Header name
    std::u16string WebClientUnity::GetHeader(std::u16string_view p_Name)
    {
        auto l_Name = std::u16string(p_Name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        if (!m_Headers.contains(l_Name))
            return u"";

        return m_Headers[l_Name];
    }
    /// @brief Set header
    /// @param p_Name  Header name
    /// @param p_Value Header value
    void WebClientUnity::SetHeader(std::u16string_view p_Name, std::u16string_view p_Value)
    {
        auto l_Name = std::u16string(p_Name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        m_Headers[l_Name] = std::u16string(p_Value);
    }
    /// @brief Remove header
    /// @param p_Name Header name
    void WebClientUnity::RemoveHeader(std::u16string_view p_Name)
    {
        auto l_Name = std::u16string(p_Name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        auto l_It = m_Headers.find(l_Name);
        if (l_It != m_Headers.end())
            m_Headers.erase(l_It);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do Async GET query
    /// @param p_URL       Target URL
    /// @param p_Token     Cancellation token
    /// @param p_Callback  Callback
    /// @param p_DontRetry Should not retry
    /// @param p_Progress  Progress reporter
    void WebClientUnity::GetAsync(std::u16string_view p_URL, CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry, _v::CActionRef<float> p_Progress)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"GetAsync", u"GET", GetURL(p_URL), nullptr, p_Token, p_Callback, p_DontRetry, p_Progress)));
    }
    /// @brief Do Async GET query
    /// @param p_URL       Target URL
    /// @param p_Token     Cancellation token
    /// @param p_Callback  Callback
    /// @param p_DontRetry Should not retry
    /// @param p_Progress  Progress reporter
    void WebClientUnity::DownloadAsync(std::u16string_view p_URL, CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry, _v::CActionRef<float> p_Progress)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"DownloadAsync", u"DOWNLOAD", GetURL(p_URL), nullptr, p_Token, p_Callback, p_DontRetry, p_Progress)));
    }
    /// @brief Do Async POST query
    /// @param p_URL         Target URL
    /// @param p_Content     Optional content to post
    /// @param p_Token       Cancellation token
    /// @param p_Callback    Callback
    /// @param p_DontRetry   Should not retry
    void WebClientUnity::PostAsync(std::u16string_view p_URL, const WebContent::Ptr& p_Content, CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"PostAsync", u"POST", GetURL(p_URL), p_Content, p_Token, p_Callback, p_DontRetry, nullptr)));
    }
    /// @brief Do Async PATCH query
    /// @param p_URL         Target URL
    /// @param p_Content     Optional content to post
    /// @param p_Token       Cancellation token
    /// @param p_Callback    Callback
    /// @param p_DontRetry   Should not retry
    void WebClientUnity::PatchAsync(std::u16string_view p_URL, const WebContent::Ptr& p_Content, CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"PatchAsync", u"PATCH", GetURL(p_URL), p_Content, p_Token, p_Callback, p_DontRetry, nullptr)));
    }
    /// @brief Do Async GET query
    /// @param p_URL       Target URL
    /// @param p_Token     Cancellation token
    /// @param p_Callback  Callback
    /// @param p_DontRetry Should not retry
    /// @param p_Progress  Progress reporter
    void WebClientUnity::DeleteAsync(std::u16string_view p_URL, CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"DownloadAsync", u"DOWNLOAD", GetURL(p_URL), nullptr, p_Token, p_Callback, p_DontRetry, nullptr)));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get URL
    /// @param p_URL Request URL
    std::u16string WebClientUnity::GetURL(std::u16string_view p_URL)
    {
        if (m_BaseAddress.size() == 0)                      return std::u16string(p_URL);
        if (p_URL.find(u"://") != std::u16string::npos)     return std::u16string(p_URL);
        if (m_BaseAddress[m_BaseAddress.size() - 1] == '/') return m_BaseAddress + std::u16string(p_URL);

        return m_BaseAddress + u"/" + std::u16string(p_URL);
    }
    /// @brief Safe URL parsing
    /// @param p_URL Source URL
    std::u16string WebClientUnity::SafeURL(std::u16string_view p_URL)
    {
        auto l_Position = p_URL.find_first_of('?');
        if (l_Position != std::u16string::npos)
            return std::u16string(p_URL).substr(0, l_Position);

        return std::u16string(p_URL);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Prepare request
    /// @param p_Request    Request to prepare
    /// @param p_IsDownload Is a download request?
    void WebClientUnity::PrepareRequest(UnityWebRequest* p_Request, bool p_IsDownload)
    {
        if (p_Request->get_downloadHandler() == nullptr)
            p_Request->set_downloadHandler(DownloadHandlerBuffer::New_ctor());

        p_Request->set_timeout(p_IsDownload ? DownloadTimeout : m_Timeout);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        for (auto const& [l_Header, l_Value] : m_Headers)
            p_Request->SetRequestHeader(l_Header, l_Value);
    }
    /// @brief Do request
    /// @param p_DebugName   Method name for logs
    /// @param p_HttpMethod  Http method
    /// @param p_URL         Target URL
    /// @param p_Content     Optional content to post
    /// @param p_Token       Cancellation token
    /// @param p_Callback    Callback
    /// @param p_DontRetry   Should not retry
    /// @param p_Progress    Progress reporter
    custom_types::Helpers::Coroutine WebClientUnity::Coroutine_DoRequest(Ptr                            p_Self,
                                                                         std::u16string                 p_DebugName,
                                                                         std::u16string                 p_HttpMethod,
                                                                         std::u16string                 p_URL,
                                                                         WebContent::Ptr                p_Content,
                                                                         CancellationToken              p_Token,
                                                                         _v::Action<WebResponse::Ptr>   p_Callback,
                                                                         bool                           p_DontRetry,
                                                                         _v::Action<float>              p_Progress)
    {
#if DEBUG
        ChatPlexSDK::Logger()->Debug(u"[CP_SDK.Network][WebClientUnity." + p_DebugName + u"] " + p_HttpMethod + u" " + p_URL);
#endif

        auto l_Reply = (WebResponse::Ptr)nullptr;
        for (int l_RetryI = 1; l_RetryI <= p_Self->MaxRetry; l_RetryI++)
        {
            if (p_Token.get_IsCancellationRequested())
                break;

            auto l_Request = _v::MonoPtr<UnityWebRequest>(nullptr);
            if (p_HttpMethod == u"GET" || p_HttpMethod == u"DOWNLOAD")
                l_Request = UnityWebRequest::Get(p_URL);
            else if (p_HttpMethod == u"POST" || p_HttpMethod == u"PATCH")
            {
                static auto s_UploadHandler_InternalSetContentType = il2cpp_utils::resolve_icall<void, UploadHandler*, StringW>("UnityEngine.Networking.UploadHandler::InternalSetContentType");

                auto l_UploadHandler = UploadHandlerRaw::New_ctor(p_Content->Bytes.Ptr());
                s_UploadHandler_InternalSetContentType(l_UploadHandler, p_Content->Type);

                l_Request = UnityWebRequest::New_ctor(p_URL, p_HttpMethod);
                l_Request->set_uploadHandler(l_UploadHandler);
                l_Request->set_downloadHandler(DownloadHandlerBuffer::New_ctor());
            }
            else if (p_HttpMethod == u"DELETE")
                l_Request = UnityWebRequest::New_ctor(p_URL, p_HttpMethod);

            p_Self->PrepareRequest(l_Request.Ptr(), p_HttpMethod == u"DOWNLOAD");

            if (!p_Progress.IsValid())
                co_yield reinterpret_cast<System::Collections::IEnumerator*>(l_Request->SendWebRequest());
            else
            {
                try { p_Progress(0.0f); } catch (const std::exception&) { }
                l_Request->SendWebRequest();

                auto l_Waiter = WaitForSecondsRealtime::New_ctor(0.05f);
                do
                {
                    co_yield l_Waiter->i_IEnumerator();
                    try { p_Progress(l_Request->get_downloadProgress()); } catch (const std::exception&) { }

                    if (p_Token.get_IsCancellationRequested() || l_Request->get_isDone() || l_Request->get_isHttpError() || l_Request->get_isNetworkError())
                        break;
                } while (true);
            }

            if (p_Token.get_IsCancellationRequested())
                break;

            l_Reply = std::make_shared<WebResponse>(l_Request.Ptr());

            if (!l_Reply->IsSuccessStatusCode() && l_Reply->StatusCode() == (HttpStatusCode)429)
            {
              /*  var l_Limits = RateLimitInfo.Get(l_Request);
                if (l_Limits != nullptr)
                {
                    int l_TotalMilliseconds = (int)(l_Limits.Reset - DateTime.Now).TotalMilliseconds;
                    if (l_TotalMilliseconds > 0)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Network][WebClientUnity." + p_DebugName + u"] Request {SafeURL(p_URL)} was rate limited, retrying in {l_TotalMilliseconds}ms...");

                        co_yield WaitForSecondsRealtime::New_ctor(RetryInterval)->i_IEnumerator();
                        continue;
                    }
                }*/
            }

            if (!l_Reply->IsSuccessStatusCode())
            {
                auto l_LogPrefix = u"[CP_SDK.Network][WebClientUnity." + p_DebugName + u"] Request " + p_Self->SafeURL(p_URL) + u" failed with code ";
                l_LogPrefix += StringW(std::to_string((int)l_Reply->StatusCode()));
                l_LogPrefix += u":\"" + l_Reply->ReasonPhrase() + "\", ";

                if (!l_Reply->ShouldRetry() || p_DontRetry)
                {
                    ChatPlexSDK::Logger()->Error(l_LogPrefix + u" not retrying");
                    break;
                }

                ChatPlexSDK::Logger()->Error(l_LogPrefix + u" next try in " + (std::u16string)StringW(std::to_string(p_Self->RetryInterval)) + u" seconds...");

                co_yield WaitForSecondsRealtime::New_ctor(p_Self->RetryInterval)->i_IEnumerator();
                continue;
            }
            else
            {
                if (p_Progress.IsValid())
                    try { p_Progress(1.0f); } catch (const std::exception&) { }

                break;
            }
        }

        if (!p_Token.get_IsCancellationRequested() && p_Callback.IsValid())
            Unity::MTThreadInvoker::EnqueueOnThread([=]() -> void { p_Callback(l_Reply); });
    }

}   ///< namespace CP_SDK::Network