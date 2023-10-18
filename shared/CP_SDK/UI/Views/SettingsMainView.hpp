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

    /// @brief Settings main view controller
    class SettingsMainView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.Views", SettingsMainView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(SettingsMainView, ViewController);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(SettingsMainView, ViewController);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            std::map<IModuleBase*, Components::CToggle*> m_ModulesToggles;

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();
            /// @brief On view creation
            void OnViewActivation_Impl();

        private:
            /// @brief On module toggled
            /// @param p_Module  Module instance
            /// @param p_Enabled Is enabled
            void OnModuleToggled(IModuleBase* p_Module, bool p_Enabled);
            /// @brief On documentation button
            /// @param p_Module Module instance
            void OnDocumentationButton(IModuleBase* p_Module);

        private:
            /// @brief Check for chat tutorial
            /// @param p_Module Module instance
            void CheckChatTutorial(IModuleBase* p_Module);

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::SettingsMainView);