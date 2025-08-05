#pragma once

#include "CP_SDK/Utils/Json.hpp"
#include "JsonRPCResult.hpp"
#include "WebClientCore.hpp"
#include <string_view>

namespace CP_SDK::Network {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief JsonRPCClient
    class CP_SDK_EXPORT JsonRPCClient : public std::enable_shared_from_this<JsonRPCClient>
    {
        CP_SDK_NO_COPYMOVE_CTORS(JsonRPCClient);
        CP_SDK_PRIV_TAG();

        private:
            WebClientCore::Ptr m_WebClient;

        public:
            using Ptr = std::shared_ptr<JsonRPCClient>;

            /// @brief Constructor
            /// @param webClient Web client instance
            JsonRPCClient(CP_SDK_PRIV_TAG_ARG(), WebClientCore::Ptr& webClient);
            /// @brief Destructor
            ~JsonRPCClient();

        public:
            /// @brief Create
            /// @param webClient Raw web response
            static Ptr Create(WebClientCore::Ptr& webClient);

        public:
            /// @brief Do a RPC request
            /// @param method Target method
            /// @param parameters Request parameters
            /// @param dontRetry Should not retry?
            JsonRPCResult::Ptr Request(std::u16string_view method, std::shared_ptr<_v::Json::U16Value> parameters, bool dontRetry = false);

        public:
            /// @brief Do a RPC request
            /// @param method Target method
            /// @param parameters Request parameters
            /// @param token Cancellation token
            /// @param callback Callback
            /// @param dontRetry Should not retry?
            void RequestAsync(
                    std::u16string_view                 method,
                    std::shared_ptr<_v::Json::U16Value> parameters,
                    _u::CancellationToken               token,
                    _v::CActionRef<JsonRPCResult::Ptr>  callback,
                    bool                                dontRetry = false
                );

        private:
            /// @brief Do a RPC request
            /// @param method Target method
            /// @param content Request content
            /// @param dontRetry Should not retry?
            JsonRPCResult::Ptr DoRequest(
                    std::u16string                          method,
                    std::shared_ptr<_v::Json::U16Document>& content,
                    bool                                    dontRetry
                );
            /// @brief Do a RPC request
            /// @param method Target method
            /// @param content Request content
            /// @param token Cancellation token
            /// @param callback Callback
            /// @param dontRetry Should not retry?
            void DoRequestAsync(
                    std::u16string                          method,
                    std::shared_ptr<_v::Json::U16Document>& content,
                    _u::CancellationToken                   token,
                    _v::CActionRef<JsonRPCResult::Ptr>      callback,
                    bool                                    dontRetry
                );
            /// @brief Do a RPC request
            /// @param method Target method
            /// @param rawResponse Web response
            JsonRPCResult::Ptr HandleWebResponse(std::u16string method, WebResponse::Ptr& rawResponse);

    };

}   ///< namespace CP_SDK::Network