#include "CP_SDK/UI/FlowCoordinators/MainFlowCoordinator.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;

namespace CP_SDK::UI::FlowCoordinators {

    CP_SDK_IL2CPP_INHERIT_INIT(MainFlowCoordinator);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(MainFlowCoordinator, FlowCoordinator)
    {
        if (!m_TitleOverride.empty())
            m_TitleValue = u"ChatPlexSDK " + ChatPlexSDK::ProductVersion();
        else
            m_TitleValue = m_TitleOverride + u" " + ChatPlexSDK::ProductVersion();

        m_Title                     = [this]() -> std::u16string_view { return m_TitleValue; };

        m_Init                      = {this, &MainFlowCoordinator::Init_Impl};
        m_GetInitialViewsController = {this, &MainFlowCoordinator::GetInitialViewsController_Impl};
        m_OnBackButtonPressed       = {this, &MainFlowCoordinator::OnBackButtonPressed_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(MainFlowCoordinator, FlowCoordinator)
    {
        UISystem::DestroyUI(&m_SettingsRightView);
        UISystem::DestroyUI(&m_SettingsMainView);
        UISystem::DestroyUI(&m_SettingsLeftView);

        UISystem::DestroyUI(&m_MainRightView);
        UISystem::DestroyUI(&m_MainMainView);
        UISystem::DestroyUI(&m_MainLeftView);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string MainFlowCoordinator::m_TitleOverride;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Override the title
    /// @param p_Override New override
    void MainFlowCoordinator::OverrideTitle(std::u16string_view p_Override)
    {
        m_TitleOverride = p_Override;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    void MainFlowCoordinator::Init_Impl()
    {
        m_MainLeftView   = UISystem::CreateViewController<Views::MainLeftView*>();
        m_MainMainView   = UISystem::CreateViewController<Views::MainMainView*>();
        m_MainRightView  = UISystem::CreateViewController<Views::MainRightView*>();

        m_SettingsLeftView  = UISystem::CreateViewController<Views::SettingsLeftView*>();
        m_SettingsMainView  = UISystem::CreateViewController<Views::SettingsMainView*>();
        m_SettingsRightView = UISystem::CreateViewController<Views::SettingsRightView*>();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get initial views controller
    /// @return (Main, Left, Right)
    MainFlowCoordinator::t_InitialViews MainFlowCoordinator::GetInitialViewsController_Impl()
    {
        return { m_MainMainView.Ptr(), m_MainLeftView.Ptr(), m_MainRightView.Ptr() };
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On back button pressed
    /// @param p_MainViewController Current main view controller
    /// @return True if the event is catched, false if we should dismiss the flow coordinator
    bool MainFlowCoordinator::OnBackButtonPressed_Impl(_v::CMonoPtrRef<IViewController> p_MainViewController)
    {
        if (p_MainViewController != m_MainMainView)
        {
            SwitchToMainView();
            return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Switch to main view
    void MainFlowCoordinator::SwitchToMainView()
    {
        ChangeViewControllers(m_MainMainView.Ptr(), m_MainLeftView.Ptr(), m_MainRightView.Ptr());
    }
    /// @brief Switch to settings view
    void MainFlowCoordinator::SwitchToSettingsView()
    {
        ChangeViewControllers(m_SettingsMainView.Ptr(), m_SettingsLeftView.Ptr(), m_SettingsRightView.Ptr());
    }

}   ///< namespace CP_SDK::UI::FlowCoordinators
