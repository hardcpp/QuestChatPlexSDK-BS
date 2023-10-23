#pragma once

#include "../ViewController.hpp"
#include "../ModButton.hpp"
#include "../../XUI/XUI.hpp"

#include <map>

namespace CP_SDK::UI::Views {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::UI::Components;
        using namespace CP_SDK::Utils;
        using namespace CP_SDK::XUI;
    }

    /// @brief Mod menu view controller
    class ModMenuView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", ModMenuView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(ModMenuView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(ModMenuView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            std::map<ModButton::Ptr, _v::MonoPtr<_v::CPOrSButton>>  m_Buttons;
            _v::XUIGLayout::Ptr                                     m_GLayout;

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();
            /// @brief On view activation
            void OnViewActivation_Impl();

        public:
            /// @brief Refresh buttons
            void Refresh();

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::ModMenuView);