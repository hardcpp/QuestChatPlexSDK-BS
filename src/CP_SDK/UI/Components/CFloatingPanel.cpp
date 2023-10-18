#include "CP_SDK/UI/Components/CFloatingPanel.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CFloatingPanel);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(CFloatingPanel, IScreen)
    {
        m_AllowMovement         = false;
        m_AutoLockOnSceneSwitch = true;
        m_AlignWithFloor        = true;

        CP_SDK_UI_IL2CPP_BIND_FIELD(CurrentViewController, m_CurrentViewController);

        m_SetViewController = {this, &CFloatingPanel::SetViewController_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(CFloatingPanel, IScreen)
    {
        /// Unbind event
        ChatPlexSDK::OnGenericSceneChange -= {this, &CFloatingPanel::ChatPlexSDK_OnGenericSceneChange};

        /// Discard any view controller
        SetViewController(nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Replace active view controller
    /// @param p_ViewController New view controller
    void CFloatingPanel::SetViewController_Impl(_v::CMonoPtrRef<IViewController> p_ViewController)
    {
        if (p_ViewController && p_ViewController->CurrentScreen().Ptr(false) == this)
            return;

        if (m_CurrentViewController)
            m_CurrentViewController->__Deactivate();

        if (p_ViewController && p_ViewController->CurrentScreen())
            p_ViewController->CurrentScreen()->SetViewController(nullptr);

        m_CurrentViewController = p_ViewController;
        if (m_CurrentViewController) m_CurrentViewController->__Activate(this);

        if (m_Background)   m_Background->get_transform()->SetAsFirstSibling();
        if (m_LockIcon)     m_LockIcon->get_transform()->SetAsLastSibling();
        if (m_GearIcon)     m_GearIcon->get_transform()->SetAsLastSibling();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On grab event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CFloatingPanel* CFloatingPanel::OnGrab(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add)
    {
        m_OnGrab(p_Functor, p_Add);
        return this;
    }
    /// @brief On release event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CFloatingPanel* CFloatingPanel::OnRelease(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add)
    {
        m_OnRelease(p_Functor, p_Add);
        return this;
    }
    /// @brief On scene relocated icon event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CFloatingPanel* CFloatingPanel::OnSceneRelocated(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add)
    {
        if (p_Add) m_OnSceneRelocated += p_Functor;
        else       m_OnSceneRelocated -= p_Functor;

        return this;
    }
    /// @brief On scene relocated icon event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CFloatingPanel* CFloatingPanel::OnSizeChanged(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>, Vector2> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnSizeChanged += p_Functor;
        else        m_OnSizeChanged -= p_Functor;

        return this;
    }
    /// @brief On gear icon event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CFloatingPanel* CFloatingPanel::OnGearIcon(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add)
    {
        if (p_Add) m_OnGearIcon += p_Functor;
        else       m_OnGearIcon -= p_Functor;

        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get allow movement
    bool CFloatingPanel::GetAllowMovement()
    {
        return m_AllowMovement;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set align with floor
    /// @param p_Align Align
    CFloatingPanel* CFloatingPanel::SetAlignWithFloor(bool p_Align)
    {
        m_AlignWithFloor = p_Align;
        return this;
    }
    /// @brief Set allow movements
    /// @param p_Allow Is allowed?
    CFloatingPanel* CFloatingPanel::SetAllowMovement(bool p_Allow)
    {
        m_AllowMovement = p_Allow;
        if (m_LockIcon)
        {
            m_LockIcon->SetSprite(p_Allow ? UISystem::GetUIIconUnlockedSprite().Ptr() : UISystem::GetUIIconLockedSprite().Ptr());
            m_LockIcon->SetColor(GetAllowMovement() ? ColorU::ToUnityColor("#D0FCB3") : Color::get_white());
        }

        m_SetAllowMovement(p_Allow);

        return this;
    }
    /// @brief Set background state
    /// @param p_Enabled Is enabled?
    /// @param p_Color Optional color
    CFloatingPanel* CFloatingPanel::SetBackground(bool p_Enabled, std::optional<Color> p_Color)
    {
        if (p_Enabled)
        {
            if (!m_Background)
            {
                m_Background = reinterpret_cast<Image*>(GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
                m_Background->get_gameObject()->set_layer(UISystem::UILayer);
                m_Background->get_rectTransform()->SetParent           (get_transform(), false);
                m_Background->get_rectTransform()->set_localPosition   (Vector3::get_zero());
                m_Background->get_rectTransform()->set_localRotation   (Quaternion::get_identity());
                m_Background->get_rectTransform()->set_localScale      (Vector3::get_one());
                m_Background->get_rectTransform()->set_anchorMin       (Vector2::get_zero());
                m_Background->get_rectTransform()->set_anchorMax       (Vector2::get_one());
                m_Background->get_rectTransform()->set_sizeDelta       (Vector2::get_zero());
                m_Background->set_raycastTarget                        (true);
                m_Background->set_material                             (UISystem::Override_GetUIMaterial());
            }

            m_Background->set_pixelsPerUnitMultiplier(1);
            m_Background->set_type(Image::Type::Sliced);
            m_Background->set_sprite(UISystem::GetUIRoundBGSprite().Ptr());
            m_Background->set_color(p_Color.has_value() ? p_Color.value() : UISystem::DefaultBGColor);
            m_Background->set_enabled(true);
        }
        else if (m_Background)
        {
            Destroy(m_Background->get_gameObject());
            m_Background = nullptr;
        }

        return this;
    }
    /// @brief Set background color
    /// @param p_Color New background color
    CFloatingPanel* CFloatingPanel::SetBackgroundColor(Color p_Color)
    {
        if (!m_Background)
            return this;

        m_Background->set_color(p_Color);

        return this;
    }
    /// @brief Set background sprite
    /// @param p_Sprite New sprite
    /// @param p_Type Image type
    CFloatingPanel* CFloatingPanel::SetBackgroundSprite(Sprite* p_Sprite, Image::Type p_Type)
    {
        if (!m_Background)
            return this;

        m_Background->set_type  (p_Type);
        m_Background->set_sprite(p_Sprite);

        return this;
    }
    /// @brief Set lock icon mode
    /// @param p_Corner Corner or none
    CFloatingPanel* CFloatingPanel::SetLockIcon(ECorner p_Corner)
    {
        if (p_Corner == ECorner::None)
        {
            if (m_LockIcon)
            {
                GameObject::Destroy(m_LockIcon->get_gameObject());
                m_LockIcon = nullptr;
            }
            return this;
        }

        float l_Width   = 5.0f;
        float l_Height  = 5.0f;

        if (!m_LockIcon)
        {
            m_LockIcon = UISystem::IconButtonFactory->Create(u"LockIcon", get_transform());
            m_LockIcon->RTransform()->SetAsLastSibling();
            m_LockIcon->LElement()->set_enabled(false);
            m_LockIcon->SetWidth(l_Width)->SetHeight(l_Height);
            m_LockIcon->SetSprite(UISystem::GetUIIconLockedSprite().Ptr());
            m_LockIcon->SetColor(GetAllowMovement() ? ColorU::ToUnityColor("#D0FCB3") : Color::get_white());
            m_LockIcon->OnClick([this]() -> void { SetAllowMovement(!GetAllowMovement()); });
        }

        if (p_Corner == ECorner::TopLeft)
        {
            m_LockIcon->RTransform()->set_anchorMin       (Vector2(0.0f, 1.0f));
            m_LockIcon->RTransform()->set_anchorMax       (Vector2(0.0f, 1.0f));
            m_LockIcon->RTransform()->set_anchoredPosition(Vector2(l_Width, -l_Height));
        }
        else if (p_Corner == ECorner::TopRight)
        {
            m_LockIcon->RTransform()->set_anchorMin       (Vector2(1.0f, 1.0f));
            m_LockIcon->RTransform()->set_anchorMax       (Vector2(1.0f, 1.0f));
            m_LockIcon->RTransform()->set_anchoredPosition(Vector2(-l_Width, -l_Height));
        }
        else if (p_Corner == ECorner::BottomLeft)
        {
            m_LockIcon->RTransform()->set_anchorMin       (Vector2(0.0f, 0.0f));
            m_LockIcon->RTransform()->set_anchorMax       (Vector2(0.0f, 0.0f));
            m_LockIcon->RTransform()->set_anchoredPosition(Vector2(l_Width, l_Height));
        }
        else if (p_Corner == ECorner::BottomRight)
        {
            m_LockIcon->RTransform()->set_anchorMin       (Vector2(1.0f, 0.0f));
            m_LockIcon->RTransform()->set_anchorMax       (Vector2(1.0f, 0.0f));
            m_LockIcon->RTransform()->set_anchoredPosition(Vector2(-l_Width, l_Height));
        }

        return this;
    }
    /// @brief Set gear icon mode
    /// @param p_Corner Corner or none
    CFloatingPanel* CFloatingPanel::SetGearIcon(ECorner p_Corner)
    {
        if (p_Corner == ECorner::None)
        {
            if (m_GearIcon)
            {
                GameObject::Destroy(m_GearIcon->get_gameObject());
                m_GearIcon = nullptr;
            }
            return this;
        }

        float l_Width   = 5.0f;
        float l_Height  = 5.0f;

        if (!m_GearIcon)
        {
            m_GearIcon = UISystem::IconButtonFactory->Create(u"GearIcon", get_transform());
            m_GearIcon->RTransform()->SetAsLastSibling();
            m_GearIcon->LElement()->set_enabled(false);
            m_GearIcon->SetWidth(l_Width)->SetHeight(l_Height);
            m_GearIcon->SetSprite(UISystem::GetUIIconGearSprite().Ptr());
            m_GearIcon->OnClick([this]() -> void {
                try { m_OnGearIcon(this); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Components][CFloatingPanel.SetGearIcon] Error:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            });
        }

        if (p_Corner == ECorner::TopLeft)
        {
            m_GearIcon->RTransform()->set_anchorMin       (Vector2(0.0f, 1.0f));
            m_GearIcon->RTransform()->set_anchorMax       (Vector2(0.0f, 1.0f));
            m_GearIcon->RTransform()->set_anchoredPosition(Vector2(l_Width, -l_Height));
        }
        else if (p_Corner == ECorner::TopRight)
        {
            m_GearIcon->RTransform()->set_anchorMin       (Vector2(1.0f, 1.0f));
            m_GearIcon->RTransform()->set_anchorMax       (Vector2(1.0f, 1.0f));
            m_GearIcon->RTransform()->set_anchoredPosition(Vector2(-l_Width, -l_Height));
        }
        else if (p_Corner == ECorner::BottomLeft)
        {
            m_GearIcon->RTransform()->set_anchorMin       (Vector2(0.0f, 0.0f));
            m_GearIcon->RTransform()->set_anchorMax       (Vector2(0.0f, 0.0f));
            m_GearIcon->RTransform()->set_anchoredPosition(Vector2(l_Width, l_Height));
        }
        else if (p_Corner == ECorner::BottomRight)
        {
            m_GearIcon->RTransform()->set_anchorMin       (Vector2(1.0f, 0.0f));
            m_GearIcon->RTransform()->set_anchorMax       (Vector2(1.0f, 0.0f));
            m_GearIcon->RTransform()->set_anchoredPosition(Vector2(-l_Width, l_Height));
        }

        return this;
    }
    /// @brief Set radius on supported games
    /// @param p_Radius Canvas radius
    CFloatingPanel* CFloatingPanel::SetRadius(float p_Radius)
    {
        m_SetRadius(p_Radius);
        return this;
    }
    /// @brief Set on scene release
    /// @param p_Scene Target scene
    /// @param p_Callback Callback
    CFloatingPanel* CFloatingPanel::OnSceneRelease(ChatPlexSDK::EGenericScene p_Scene, const _v::Delegate<void(Vector3, Vector3)>& p_Callback)
    {
        m_OnSceneRelease[p_Scene] = p_Callback;
        return this;
    }
    /// @brief Set scene transform
    /// @param p_Scene Target scene
    /// @param p_LocalPosition Local position
    /// @param p_LocalEulerAngles Local euler angles
    CFloatingPanel* CFloatingPanel::SetSceneTransform(ChatPlexSDK::EGenericScene p_Scene, Vector3 p_LocalPosition, Vector3 p_LocalEulerAngles)
    {
        m_SceneTransforms[p_Scene] = std::make_tuple(p_LocalPosition, p_LocalEulerAngles);

        if (p_Scene == ChatPlexSDK::ActiveGenericScene())
        {
            RTransform()->set_localPosition   (p_LocalPosition);
            RTransform()->set_localEulerAngles(p_LocalEulerAngles);

            try { m_OnSceneRelocated(this); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Components][CFloatingPanel.SetSceneTransform] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }

        return this;
    }
    /// @brief Set size
    /// @param p_Size New size
    CFloatingPanel* CFloatingPanel::SetSize(Vector2 p_Size)
    {
        RTransform()->set_sizeDelta(p_Size);

        try { m_OnSizeChanged(this, p_Size); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Components][CFloatingPanel.SetSize] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        m_SetSize(p_Size);

        return this;
    }
    /// @brief Set transform direct
    /// @param p_LocalPosition Local position
    /// @param p_LocalEulerAngles Local euler angles
    CFloatingPanel* CFloatingPanel::SetTransformDirect(Vector3 p_LocalPosition, Vector3 p_LocalEulerAngles)
    {
        RTransform()->set_localPosition   (p_LocalPosition);
        RTransform()->set_localEulerAngles(p_LocalEulerAngles);

        try { m_OnSceneRelocated(this); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Components][CFloatingPanel.SetTransformDirect] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void CFloatingPanel::Awake()
    {
        /// Bind event
        ChatPlexSDK::OnGenericSceneChange -= {this, &CFloatingPanel::ChatPlexSDK_OnGenericSceneChange};
        ChatPlexSDK::OnGenericSceneChange += {this, &CFloatingPanel::ChatPlexSDK_OnGenericSceneChange};

        OnRelease([this](const _v::MonoPtr<CFloatingPanel>&) -> void
        {
            if (m_AlignWithFloor)
                RTransform()->set_localEulerAngles(Vector3(RTransform()->get_localEulerAngles().x, RTransform()->get_localEulerAngles().y, 0));

            if (m_SceneTransforms.contains(ChatPlexSDK::ActiveGenericScene()))
                SetSceneTransform(ChatPlexSDK::ActiveGenericScene(), RTransform()->get_localPosition(), RTransform()->get_localEulerAngles());

            if (m_OnSceneRelease.contains(ChatPlexSDK::ActiveGenericScene()) && m_OnSceneRelease[ChatPlexSDK::ActiveGenericScene()].IsValid())
            {
                auto& l_OnSceneReleaseCallback = m_OnSceneRelease[ChatPlexSDK::ActiveGenericScene()];
                try { l_OnSceneReleaseCallback(RTransform()->get_localPosition(), RTransform()->get_localEulerAngles()); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Components][CFloatingPanel.OnRelease] Error:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }
        });
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On generic scene change
    /// @param p_ActiveScene New active scene
    void CFloatingPanel::ChatPlexSDK_OnGenericSceneChange(ChatPlexSDK::EGenericScene p_ActiveScene)
    {
        if (m_AutoLockOnSceneSwitch)
            SetAllowMovement(false);

        if (m_SceneTransforms.contains(p_ActiveScene))
        {
            const auto& l_SceneTransform = m_SceneTransforms[p_ActiveScene];

            RTransform()->set_localPosition   (std::get<0>(l_SceneTransform));
            RTransform()->set_localEulerAngles(std::get<1>(l_SceneTransform));

            try { m_OnSceneRelocated(this); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Components][CFloatingPanel.ChatPlexSDK_OnGenericSceneChange] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
    }

}   ///< namespace CP_SDK::UI::Components