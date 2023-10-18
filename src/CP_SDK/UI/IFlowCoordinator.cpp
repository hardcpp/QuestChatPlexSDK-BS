#include "CP_SDK/UI/IFlowCoordinator.hpp"
#include "CP_SDK/UI/IScreen.hpp"
#include "CP_SDK/UI/ScreenSystem.hpp"

using namespace UnityEngine;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(IFlowCoordinator);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(IFlowCoordinator)
    {
        m_FirstActivation = true;
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(IFlowCoordinator)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string_view IFlowCoordinator::Title()
    {
        if (!m_Title.IsValid())
        {
            static constexpr std::u16string_view _default_Text = u"Default...";
            return _default_Text;
        }

        return m_Title();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Activate
    void IFlowCoordinator::__Activate()
    {
        ChangeViewControllers(m_MainViewController, m_LeftViewController, m_RightViewController);
    }
    /// @brief Deactivate
    void IFlowCoordinator::__Deactivate()
    {
        if (m_LeftViewController && m_LeftViewController->CurrentScreen())
            m_LeftViewController->CurrentScreen()->SetViewController(nullptr);

        if (m_MainViewController && m_MainViewController->CurrentScreen())
            m_MainViewController->CurrentScreen()->SetViewController(nullptr);

        if (m_RightViewController && m_RightViewController->CurrentScreen())
            m_RightViewController->CurrentScreen()->SetViewController(nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    void IFlowCoordinator::Init()
    {
        m_Init();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Present this FlowCoordinator
    /// @param p_IgnoreBackuping Ignore existing flow coordinator
    void IFlowCoordinator::Present(bool p_IgnoreBackuping)
    {
        if (ScreenSystem::Instance()->CurrentFlowCoordinator() == this)
            return;

        m_BackupFlowCoordinator = p_IgnoreBackuping ? nullptr : ScreenSystem::Instance()->CurrentFlowCoordinator();

        ScreenSystem::Instance()->Present();
        ScreenSystem::Instance()->SetFlowCoordinator(this, true);

        if (m_FirstActivation)
        {
            try { Init(); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][IFlowCoordinator.Present] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            auto l_Initial = GetInitialViewsController();
            ChangeViewControllers(l_Initial.Main, l_Initial.Left, l_Initial.Right);
            m_FirstActivation = false;
        }
        else
            ChangeViewControllers(m_MainViewController, m_LeftViewController, m_RightViewController);
    }
    /// @brief Dismiss this FlowCoordinator
    void IFlowCoordinator::Dismiss()
    {
        if (m_BackupFlowCoordinator)
        {
            ScreenSystem::Instance()->SetFlowCoordinator(m_BackupFlowCoordinator, false);
            m_BackupFlowCoordinator = nullptr;
            return;
        }

        ScreenSystem::Instance()->Dismiss();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get initial views controller
    /// @return (Main, Left, Right)
    IFlowCoordinator::t_InitialViews IFlowCoordinator::GetInitialViewsController()
    {
        return m_GetInitialViewsController();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On back button pressed
    /// @param p_MainViewController Current main view controller
    /// @return True if the event is catched, false if we should dismiss the flow coordinator
    bool IFlowCoordinator::OnBackButtonPressed(_v::CMonoPtrRef<IViewController> p_MainViewController)
    {
        if (!m_OnBackButtonPressed.IsValid())
            return false;

        return m_OnBackButtonPressed(p_MainViewController);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Change view controllers
    /// @param p_MainViewController  New main view controller
    /// @param p_LeftViewController  New left view controller
    /// @param p_RightViewController New right view controller
    void IFlowCoordinator::ChangeViewControllers(_v::CMonoPtrRef<IViewController> p_MainViewController, _v::CMonoPtrRef<IViewController> p_LeftViewController, _v::CMonoPtrRef<IViewController> p_RightViewController)
    {
        if (ScreenSystem::Instance()->CurrentFlowCoordinator() != this)
            return;

        SetLeftViewController(p_LeftViewController);
        SetMainViewController(p_MainViewController);
        SetRightViewController(p_RightViewController);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set left view controller
    /// @param p_ViewController New view controller
    void IFlowCoordinator::SetLeftViewController(_v::CMonoPtrRef<IViewController> p_ViewController)
    {
        if (ScreenSystem::Instance()->CurrentFlowCoordinator() != this)
            return;

        m_LeftViewController = p_ViewController;
        try { ScreenSystem::Instance()->LeftScreen()->SetViewController(p_ViewController); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][IFlowCoordinator.SetLeftViewController] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief Set main view controller
    /// @param p_ViewController New view controller
    void IFlowCoordinator::SetMainViewController(_v::CMonoPtrRef<IViewController> p_ViewController)
    {
        if (ScreenSystem::Instance()->CurrentFlowCoordinator() != this)
            return;

        m_MainViewController = p_ViewController;
        try { ScreenSystem::Instance()->MainScreen()->SetViewController(p_ViewController); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][IFlowCoordinator.SetMainViewController] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief Set left view controller
    /// @param p_ViewController New view controller
    void IFlowCoordinator::SetRightViewController(_v::CMonoPtrRef<IViewController> p_ViewController)
    {
        if (ScreenSystem::Instance()->CurrentFlowCoordinator() != this)
            return;

        m_RightViewController = p_ViewController;
        try { ScreenSystem::Instance()->RightScreen()->SetViewController(p_ViewController); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][IFlowCoordinator.SetRightViewController] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI