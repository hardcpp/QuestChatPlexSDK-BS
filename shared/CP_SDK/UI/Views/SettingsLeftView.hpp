#pragma once

#include "../../ChatPlexService.hpp"
#include "../../XUI/XUI.hpp"
#include "../ViewController.hpp"

namespace CP_SDK::UI::Views {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
        using namespace CP_SDK::XUI;
    }

    /// @brief Settings left view controller
    class CP_SDK_EXPORT SettingsLeftView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", SettingsLeftView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(SettingsLeftView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(SettingsLeftView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            _v::XUIText::Ptr            m_StatusText;
            _v::XUIText::Ptr            m_SubscriptionText;
            _v::XUIPrimaryButton::Ptr   m_PrimaryButton;
            _v::XUISecondaryButton::Ptr m_SecondaryButton;

        private:
            bool m_IsLinking = false;

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();
            /// @brief On view deactivation
            void OnViewDeactivation_Impl();

        private:
            /// @brief On primary button pressed
            void OnPrimaryButtonPressed();
            /// @brief On secondary button pressed
            void OnSecondaryButtonPressed();
            /// @brief On Loading cancel
            void OnLoadingCancel();

        private:
            /// @brief On ChatPlex service state change
            /// @param oldState Old state
            /// @param newState New state
            void ChatPlexService_StateChanged(ChatPlexService::EState oldState, ChatPlexService::EState newState);

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::SettingsLeftView);