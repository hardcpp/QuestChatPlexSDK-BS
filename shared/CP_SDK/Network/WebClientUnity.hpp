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
            /// @param p_BaseAddress       Base address
            /// @param p_TimeOut           Requests timeout
            /// @param p_ForceCacheDiscard Should force server cache discard
            WebClientUnity(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_BaseAddress, _u::TimeSpan p_TimeOut, bool p_ForceCacheDiscard);

            /// @brief Constructor
            /// @param p_BaseAddress       Base address
            /// @param p_TimeOut           Requests timeout
            /// @param p_ForceCacheDiscard Should force server cache discard
            static Ptr Make(std::u16string_view p_BaseAddress, _u::TimeSpan p_TimeOut, bool p_ForceCacheDiscard = false);

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
            /// @brief Do Async GET query
            /// @param p_URL       Target URL
            /// @param p_Token     Cancellation token
            /// @param p_Callback  Callback
            /// @param p_DontRetry Should not retry
            /// @param p_Progress  Progress reporter
            virtual void GetAsync(std::u16string_view p_URL, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false, _v::CActionRef<float> p_Progress = nullptr) override final;
            /// @brief Do Async GET query
            /// @param p_URL       Target URL
            /// @param p_Token     Cancellation token
            /// @param p_Callback  Callback
            /// @param p_DontRetry Should not retry
            /// @param p_Progress  Progress reporter
            virtual void DownloadAsync(std::u16string_view p_URL, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false, _v::CActionRef<float> p_Progress = nullptr) override final;
            /// @brief Do Async POST query
            /// @param p_URL         Target URL
            /// @param p_Content     Optional content to post
            /// @param p_Token       Cancellation token
            /// @param p_Callback    Callback
            /// @param p_DontRetry   Should not retry
            virtual void PostAsync(std::u16string_view p_URL, const WebContent::Ptr& p_Content, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false) override final;
            /// @brief Do Async PATCH query
            /// @param p_URL         Target URL
            /// @param p_Content     Optional content to post
            /// @param p_Token       Cancellation token
            /// @param p_Callback    Callback
            /// @param p_DontRetry   Should not retry
            virtual void PatchAsync(std::u16string_view p_URL, const WebContent::Ptr& p_Content, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false) override final;
            /// @brief Do Async DELETE query
            /// @param p_URL       Target URL
            /// @param p_Token     Cancellation token
            /// @param p_Callback  Callback
            /// @param p_DontRetry Should not retry
            virtual void DeleteAsync(std::u16string_view p_URL, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false) override final;

        private:
            /// @brief Get URL
            /// @param p_URL Request URL
            std::u16string GetURL(std::u16string_view p_URL);
            /// @brief Safe URL parsing
            /// @param p_URL Source URL
            std::u16string SafeURL(std::u16string_view p_URL);

        private:
            /// @brief Prepare request
            /// @param p_Request    Request to prepare
            /// @param p_IsDownload Is a download request?
            void PrepareRequest(_u::UnityWebRequest* p_Request, bool p_IsDownload);
            /// @brief Do request
            /// @param p_DebugName   Method name for logs
            /// @param p_HttpMethod  Http method
            /// @param p_URL         Target URL
            /// @param p_Content     Optional content to post
            /// @param p_Token       Cancellation token
            /// @param p_Callback    Callback
            /// @param p_DontRetry   Should not retry
            /// @param p_Progress    Progress reporter
            static custom_types::Helpers::Coroutine Coroutine_DoRequest(Ptr                             p_Self,
                                                                        std::u16string                  p_DebugName,
                                                                        std::u16string                  p_HttpMethod,
                                                                        std::u16string                  p_URL,
                                                                        WebContent::Ptr                 p_Content,
                                                                        _u::CancellationToken           p_Token,
                                                                        _v::Action<WebResponse::Ptr>    p_Callback,
                                                                        bool                            p_DontRetry,
                                                                        _v::Action<float>               p_Progress);

    };

}   ///< namespace CP_SDK::Network