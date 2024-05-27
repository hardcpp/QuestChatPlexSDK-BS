#pragma once

#include "IFlowCoordinator.hpp"
#include "Components/CFloatingPanel.hpp"
#include "Views/TopNavigationView.hpp"

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief ScreenSystem widget
    class CP_SDK_EXPORT_VISIBILITY ScreenSystem : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", ScreenSystem, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(ScreenSystem);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(ScreenSystem);

        static ScreenSystem* m_Instance;

        private:
            _v::MonoPtr<_u::Transform>              m_ScreenContainer;
            _v::MonoPtr<IFlowCoordinator>           m_CurrentFlowCoordinator;
            _v::MonoPtr<Components::CFloatingPanel> m_LeftScreen;
            _v::MonoPtr<Components::CFloatingPanel> m_TopScreen;
            _v::MonoPtr<Components::CFloatingPanel> m_MainScreen;
            _v::MonoPtr<Components::CFloatingPanel> m_RightScreen;
            _v::MonoPtr<Views::TopNavigationView>   m_TopNavigationViewController;

        public:
            static ScreenSystem* Instance();

            static _v::Event<> OnCreated;
            static _v::Event<> OnPresent;
            static _v::Event<> OnDismiss;

        public:
            UIFieldRefDel<IFlowCoordinator>             CurrentFlowCoordinator;
            UIFieldRefDel<_u::Transform>                ScreenContainer;
            UIFieldRefDel<Components::CFloatingPanel>   LeftScreen;
            UIFieldRefDel<Components::CFloatingPanel>   TopScreen;
            UIFieldRefDel<Components::CFloatingPanel>   MainScreen;
            UIFieldRefDel<Components::CFloatingPanel>   RightScreen;

        public:
            /// @brief Create the screen system
            static void Create();
            /// @brief Destroy
            static void Destroy();

        public:
            /// @brief Present the screen system
            void Present();
            /// @brief Dismiss the screen system
            void Dismiss();

        public:
            /// @brief Set current flow coordinator
            /// @param p_FlowCoordinator New flow coordinator
            /// @param p_OnPresent       On present?
            void SetFlowCoordinator(_v::CMonoPtrRef<IFlowCoordinator> p_FlowCoordinator, bool p_OnPresent);

        private:
            /// @brief Init the screen system
            void Init();

        private:
            /// @brief On back button pressed
            void TopNavigationViewController_OnBackButton();

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::ScreenSystem);