#pragma once

#include "../../CP_SDK/UI/IViewController.hpp"
#include "../../CP_SDK/UI/IScreen.hpp"

#include <HMUI/ViewController.hpp>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }

    class IHMUIViewController;

    /// @brief CP_SDK.UI.IViewController bridge component
    class CP_SDK_EXPORT CP_SDK_UI_IViewControllerBridge : public CP_SDK::UI::IViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI", CP_SDK_UI_IViewControllerBridge, CP_SDK::UI::IViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(CP_SDK_UI_IViewControllerBridge);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(CP_SDK_UI_IViewControllerBridge);

        private:
            CP_SDK::Utils::MonoPtr<_u::RectTransform>     m_RTransform;
            CP_SDK::Utils::MonoPtr<_u::RectTransform>     m_ModalContainerRTransform;
            CP_SDK::Utils::MonoPtr<_u::CanvasGroup>       m_CGroup;
            CP_SDK::Utils::MonoPtr<CP_SDK::UI::IScreen>   m_CurrentScreen;

            CP_SDK::Utils::MonoPtr<IHMUIViewController>   m_IHMUIViewController;

        private:
            /// @brief Check IHMUIViewController instance
            bool CheckIHMUIViewController();

    };

}   ///< CP_SDK_BS::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::CP_SDK_UI_IViewControllerBridge);