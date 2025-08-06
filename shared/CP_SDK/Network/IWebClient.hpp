#pragma once

#include "WebContent.hpp"
#include "WebResponse.hpp"
#include "../Utils/Delegate.hpp"

#include <System/Threading/CancellationToken.hpp>

#include <string>

namespace CP_SDK::Network {

    namespace _u
    {
        using namespace System::Threading;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Web Client interface
    class IWebClient
    {
        CP_SDK_NO_COPYMOVE_CTORS(IWebClient);

        public:
            using IPtr = std::shared_ptr<IWebClient>;

        protected:
            /// @brief Constructor
            IWebClient() = default;

        public:
            /// @brief Get header
            /// @param name Header name
            virtual std::u16string GetHeader(std::u16string_view name) = 0;
            /// @brief Set header
            /// @param name  Header name
            /// @param value Header value
            virtual void SetHeader(std::u16string_view name, std::u16string_view value) = 0;
            /// @brief Remove header
            /// @param name Header name
            virtual void RemoveHeader(std::u16string_view name) = 0;

        public:
            /// @brief Do Async GET query
            /// @param url       Target URL
            /// @param token     Cancellation token
            /// @param callback  Callback
            /// @param dontRetry Should not retry
            /// @param progress  Progress reporter
            virtual void GetAsync(std::u16string_view url, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false, _v::CActionRef<float> progress = nullptr) = 0;
            /// @brief Do Async GET query
            /// @param url       Target URL
            /// @param token     Cancellation token
            /// @param callback  Callback
            /// @param dontRetry Should not retry
            /// @param progress  Progress reporter
            virtual void DownloadAsync(std::u16string_view url, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false, _v::CActionRef<float> progress = nullptr) = 0;
            /// @brief Do Async POST query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            virtual void PostAsync(std::u16string_view url, const WebContent::Ptr& content, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) = 0;
            /// @brief Do Async PATCH query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            virtual void PatchAsync(std::u16string_view url, const WebContent::Ptr& content, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) = 0;
            /// @brief Do Async PUT query
            /// @param url         Target URL
            /// @param content     Optional content to post
            /// @param token       Cancellation token
            /// @param callback    Callback
            /// @param dontRetry   Should not retry
            virtual void PutAsync(std::u16string_view url, const WebContent::Ptr& content, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) = 0;
            /// @brief Do Async DELETE query
            /// @param url       Target URL
            /// @param token     Cancellation token
            /// @param callback  Callback
            /// @param dontRetry Should not retry
            virtual void DeleteAsync(std::u16string_view url, _u::CancellationToken token, _v::CActionRef<WebResponse::Ptr> callback, bool dontRetry = false) = 0;

    };

}   ///< namespace CP_SDK::Network