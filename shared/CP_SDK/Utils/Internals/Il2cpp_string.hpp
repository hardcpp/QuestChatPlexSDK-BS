#include <string>
#include <string_view>

namespace CP_SDK::Utils {

    extern std::u16string StrToU16Str(std::string_view p_Str);
    extern std::string U16StrToStr(std::u16string_view p_Str);

    extern bool U16EqualsToCaseInsensitive(std::u16string_view p_Left, std::u16string_view p_Right);

    extern std::u16string U16UrlEncode(std::u16string_view p_Input);

}   ///< namespace CP_SDK::Utils

static std::string operator+(std::string_view p_Left, std::u16string_view p_Right)
{
    return std::string(p_Left) + CP_SDK::Utils::U16StrToStr(p_Right);
}
/*static std::u16string operator+(const char* p_Left, std::string_view p_Right)
{
    return std::string(p_Left) + std::string(p_Right);
}*/
static std::string operator+(std::string_view p_Left, const char* p_Right)
{
    return std::string(p_Left) + std::string(p_Right);
}
/*static std::string operator+(std::string p_Left, std::string_view p_Right)
{
    return p_Left + std::string(p_Right);
}*/

static std::u16string operator+(std::u16string_view p_Left, std::string_view p_Right)
{
    return std::u16string(p_Left) + CP_SDK::Utils::StrToU16Str(p_Right);
}
static std::u16string operator+(const char16_t* p_Left, std::u16string_view p_Right)
{
    return std::u16string(p_Left) + std::u16string(p_Right);
}
static std::u16string operator+(std::u16string p_Left, std::u16string_view p_Right)
{
    return p_Left + std::u16string(p_Right);
}