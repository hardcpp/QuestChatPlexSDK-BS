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

    /// @brief Settings right view controller
    class CP_SDK_EXPORT SettingsRightView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", SettingsRightView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(SettingsRightView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(SettingsRightView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::SettingsRightView);