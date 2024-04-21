#include "CP_SDK/Animation/AnimationLoader.hpp"
#include "CP_SDK/Animation/WEBP/WEBPDecoder.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

using namespace UnityEngine;

namespace CP_SDK::Animation {

    /// @brief Load animation
    /// @param p_Type           Animation type
    /// @param p_Data           Raw data
    /// @param p_Callback       Animated callback
    /// @param p_StaticCallback Static callback
    void AnimationLoader::Load(EAnimationType p_Type, _v::CMonoPtrRef<::Array<uint8_t>> p_Data, const t_AnimatedCallback& p_Callback, const t_StaticCallback& p_StaticCallback)
    {
        switch (p_Type)
        {
            case EAnimationType::GIF:
                throw std::runtime_error("EAnimationType::GIF is not implemented!");
                //GIF::GIFDecoder::Process(
                //    p_Data,
                //    _v::MakeDelegate([=](AnimationInfo::Ptr p_AnimationInfo) -> void { Unity::MTCoroutineStarter::EnqueueFromThread(Coroutine_ProcessLoadedAnimation(p_AnimationInfo, p_Callback)); })
                //);
                break;
            case EAnimationType::APNG:
                throw std::runtime_error("EAnimationType::APNG is not implemented!");
                //APNG::APNGUnityDecoder::Process(
                //    p_Data,
                //    _v::MakeDelegate([=](AnimationInfo::Ptr p_AnimationInfo) -> void { Unity::MTCoroutineStarter::EnqueueFromThread(Coroutine_ProcessLoadedAnimation(p_AnimationInfo, p_Callback)); })
                //);
                break;
            case EAnimationType::WEBP:
                WEBP::WEBPDecoder::Process(p_Data,
                    [=](const AnimationInfo::Ptr& p_AnimationInfo) -> void {
                        Unity::MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(Coroutine_ProcessLoadedAnimation(p_AnimationInfo, p_Callback)));
                    },
                    p_StaticCallback
                );
                break;

            default:
                break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Process loaded animation
    /// @param p_AnimationInfo Animation infos
    /// @param p_Callback      Callback
    custom_types::Helpers::Coroutine AnimationLoader::Coroutine_ProcessLoadedAnimation(AnimationInfo::Ptr p_AnimationInfo, t_AnimatedCallback p_Callback)
    {
        if (!p_AnimationInfo)
            p_Callback(nullptr, {}, {}, 0, 0);

        auto l_MaxAtlasTextureSize = GetMaxAtlasTextureSize(p_AnimationInfo);

        _v::MonoPtr<Texture2D>           l_AtlasTexture  = Texture2D::New_ctor(p_AnimationInfo->Width, p_AnimationInfo->Height);
        _v::MonoPtr<::Array<Texture2D*>> l_SubTextures   = ::Array<Texture2D*>::NewLength(p_AnimationInfo->Frames.size());

        for (int l_FrameI = 0; l_FrameI < p_AnimationInfo->Frames.size(); ++l_FrameI)
        {
            auto l_FrameTexture = Texture2D::New_ctor(p_AnimationInfo->Width, p_AnimationInfo->Height, TextureFormat::RGBA32, false);
            l_FrameTexture->set_wrapMode(TextureWrapMode::Clamp);
            l_FrameTexture->LoadRawTextureData(p_AnimationInfo->Frames[l_FrameI].Ptr());

            l_SubTextures->_values[l_FrameI] = l_FrameTexture;

            co_yield nullptr;
        }

        auto l_UVs = l_AtlasTexture->PackTextures(l_SubTextures.Ptr(), 2, l_MaxAtlasTextureSize, true);

        for (int l_I = 0; l_I < l_SubTextures->get_Length(); ++l_I)
        {
            GameObject::Destroy(l_SubTextures->_values[l_I]);
            l_SubTextures->_values[l_I] = nullptr;
        }

        auto l_UVsVec = std::vector<Rect>(l_UVs.begin(), l_UVs.end());

        p_Callback(l_AtlasTexture, l_UVsVec, p_AnimationInfo->Delays, p_AnimationInfo->Width, p_AnimationInfo->Height);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get max atlas texture size
    /// @param p_AnimationInfo Animation infos
    int AnimationLoader::GetMaxAtlasTextureSize(AnimationInfo::Ptr& p_AnimationInfo)
    {
        auto l_TestNumber        = 2;
        auto l_FramesInRow       = 0;
        auto l_FramesInColumn    = 0;
        auto l_FrameCount        = p_AnimationInfo->Frames.size();

        while (true)
        {
            /// Make sure the number of frames is cleanly divisible by our testNum
            if (!(l_FrameCount % l_TestNumber != 0))
                l_FrameCount += l_FrameCount % l_TestNumber;

            l_FramesInRow       = std::max<int>(1, l_FrameCount / l_TestNumber);
            l_FramesInColumn    = l_FrameCount / l_FramesInRow;

            if (l_FramesInRow <= l_FramesInColumn)
                break;

            l_TestNumber += 2;
        }

        auto l_TextureWidth  = Mathf::Clamp(l_FramesInRow     * p_AnimationInfo->Width,  0, 2048);
        auto l_TextureHeight = Mathf::Clamp(l_FramesInColumn  * p_AnimationInfo->Height, 0, 2048);

        return std::max<int>(l_TextureWidth, l_TextureHeight);
    }

}   ///< namespace CP_SDK::Animation