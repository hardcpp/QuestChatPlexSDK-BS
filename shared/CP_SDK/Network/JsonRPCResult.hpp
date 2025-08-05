#pragma once

#include "../Utils/Json.hpp"
#include "WebResponse.hpp"

namespace CP_SDK::Network {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief JsonRPCResult
    class CP_SDK_EXPORT JsonRPCResult
    {
        CP_SDK_NO_COPYMOVE_CTORS(JsonRPCResult);
        CP_SDK_PRIV_TAG();

        public:
            WebResponse::Ptr                       RawResponse;
            std::shared_ptr<_v::Json::U16Document> Result;
            std::shared_ptr<_v::Json::U16Document> Error;

        public:
            using Ptr = std::shared_ptr<JsonRPCResult>;

            /// @brief Constructor
            /// @param rawResponse Raw web response
            /// @param result Result json
            /// @param error Json content
            JsonRPCResult(
                    CP_SDK_PRIV_TAG_ARG(),
                    WebResponse::Ptr&                      rawResponse,
                    std::shared_ptr<_v::Json::U16Document> result,
                    std::shared_ptr<_v::Json::U16Document> error
                );
            /// @brief Destructor
            ~JsonRPCResult();

        public:
            /// @brief Create
            /// @param rawResponse Raw web response
            /// @param result Result json
            /// @param error Json content
            static Ptr Create(
                    WebResponse::Ptr&                      rawResponse,
                    std::shared_ptr<_v::Json::U16Document> result,
                    std::shared_ptr<_v::Json::U16Document> error
                );

    };

}   ///< namespace CP_SDK::Network