#include "CP_SDK/Network/JsonRPCResult.hpp"

#include <System/Text/Encoding.hpp>
#include <System/Text/UTF8Encoding.hpp>

using namespace System::Text;

namespace CP_SDK::Network {

    /// @brief Constructor
    /// @param rawResponse Raw web response
    /// @param result Result json
    /// @param error Json content
    JsonRPCResult::JsonRPCResult(
            CP_SDK_PRIV_TAG_ARG(),
            WebResponse::Ptr&                      rawResponse,
            std::shared_ptr<_v::Json::U16Document> result,
            std::shared_ptr<_v::Json::U16Document> error
        )
    {
        RawResponse = rawResponse;
        Result      = result;
        Error       = error;
    }
    /// @brief Destructor
    JsonRPCResult::~JsonRPCResult()
    {
        RawResponse = nullptr;
        Result      = nullptr;
        Error       = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create
    /// @param rawResponse Raw web response
    /// @param result Result json
    /// @param error Json content
    JsonRPCResult::Ptr JsonRPCResult::Create(
            WebResponse::Ptr&                      rawResponse,
            std::shared_ptr<_v::Json::U16Document> result,
            std::shared_ptr<_v::Json::U16Document> error
        )
    {
        return std::make_shared<JsonRPCResult>(CP_SDK_PRIV_TAG_VAL(), rawResponse, result, error);
    }

}   ///< namespace CP_SDK::Network