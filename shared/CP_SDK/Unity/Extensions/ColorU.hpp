#pragma once

#include <UnityEngine/Color.hpp>
#include <UnityEngine/Color32.hpp>

namespace CP_SDK::Unity::Extensions {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Unity Color tools
    class CP_SDK_EXPORT_VISIBILITY ColorU
    {
        public:
            /// @brief Get color with alpha
            /// @param p_Color Source color
            /// @param p_Alpha Target alpha
            inline static _u::Color WithAlpha(_u::Color p_Color, float p_Alpha)
            {
                p_Color.a = p_Alpha;
                return p_Color;
            }
            /// @brief Get color with alpha
            /// @param p_Color Source color
            /// @param p_Alpha Target alpha
            inline static _u::Color WithAlpha(const char* p_Color, float p_Alpha)
            {
                return WithAlpha(ToUnityColor(p_Color), p_Alpha);
            }

        public:
            /// @brief Convert a Color32 to Color
            /// @param p_Src Input Color32
            /// @return Converted color
            inline static _u::Color Convert(_u::Color32 p_Src)
            {
                 return _u::Color(
                    static_cast<float>(p_Src.r) / 255.0f,
                    static_cast<float>(p_Src.g) / 255.0f,
                    static_cast<float>(p_Src.b) / 255.0f,
                    static_cast<float>(p_Src.a) / 255.0f
                );
            }
            /// @brief Convert a Color to Color32
            /// @param p_Src Input Color32
            /// @return Converted color
            inline static _u::Color32 Convert(_u::Color p_Src)
            {
                return _u::Color32(
                    0,
                    (uint8_t)(p_Src.r * 255.0f),
                    (uint8_t)(p_Src.g * 255.0f),
                    (uint8_t)(p_Src.b * 255.0f),
                    (uint8_t)(p_Src.a * 255.0f)
                );
            }

        public:
            /// @brief String to unity color
            /// @param p_Src   Input string
            /// @param p_Color Output color
            inline static bool TryToUnityColor(const char* p_Src, _u::Color* p_Color)
            {
                *p_Color = _u::Color(0.0f, 0.0f, 0.0f, 1.0f);
                if (p_Src == nullptr)
                    return false;

                auto l_Length = strlen(p_Src);
                if (l_Length == 0)
                    return false;

                auto l_Offset = p_Src[0] == '#' ? 1 : 0;
                auto l_R = (ConvertSingleByte(p_Src, l_Length, l_Offset + 0) << 4) | ConvertSingleByte(p_Src, l_Length, l_Offset + 1);
                auto l_G = (ConvertSingleByte(p_Src, l_Length, l_Offset + 2) << 4) | ConvertSingleByte(p_Src, l_Length, l_Offset + 3);
                auto l_B = (ConvertSingleByte(p_Src, l_Length, l_Offset + 4) << 4) | ConvertSingleByte(p_Src, l_Length, l_Offset + 5);
                auto l_A = 255;

                if ((l_Length - l_Offset) > 6)
                    l_A = (ConvertSingleByte(p_Src, l_Length, l_Offset + 6) << 4) | ConvertSingleByte(p_Src, l_Length, l_Offset + 7);

                *p_Color = Convert(_u::Color32(0, (uint8_t)l_R, (uint8_t)l_G, (uint8_t)l_B, (uint8_t)l_A));

                return true;
            }
            /// @brief String to unity color
            /// @param p_Src Input string
            inline static _u::Color ToUnityColor(const char* p_Src)
            {
                _u::Color l_Color;
                TryToUnityColor(p_Src, &l_Color);
                return l_Color;
            }

        public:
            /// @brief To hexadecimal RGB with # prefix
            /// @param p_Color Input color
            inline static std::u16string ToHexRGB(_u::Color p_Color)
            {
                static const char16_t s_IntToHex[16] = { u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F' };

                auto l_Color32 = Convert(p_Color);
                return std::u16string(
                {
                    u'#',
                    s_IntToHex[(l_Color32.r >> 4) & 0xF],
                    s_IntToHex[(l_Color32.r >> 0) & 0xF],
                    s_IntToHex[(l_Color32.g >> 4) & 0xF],
                    s_IntToHex[(l_Color32.g >> 0) & 0xF],
                    s_IntToHex[(l_Color32.b >> 4) & 0xF],
                    s_IntToHex[(l_Color32.b >> 0) & 0xF]
                });
            }
            /// @brief To hexadecimal RGBA with # prefix
            /// @param p_Color Input color
            inline static std::u16string ToHexRGBA(_u::Color p_Color)
            {
                static const char16_t s_IntToHex[16] = { u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F' };

                auto l_Color32 = Convert(p_Color);
                return std::u16string(
                {
                    u'#',
                    s_IntToHex[(l_Color32.r >> 4) & 0xF],
                    s_IntToHex[(l_Color32.r >> 0) & 0xF],
                    s_IntToHex[(l_Color32.g >> 4) & 0xF],
                    s_IntToHex[(l_Color32.g >> 0) & 0xF],
                    s_IntToHex[(l_Color32.b >> 4) & 0xF],
                    s_IntToHex[(l_Color32.b >> 0) & 0xF],
                    s_IntToHex[(l_Color32.a >> 4) & 0xF],
                    s_IntToHex[(l_Color32.a >> 0) & 0xF]
                });
            }

        private:
            /// @brief Convert single hex byte to integer
            /// @param p_Src  Source string
            /// @param p_Size Source size
            /// @param p_Pos  Position to convert
            /// @return Converted integer
            inline static int ConvertSingleByte(const char* p_Src, int p_Size, int p_Pos)
            {
                if (p_Pos >= p_Size)
                    return 0;

                auto l_Char = p_Src[p_Pos];
                if (l_Char >= '0' && l_Char <= '9')
                    return (l_Char - '0') + 0x00;
                else if (l_Char >= 'a' && l_Char <= 'f')
                    return (l_Char - 'a') + 0x0A;
                else if (l_Char >= 'A' && l_Char <= 'F')
                    return (l_Char - 'A') + 0x0A;

                return 0;
            }

    };

}   ///< namespace CP_SDK::Unity::Extensions