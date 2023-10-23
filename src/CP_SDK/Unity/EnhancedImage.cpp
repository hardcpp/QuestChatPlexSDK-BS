#include "CP_SDK/Unity/EnhancedImage.hpp"
#include "CP_SDK/Animation/AnimationControllerManager.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/IO/File.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

static uint8_t ANIMATED_GIF_PATTERN[]   = { 0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32, 0x2E, 0x30 };
static uint8_t WEBPVP8_PATTERN[]        = { 0x57, 0x45, 0x42, 0x50, 0x56, 0x50, 0x38 };

using namespace System::IO;
using namespace UnityEngine;

namespace CP_SDK::Unity {

    /// @brief Constructor
    EnhancedImage::EnhancedImage(CP_SDK_PRIV_TAG_ARG())
    {
        m_ImageID               = u"";
        m_Sprite                = nullptr;
        m_Width                 = 0;
        m_Height                = 0;
        m_AnimControllerData    = nullptr;
    }
    /// @brief Destructor
    EnhancedImage::~EnhancedImage()
    {
        m_Sprite = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get ID of the image
    std::u16string_view EnhancedImage::ImageID()
    {
        return m_ImageID;
    }
    /// @brief Get ID of the image
    Sprite* EnhancedImage::Sprite()
    {
        if (m_Sprite)
            return m_Sprite.Ptr();

        return nullptr;
    }
    /// @brief Get width
    int EnhancedImage::Width()
    {
        return m_Width;
    }
    /// @brief Get height
    int EnhancedImage::Height()
    {
        return m_Height;
    }
    /// @brief Get AnimControllerData
    Animation::AnimationControllerInstance::Ptr& EnhancedImage::AnimControllerData()
    {
        return m_AnimControllerData;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Ensure image valid
    /// @param p_ForcedHeight Forced height
    void EnhancedImage::EnsureValidForHeight(int p_ForcedHeight)
    {
        if (m_Height < 0 || m_Width < 0)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][EnhancedImageInfo.EnsureValidForHeight] Invalid emote ImageID " + m_ImageID + u" Width " + StringW(std::to_string(m_Width)).operator std::__ndk1::u16string() + u" height " + StringW(std::to_string(m_Height)).operator std::__ndk1::u16string());
            m_Width   = p_ForcedHeight;
            m_Height  = p_ForcedHeight;
            return;
        }

        if (m_Height > p_ForcedHeight)
            m_Height = p_ForcedHeight;

        if (m_Width > (6 * p_ForcedHeight))
        {
            m_Width = p_ForcedHeight;
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][EnhancedImageInfo.EnsureValidForHeight] Too wide emote ImageID " + m_ImageID + u" Width " + StringW(std::to_string(m_Width)).operator std::__ndk1::u16string() + u" height " + StringW(std::to_string(m_Height)).operator std::__ndk1::u16string());
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief From raw
    /// @param p_ID           ID of the image
    /// @param p_Bytes        Result bytes
    /// @param p_Callback     Callback
    /// @param p_ForcedHeight Forced height
    void EnhancedImage::FromRawStatic(std::u16string p_ID, _v::CMonoPtrRef<::Array<uint8_t>> p_Bytes, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight)
    {
        SpriteU::CreateFromRawThreaded(
            p_Bytes,
            [=](UnityEngine::Sprite* p_Sprite) -> void {
                OnRawStaticCallback(p_ID, p_Sprite, p_Callback, p_ForcedHeight);
            }
        );
    }
    /// @brief From raw animated
    /// @param p_ID           ID of the image
    /// @param p_Type         Image type
    /// @param p_Bytes        Result bytes
    /// @param p_Callback     Callback
    /// @param p_ForcedHeight Forced height
    void EnhancedImage::FromRawAnimated(std::u16string p_ID, Animation::EAnimationType p_Type, _v::CMonoPtrRef<::Array<uint8_t>> p_Bytes, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight)
    {
        if (p_Type == Animation::EAnimationType::AUTODETECT && p_Bytes->Length() > 0)
        {
            if (p_Bytes->Length() > 3 && p_Bytes->values[0] == 0x47 && ContainBytePattern(p_Bytes, ANIMATED_GIF_PATTERN, sizeof(ANIMATED_GIF_PATTERN)))
                p_Type = Animation::EAnimationType::GIF;
            else if (p_Bytes->Length() > 16 && ContainBytePattern(p_Bytes, WEBPVP8_PATTERN, sizeof(WEBPVP8_PATTERN)))
                p_Type = Animation::EAnimationType::WEBP;
            else
                p_Type = Animation::EAnimationType::NONE;
        }

        if (p_Type == Animation::EAnimationType::NONE)
        {
            FromRawStatic(p_ID, p_Bytes, p_Callback, p_ForcedHeight);
            return;
        }

        Animation::AnimationLoader::Load(
            p_Type,
            p_Bytes,
            [=](_v::CMonoPtrRef<Texture2D> p_Texture, const std::vector<Rect>& p_UVs, const std::vector<uint16_t>& p_Delays, int p_Width, int p_Height) -> void {
                OnRawAnimatedCallback(p_ID, p_Texture, p_UVs, p_Delays, p_Width, p_Height, p_Callback, p_ForcedHeight);
            },
            [=](_v::CMonoPtrRef<UnityEngine::Sprite> p_Sprite) -> void {
                OnRawStaticCallback(p_ID, p_Sprite, p_Callback, p_ForcedHeight);
            }
        );
    }
    /// @brief From file
    /// @param p_FileName File name
    /// @param p_ID       ID of the image
    /// @param p_Callback Callback
    void EnhancedImage::FromFile(std::u16string p_FileName, std::u16string p_ID, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback)
    {
        StringW l_FileName = p_FileName;
        if (l_FileName->ToLower()->EndsWith(".png"))
        {
            FromRawStatic(
                p_ID,
                reinterpret_cast<::Array<uint8_t>*>(File::ReadAllBytes(p_FileName).convert()),
                [=](const EnhancedImage::Ptr& p_Result) -> void
                {
                    if (p_Result)
                    {
                        p_Result->Sprite()->get_texture()->set_wrapMode(TextureWrapMode::Mirror);
                        p_Callback(p_Result);
                    }
                    else
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][EnhancedImage.FromFile] Failed to load image " + p_FileName);
                }
            );
        }
        else if (l_FileName->ToLower()->EndsWith(".gif"))
        {
            FromRawAnimated(
                p_ID,
                Animation::EAnimationType::AUTODETECT,
                reinterpret_cast<::Array<uint8_t>*>(File::ReadAllBytes(p_FileName).convert()),
                [=](const EnhancedImage::Ptr& p_Result) -> void
                {
                    if (p_Result)
                        p_Callback(p_Result);
                    else
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][EnhancedImage.FromFile] Failed to load image " + p_FileName);
                }
            );
        }
        else if (l_FileName->ToLower()->EndsWith(".apng"))
        {
            FromRawAnimated(
                p_ID,
                Animation::EAnimationType::APNG,
                reinterpret_cast<::Array<uint8_t>*>(File::ReadAllBytes(p_FileName).convert()),
                [=](const EnhancedImage::Ptr& p_Result) -> void
                {
                    if (p_Result)
                        p_Callback(p_Result);
                    else
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][EnhancedImage.FromFile] Failed to load image " + p_FileName);
                }
            );
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Static callback
    /// @param p_ID           ID of the image
    /// @param p_Sprite       Sprite
    /// @param p_Callback     Callback
    /// @param p_ForcedHeight Forced height
    void EnhancedImage::OnRawStaticCallback(std::u16string p_ID, _v::CMonoPtrRef<_u::Sprite> p_Sprite, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight)
    {
        /// RUN ON MAIN THREAD

        int l_SpriteWidth = 0;
        int l_SpriteHeight = 0;

        if (p_Sprite)
        {
            l_SpriteWidth   = p_Sprite.Ptr()->get_texture()->get_width();
            l_SpriteHeight  = p_Sprite.Ptr()->get_texture()->get_height();
        }

        auto l_Result = EnhancedImage::Ptr(nullptr);
        if (p_Sprite)
        {
            if (p_ForcedHeight != -1)
                ComputeImageSizeForHeight(&l_SpriteWidth, &l_SpriteHeight, p_ForcedHeight);

            l_Result = std::make_shared<EnhancedImage>(CP_SDK_PRIV_TAG_VAL());
            l_Result->m_ImageID             = p_ID;
            l_Result->m_Sprite              = p_Sprite;
            l_Result->m_Width               = l_SpriteWidth;
            l_Result->m_Height              = l_SpriteHeight;
            l_Result->m_AnimControllerData  = nullptr;

            if (p_ForcedHeight != -1)
                l_Result->EnsureValidForHeight(p_ForcedHeight);
        }

        p_Callback(l_Result);
    }
    /// @brief Animated callback
    /// @param p_ID           ID of the image
    /// @param p_Texture      Atlas
    /// @param p_UVs          Frame UVs
    /// @param p_Delays       Delays
    /// @param p_Width        Width
    /// @param p_Height       Height
    /// @param p_Callback     Callback
    /// @param p_ForcedHeight Forced height
    void EnhancedImage::OnRawAnimatedCallback(std::u16string p_ID, _v::CMonoPtrRef<Texture2D> p_Texture, const std::vector<Rect>& p_UVs, const std::vector<uint16_t>& p_Delays, int p_Width, int p_Height, _v::CActionRef<const EnhancedImage::Ptr&> p_Callback, int p_ForcedHeight)
    {
        /// RUN ON MAIN THREAD

        if (!p_Texture)
        {
            p_Callback(nullptr);
            return;
        }

        auto l_AnimControllerData    = Animation::AnimationControllerManager::Instance()->Register(p_ID, p_Texture, p_UVs, p_Delays);
        auto l_AnimResult            = EnhancedImage::Ptr(nullptr);

        if (l_AnimControllerData != nullptr)
        {
            if (p_ForcedHeight != -1)
                ComputeImageSizeForHeight(&p_Width, &p_Height, p_ForcedHeight);

            l_AnimResult = std::make_shared<EnhancedImage>(CP_SDK_PRIV_TAG_VAL());
            l_AnimResult->m_ImageID             = p_ID;
            l_AnimResult->m_Sprite              = l_AnimControllerData->FirstFrame;
            l_AnimResult->m_Width               = p_Width;
            l_AnimResult->m_Height              = p_Height;
            l_AnimResult->m_AnimControllerData  = l_AnimControllerData;

            if (p_ForcedHeight != -1)
                l_AnimResult->EnsureValidForHeight(p_ForcedHeight);
        }

        p_Callback(l_AnimResult);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Compute image size for specific height
    /// @param p_SpriteWidth  Base width
    /// @param p_SpriteHeight Base height
    /// @param p_Height       Desired height
    void EnhancedImage::ComputeImageSizeForHeight(int* p_SpriteWidth, int* p_SpriteHeight, int p_Height)
    {
        /// Quick exit
        if (*p_SpriteHeight == p_Height)
            return;

        /// 1:1 ratio quick case
        if (*p_SpriteHeight == *p_SpriteWidth)
        {
            *p_SpriteWidth   = p_Height;
            *p_SpriteHeight  = p_Height;
        }
        else
        {
            double l_Scale = ((double)p_Height) / ((double)*p_SpriteHeight);

            *p_SpriteWidth   = (int)(l_Scale * ((double)*p_SpriteWidth));
            *p_SpriteHeight  = (int)(l_Scale * ((double)*p_SpriteHeight));
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Fast lookup for byte pattern
    /// @param p_Array       Input array
    /// @param p_Pattern     Lookup pattern
    /// @param p_PatternSize Pattern size
    bool EnhancedImage::ContainBytePattern(_v::CMonoPtrRef<::Array<uint8_t>> p_Array, const uint8_t* p_Pattern, int p_PatternSize)
    {
        auto l_PatternPosition = 0;
        for (int l_I = 0; l_I < p_Array->Length(); ++l_I)
        {
            if (p_Array->values[l_I] != p_Pattern[l_PatternPosition])
            {
                l_PatternPosition = 0;
                continue;
            }

            l_PatternPosition++;
            if (l_PatternPosition == p_PatternSize)
                return true;
        }

        return l_PatternPosition == p_PatternSize;
    }

}   ///< namespace CP_SDK::Unity