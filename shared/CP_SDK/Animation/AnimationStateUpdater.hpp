#pragma once

#include "AnimationControllerInstance.hpp"
#include "../Utils/MonoPtr.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/UI/Image.hpp>

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

    /// @briefAnimation state updater
    class AnimationStateUpdater : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.Animation", AnimationStateUpdater, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(AnimationStateUpdater);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(AnimationStateUpdater);

        private:
            AnimationControllerInstance::Ptr m_ControllerDataInstance;

        public:
            _v::MonoPtr<_u::Image> TargetImage;

            /// @brief Set the new AnimationControllerInstance
            /// @param p_ControllerDataInstance New controller
            void SetControllerDataInstance(AnimationControllerInstance::Ptr& p_ControllerDataInstance);

        public:
            /// @brief On destroy
            DECLARE_INSTANCE_METHOD(void, OnDestroy);

        public:
            /// @brief On enabled
            DECLARE_INSTANCE_METHOD(void, OnEnable);
            /// @brief On disable
            DECLARE_INSTANCE_METHOD(void, OnDisable);

    };

}   ///< namespace CP_SDK::Animation

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::Animation::AnimationStateUpdater);