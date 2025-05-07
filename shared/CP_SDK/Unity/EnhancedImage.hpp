#pragma once

#include "../Animation/AnimationControllerInstance.hpp"
#include "../Animation/AnimationLoader.hpp"
#include "../Utils/Il2cpp.hpp"

#include <UnityEngine/Sprite.hpp>

#include <memory>
#include <string>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Enhanced image info
    class CP_SDK_EXPORT EnhancedImage
    {
        CP_SDK_NO_COPYMOVE_CTORS(EnhancedImage);
        CP_SDK_PRIV_TAG();

        public:
            using Ptr = std::shared_ptr<EnhancedImage>;

        private:
            std::u16string                              m_ImageID;
            _v::MonoPtr<_u::Sprite>                     m_Sprite;
            int                                         m_Width;
            int                                         m_Height;
            Animation::AnimationControllerInstance::Ptr m_AnimControllerData;

        public:
            /// @brief Constructor
            EnhancedImage(CP_SDK_PRIV_TAG_ARG());
            /// @brief Destructor
            ~EnhancedImage();

        public:
            /// @brief Get ID of the image
            std::u16string_view ImageID();
            /// @brief Get ID of the image
            _u::Sprite* Sprite();
            /// @brief Get width
            int Width();
            /// @brief Get height
            int Height();
            /// @brief Get AnimControllerData
            Animation::AnimationControllerInstance::Ptr& AnimControllerData();

        public:
            /// @brief Ensure image valid
            /// @param p_ForcedHeight Forced height
            void EnsureValidForHeight(int p_ForcedHeight);

        public:
            /// @brief From raw
            /// @param p_ID           ID of the image
            /// @param p_Bytes        Result bytes
            /// @param p_Callback     Callback
            /// @param p_ForcedHeight Forced height
            static void FromRawStatic(std::u16string p_ID, _v::CMonoPtrRef<::Array<uint8_t>> p_Bytes, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight = -1);
            /// @brief From raw animated
            /// @param p_ID           ID of the image
            /// @param p_Type         Image type
            /// @param p_Bytes        Result bytes
            /// @param p_Callback     Callback
            /// @param p_ForcedHeight Forced height
            static void FromRawAnimated(std::u16string p_ID, Animation::EAnimationType p_Type, _v::CMonoPtrRef<::Array<uint8_t>> p_Bytes, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight = -1);
            /// @brief From file
            /// @param p_FileName File name
            /// @param p_ID       ID of the image
            /// @param p_Callback Callback
            static void FromFile(std::u16string p_FileName, std::u16string p_ID, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback);

        private:
            /// @brief Static callback
            /// @param p_ID           ID of the image
            /// @param p_Sprite       Sprite
            /// @param p_Callback     Callback
            /// @param p_ForcedHeight Forced height
            static void OnRawStaticCallback(std::u16string p_ID, _v::CMonoPtrRef<_u::Sprite> p_Sprite, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight = -1);
            /// @brief Animated callback
            /// @param p_ID           ID of the image
            /// @param p_Texture      Atlas
            /// @param p_UVs          Frame UVs
            /// @param p_Delays       Delays
            /// @param p_Width        Width
            /// @param p_Height       Height
            /// @param p_Callback     Callback
            /// @param p_ForcedHeight Forced height
            static void OnRawAnimatedCallback(std::u16string p_ID, _v::CMonoPtrRef<_u::Texture2D> p_Texture, const std::vector<_u::Rect>& p_UVs, const std::vector<uint16_t>& p_Delays, int p_Width, int p_Height, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight = -1);

        private:
            /// @brief Compute image size for specific height
            /// @param p_SpriteWidth  Base width
            /// @param p_SpriteHeight Base height
            /// @param p_Height       Desired height
            static void ComputeImageSizeForHeight(int* p_SpriteWidth, int* p_SpriteHeight, int p_Height);

        private:
            /// @brief Fast lookup for byte pattern
            /// @param p_Array       Input array
            /// @param p_Pattern     Lookup pattern
            /// @param p_PatternSize Pattern size
            static bool ContainBytePattern(_v::CMonoPtrRef<::Array<uint8_t>> p_Array, const uint8_t* p_Pattern, int p_PatternSize);

    };

}   ///< namespace CP_SDK::Unity