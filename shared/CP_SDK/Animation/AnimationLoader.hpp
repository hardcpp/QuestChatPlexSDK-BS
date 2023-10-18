#pragma once

#include "AnimationInfo.hpp"
#include "../Utils/Delegate.hpp"
#include "../Utils/MonoPtr.hpp"

#include <custom-types/shared/coroutine.hpp>
#include <UnityEngine/Rect.hpp>
#include <UnityEngine/Sprite.hpp>
#include <UnityEngine/Texture2D.hpp>

namespace CP_SDK::Animation {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Animation type
    enum class EAnimationType
    {
        NONE,
        GIF,
        APNG,
        WEBP,
        AUTODETECT
    };

    /// @brief Animation loader
    class AnimationLoader
    {
        DISABLE_CONSTRUCTORS(AnimationLoader);

        public:
            using t_AnimatedCallback    = _v::Action<_v::CMonoPtrRef<_u::Texture2D>, const std::vector<_u::Rect>&, const std::vector<uint16_t>&, int, int>;
            using t_StaticCallback      = _v::Action<_v::CMonoPtrRef<_u::Sprite>>;

        public:
            /// @brief Load animation
            /// @param p_Type           Animation type
            /// @param p_Data           Raw data
            /// @param p_Callback       Animated callback
            /// @param p_StaticCallback Static callback
            static void Load(EAnimationType p_Type, _v::CMonoPtrRef<::Array<uint8_t>> p_Data, const t_AnimatedCallback& p_Callback, const t_StaticCallback& p_StaticCallback);

        private:
            /// @brief Process loaded animation
            /// @param p_AnimationInfo Animation infos
            /// @param p_Callback      Callback
            static custom_types::Helpers::Coroutine Coroutine_ProcessLoadedAnimation(AnimationInfo::Ptr p_AnimationInfo, t_AnimatedCallback p_Callback);

        private:
            /// @brief Get max atlas texture size
            /// @param p_AnimationInfo Animation infos
            static int GetMaxAtlasTextureSize(AnimationInfo::Ptr& p_AnimationInfo);

    };

}   ///< namespace CP_SDK::Animation