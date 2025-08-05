#pragma once

#include "IWebClient.hpp"

#include <custom-types/shared/coroutine.hpp>
#include <System/TimeSpan.hpp>

#include <map>
#include <mutex>

namespace CP_SDK::Network {

    namespace _u
    {
        using namespace System;
        using namespace System::Threading;
        using namespace UnityEngine::Networking;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief WebClientUnity using unity web requests
    class CP_SDK_EXPORT WebClientUnity : public IWebClient, public std::enable_shared_from_this<WebClientUnity>
    {
        CP_SDK_NO_COPYMOVE_CTORS(WebClientUnity);
        CP_SDK_PRIV_TAG();

        public:
            using Ptr = std::shared_ptr<WebClientUnity>;

        private:
            static Ptr m_GlobalClient;

        public:
            /// @brief Global client instance
            static WebClientUnity* GlobalClient();

        private:
            std::u16string                              m_BaseAddress;
            int                                         m_Timeout;
            std::map<std::u16string, std::u16string>    m_Headers;
            std::mutex                                  m_HeadersLock;

        public:
            /// @brief Timeout seconds
            int DownloadTimeout;
            /// @brief Maximum retry attempt
            int MaxRetry;
            /// @brief Delay between each retry
            int RetryInterval;

        public:
            /// @brief Constructor
            /// @param baseAddress       Base address
            /// @param timeOut           Requests timeout
            /// @param forceCacheDiscard Should force server cache discard
            WebClientUnity(CP_SDK_PRIV_TAG_ARG(), std::u16string_view baseAddress, _u::TimeSpan timeOut, bool forceCacheDiscard);

            /// @brief Constructor
            /// @param baseAddress       Base address
            /// @param timeOut           Requests timeout
            /// @param forceCacheDiscard Should force server cache discard
            static Ptr Make(std::u16string_view baseAddress, _u::TimeSpan timeOut, bool forceCacheDiscard = false);

        public:
            /// @brief Get header
            /// @param name Header name
            virtual std::u16string GetHeader(std::u16string_view name) override final;
            /// @brief Set header
            /// @param name  Header name
            /// @param value Header value
            virtual void SetHeader(std::u16string_view name, std::u16string_view value) override final;
            /// @brief Remove header
            /// @param name Header name
            virtual void RemoveHeader(std::u16string_view name) override final;

        public:
            /// @brief Do Async GET query
            /// @param url       Target URL
            /// @param token     Cancellation token
            /// @param callback  Callback
            /// @param dontRetry Should not retry
            /// @param progress  Progress reporter
            virtual void GetAsync(std::u16string_view url, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false, _v::CActionRef<float> progress = nullptr) override final;
            /// @brief Do Async GET query
            /// @param url       Target URL
            /// @param token     Cancellation token
            /// @param callback  Callback
            /// @param dontRetry Should not retry
            /// @param progress  Progress reporter
            virtual void DownloadAsync(std::u16string_view url, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false, _v::CActionRef<float> progress = nullptr) override final;
            /// @brief Do Async POST query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            virtual void PostAsync(std::u16string_view url, const WebContent::Ptr& content, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) override final;
            /// @brief Do Async PATCH query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            virtual void PatchAsync(std::u16string_view url, const WebContent::Ptr& content, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) override final;
            /// @brief Do Async PUT query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            virtual void PutAsync(std::u16string_view url, const WebContent::Ptr& content, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) override final;
            /// @brief Do Async DELETE query
            /// @param url       Target URL
            /// @param token     Cancellation token
            /// @param callback  Callback
            /// @param dontRetry Should not retry
            virtual void DeleteAsync(std::u16string_view url, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) override final;

        private:
            /// @brief Get URL
            /// @param url Request URL
            std::u16string GetURL(std::u16string_view url);
            /// @brief Safe URL parsing
            /// @param url Source URL
            std::u16string SafeURL(std::u16string_view url);

        private:
            /// @brief Prepare request
            /// @param request    Request to prepare
            /// @param isDownload Is a download request?
            void PrepareRequest(_u::UnityWebRequest* request, bool isDownload);
            /// @brief Do request
            /// @param debugName   Method name for logs
            /// @param httpMethod  Http method
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            /// @param progress    Progress reporter
            static custom_types::Helpers::Coroutine Coroutine_DoRequest(Ptr                             self,
                                                                        std::u16string                  debugName,
                                                                        std::u16string                  httpMethod,
                                                                        std::u16string                  url,
                                                                        WebContent::Ptr                 content,
                                                                        _u::CancellationToken           token,
                                                                        _v::Action<WebResponse::Ptr>    callback,
                                                                        bool                            dontRetry,
                                                                        _v::Action<float>               progress);

    };

}   ///< namespace CP_SDK::Network