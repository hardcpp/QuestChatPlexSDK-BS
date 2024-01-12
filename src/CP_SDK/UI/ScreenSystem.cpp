#include "CP_SDK/UI/ScreenSystem.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(ScreenSystem);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ScreenSystem* ScreenSystem::m_Instance = nullptr;

    _v::Event<> ScreenSystem::OnCreated;
    _v::Event<> ScreenSystem::OnPresent;
    _v::Event<> ScreenSystem::OnDismiss;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(ScreenSystem)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(CurrentFlowCoordinator, m_CurrentFlowCoordinator);
        CP_SDK_UI_IL2CPP_BIND_FIELD(ScreenContainer,        m_ScreenContainer);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LeftScreen,             m_LeftScreen);
        CP_SDK_UI_IL2CPP_BIND_FIELD(TopScreen,              m_TopScreen);
        CP_SDK_UI_IL2CPP_BIND_FIELD(MainScreen,             m_MainScreen);
        CP_SDK_UI_IL2CPP_BIND_FIELD(RightScreen,            m_RightScreen);
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(ScreenSystem)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ScreenSystem* ScreenSystem::Instance()
    {
        return m_Instance;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create the screen system
    void ScreenSystem::Create()
    {
        if (m_Instance)
            return;

        m_Instance = GameObject::New_ctor("[CP_SDK.UI.ScreenSystem]", ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(ScreenSystem*).convert()),
        }))->GetComponent<ScreenSystem*>();
        GameObject::DontDestroyOnLoad(m_Instance->get_gameObject());
    }
    /// @brief Destroy
    void ScreenSystem::Destroy()
    {
        if (!m_Instance)
            return;

        UISystem::DestroyUI(&m_Instance->m_RightScreen);
        UISystem::DestroyUI(&m_Instance->m_MainScreen);
        UISystem::DestroyUI(&m_Instance->m_TopScreen, &m_Instance->m_TopNavigationViewController);
        UISystem::DestroyUI(&m_Instance->m_LeftScreen);

        GameObject::Destroy(m_Instance->get_gameObject());
        m_Instance = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Present the screen system
    void ScreenSystem::Present()
    {
        if (!m_ScreenContainer)
            Init();

        if (get_gameObject()->get_activeSelf())
            return;

        try { OnPresent(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ScreenSystem.Present] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        get_gameObject()->SetActive(true);
    }
    /// @brief Dismiss the screen system
    void ScreenSystem::Dismiss()
    {
        if (!get_gameObject()->get_activeSelf())
            return;

        SetFlowCoordinator(nullptr, false);

        get_gameObject()->SetActive(false);

        try { OnDismiss(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ScreenSystem.Dismiss] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set current flow coordinator
    /// @param p_FlowCoordinator New flow coordinator
    /// @param p_OnPresent       On present?
    void ScreenSystem::SetFlowCoordinator(const _v::MonoPtr<IFlowCoordinator>& p_FlowCoordinator, bool p_OnPresent)
    {
        if (m_CurrentFlowCoordinator)
            m_CurrentFlowCoordinator->__Deactivate();

        m_LeftScreen->SetViewController(nullptr);
        m_MainScreen->SetViewController(nullptr);
        m_RightScreen->SetViewController(nullptr);

        m_CurrentFlowCoordinator = p_FlowCoordinator;

        if (m_CurrentFlowCoordinator && !p_OnPresent)
            m_CurrentFlowCoordinator->__Activate();

        if (m_CurrentFlowCoordinator)
            m_TopNavigationViewController->SetTitle(m_CurrentFlowCoordinator->Title());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init the screen system
    void ScreenSystem::Init()
    {
        get_transform()->set_position  (Vector3::get_zero());
        get_transform()->set_localScale(Vector3(1.5f, 1.5f, 1.5f));

        m_ScreenContainer = GameObject::New_ctor("[CP_SDK.UI.ScreenSystem.ScreenContainer]")->get_transform();
        m_ScreenContainer->SetParent(get_transform(), false);
        m_ScreenContainer->set_localPosition(Vector3(0.00f, 0.85f, 2.90f));

        m_LeftScreen = UISystem::FloatingPanelFactory->Create(u"[CP_SDK.UI.ScreenSystem.ScreenContainer.LeftScreen]", m_ScreenContainer.Ptr());
        m_LeftScreen->SetTransformDirect(Vector3(-2.60f, 0.00f, -0.815f), Vector3(0.00f, -40.00f, 0.00f));
        m_LeftScreen->SetSize(Vector2(120.0f, 80.0f));
        m_LeftScreen->SetRadius(0.0f);
        m_LeftScreen->SetBackground(false);

        m_TopScreen = UISystem::FloatingPanelFactory->Create(u"[CP_SDK.UI.ScreenSystem.ScreenContainer.TopScreen]", m_ScreenContainer.Ptr());
        m_TopScreen->SetTransformDirect(Vector3(0.00f, 0.9f, 0.00f), Vector3(0.00f, 0.00f, 0.00f));
        m_TopScreen->SetSize(Vector2(150.0f, 8.0f));
        m_TopScreen->SetRadius(0.0f);
        m_TopScreen->SetBackground(false);

        m_MainScreen = UISystem::FloatingPanelFactory->Create(u"[CP_SDK.UI.ScreenSystem.ScreenContainer.MainScreen]", m_ScreenContainer.Ptr());
        m_MainScreen->SetTransformDirect(Vector3(0.00f, 0.00f, 0.00f), Vector3(0.00f, 0.00f, 0.00f));
        m_MainScreen->SetSize(Vector2(150.0f, 80.0f));
        m_MainScreen->SetRadius(0.0f);
        m_MainScreen->SetBackground(false);

        m_RightScreen = UISystem::FloatingPanelFactory->Create(u"[CP_SDK.UI.ScreenSystem.ScreenContainer.RightScreen]", m_ScreenContainer.Ptr());
        m_RightScreen->SetTransformDirect(Vector3(2.60f, 0.00f, -0.815f), Vector3(0.00f, 40.00f, 0.00f));
        m_RightScreen->SetSize(Vector2(120.0f, 80.0f));
        m_RightScreen->SetRadius(0.0f);
        m_RightScreen->SetBackground(false);

        m_TopNavigationViewController = UISystem::CreateViewController<Views::TopNavigationView*>();
        m_TopNavigationViewController->OnBackButton += {this, &ScreenSystem::TopNavigationViewController_OnBackButton};
        m_TopScreen->SetViewController(m_TopNavigationViewController.Ptr());

        try { OnCreated(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ScreenSystem.Init] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        get_gameObject()->SetActive(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On back button pressed
    void ScreenSystem::TopNavigationViewController_OnBackButton()
    {
        if (!m_CurrentFlowCoordinator
            || !m_CurrentFlowCoordinator->OnBackButtonPressed(m_MainScreen->CurrentViewController()))
        {
            Dismiss();
            return;
        }

        return;
    }

}   ///< namespace CP_SDK::UI