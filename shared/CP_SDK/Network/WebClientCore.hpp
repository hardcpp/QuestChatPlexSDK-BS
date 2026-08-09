#pragma once

#include "IWebClient.hpp"

#include <System/TimeSpan.hpp>

#include <atomic>
#include <map>
#include <mutex>

namespace CP_SDK::Network {

    namespace _u
    {
        using namespace System;
        using namespace System::Threading;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief WebClientCore
    class CP_SDK_EXPORT WebClientCore : public IWebClient, public std::enable_shared_from_this<WebClientCore>
    {
        CP_SDK_NO_COPYMOVE_CTORS(WebClientCore);
        CP_SDK_PRIV_TAG();

        public:
            using Ptr = std::shared_ptr<WebClientCore>;

        private:
            static Ptr m_GlobalClient;

        public:
            /// @brief Global client instance
            static WebClientCore* GlobalClient();

        private:
            std::u16string                              m_BaseAddress;
            int                                         m_TimeOut;
            std::map<std::u16string, std::u16string>    m_Headers;
            std::mutex                                  m_HeadersLock;

        public:
            /// @brief Maximum retry attempt
            int MaxRetry;
            /// @brief Delay between each retry
            int RetryInterval;

        public:
            /// @brief Constructor
            /// @param baseAddress       Base address
            /// @param timeOut           Requests timeout
            /// @param keepAlive         Should try to keep the connection alive
            /// @param forceCacheDiscard Should force server cache discard
            WebClientCore(CP_SDK_PRIV_TAG_ARG(), std::u16string_view baseAddress, _u::TimeSpan timeOut, bool keepAlive, bool forceCacheDiscard);

            /// @brief Constructor
            /// @param baseAddress       Base address
            /// @param timeOut           Requests timeout
            /// @param keepAlive         Should try to keep the connection alive
            /// @param forceCacheDiscard Should force server cache discard
            static Ptr Make(std::u16string_view baseAddress, _u::TimeSpan timeOut, bool keepAlive = true, bool forceCacheDiscard = false);

        public:
            /// @brief Get header
            /// @param p_Name Header name
            virtual std::u16string GetHeader(std::u16string_view p_Name) override final;
            /// @brief Set header
            /// @param p_Name  Header name
            /// @param p_Value Header value
            virtual void SetHeader(std::u16string_view p_Name, std::u16string_view p_Value) override final;
            /// @brief Remove header
            /// @param p_Name Header name
            virtual void RemoveHeader(std::u16string_view p_Name) override final;

        public:
            /// @brief Do GET query
            /// @param url       Target URL
            /// @param dontRetry Should not retry
            /// @param progress  Progress reporter
            WebResponse::Ptr Get(std::u16string_view url, bool dontRetry = false, _v::CActionRef<float> progress = nullptr);
            /// @brief Do GET query
            /// @param url       Target URL
            /// @param dontRetry Should not retry
            /// @param progress  Progress reporter
            WebResponse::Ptr Download(std::u16string_view url, bool dontRetry = false, _v::CActionRef<float> progress = nullptr);
            /// @brief Do POST query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param dontRetry   Should not retry
            WebResponse::Ptr Post(std::u16string_view url, const WebContent::Ptr& content, bool dontRetry = false);
            /// @brief Do PATCH query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param dontRetry   Should not retry
            WebResponse::Ptr Patch(std::u16string_view url, const WebContent::Ptr& content, bool dontRetry = false);
            /// @brief Do PUT query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param dontRetry   Should not retry
            WebResponse::Ptr Put(std::u16string_view url, const WebContent::Ptr& content, bool dontRetry = false);
            /// @brief Do DELETE query
            /// @param url       Target URL
            /// @param dontRetry Should not retry
            WebResponse::Ptr Delete(std::u16string_view url, bool dontRetry = false);

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
            /// @brief Do request
            /// @param debugName   Method name for logs
            /// @param httpMethod  Http method
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            /// @param progress    Progress reporter
            static void DoRequest(
                    Ptr                             self,
                    std::u16string                  debugName,
                    std::u16string                  httpMethod,
                    std::u16string                  url,
                    WebContent::Ptr                 content,
                    _u::CancellationToken           token,
                    _v::Action<WebResponse::Ptr>    callback,
                    bool                            dontRetry,
                    _v::Action<float>               progress,
                    bool                            dispatchCallback
            );

    };

}   ///< namespace CP_SDK::Network
