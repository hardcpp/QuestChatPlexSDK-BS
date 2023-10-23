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

    /// @brief Top navigation view
    class TopNavigationView : public ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Views", TopNavigationView, ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(TopNavigationView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(TopNavigationView);
        CP_SDK_UI_VIEW_CONTROLLER_INSTANCE();

        private:
            _v::XUIText::Ptr m_Title;

        public:
            _v::Event<> OnBackButton;

        private:
            /// @brief On view creation
            void OnViewCreation_Impl();

        public:
            /// @brief Set title
            /// @param p_Title New title
            void SetTitle(std::u16string_view p_Title);

        private:
            /// @brief On back button pressed
            void OnBackButtonPressed();

    };

}   ///< namespace CP_SDK::UI::Views

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Views::TopNavigationView);