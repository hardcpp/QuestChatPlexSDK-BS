#include "CP_SDK/Network/WebClientCore.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/Threading/zzzz__CancellationToken_def.hpp>
#include <beatsaber-hook/shared/utils/il2cpp-utils.hpp>
#include <cstdint>
#include <cstring>
#include <libcurl/shared/curl.h>
#include <libcurl/shared/easy.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

using namespace System;
using namespace System::Net;
using namespace System::Threading;

struct ScopedCURL
{
            CURL*                   Instance;
    struct  curl_slist*             Headers = NULL;
            std::vector<uint8_t>*   Data;

    ScopedCURL()
    {
        Instance    = curl_easy_init();
        Data        = new std::vector<uint8_t>();
    }
    ~ScopedCURL()
    {
        curl_easy_cleanup(Instance);
        if (Headers != NULL)
            curl_slist_free_all(Headers);

        delete Data;
    }
};

namespace CP_SDK::Network {

    WebClientCore::Ptr WebClientCore::m_GlobalClient;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Global client instance
    WebClientCore* WebClientCore::GlobalClient()
    {
        if (!m_GlobalClient)
            m_GlobalClient = WebClientCore::Make(u"", TimeSpan::FromSeconds(10), true);

        return m_GlobalClient.get();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param baseAddress       Base address
    /// @param timeOut           Requests timeout
    /// @param forceCacheDiscard Should force server cache discard
    WebClientCore::WebClientCore(CP_SDK_PRIV_TAG_ARG(), std::u16string_view baseAddress, TimeSpan timeOut, bool keepAlive, bool forceCacheDiscard)
        : m_Headers({})
    {
        MaxRetry        = 2;
        RetryInterval   = 5;

        m_BaseAddress = baseAddress;

        m_TimeOut = (int)timeOut.get_TotalSeconds();

        if (forceCacheDiscard)
            m_Headers[u"Cache-Control"] = u"no-cache, must-revalidate, proxy-revalidate, max-age=0, s-maxage=0, max-stale=0";
    }

    /// @brief Constructor
    /// @param baseAddress       Base address
    /// @param timeOut           Requests timeout
    /// @param forceCacheDiscard Should force server cache discard
    WebClientCore::Ptr WebClientCore::Make(std::u16string_view baseAddress, TimeSpan timeOut, bool keepAlive, bool forceCacheDiscard)
    {
        return std::make_shared<WebClientCore>(CP_SDK_PRIV_TAG_VAL(), baseAddress, timeOut, keepAlive, forceCacheDiscard);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get header
    /// @param name Header name
    std::u16string WebClientCore::GetHeader(std::u16string_view name)
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
    void WebClientCore::SetHeader(std::u16string_view name, std::u16string_view value)
    {
        auto l_Name = std::u16string(name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        m_Headers[l_Name] = std::u16string(value);
    }
    /// @brief Remove header
    /// @param name Header name
    void WebClientCore::RemoveHeader(std::u16string_view name)
    {
        auto l_Name = std::u16string(name);

        std::lock_guard<std::mutex> l_Guard(m_HeadersLock);
        auto l_It = m_Headers.find(l_Name);
        if (l_It != m_Headers.end())
            m_Headers.erase(l_It);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do GET query
    /// @param url       Target URL
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    WebResponse::Ptr WebClientCore::Get(std::u16string_view url, bool dontRetry, _v::CActionRef<float> progress)
    {
        auto l_Reply = WebResponse::Ptr(nullptr);

        DoRequest(
            shared_from_this(),
            u"Get",
            u"GET",
            GetURL(url),
            nullptr,
            CancellationToken::get_None(),
            [&](WebResponse::Ptr p_Result) -> void { l_Reply = p_Result; },
            dontRetry,
            progress
        );

        return l_Reply;
    }
    /// @brief Do GET query
    /// @param url       Target URL
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    WebResponse::Ptr WebClientCore::Download(std::u16string_view url, bool dontRetry, _v::CActionRef<float> progress)
    {
        auto l_Reply = WebResponse::Ptr(nullptr);

        DoRequest(
            shared_from_this(),
            u"Download",
            u"DOWNLOAD",
            GetURL(url),
            nullptr,
            CancellationToken::get_None(),
            [&](WebResponse::Ptr p_Result) -> void { l_Reply = p_Result; },
            dontRetry,
            progress
        );

        return l_Reply;
    }
    /// @brief Do POST query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param dontRetry   Should not retry
    WebResponse::Ptr WebClientCore::Post(std::u16string_view url, const WebContent::Ptr& content, bool dontRetry)
    {
        auto l_Reply = WebResponse::Ptr(nullptr);

        DoRequest(
            shared_from_this(),
            u"Post",
            u"POST",
            GetURL(url),
            content,
            CancellationToken::get_None(),
            [&](WebResponse::Ptr p_Result) -> void { l_Reply = p_Result; },
            dontRetry,
            nullptr
        );

        return l_Reply;
    }
    /// @brief Do PATCH query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param dontRetry   Should not retry
    WebResponse::Ptr WebClientCore::Patch(std::u16string_view url, const WebContent::Ptr& content, bool dontRetry)
    {
        auto l_Reply = WebResponse::Ptr(nullptr);

        DoRequest(
            shared_from_this(),
            u"Patch",
            u"PATCH",
            GetURL(url),
            content,
            CancellationToken::get_None(),
            [&](WebResponse::Ptr p_Result) -> void { l_Reply = p_Result; },
            dontRetry,
            nullptr
        );

        return l_Reply;
    }
    /// @brief Do PUT query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param dontRetry   Should not retry
    WebResponse::Ptr WebClientCore::Put(std::u16string_view url, const WebContent::Ptr& content, bool dontRetry)
    {
        auto l_Reply = WebResponse::Ptr(nullptr);

        DoRequest(
            shared_from_this(),
            u"Put",
            u"PUT",
            GetURL(url),
            content,
            CancellationToken::get_None(),
            [&](WebResponse::Ptr p_Result) -> void { l_Reply = p_Result; },
            dontRetry,
            nullptr
        );

        return l_Reply;
    }
    /// @brief Do DELETE query
    /// @param url       Target URL
    /// @param dontRetry Should not retry
    WebResponse::Ptr WebClientCore::Delete(std::u16string_view url, bool dontRetry)
    {
        auto l_Reply = WebResponse::Ptr(nullptr);

        DoRequest(
            shared_from_this(),
            u"Delete",
            u"DELETE",
            GetURL(url),
            nullptr,
            CancellationToken::get_None(),
            [&](WebResponse::Ptr p_Result) -> void { l_Reply = p_Result; },
            dontRetry,
            nullptr
        );

        return l_Reply;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do Async GET query
    /// @param url       Target URL
    /// @param token     Cancellation token
    /// @param callback  Callback
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    void WebClientCore::GetAsync(std::u16string_view url, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry, _v::CActionRef<float> progress)
    {
        il2cpp_utils::il2cpp_aware_thread(
            &DoRequest,
            shared_from_this(),
            u"GetAsync",
            u"GET",
            GetURL(url),
            nullptr,
            token,
            callback,
            dontRetry,
            progress
        ).detach();
    }
    /// @brief Do Async GET query
    /// @param url       Target URL
    /// @param token     Cancellation token
    /// @param callback  Callback
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    void WebClientCore::DownloadAsync(std::u16string_view url, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry, _v::CActionRef<float> progress)
    {
        il2cpp_utils::il2cpp_aware_thread(
            &DoRequest,
            shared_from_this(),
            u"DownloadAsync",
            u"DOWNLOAD",
            GetURL(url),
            nullptr,
            token,
            callback,
            dontRetry,
            progress
        ).detach();
    }
    /// @brief Do Async POST query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    void WebClientCore::PostAsync(std::u16string_view url, const WebContent::Ptr& content, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        il2cpp_utils::il2cpp_aware_thread(
            &DoRequest,
            shared_from_this(),
            u"PostAsync",
            u"POST",
            GetURL(url),
            content,
            token,
            callback,
            dontRetry,
            nullptr
        ).detach();
    }
    /// @brief Do Async PATCH query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    void WebClientCore::PatchAsync(std::u16string_view url, const WebContent::Ptr& content, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        il2cpp_utils::il2cpp_aware_thread(
            &DoRequest,
            shared_from_this(),
            u"PatchAsync",
            u"PATCH",
            GetURL(url),
            content,
            token,
            callback,
            dontRetry,
            nullptr
        ).detach();
    }
    /// @brief Do Async PATCH query
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    void WebClientCore::PutAsync(std::u16string_view url, const WebContent::Ptr& content, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        il2cpp_utils::il2cpp_aware_thread(
            &DoRequest,
            shared_from_this(),
            u"PutAsync",
            u"PUT",
            GetURL(url),
            content,
            token,
            callback,
            dontRetry,
            nullptr
        ).detach();
    }
    /// @brief Do Async GET query
    /// @param url       Target URL
    /// @param token     Cancellation token
    /// @param callback  Callback
    /// @param dontRetry Should not retry
    /// @param progress  Progress reporter
    void WebClientCore::DeleteAsync(std::u16string_view url, CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry)
    {
        il2cpp_utils::il2cpp_aware_thread(
            &DoRequest,
            shared_from_this(),
            u"DeleteAsync",
            u"DELETE",
            GetURL(url),
            nullptr,
            token,
            callback,
            dontRetry,
            nullptr
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get URL
    /// @param url Request URL
    std::u16string WebClientCore::GetURL(std::u16string_view url)
    {
        if (m_BaseAddress.size() == 0)                      return std::u16string(url);
        if (url.find(u"://") != std::u16string::npos)     return std::u16string(url);
        if (m_BaseAddress[m_BaseAddress.size() - 1] == '/') return m_BaseAddress + std::u16string(url);

        return m_BaseAddress + u"/" + std::u16string(url);
    }
    /// @brief Safe URL parsing
    /// @param url Source URL
    std::u16string WebClientCore::SafeURL(std::u16string_view url)
    {
        auto l_Position = url.find_first_of('?');
        if (l_Position != std::u16string::npos)
            return std::u16string(url).substr(0, l_Position);

        return std::u16string(url);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Do request
    /// @param debugName   Method name for logs
    /// @param httpMethod  Http method
    /// @param url         Target URL
    /// @param content     Optional content to post
    /// @param token       Cancellation token
    /// @param callback    Callback
    /// @param dontRetry   Should not retry
    /// @param progress    Progress reporter
    void WebClientCore::DoRequest(
            Ptr                            self,
            std::u16string                 debugName,
            std::u16string                 httpMethod,
            std::u16string                 url,
            WebContent::Ptr                content,
            CancellationToken              token,
            _v::Action<WebResponse::Ptr>   callback,
            bool                           dontRetry,
            _v::Action<float>              progress
        )
    {
#if DEBUG
        ChatPlexSDK::Logger()->Debug(u"[CP_SDK.Network][WebClientCore." + debugName + u"] " + httpMethod + u" " + url);
#endif

        WebResponse::Ptr l_Reply = nullptr;
        for (int l_RetryI = 1; l_RetryI <= self->MaxRetry; l_RetryI++)
        {
            if (token.get_IsCancellationRequested())
                break;

            ScopedCURL l_ScopedCURL;
            l_ScopedCURL.Headers = curl_slist_append(l_ScopedCURL.Headers, "Accept: */*");

            for (auto const& [l_Header, l_Value] : self->m_Headers)
                l_ScopedCURL.Headers = curl_slist_append(l_ScopedCURL.Headers, Utils::U16StrToStr(l_Header + ": " + l_Value).c_str());

            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_URL, Utils::U16StrToStr(url).c_str());
            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_TIMEOUT, static_cast<long>(self->m_TimeOut));
            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_FOLLOWLOCATION, static_cast<long>(1));
            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_USERAGENT, Utils::U16StrToStr(ChatPlexSDK::NetworkUserAgent()).c_str());

            if (httpMethod == u"GET" || httpMethod == u"DOWNLOAD")
                curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_CUSTOMREQUEST, "GET");
            else if (httpMethod == u"POST" || httpMethod == u"PATCH" || httpMethod == u"PUT")
            {
                if (httpMethod == u"POST")
                    curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_CUSTOMREQUEST, "POST");
                if (httpMethod == u"PATCH")
                    curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_CUSTOMREQUEST, "PATCH");
                else
                    curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_CUSTOMREQUEST, "PUT");

                if (content)
                {
                    l_ScopedCURL.Headers = curl_slist_append(l_ScopedCURL.Headers, Utils::U16StrToStr(std::u16string(u"Content-Type: ") + content->Type).c_str());
                    curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_POSTFIELDSIZE, static_cast<long>(content->Bytes->get_Length()));
                    curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_POSTFIELDS, &content->Bytes->_values[0]);
                }
            }
            else if (httpMethod == u"DELETE")
                curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_CUSTOMREQUEST, "DELETE");

            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_HTTPHEADER, l_ScopedCURL.Headers);
            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_SSL_VERIFYPEER, false);

            if (progress.IsValid())
            {
                curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_NOPROGRESS, false);
                curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_XFERINFODATA, (void*)&progress);
                curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_XFERINFOFUNCTION,
                    +[] (_v::Action<float>* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
                        float percentage = (float)dlnow / (float)dltotal * 100.0f;
                        if(isnan(percentage))
                            percentage = 0.0f;
                        clientp->Invoke(percentage);
                        return 0;
                    }
                );
            }

            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_WRITEDATA, l_ScopedCURL.Data);
            curl_easy_setopt(l_ScopedCURL.Instance, CURLOPT_WRITEFUNCTION,
                +[](void *contents, std::size_t size, std::size_t nmemb, std::vector<uint8_t>* clientp) -> size_t
            {
                size_t l_SizeToWrite = size * nmemb;

                try
                {
                    auto l_WritePos = clientp->size();
                    clientp->resize(l_WritePos + l_SizeToWrite);
                    memcpy(clientp->data() + l_WritePos, contents, l_SizeToWrite);
                }
                catch(std::bad_alloc &e)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.Network][WebClientCore.DoRequest] Failed to read from response stream, allocation error");
                    return 0;
                }

                return l_SizeToWrite;
            });

            if (token.get_IsCancellationRequested())
                break;

            auto l_CURLResult = curl_easy_perform(l_ScopedCURL.Instance);

            l_Reply = std::make_shared<WebResponse>(static_cast<long>(l_CURLResult), l_ScopedCURL.Instance, l_ScopedCURL.Data);
            if (!l_Reply->IsSuccessStatusCode() && l_Reply->StatusCode() == (HttpStatusCode)429)
            {
              /*  var l_Limits = RateLimitInfo.Get(l_Request);
                if (l_Limits != nullptr)
                {
                    int l_TotalMilliseconds = (int)(l_Limits.Reset - DateTime.Now).TotalMilliseconds;
                    if (l_TotalMilliseconds > 0)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Network][WebClientCore." + debugName + u"] Request {SafeURL(url)} was rate limited, retrying in {l_TotalMilliseconds}ms...");

                        co_yield WaitForSecondsRealtime::New_ctor(RetryInterval)->i_IEnumerator();
                        continue;
                    }
                }*/
            }

            if (!l_Reply->IsSuccessStatusCode())
            {
                auto l_LogPrefix = u"[CP_SDK.Network][WebClientCore." + debugName + u"] Request " + self->SafeURL(url) + u" failed with code ";
                l_LogPrefix += StringW(std::to_string(l_Reply->StatusCode().value__));
                l_LogPrefix += u":\"" + l_Reply->ReasonPhrase() + "\", ";

                if (!l_Reply->ShouldRetry() || dontRetry)
                {
                    ChatPlexSDK::Logger()->Error(l_LogPrefix + u" not retrying");
                    break;
                }

                ChatPlexSDK::Logger()->Error(l_LogPrefix + u" next try in " + (std::u16string)StringW(std::to_string(self->RetryInterval)) + u" seconds...");

                std::this_thread::sleep_for(std::chrono::seconds(self->RetryInterval));
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