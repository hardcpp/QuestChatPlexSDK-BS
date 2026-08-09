#include "CP_SDK/Animation/AnimationLoader.hpp"
#include "CP_SDK/Animation/WEBP/WEBPDecoder.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

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
        {
            p_Callback(nullptr, {}, {}, 0, 0);
            co_return;
        }

        if (p_AnimationInfo->Width <= 0 || p_AnimationInfo->Height <= 0 || p_AnimationInfo->Frames.empty()
            || p_AnimationInfo->Frames.size() != p_AnimationInfo->Delays.size())
        {
            p_Callback(nullptr, {}, {}, 0, 0);
            co_return;
        }

        auto l_MaxAtlasTextureSize = GetMaxAtlasTextureSize(p_AnimationInfo);
        if (l_MaxAtlasTextureSize <= 0)
        {
            p_Callback(nullptr, {}, {}, 0, 0);
            co_return;
        }

        _v::MonoPtr<Texture2D>           l_AtlasTexture  = Texture2D::New_ctor(p_AnimationInfo->Width, p_AnimationInfo->Height);
        _v::MonoPtr<::Array<Texture2D*>> l_SubTextures   = ::Array<Texture2D*>::NewLength(p_AnimationInfo->Frames.size());

        for (std::size_t l_FrameI = 0; l_FrameI < p_AnimationInfo->Frames.size(); ++l_FrameI)
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
        if (!p_AnimationInfo || p_AnimationInfo->Frames.empty() || p_AnimationInfo->Width <= 0 || p_AnimationInfo->Height <= 0)
            return 0;

        const auto l_FrameCount     = static_cast<uint64_t>(p_AnimationInfo->Frames.size());
        const auto l_FramesInRow    = static_cast<uint64_t>(std::ceil(std::sqrt(static_cast<double>(l_FrameCount))));
        const auto l_FramesInColumn = (l_FrameCount + l_FramesInRow - 1) / l_FramesInRow;
        const auto l_TextureWidth   = std::min<uint64_t>(l_FramesInRow * static_cast<uint64_t>(p_AnimationInfo->Width), 2048ULL);
        const auto l_TextureHeight  = std::min<uint64_t>(l_FramesInColumn * static_cast<uint64_t>(p_AnimationInfo->Height), 2048ULL);

        return static_cast<int>(std::max(l_TextureWidth, l_TextureHeight));
    }

}   ///< namespace CP_SDK::Animation
