#pragma once

#include "../ViewController.hpp"
#include "../../XUI/XUI.hpp"

#include <map>

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

    /// @brief Main main view controller
    class CP_SDK_EXPORT MainMainView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", MainMainView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(MainMainView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(MainMainView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            std::map<IModuleBase*, Components::CPOrSButton*> m_ModulesButton;

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();
            /// @brief On view creation
            void OnViewActivation_Impl();

        private:
            /// @brief Go to settings
            void OnSettingsPressed();

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::MainMainView);