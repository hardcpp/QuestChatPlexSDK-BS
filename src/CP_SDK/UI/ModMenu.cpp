#include "CP_SDK/UI/ModMenu.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(ModMenu);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ModMenu*                    ModMenu::m_Instance = nullptr;
    std::vector<ModButton::Ptr> ModMenu::m_ModButtons;

    _v::Event<>                        ModMenu::OnCreated;
    _v::Event<const ModButton::Ptr&>   ModMenu::OnModButtonRegistered;
    _v::Event<const ModButton::Ptr&>   ModMenu::OnModButtonChanged;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(ModMenu)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(ScreenContainer,    m_ScreenContainer);
        CP_SDK_UI_IL2CPP_BIND_FIELD(Screen,             m_Screen);
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(ModMenu)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    ModMenu* ModMenu::Instance()
    {
        return m_Instance;
    }
    const std::vector<ModButton::Ptr>& ModMenu::ModButtons()
    {
        return m_ModButtons;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create the screen system
    void ModMenu::Create()
    {
        if (m_Instance)
            return;

        m_Instance = GameObject::New_ctor("[CP_SDK.UI.ModMenu]", ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(ModMenu*)),
        }))->GetComponent<ModMenu*>();
        GameObject::DontDestroyOnLoad(m_Instance->get_gameObject());

        ChatPlexSDK::OnGenericSceneChange += [](EGenericScene p_Scene) -> void {
            if (p_Scene == EGenericScene::Menu)
                return;

            if (m_Instance) m_Instance->Dismiss();
        };

        m_Instance->Dismiss();
    }
    /// @brief Destroy
    void ModMenu::Destroy()
    {
        if (!m_Instance)
            return;

        UISystem::DestroyUI(&m_Instance->m_Screen, &m_Instance->m_View);

        GameObject::Destroy(m_Instance->get_gameObject());
        m_Instance = nullptr;
    }
    /// @brief Register a mod button
    /// @param p_Button Button to register
    void ModMenu::Register(const ModButton::Ptr& p_Button)
    {
        if (std::find(m_ModButtons.begin(), m_ModButtons.end(), p_Button) != m_ModButtons.end())
            return;

        m_ModButtons.push_back(p_Button);
        std::sort(m_ModButtons.begin(), m_ModButtons.end(), [](const ModButton::Ptr& p_Left, const ModButton::Ptr& p_Right) {
            return p_Left->Text() < p_Right->Text();
        });

        if (m_Instance && m_Instance->m_View)
            m_Instance->m_View->Refresh();

        try { OnModButtonRegistered(p_Button); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ModMenu.Register] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief Fire on changed
    /// @param p_Button On button changed
    void ModMenu::FireOnModButtonChanged(const ModButton::Ptr& p_Button)
    {
        if (m_Instance && m_Instance->m_View && m_Instance->m_View->get_gameObject()->get_activeInHierarchy())
            m_Instance->m_View->Refresh();

        try { OnModButtonChanged(p_Button); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ModMenu.FireOnModButtonChanged] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Present the screen system
    void ModMenu::Present()
    {
        if (!m_ScreenContainer)
            Init();

        if (get_gameObject()->get_activeSelf())
            return;

        get_gameObject()->SetActive(true);
    }
    /// @brief Dismiss the screen system
    void ModMenu::Dismiss()
    {
        if (!get_gameObject()->get_activeSelf())
            return;

        get_gameObject()->SetActive(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init the screen system
    void ModMenu::Init()
    {
        get_transform()->set_position  (Vector3::get_zero());
        get_transform()->set_localScale(Vector3(1.5f, 1.5f, 1.5f));

        m_ScreenContainer = GameObject::New_ctor("[CP_SDK.UI.ModMenu.ScreenContainer]")->get_transform();
        m_ScreenContainer->SetParent(get_transform(), false);
        m_ScreenContainer->set_localPosition(Vector3(0.00f, 0.85f, 2.90f));

        m_Screen = UISystem::FloatingPanelFactory->Create(u"[CP_SDK.UI.ModMenu.ScreenContainer.LeftScreen]", m_ScreenContainer.Ptr());
        m_View   = UISystem::CreateViewController<Views::ModMenuView*>();
        m_Screen->SetTransformDirect(Vector3(-2.60f, 0.00f, -0.815f), Vector3(0.00f, -40.00f, 0.00f));
        m_Screen->SetSize(Vector2(120.0f, 80.0f));
        m_Screen->SetRadius(0.0f);
        m_Screen->SetBackground(false);
        m_Screen->SetViewController(m_View.Ptr());

        try { OnCreated(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ModMenu.Init] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        get_gameObject()->SetActive(false);
    }

}   ///< namespace CP_SDK::UI