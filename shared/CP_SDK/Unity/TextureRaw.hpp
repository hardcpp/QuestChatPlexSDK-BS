#pragma once

#include "../Utils/Il2cpp.hpp"

#include <vector>

#include <UnityEngine/Color.hpp>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Texture raw utilities
    class CP_SDK_EXPORT_VISIBILITY TextureRaw
    {
        CP_SDK_NO_DEF_CTORS(TextureRaw);

        public:
            using PixelArray = std::shared_ptr<std::vector<_u::Color>>;

        public:
            /// @brief Load an image into raw pixels
            /// @param p_Bytes  Input bytes
            /// @param p_Width  Ouput width
            /// @param p_Height Output height
            /// @param p_Pixels Output pixels
            static bool Load(::Array<uint8_t>* p_Bytes, int& p_Width, int& p_Height, PixelArray* p_Pixels);

        public:
            /// @brief Blur an image
            /// @param p_InWidth  Source width
            /// @param p_InHeight Source height
            /// @param p_InPixels Input image
            /// @param p_Radius   Blur radius
            static void FastGaussianBlur(int p_InWidth, int p_InHeight, PixelArray& p_InPixels, int p_Radius);
            /// @brief Multiply image A & B into A
            /// @param p_ImageA Target
            /// @param p_ImageB Additional image
            static void Multiply(PixelArray& p_ImageA, PixelArray& p_ImageB);
            /// @brief Resize an image and crop it
            /// @param p_InWidth      Source width
            /// @param p_InHeight     Source height
            /// @param p_InPixels     Input image
            /// @param p_TargetWidth  Target width
            /// @param p_TargetHeight Target height
            /// @param p_YOffsetRel   Height anchor
            static PixelArray ResampleAndCrop(int p_InWidth, int p_InHeight, PixelArray& p_InPixels, int p_TargetWidth, int p_TargetHeight, float p_YOffsetRel = 0.5f);

        private:
            static std::vector<int> BoxesForGauss(int p_Radius, int p_Count);
            static void GaussianBlur4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius);
            static void BoxBlur4 (int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius);
            static void BoxBlurH4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius);
            static void BoxBlurT4(int p_Width, int p_Height, PixelArray& p_InPixels, int p_Radius);

    };

}   ///< namespace CP_SDK::Unity