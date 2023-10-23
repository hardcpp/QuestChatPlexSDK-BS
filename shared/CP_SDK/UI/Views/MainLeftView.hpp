#pragma once

#include "../ViewController.hpp"
#include "../../XUI/XUI.hpp"

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

    /// @brief Welcome Left View controller
    class MainLeftView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", MainLeftView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(MainLeftView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(MainLeftView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();

        private:
            /// @brief Documentation button
            void OnDocumentationButton();
            /// @brief Go to discord
            void OnDiscordButton();

        private:
            /// @brief Go to donate
            void OnDonateButton();

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::MainLeftView);