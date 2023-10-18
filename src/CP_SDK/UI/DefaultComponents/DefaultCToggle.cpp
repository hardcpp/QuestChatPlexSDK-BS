#include "CP_SDK/UI/DefaultComponents/DefaultCToggle.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/Time.hpp>
#include <UnityEngine/UI/Toggle_ToggleEvent.hpp>
#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector3.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCToggle);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(DefaultCToggle, Components::CToggle)
    {
        m_PreventChange = false;

        m_SwitchAnimationSmooth     = 16.0f;
        m_HorizontalStretchAmount   = 0.8f;
        m_VerticalStretchAmount     = 0.2f;

        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(Toggle,     *(_v::MonoPtr<UnityEngine::UI::Toggle>*)(&m_Toggle));

        m_OnValueChanged    = {this, &DefaultCToggle::OnValueChanged_Impl};

        m_GetValue          = {this, &DefaultCToggle::GetValue_Impl};

        m_SetValue          = {this, &DefaultCToggle::SetValue_Impl};

        Init += {this, &DefaultCToggle::Init_DefaultCToggle};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(DefaultCToggle, Components::CToggle)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCToggle::Init_DefaultCToggle()
    {
        if (m_RTransform)
            return;

        m_OnColors = ColorBlock(
            ColorU::WithAlpha(UISystem::PrimaryColor, 200.0f / 255.0f),
            ColorU::WithAlpha(UISystem::PrimaryColor, 100.0f / 255.0f)
        );
        m_OffColors = ColorBlock(
            ColorU::WithAlpha(UISystem::SecondaryColor, 200.0f / 255.0f),
            ColorU::WithAlpha(UISystem::SecondaryColor, 100.0f / 255.0f)
        );
        m_OnHighlightedColors = ColorBlock(
            ColorU::WithAlpha(UISystem::PrimaryColor, 255.0f / 255.0f),
            ColorU::WithAlpha(UISystem::PrimaryColor, 150.0f / 255.0f)
        );
        m_OffHighlightedColors = ColorBlock(
            ColorU::WithAlpha(UISystem::SecondaryColor, 255.0f / 255.0f),
            ColorU::WithAlpha(UISystem::SecondaryColor, 150.0f / 255.0f)
        );
        m_DisabledColors = ColorBlock(
            ColorU::Convert(Color32(  0,   0,   0,  64)),
            ColorU::Convert(Color32(  0,   0,   0,  68))
        );

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_sizeDelta(Vector2(15.0f, 5.0f));

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_preferredWidth (15.0f);
        m_LElement->set_preferredHeight( 5.0f);
        m_LElement->set_minWidth       (15.0f);
        m_LElement->set_minHeight      ( 5.0f);

        auto l_View = reinterpret_cast<RectTransform*>(GameObject::New_ctor("View", ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)) }))->get_transform());
        l_View->get_gameObject()->set_layer(UISystem::UILayer);
        l_View->SetParent(get_transform(), false);
        l_View->set_anchorMin        (0.5f * Vector2::get_one());
        l_View->set_anchorMax        (0.5f * Vector2::get_one());
        l_View->set_sizeDelta        (Vector2(15.0f, 5.0f));

        m_Toggle = l_View->get_gameObject()->AddComponent<Subs::SubToggleWithCallbacks*>();
        ///m_Toggle->onValueChanged->RemoveAllListeners();
        m_Toggle->onValueChanged->AddListener(MakeUnityAction<bool>(this, il2cpp_functions::class_get_method_from_name(this->klass, "Toggle_onValueChanged", 1)));
        m_Toggle->StateDidChangeEvent += {this, &DefaultCToggle::Toggle_StateDidChange};

        m_BackgroundImage = reinterpret_cast<Image*>(GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_BackgroundImage->get_gameObject()->set_layer(UISystem::UILayer);
        m_BackgroundImage->get_rectTransform()->SetParent(l_View, false);
        m_BackgroundImage->get_rectTransform()->set_anchorMin(0.5f * Vector2::get_one());
        m_BackgroundImage->get_rectTransform()->set_anchorMax(0.5f * Vector2::get_one());
        m_BackgroundImage->get_rectTransform()->set_sizeDelta(Vector2(15.0f, 5.0f));
        m_BackgroundImage->set_sprite     (UISystem::GetUIRoundBGSprite().Ptr());
        m_BackgroundImage->set_color      (Color(0.0f, 0.0f, 0.0f, 0.5f));
        m_BackgroundImage->set_type       (Image::Type::Sliced);
        m_BackgroundImage->set_material   (UISystem::Override_GetUIMaterial());

        m_OffText = UISystem::TextFactory->Create(u"Off", m_BackgroundImage->get_rectTransform());
        m_OffText->SetText(u"0")->SetAlign(TMPro::TextAlignmentOptions::Capline);
        m_OffText->SetAlpha(0.5f);
        m_OffText->RTransform()->set_anchorMin      (Vector2( 0.50f, 0.00f));
        m_OffText->RTransform()->set_anchorMax      (Vector2( 0.50f, 1.00f));
        m_OffText->RTransform()->set_sizeDelta      (Vector2( 6.00f, 0.00f));
        m_OffText->RTransform()->set_localPosition  (Vector3( 3.25f, 0.00f, 0.00f));

        m_OnText = UISystem::TextFactory->Create(u"On", m_BackgroundImage->get_rectTransform());
        m_OnText->SetText(u"I")->SetAlign(TMPro::TextAlignmentOptions::Capline);
        m_OnText->RTransform()->set_anchorMin    (Vector2( 0.50f, 0.00f));
        m_OnText->RTransform()->set_anchorMax    (Vector2( 0.50f, 1.00f));
        m_OnText->RTransform()->set_sizeDelta    (Vector2( 6.00f, 0.00f));
        m_OnText->RTransform()->set_localPosition(Vector3(-3.25f, 0.00f, 0.00f));

        auto l_Knob = reinterpret_cast<RectTransform*>(GameObject::New_ctor("Knob", ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)) }))->get_transform());
        l_Knob->get_gameObject()->set_layer(UISystem::UILayer);
        l_Knob->SetParent(m_BackgroundImage->get_rectTransform(), false);
        l_Knob->set_anchorMin    (Vector2( 0.50f, 0.0f));
        l_Knob->set_anchorMax    (Vector2( 0.50f, 1.0f));
        l_Knob->set_sizeDelta    (Vector2( 7.50f, 0.0f));
        l_Knob->set_localPosition(Vector3( 0.00f, 0.0f, 0.00f));

        m_KnobImage = reinterpret_cast<Image*>(GameObject::New_ctor("Image", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_KnobImage->get_gameObject()->set_layer(UISystem::UILayer);
        m_KnobImage->get_rectTransform()->SetParent(l_Knob, false);
        m_KnobImage->get_rectTransform()->set_anchorMin    (Vector2( 0.00f, 0.5f));
        m_KnobImage->get_rectTransform()->set_anchorMax    (Vector2( 0.00f, 0.5f));
        m_KnobImage->get_rectTransform()->set_sizeDelta    (Vector2( 6.55f, 4.0f));
        m_KnobImage->get_rectTransform()->set_localPosition(Vector3(-3.75f, 0.0f, 0.0f));
        m_KnobImage->set_sprite          (UISystem::GetUIRoundBGSprite().Ptr());
        m_KnobImage->set_color           (Color(0.0f, 0.0f, 0.0f, 0.5f));
        m_KnobImage->set_type            (Image::Type::Sliced);
        m_KnobImage->set_material        (UISystem::Override_GetUIMaterial());
        m_KnobImage->set_raycastTarget   (false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Component first frame
    void DefaultCToggle::Start()
    {
        m_SwitchAmount      = (m_Toggle->get_isOn() ? 1.0f : 0.0f);

        m_HighlightAmount   = 0.0f;
        m_DisabledAmount    = (m_Toggle->IsInteractable() ? 0.0f : 1.0f);

        m_AnimationState    = AnimationState::Idle;

        m_OriginalKnobWidth     = m_KnobImage->get_rectTransform()->get_sizeDelta().x;
        m_OriginalKnobHeight    = m_KnobImage->get_rectTransform()->get_sizeDelta().y;

        LerpColors(m_SwitchAmount, m_HighlightAmount, m_DisabledAmount);
        LerpPosition(m_SwitchAmount);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    /// @brief On frame
    void DefaultCToggle::Update()
    {
        if (m_AnimationState == AnimationState::Idle)
        {
            set_enabled(false);
            return;
        }

        if ((m_AnimationState & AnimationState::SwitchingOn) != 0)
        {
            m_SwitchAmount = Mathf::Lerp(m_SwitchAmount, 1.0f, Time::get_deltaTime() * m_SwitchAnimationSmooth);
            if (m_SwitchAmount >= 0.99f)
            {
                m_SwitchAmount = 1.0f;
                m_AnimationState &= ~AnimationState::SwitchingOn;
            }
        }
        else if ((m_AnimationState & AnimationState::SwitchingOff) != 0)
        {
            m_SwitchAmount = Mathf::Lerp(m_SwitchAmount, 0.0f, Time::get_deltaTime() * m_SwitchAnimationSmooth);
            if (m_SwitchAmount <= 0.01f)
            {
                m_SwitchAmount = 0.0f;
                m_AnimationState &= ~AnimationState::SwitchingOff;
            }
        }

        if ((m_AnimationState & AnimationState::HighlightingOn) != 0 && m_DisabledAmount <= 0.0f)
        {
            m_HighlightAmount = 1.0f;
            m_AnimationState &= ~AnimationState::HighlightingOn;
        }
        else if ((m_AnimationState & AnimationState::HighlightingOff) != 0)
        {
            m_HighlightAmount = 0.0f;
            m_AnimationState &= ~AnimationState::HighlightingOff;
        }

        if ((m_AnimationState & AnimationState::DisablingOn) != 0)
        {
            m_DisabledAmount = 1.0f;
            m_AnimationState &= ~AnimationState::DisablingOn;
        }
        else if ((m_AnimationState & AnimationState::DisablingOff) != 0)
        {
            m_DisabledAmount = 0.0f;
            m_AnimationState &= ~AnimationState::DisablingOff;
        }

        LerpColors(m_SwitchAmount, m_HighlightAmount, m_DisabledAmount);
        LerpPosition(m_SwitchAmount);

        if (!Mathf::Approximately(0.0f, m_HorizontalStretchAmount))
            LerpStretch(m_SwitchAmount);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCToggle::OnValueChanged_Impl(_v::CActionRef<bool> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnChange += p_Functor;
        else        m_OnChange -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get text
    bool DefaultCToggle::GetValue_Impl()
    {
        return m_Toggle->get_isOn();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    void DefaultCToggle::SetValue_Impl(bool p_Value, bool p_Notify)
    {
        m_PreventChange = !p_Notify;
        m_Toggle->set_isOn(p_Value);
        m_PreventChange = false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Toggle event onValueChanged
    /// @param p_Value
    void DefaultCToggle::Toggle_onValueChanged(bool p_Value)
    {
        if (p_Value)
        {
            m_AnimationState |= AnimationState::SwitchingOn;
            m_AnimationState &= ~AnimationState::SwitchingOff;
        }
        else
        {
            m_AnimationState |= AnimationState::SwitchingOff;
            m_AnimationState &= ~AnimationState::SwitchingOn;
        }

        set_enabled(true);

        try { if (!m_PreventChange) m_OnChange(p_Value); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCToggle.Toggle_onValueChanged] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);
    }
    /// @brief Toggle event stateDidChangeEvent
    /// @param p_SelectionState New state
    void DefaultCToggle::Toggle_StateDidChange(Subs::SubToggleWithCallbacks::ESelectionState p_SelectionState)
    {
        if (p_SelectionState == Subs::SubToggleWithCallbacks::ESelectionState::Disabled)
        {
            m_AnimationState |= AnimationState::DisablingOn;
            m_AnimationState &= ~AnimationState::DisablingOff;
        }
        else
        {
            m_AnimationState |= AnimationState::DisablingOff;
            m_AnimationState &= ~AnimationState::DisablingOn;
        }

        if (   p_SelectionState == Subs::SubToggleWithCallbacks::ESelectionState::Highlighted
            || p_SelectionState == Subs::SubToggleWithCallbacks::ESelectionState::Pressed
            || p_SelectionState == Subs::SubToggleWithCallbacks::ESelectionState::Selected)
        {
            if (m_DisabledAmount <= 0.0f || (m_AnimationState & AnimationState::DisablingOff) != 0)
            {
                m_AnimationState |= AnimationState::HighlightingOn;
                m_AnimationState &= ~AnimationState::HighlightingOff;
            }
        }
        else
        {
            m_AnimationState |= AnimationState::HighlightingOff;
            m_AnimationState &= ~AnimationState::HighlightingOn;
        }

        set_enabled(true);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Lerp position
    /// @param p_SwitchAmount Switch amount
    void DefaultCToggle::LerpPosition(float p_SwitchAmount)
    {
        auto l_AnchorMin = m_KnobImage->get_rectTransform()->get_anchorMin();
        auto l_AnchorMax = m_KnobImage->get_rectTransform()->get_anchorMax();

        l_AnchorMin.x = p_SwitchAmount;
        l_AnchorMax.x = p_SwitchAmount;

        m_KnobImage->get_rectTransform()->set_anchorMin(l_AnchorMin);
        m_KnobImage->get_rectTransform()->set_anchorMax(l_AnchorMax);
    }
    /// @brief Lerp stretching
    /// @param p_SwitchAmount Switch amount
    void DefaultCToggle::LerpStretch(float p_SwitchAmount)
    {
        auto l_Factor    = 1.0f - std::abs(p_SwitchAmount - 0.5f) * 2.0f;
        auto l_Width     = m_OriginalKnobWidth  * (1.0f + m_HorizontalStretchAmount * l_Factor);
        auto l_Height    = m_OriginalKnobHeight * (1.0f - m_VerticalStretchAmount   * l_Factor);

        auto l_Size = m_KnobImage->get_rectTransform()->get_sizeDelta();
        l_Size.x = l_Width;
        l_Size.y = l_Height;
        m_KnobImage->get_rectTransform()->set_sizeDelta(l_Size);
    }
    /// @brief Lerp all colors
    /// @param p_SwitchAmount    Switch amount
    /// @param p_HighlightAmount Highlight amount
    /// @param p_DisabledAmount  Disabled amount
    void DefaultCToggle::LerpColors(float p_SwitchAmount, float p_HighlightAmount, float p_DisabledAmount)
    {
        m_BackgroundImage->set_color(LerpColor(p_SwitchAmount, p_HighlightAmount, p_DisabledAmount, [](ColorBlock& x) -> Color { return x.BackgroundColor; }));
        m_KnobImage->set_color      (LerpColor(p_SwitchAmount, p_HighlightAmount, p_DisabledAmount, [](ColorBlock& x) -> Color { return x.KnobColor;       }));

        m_OnText->SetAlpha(p_SwitchAmount);
        m_OffText->SetAlpha((1.0f - p_SwitchAmount) * 0.5f);
    }
    /// @brief Lerp specific color
    /// @param p_SwitchAmount    Switch amount
    /// @param p_HighlightAmount Highlight amount
    /// @param p_DisabledAmount  Disabled amount
    /// @param p_Delegate        Get specific sub color delegate
    Color DefaultCToggle::LerpColor(float p_SwitchAmount, float p_HighlightAmount, float p_DisabledAmount, const _v::Delegate<Color(ColorBlock&)>& p_Delegate)
    {
        auto l_A = Color::Lerp(p_Delegate(m_OffColors),              p_Delegate(m_OnColors),              p_SwitchAmount);
        auto l_B = Color::Lerp(p_Delegate(m_OffHighlightedColors),   p_Delegate(m_OnHighlightedColors),   p_SwitchAmount);

        return Color::Lerp(Color::Lerp(l_A, l_B, p_HighlightAmount), p_Delegate(m_DisabledColors), p_DisabledAmount);
    }

}   ///< namespace CP_SDK::UI::DefaultComponents