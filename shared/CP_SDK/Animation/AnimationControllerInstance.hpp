#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <UnityEngine/Rect.hpp>
#include <UnityEngine/Sprite.hpp>
#include <UnityEngine/Texture2D.hpp>
#include <UnityEngine/UI/Image.hpp>

#include <memory>
#include <vector>
#include <stdint.h>

namespace CP_SDK::Animation {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Animation controller data object
    class AnimationControllerInstance
    {
        CP_SDK_NO_COPYMOVE_CTORS(AnimationControllerInstance);
        CP_SDK_PRIV_TAG();

        public:
            using Ptr = std::shared_ptr<AnimationControllerInstance>;

        private:
            bool                                m_IsDelayConsistent;
            int64_t                             m_LastFrameChange;
            std::vector<_v::MonoPtr<_u::Image>> m_ActiveImages;
            int                                 m_ActiveCount;
            std::vector<_u::Rect>               m_UVs;

        public:
            _v::MonoPtr<_u::Sprite>                 FirstFrame;
            std::vector<_v::MonoPtr<_u::Sprite>>    Frames;
            std::vector<uint16_t>                   Delays;
            uint32_t                                CurrentFrameIndex;

        public:
            /// @brief Constructor
            /// @param p_Texture Texture atlas
            /// @param p_UVs     UVs of sub sprites
            /// @param p_Delays  Delays of frames
            AnimationControllerInstance(CP_SDK_PRIV_TAG_ARG(), _v::CMonoPtrRef<_u::Texture2D> p_Texture, const std::vector<_u::Rect>& p_UVs, const std::vector<uint16_t>& p_Delays);
            /// @brief Destructor
            ~AnimationControllerInstance();

        public:
            /// @brief Constructor
            /// @param p_Texture Texture atlas
            /// @param p_UVs     UVs of sub sprites
            /// @param p_Delays  Delays of frames
            static Ptr Make(_v::CMonoPtrRef<_u::Texture2D> p_Texture, const std::vector<_u::Rect>& p_UVs, const std::vector<uint16_t>& p_Delays);

        public:
            /// @brief Register an image
            /// @param p_TargetImage Target
            void Register(_u::Image* p_TargetImage);
            /// @brief Unregister an image
            /// @param p_TargetImage Target
            void Unregister(_u::Image* p_TargetImage);

        public:
            /// @brief Check if we should move to the next frame
            /// @param p_Now Current time in MS
            void CheckForNextFrame(int64_t p_Now);

    };

}   ///< namespace CP_SDK::Animation