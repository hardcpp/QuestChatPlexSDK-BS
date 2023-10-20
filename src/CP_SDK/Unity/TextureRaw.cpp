#include "CP_SDK/Unity/TextureRaw.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/Vector2.hpp>
#include <System/Math.hpp>

using namespace System;
using namespace UnityEngine;

namespace CP_SDK::Unity {

    /// @brief Load an image into raw pixels
    /// @param p_Bytes  Input bytes
    /// @param p_Width  Ouput width
    /// @param p_Height Output height
    /// @param p_Pixels Output pixels
    bool TextureRaw::Load(::Array<uint8_t>* p_Bytes, int& p_Width, int& p_Height, PixelArray* p_Pixels)
    {
        if (!p_Pixels)
            return false;

        *p_Pixels = nullptr;

        p_Width     = 0;
        p_Height    = 0;

        if (p_Bytes == nullptr && p_Bytes->Length() == 0)
            return false;

        int l_InputChannels;

        if (!stbi_info_from_memory(p_Bytes->values, p_Bytes->Length(), &p_Width, &p_Height, &l_InputChannels))
            return false;

        stbi_set_flip_vertically_on_load(1);

        auto l_STBIBuffer = stbi_load_from_memory(p_Bytes->values, p_Bytes->Length(), &p_Width, &p_Height, &l_InputChannels, 4);
        if (!l_STBIBuffer)
            return false;

        *p_Pixels = std::make_shared<std::vector<Color>>();

        auto& l_Pixels = *(*p_Pixels).get();
        l_Pixels.resize(p_Width * p_Height);

        for (auto l_I = 0; l_I < (p_Width * p_Height); ++l_I)
        {
            auto l_SrcPixel = &l_STBIBuffer[l_I * 4];
            l_Pixels[l_I] = Extensions::ColorU::Convert(Color32(l_SrcPixel[0], l_SrcPixel[1], l_SrcPixel[2], l_SrcPixel[3]));
        }

        stbi_image_free(l_STBIBuffer);

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Blur an image
    /// @param p_InWidth  Source width
    /// @param p_InHeight Source height
    /// @param p_InPixels Input image
    /// @param p_Radius   Blur radius
    void TextureRaw::FastGaussianBlur(int p_InWidth, int p_InHeight, PixelArray& p_InPixels, int p_Radius)
    {
        GaussianBlur4(p_InWidth, p_InHeight, p_InPixels, p_Radius);
    }
    /// @brief Multiply image A & B into A
    /// @param p_ImageA Target
    /// @param p_ImageB Additional image
    void TextureRaw::Multiply(PixelArray& p_ImageA, PixelArray& p_ImageB)
    {
        if (p_ImageA->size() != p_ImageB->size())
            throw std::runtime_error("[CP_SDK.Unity][TextureRaw.Multiply] Size differ! " + std::to_string(p_ImageA->size()) + " vs " + std::to_string(p_ImageB->size()));

        auto& l_ImageA = *(p_ImageA.get());
        auto& l_ImageB = *(p_ImageB.get());

        for (auto l_I = 0; l_I < p_ImageA->size(); ++l_I)
            l_ImageA[l_I] = l_ImageA[l_I] * l_ImageB[l_I];
    }
    /// @brief Resize an image and crop it
    /// @param p_InWidth      Source width
    /// @param p_InHeight     Source height
    /// @param p_InPixels     Input image
    /// @param p_TargetWidth  Target width
    /// @param p_TargetHeight Target height
    /// @param p_YOffsetRel   Height anchor
    TextureRaw::PixelArray TextureRaw::ResampleAndCrop(int p_InWidth, int p_InHeight, PixelArray& p_InPixels, int p_TargetWidth, int p_TargetHeight, float p_YOffsetRel)
    {
        float l_SourceAspect = (float)p_InWidth / p_InHeight;
        float l_TargetAspect = (float)p_TargetWidth / p_TargetHeight;

        int l_XOffset = 0;
        int l_YOffset = 0;

        float l_Factor;

        /// Crop width
        if (l_SourceAspect > l_TargetAspect)
        {
            l_Factor  = (float)p_TargetHeight / p_InHeight;
            l_XOffset = (int)((p_InWidth - p_InHeight * l_TargetAspect) * 0.5f);

        }
        /// Crop height
        else
        {
            l_Factor  = (float)p_TargetWidth / p_InWidth;
            l_YOffset = (int)((p_InHeight - p_InWidth / l_TargetAspect) * (1.0f - p_YOffsetRel));
        }

        auto  l_ResultPtr   = std::make_shared<std::vector<Color>>();
        auto& l_Result      = *(l_ResultPtr.get());
        auto& l_InPixels    = *(p_InPixels.get());

        l_Result.resize(p_TargetWidth * p_TargetHeight);

        for (int l_Y = 0; l_Y < p_TargetHeight; ++l_Y)
        {
            for (int l_X = 0; l_X < p_TargetWidth; ++l_X)
            {
                auto l_Pixel = Vector2(std::clamp<float>(l_XOffset + l_X / l_Factor, 0, p_InWidth - 1), std::clamp<float>(l_YOffset + l_Y / l_Factor, 0, p_InHeight - 1));

                /// Optimizations
                auto l_FX = Mathf::FloorToInt(l_Pixel.x);
                auto l_FY = Mathf::FloorToInt(l_Pixel.y);
                auto l_CX = Mathf::CeilToInt(l_Pixel.x);
                auto l_CY = Mathf::CeilToInt(l_Pixel.y);

                /// Bilinear filtering
                auto l_C11 = l_InPixels[l_FX + p_InWidth * l_FY];
                auto l_C12 = l_InPixels[l_FX + p_InWidth * l_CY];
                auto l_C21 = l_InPixels[l_CX + p_InWidth * l_FY];
                auto l_C22 = l_InPixels[l_CX + p_InWidth * l_CY];

                l_Result[l_X + l_Y * p_TargetWidth] = Color::Lerp(Color::Lerp(l_C11, l_C12, l_Pixel.y), Color::Lerp(l_C21, l_C22, l_Pixel.y), l_Pixel.x);
            }
        }

        return l_ResultPtr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::vector<int> TextureRaw::BoxesForGauss(int p_Radius, int p_Count)
    {
        auto l_WIdeal   = Math::Sqrt((12 * p_Radius * p_Radius / p_Count) + 1);
        auto l_WL       = (int)Math::Floor(l_WIdeal);
        if (l_WL % 2 == 0)
            l_WL--;

        auto l_WU        = l_WL + 2;
        auto l_MIdeal    = (double)(12 * p_Radius * p_Radius - p_Count * l_WL * l_WL - 4 * p_Count * l_WL - 3 * p_Count) / (-4 * l_WL - 4);
        auto l_M         = Math::Round(l_MIdeal);

        std::vector<int> l_Sizes;
        l_Sizes.resize(p_Count);

        for (auto l_I = 0; l_I < p_Count; ++l_I)
            l_Sizes[l_I] = l_I < l_M ? l_WL : l_WU;

        return l_Sizes;
    }
    void TextureRaw::GaussianBlur4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius)
    {
        auto l_Boxes = BoxesForGauss(p_Radius, 3);
        BoxBlur4(p_Width, p_Height, p_InPixels, (l_Boxes[0] - 1) / 2);
        BoxBlur4(p_Width, p_Height, p_InPixels, (l_Boxes[1] - 1) / 2);
        BoxBlur4(p_Width, p_Height, p_InPixels, (l_Boxes[2] - 1) / 2);
    }
    void TextureRaw::BoxBlur4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius)
    {
        BoxBlurH4(p_Width, p_Height, p_InPixels, p_Radius);
        BoxBlurT4(p_Width, p_Height, p_InPixels, p_Radius);
    }
    void TextureRaw::BoxBlurH4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius)
    {
        auto& l_InPixels    = *(p_InPixels.get());
        auto  l_Mult        = (float)1 / (p_Radius + p_Radius + 1);
        for (auto l_I = 0; l_I < p_Height; ++l_I)
        {
            auto l_TI  = l_I * p_Width;
            auto l_LI  = l_TI;
            auto l_RI  = l_TI + p_Radius;
            auto l_FV  = l_InPixels[l_TI];
            auto l_LV  = l_InPixels[l_TI + p_Width - 1];
            auto l_Val = (p_Radius + 1) * l_FV;

            for (auto l_J = 0; l_J < p_Radius; ++l_J)
                l_Val = l_Val + (l_InPixels[l_TI + l_J]);

            for (auto l_J = 0; l_J <= p_Radius; ++l_J)
            {
                l_Val = l_Val + (l_InPixels[l_RI++] - l_FV);
                l_InPixels[l_TI++] = l_Val * l_Mult;
            }

            for (auto l_J = p_Radius + 1; l_J < p_Width - p_Radius; ++l_J)
            {
                l_Val = l_Val + (l_InPixels[l_RI++] - l_InPixels[l_LI++]);
                l_InPixels[l_TI++] = l_Val * l_Mult;
            }

            for (auto l_J = p_Width - p_Radius; l_J < p_Width; ++l_J)
            {
                l_Val = l_Val + (l_LV - l_InPixels[l_LI++]);
                l_InPixels[l_TI++] = l_Val * l_Mult;
            }
        };
    }
    void TextureRaw::BoxBlurT4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius)
    {
        auto& l_InPixels    = *(p_InPixels.get());
        auto  l_Mult        = (float)1 / (p_Radius + p_Radius + 1);
        for (auto l_I = 0; l_I < p_Width; l_I++)
        {
            auto l_TI  = l_I;
            auto l_LI  = l_TI;
            auto l_RI  = l_TI + p_Radius * p_Width;
            auto l_FV  = l_InPixels[l_TI];
            auto l_LV  = l_InPixels[l_TI + p_Width * (p_Height - 1)];
            auto l_Val = (p_Radius + 1) * l_FV;

            for (auto l_J = 0; l_J < p_Radius; ++l_J)
                l_Val = l_Val + (l_InPixels[l_TI + l_J * p_Width]);

            for (auto l_J = 0; l_J <= p_Radius; ++l_J)
            {
                l_Val = l_Val + (l_InPixels[l_RI] - l_FV);
                l_InPixels[l_TI] = l_Val * l_Mult;
                l_RI += p_Width;
                l_TI += p_Width;
            }

            for (auto l_J = p_Radius + 1; l_J < p_Height - p_Radius; ++l_J)
            {
                l_Val = l_Val + (l_InPixels[l_RI] - l_InPixels[l_LI]);
                l_InPixels[l_TI] = l_Val * l_Mult;
                l_LI += p_Width;
                l_RI += p_Width;
                l_TI += p_Width;
            }

            for (auto l_J = p_Height - p_Radius; l_J < p_Height; ++l_J)
            {
                l_Val = l_Val + (l_LV - l_InPixels[l_LI]);
                l_InPixels[l_TI] = l_Val * l_Mult;
                l_LI += p_Width;
                l_TI += p_Width;
            }
        }
    }

}   ///< namespace CP_SDK::Unity