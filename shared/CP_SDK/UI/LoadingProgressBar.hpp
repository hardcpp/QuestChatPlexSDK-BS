#pragma once

#include "Components/CText.hpp"
#include "../Utils/Il2cpp.hpp"
#include "../Unity/PersistantSingleton.hpp"
#include "../ChatPlexSDK.hpp"

#include <custom-types/shared/coroutine.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/UI/Image.hpp>
#include <UnityEngine/Canvas.hpp>

#include <string>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::UI::Components;
        using namespace CP_SDK::Utils;
    }

    /// @brief Loading progress bar
    class LoadingProgressBar : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", LoadingProgressBar, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(LoadingProgressBar);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(LoadingProgressBar);
        CP_SDK_UNITY_PERSISTANT_SINGLETON_DECL(CP_SDK::UI::LoadingProgressBar);

        private:
            _v::MonoPtr<_u::Canvas> m_Canvas;
            _v::MonoPtr<_v::CText>  m_HeaderText;
            _v::MonoPtr<_u::Image>  m_LoadingBackground;
            _v::MonoPtr<_u::Image>  m_LoadingBar;

        public:
            /// @brief On component creation
            DECLARE_INSTANCE_METHOD(void, Awake);

        public:
            /// @brief Show a message with a hide timer
            /// @param p_Message Message to display
            /// @param p_Time    Time before disapearing
            void ShowTimedMessage(std::u16string_view p_Message, float p_Time);
            /// @brief Show loading progress bar with a message
            /// @param p_Message  Message to display
            /// @param p_Progress Loading progress
            void ShowLoadingProgressBar(std::u16string_view p_Message, float p_Progress);
            /// @brief Set current progress and displayed message
            /// @param p_Message  Message to display
            /// @param p_Progress Loading progress
            void SetProgress(std::u16string_view p_Message, float p_Progress);
            /// @brief Set hide timer
            /// @param p_Time Time in seconds
            void HideTimed(float p_Time);

        private:
            /// @brief On scene changed
            /// @param p_NewScene New scene type
            void ChatPlexSDK_OnGenericSceneChange(ChatPlexSDK::EGenericScene p_NewScene);
            /// @brief Timed canvas disabler
            /// @param p_Time Time in seconds
            static custom_types::Helpers::Coroutine Coroutine_DisableCanvas(LoadingProgressBar* p_Self, float p_Time);

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::LoadingProgressBar);