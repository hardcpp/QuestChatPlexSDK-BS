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

        protected:
            /// @brief Constructor
            IWebClient() = default;

        public:
            /// @brief Get header
            /// @param p_Name Header name
            virtual std::u16string GetHeader(std::u16string_view p_Name) = 0;
            /// @brief Set header
            /// @param p_Name  Header name
            /// @param p_Value Header value
            virtual void SetHeader(std::u16string_view p_Name, std::u16string_view p_Value) = 0;
            /// @brief Remove header
            /// @param p_Name Header name
            virtual void RemoveHeader(std::u16string_view p_Name) = 0;

        public:
            /// @brief Do Async GET query
            /// @param p_URL       Target URL
            /// @param p_Token     Cancellation token
            /// @param p_Callback  Callback
            /// @param p_DontRetry Should not retry
            /// @param p_Progress  Progress reporter
            virtual void GetAsync(std::u16string_view p_URL, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false, _v::CActionRef<float> p_Progress = nullptr) = 0;
            /// @brief Do Async GET query
            /// @param p_URL       Target URL
            /// @param p_Token     Cancellation token
            /// @param p_Callback  Callback
            /// @param p_DontRetry Should not retry
            /// @param p_Progress  Progress reporter
            virtual void DownloadAsync(std::u16string_view p_URL, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false, _v::CActionRef<float> p_Progress = nullptr) = 0;
            /// @brief Do Async POST query
            /// @param p_URL         Target URL
            /// @param p_Content     Optional content to post
            /// @param p_Token       Cancellation token
            /// @param p_Callback    Callback
            /// @param p_DontRetry   Should not retry
            virtual void PostAsync(std::u16string_view p_URL, const WebContent::Ptr& p_Content, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false) = 0;
            /// @brief Do Async PATCH query
            /// @param p_URL         Target URL
            /// @param p_Content     Optional content to post
            /// @param p_Token       Cancellation token
            /// @param p_Callback    Callback
            /// @param p_DontRetry   Should not retry
            virtual void PatchAsync(std::u16string_view p_URL, const WebContent::Ptr& p_Content, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false) = 0;
            /// @brief Do Async DELETE query
            /// @param p_URL       Target URL
            /// @param p_Token     Cancellation token
            /// @param p_Callback  Callback
            /// @param p_DontRetry Should not retry
            virtual void DeleteAsync(std::u16string_view p_URL, _u::CancellationToken p_Token, _v::CActionRef<WebResponse::Ptr> p_Callback, bool p_DontRetry = false) = 0;

    };

}   ///< namespace CP_SDK::Network