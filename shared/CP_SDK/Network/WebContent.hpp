#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <string>
#include <stdint.h>

namespace CP_SDK::Network {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief WebContent
    class CP_SDK_EXPORT WebContent
    {
        CP_SDK_NO_COPYMOVE_CTORS(WebContent);
        CP_SDK_PRIV_TAG();

        public:
            /// @brief Content
            _v::MonoPtr<::Array<uint8_t>>   Bytes;
            /// @brief Content type
            std::u16string                  Type;

        public:
            using Ptr = std::shared_ptr<WebContent>;

            /// @brief Constructor
            /// @param p_Bytes  Bytes
            /// @param p_Type   Content
            WebContent(CP_SDK_PRIV_TAG_ARG(), ::Array<uint8_t>* p_Bytes, std::u16string_view p_Type);
            /// @brief Destructor
            ~WebContent();

        public:
            /// @brief Constructor from Json
            /// @param p_Content Json content
            static Ptr FromJson(std::u16string_view p_Content);

    };

}   ///< namespace CP_SDK::Network