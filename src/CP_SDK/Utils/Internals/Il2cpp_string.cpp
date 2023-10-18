#include "CP_SDK/Utils/Internals/Il2cpp_string.hpp"

#include <codecvt>
#include <locale>

namespace CP_SDK::Utils {

    std::u16string StrToU16Str(std::string_view p_Str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> l_Converted;
        return l_Converted.from_bytes(p_Str.begin(), p_Str.end());
    }
    std::string U16StrToStr(std::u16string_view p_Str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> l_Converted;
        return l_Converted.to_bytes(p_Str.begin(), p_Str.end());
    }

    bool U16EqualsToCaseInsensitive(std::u16string_view p_Left, std::u16string_view p_Right)
    {
        if (p_Left.length() != p_Right.length())
            return false;

        for (auto l_I = 0; l_I < p_Left.length(); ++l_I)
        {
            if (std::towlower(p_Left[l_I]) != std::towlower(p_Right[l_I]))
                return false;
        }

        return true;
    }

    std::u16string U16UrlEncode(std::u16string_view p_Input)
    {
        static const char16_t s_IntToHex[16] = { u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F' };

        std::u16string l_Result;
        l_Result.reserve(p_Input.size());
        for (char16_t l_Char : p_Input)
        {
            if ((l_Char >= u'0' && l_Char <= u'9') ||
                (l_Char >= u'A' && l_Char <= u'Z') ||
                (l_Char >= u'a' && l_Char <= u'z') ||
                 l_Char == u'-' || l_Char == u'_'  || l_Char == u'.' || l_Char == u'~')
            {
                l_Result += l_Char;
            }
            else
            {
                l_Result += u'%';
                l_Result += s_IntToHex[(l_Char >> 4) & 0x0F];
                l_Result += s_IntToHex[(l_Char >> 0) & 0x0F];
            }
        }
        return l_Result;
    }

}   ///< namespace CP_SDK::Utils