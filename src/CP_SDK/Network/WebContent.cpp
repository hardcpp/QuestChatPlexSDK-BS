#include "CP_SDK/Network/WebContent.hpp"

#include <System/Text/Encoding.hpp>
#include <System/Text/UTF8Encoding_UTF8Decoder.hpp>
#include <System/Text/UTF8Encoding_UTF8Encoder.hpp>

using namespace System::Text;

namespace CP_SDK::Network {

    /// @brief Constructor
    /// @param p_Bytes  Bytes
    /// @param p_Type   Content
    WebContent::WebContent(PRIVATE_TAG_ARG(), ::Array<uint8_t>* p_Bytes, std::u16string_view p_Type)
    {
        Bytes   = p_Bytes;
        Type    = p_Type;
    }
    /// @brief Destructor
    WebContent::~WebContent()
    {
        Bytes = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor from Json
    /// @param p_Content Json content
    WebContent::Ptr WebContent::FromJson(std::u16string_view p_Content)
    {
        auto l_Array = Encoding::get_UTF8()->GetBytes(p_Content).operator Array<uint8_t> *();
        return std::make_shared<WebContent>(PRIVATE_TAG_VAL(), l_Array, u"application/json; charset=utf-8");
    }

}   ///< namespace CP_SDK::Network