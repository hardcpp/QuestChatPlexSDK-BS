#include "CP_SDK/Network/WebClientUnity.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/Net/HttpStatusCode.hpp>
#include <UnityEngine/Networking/DownloadHandlerBuffer.hpp>
#include <UnityEngine/Networking/UploadHandler.hpp>
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
    /// @param baseAddress       Base address
    /// @param timeOut           Requests timeout
    /// @param forceCacheDiscard Should force server cache discard
    WebClientUnity::WebClientUnity(CP_SDK_PRIV_TAG_ARG(), std::u16string_view baseAddress, TimeSpan timeOut, bool forceCacheDiscard)
        : m_Headers({})
    {
        DownloadTimeout = 2 * 60;
        MaxRetry        = 2;
        RetryInterval   = 5;

        m_BaseAddress = baseAddress;

        m_Timeout = (int)timeOut.get_TotalSeconds();

        if (forceCacheDiscard)
            m_Headers[u"Cache-Control"] = u"no-cache, must-revalidate, proxy-revalidate, max-age=0, s-maxage=0, max-stale=0";
    }

    /// @brief Constructor
    /// @param baseAddress       Base address
    /// @param timeOut           Requests timeout
    /// @param forceCacheDiscard Should force server cache discard
    WebClientUnity::Ptr WebClientUnity::Make(std::u16string_view baseAddress, TimeSpan timeOut, bool forceCacheDiscard)
    {
        return std::make_shared<WebClientUnity>(CP_SDK_PRIV_TAG_VAL(), baseAddress, timeOut, forceCacheDiscard);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get header
    /// @param name Header name
    std::u16string WebClientUnity::GetHeader(std::u16string_view name)
    {
        auto l_Name = std::u16string(name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        if (!m_Headers.contains(l_Name))
            return u"";

        return m_Headers[l_Name];
    }
    /// @brief Set header
    /// @param name  Header name
    /// @param value Header value
    void WebClientUnity::SetHeader(std::u16string_view name, std::u16string_view value)
    {
        auto l_Name = std::u16string(name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        m_Headers[l_Name] = std::u16string(value);
    }
    /// @brief Remove header
    /// @param name Header name
    void WebClientUnity::RemoveHeader(std::u16string_view name)
    {
        auto l_Name = std::u16string(name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        auto l_It = m_Headers.find(l_Name);
        if (l_It != m_Headers.end())
            m_Headers.erase(l_It);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do Async GET query
    /// @param url       Target URL
    /// @param token     Cancellation token
    /// @param callback  Callback
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    void WebClientUnity::GetAsync(std::u16string_view url, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry, _v::CActionRef<float> progress)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"GetAsync", u"GET", GetURL(url), nullptr, token, callback, dontRetry, progress)));
    }
    /// @brief Do Async GET query
    /// @param url       Target URL
    /// @param token     Cancellation token
    /// @param callback  Callback
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    void WebClientUnity::DownloadAsync(std::u16string_view url, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry, _v::CActionRef<float> progress)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"DownloadAsync", u"DOWNLOAD", GetURL(url), nullptr, token, callback, dontRetry, progress)));
    }
    /// @brief Do Async POST query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    void WebClientUnity::PostAsync(std::u16string_view url, const WebContent::Ptr& content, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"PostAsync", u"POST", GetURL(url), content, token, callback, dontRetry, nullptr)));
    }
    /// @brief Do Async PATCH query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    void WebClientUnity::PatchAsync(std::u16string_view url, const WebContent::Ptr& content, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"PatchAsync", u"PATCH", GetURL(url), content, token, callback, dontRetry, nullptr)));
    }
    /// @brief Do Async PUT query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    void WebClientUnity::PutAsync(std::u16string_view url, const WebContent::Ptr& content, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"PutAsync", u"PUT", GetURL(url), content, token, callback, dontRetry, nullptr)));
    }
    /// @brief Do Async GET query
    /// @param url       Target URL
    /// @param token     Cancellation token
    /// @param callback  Callback
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    void WebClientUnity::DeleteAsync(std::u16string_view url, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_DoRequest(shared_from_this(), u"DownloadAsync", u"DOWNLOAD", GetURL(url), nullptr, token, callback, dontRetry, nullptr)));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get URL
    /// @param url Request URL
    std::u16string WebClientUnity::GetURL(std::u16string_view url)
    {
        if (m_BaseAddress.size() == 0)                      return std::u16string(url);
        if (url.find(u"://") != std::u16string::npos)     return std::u16string(url);
        if (m_BaseAddress[m_BaseAddress.size() - 1] == '/') return m_BaseAddress + std::u16string(url);

        return m_BaseAddress + u"/" + std::u16string(url);
    }
    /// @brief Safe URL parsing
    /// @param url Source URL
    std::u16string WebClientUnity::SafeURL(std::u16string_view url)
    {
        auto l_Position = url.find_first_of('?');
        if (l_Position != std::u16string::npos)
            return std::u16string(url).substr(0, l_Position);

        return std::u16string(url);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Prepare request
    /// @param request    Request to prepare
    /// @param isDownload Is a download request?
    void WebClientUnity::PrepareRequest(UnityWebRequest* request, bool isDownload)
    {
        if (request->get_downloadHandler() == nullptr)
            request->set_downloadHandler(DownloadHandlerBuffer::New_ctor());

        request->set_timeout(isDownload ? DownloadTimeout : m_Timeout);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);

        static auto s_UnityWebRequest_InternalSetRequestHeader = il2cpp_utils::resolve_icall<UnityWebRequest::UnityWebRequestError, UnityWebRequest*, StringW, StringW>("UnityEngine.Networking.UnityWebRequest::InternalSetRequestHeader");
        for (auto const& [l_Header, l_Value] : m_Headers)
            s_UnityWebRequest_InternalSetRequestHeader(request, l_Header, l_Value);
    }
    /// @brief Do request
    /// @param debugName   Method name for logs
    /// @param httpMethod  Http method
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    /// @param progress    Progress reporter
    custom_types::Helpers::Coroutine WebClientUnity::Coroutine_DoRequest(Ptr                            self,
                                                                         std::u16string                 debugName,
                                                                         std::u16string                 httpMethod,
                                                                         std::u16string                 url,
                                                                         WebContent::Ptr                content,
                                                                         CancellationToken              token,
                                                                         _v::Action<WebResponse::Ptr>   callback,
                                                                         bool                           dontRetry,
                                                                         _v::Action<float>              progress)
    {
#if DEBUG
        ChatPlexSDK::Logger()->Debug(u"[CP_SDK.Network][WebClientUnity." + debugName + u"] " + httpMethod + u" " + url);
#endif

        WebResponse::Ptr l_Reply = nullptr;
        for (int l_RetryI = 1; l_RetryI <= self->MaxRetry; l_RetryI++)
        {
            if (token.get_IsCancellationRequested())
                break;

            auto l_Request = _v::MonoPtr<UnityWebRequest>(nullptr);
            if (httpMethod == u"GET" || httpMethod == u"DOWNLOAD")
                l_Request = UnityWebRequest::Get(url);
            else if (httpMethod == u"POST" || httpMethod == u"PATCH" || httpMethod == u"PUT")
            {
                ChatPlexSDK::Logger()->Error(u"WebClientUnity POST & PATCH & PUT are disabled for now");
                throw std::runtime_error("WebClientUnity POST & PATCH are disabled for now");
                /// TODO Disabled until fixed

                /*static auto s_UploadHandler_InternalSetContentType
                    = il2cpp_utils::resolve_icall<void, UploadHandler*, StringW>("UnityEngine.Networking.UploadHandler::InternalSetContentType");

                auto l_UploadHandler = UploadHandlerRaw::New_ctor(content->Bytes.Ptr());
                s_UploadHandler_InternalSetContentType(l_UploadHandler, content->Type);

                l_Request = UnityWebRequest::New_ctor(url, httpMethod, DownloadHandlerBuffer::New_ctor(), l_UploadHandler);*/
            }
            else if (httpMethod == u"DELETE")
                l_Request = UnityWebRequest::New_ctor(url, httpMethod, nullptr, nullptr);

            self->PrepareRequest(l_Request.Ptr(), httpMethod == u"DOWNLOAD");

            if (!progress.IsValid())
                co_yield reinterpret_cast<Collections::IEnumerator*>(l_Request->SendWebRequest());
            else
            {
                try { progress(0.0f); } catch (const std::exception&) { }
                l_Request->SendWebRequest();

                auto l_Waiter = WaitForSecondsRealtime::New_ctor(0.05f);
                do
                {
                    co_yield l_Waiter->i___System__Collections__IEnumerator();
                    try {
                        static auto s_UnityWebRequest_IsExecuting           = il2cpp_utils::resolve_icall<float, UnityWebRequest*>("UnityEngine.Networking.UnityWebRequest::IsExecuting");
                        static auto s_UnityWebRequest_GetDownloadProgress   = il2cpp_utils::resolve_icall<float, UnityWebRequest*>("UnityEngine.Networking.UnityWebRequest::GetDownloadProgress");

                        auto l_Progress = (!s_UnityWebRequest_IsExecuting(l_Request.Ptr()) && !l_Request->get_isDone()) ? -1.0f : s_UnityWebRequest_GetDownloadProgress(l_Request.Ptr());
                        progress(l_Progress);
                    } catch (const std::exception&) { }

                    if (token.get_IsCancellationRequested() || l_Request->get_isDone() || l_Request->get_result() == UnityWebRequest::Result::ProtocolError || l_Request->get_result() == UnityWebRequest::Result::ConnectionError)
                        break;
                } while (true);
            }

            if (token.get_IsCancellationRequested())
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
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Network][WebClientUnity." + debugName + u"] Request {SafeURL(url)} was rate limited, retrying in {l_TotalMilliseconds}ms...");

                        co_yield WaitForSecondsRealtime::New_ctor(RetryInterval)->i_IEnumerator();
                        continue;
                    }
                }*/
            }

            if (!l_Reply->IsSuccessStatusCode())
            {
                auto l_LogPrefix = u"[CP_SDK.Network][WebClientUnity." + debugName + u"] Request " + self->SafeURL(url) + u" failed with code ";
                l_LogPrefix += StringW(std::to_string(l_Reply->StatusCode().value__));
                l_LogPrefix += u":\"" + l_Reply->ReasonPhrase() + "\", ";

                if (!l_Reply->ShouldRetry() || dontRetry)
                {
                    ChatPlexSDK::Logger()->Error(l_LogPrefix + u" not retrying");
                    break;
                }

                ChatPlexSDK::Logger()->Error(l_LogPrefix + u" next try in " + (std::u16string)StringW(std::to_string(self->RetryInterval)) + u" seconds...");

                co_yield WaitForSecondsRealtime::New_ctor(self->RetryInterval)->i___System__Collections__IEnumerator();
                continue;
            }
            else
            {
                if (progress.IsValid())
                    try { progress(1.0f); } catch (const std::exception&) { }

                break;
            }
        }

        if (!token.get_IsCancellationRequested() && callback.IsValid())
            Unity::MTThreadInvoker::EnqueueOnThread([=]() -> void { callback(l_Reply); });
    }

}   ///< namespace CP_SDK::Network