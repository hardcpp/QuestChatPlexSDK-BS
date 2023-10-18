#pragma once

#include "../FlowCoordinator.hpp"
#include "../Views/MainLeftView.hpp"
#include "../Views/MainMainView.hpp"
#include "../Views/MainRightView.hpp"
#include "../Views/SettingsLeftView.hpp"
#include "../Views/SettingsMainView.hpp"
#include "../Views/SettingsRightView.hpp"

namespace CP_SDK::UI::FlowCoordinators {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief UI flow coordinator
    class MainFlowCoordinator : public FlowCoordinator
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.FlowCoordinators", MainFlowCoordinator, FlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(MainFlowCoordinator, FlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(MainFlowCoordinator, FlowCoordinator);
        CP_SDK_UI_FLOW_COORDINATOR_INSTANCE();
        CP_SDK_UI_FLOW_COORDINATOR_DESTROY();

        private:
            std::u16string                          m_TitleValue;
            _v::MonoPtr<Views::MainLeftView>        m_MainLeftView;
            _v::MonoPtr<Views::MainMainView>        m_MainMainView;
            _v::MonoPtr<Views::MainRightView>       m_MainRightView;
            _v::MonoPtr<Views::SettingsLeftView>    m_SettingsLeftView;
            _v::MonoPtr<Views::SettingsMainView>    m_SettingsMainView;
            _v::MonoPtr<Views::SettingsRightView>   m_SettingsRightView;

        private:
            /// @brief Init
            void Init_Impl();

        private:
            /// @brief Get initial views controller
            /// @return (Main, Left, Right)
            t_InitialViews GetInitialViewsController_Impl();

        private:
            /// @brief On back button pressed
            /// @param p_MainViewController Current main view controller
            /// @return True if the event is catched, false if we should dismiss the flow coordinator
            bool OnBackButtonPressed_Impl(_v::CMonoPtrRef<IViewController> p_MainViewController);

        public:
            /// @brief Switch to main view
            void SwitchToMainView();
            /// @brief Switch to settings view
            void SwitchToSettingsView();

    };

}   ///< namespace CP_SDK::UI::FlowCoordinators

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::FlowCoordinators::MainFlowCoordinator);