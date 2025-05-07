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

    /// @brief Welcome Right View controller
    class CP_SDK_EXPORT MainRightView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", MainRightView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(MainRightView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(MainRightView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::MainRightView);