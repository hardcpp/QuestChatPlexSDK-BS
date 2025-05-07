#include "CP_SDK/UI/DefaultComponents/DefaultCFloatingPanel.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/AdditionalCanvasShaderChannels.hpp>
#include <UnityEngine/Canvas.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/CanvasScaler.hpp>
#include <UnityEngine/Quaternion.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCFloatingPanel);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCFloatingPanel)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);

        m_OnGrab    = {this, &DefaultCFloatingPanel::OnGrab_Impl};
        m_OnRelease = {this, &DefaultCFloatingPanel::OnRelease_Impl};

        Init += {this, &DefaultCFloatingPanel::Init_DefaultCFloatingPanel};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCFloatingPanel)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCFloatingPanel::Init_DefaultCFloatingPanel()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);
        m_RTransform->set_localPosition(Vector3::get_zero());
        m_RTransform->set_localRotation(Quaternion::get_identity());
        m_RTransform->set_localScale   (Vector3(0.02f, 0.02f, 0.02f));

        auto l_Canvas = get_gameObject()->AddComponent<Canvas*>();
        l_Canvas->set_additionalShaderChannels(AdditionalCanvasShaderChannels::TexCoord1.value__ | AdditionalCanvasShaderChannels::TexCoord2.value__);
        l_Canvas->set_sortingOrder            (3);

        auto l_CanvasScaler = get_gameObject()->AddComponent<CanvasScaler*>();
        l_CanvasScaler->set_dynamicPixelsPerUnit  (3.44f);
        l_CanvasScaler->set_referencePixelsPerUnit(10.0f);

        get_gameObject()->AddComponent<RectMask2D*>();

        SetBackground(true);
        SetSize(Vector2(20.0f, 20.0f));

        try { UISystem::OnScreenCreated(this); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCFloatingPanel.Init_DefaultCFloatingPanel] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief When grabbed
    void DefaultCFloatingPanel::FireOnGrab()
    {
        try { m_OnGrabEvent(this); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCFloatingPanel.FireOnGrab] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief When released
    void DefaultCFloatingPanel::FireOnRelease()
    {
        try { m_OnReleaseEvent(this); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCFloatingPanel.FireOnRelease] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On grab event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCFloatingPanel::OnGrab_Impl(_v::CActionRef<_v::CMonoPtrRef<Components::CFloatingPanel>> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnGrabEvent += p_Functor;
        else        m_OnGrabEvent -= p_Functor;
    }
    /// @brief On release event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCFloatingPanel::OnRelease_Impl(_v::CActionRef<_v::CMonoPtrRef<Components::CFloatingPanel>> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnReleaseEvent += p_Functor;
        else        m_OnReleaseEvent -= p_Functor;
    }

}   ///< namespace CP_SDK::UI::DefaultComponents