#include "CP_SDK/UI/DefaultComponents/DefaultCTextInput.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/Button.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCTextInput);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCTextInput)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);

        m_OnValueChanged    = {this, &DefaultCTextInput::OnValueChanged_Impl};

        m_GetValue          = {this, &DefaultCTextInput::GetValue_Impl};

        m_SetInteractable   = {this, &DefaultCTextInput::SetInteractable_Impl};
        m_SetIsPassword     = {this, &DefaultCTextInput::SetIsPassword_Impl};
        m_SetPlaceHolder    = {this, &DefaultCTextInput::SetPlaceHolder_Impl};
        m_SetValue          = {this, &DefaultCTextInput::SetValue_Impl};

        Init += {this, &DefaultCTextInput::Init_DefaultCTextInput};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCTextInput)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCTextInput::Init_DefaultCTextInput()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);
        m_RTransform->set_sizeDelta(Vector2(60.0f, 5.50f));

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minWidth       (40.0f);
        m_LElement->set_minHeight      (5.0f);
        m_LElement->set_preferredHeight(5.0f);
        m_LElement->set_flexibleWidth  (150.0f);

        m_BG = GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        m_BG->get_gameObject()->set_layer(UISystem::UILayer);
        m_BG->get_rectTransform()->SetParent           (get_transform(), false);
        m_BG->get_rectTransform()->set_anchorMin       (Vector2::get_zero());
        m_BG->get_rectTransform()->set_anchorMax       (Vector2::get_one());
        m_BG->get_rectTransform()->set_anchoredPosition(Vector2::get_zero());
        m_BG->get_rectTransform()->set_sizeDelta       (Vector2::get_zero());
        m_BG->set_material                     (UISystem::Override_GetUIMaterial());
        m_BG->set_color                        (UISystem::PrimaryColor);
        m_BG->set_type                         (Image::Type::Sliced);
        m_BG->set_pixelsPerUnitMultiplier      (1);
        m_BG->set_sprite                       (UISystem::GetUIRoundBGSprite().Ptr());

        m_Icon = UISystem::TextFactory->Create(u"Icon", m_RTransform.Ptr());
        m_Icon->SetText(u"📝");
        m_Icon->SetMargins(0, 0, 0, 0);
        m_Icon->SetAlign(TMPro::TextAlignmentOptions::Midline);
        m_Icon->RTransform()->set_pivot             (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchorMin         (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchorMax         (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchoredPosition  (Vector2( 0.0f,  0.0f));
        m_Icon->RTransform()->set_sizeDelta         (Vector2( 5.0f,  5.0f));

        m_ValueText = UISystem::TextFactory->Create(u"Value", m_RTransform.Ptr());
        m_ValueText->SetMargins(1, 0, 1, 0);
        m_ValueText->SetOverflowMode(TMPro::TextOverflowModes::Ellipsis);
        m_ValueText->SetAlign(TMPro::TextAlignmentOptions::MidlineLeft);
        m_ValueText->RTransform()->set_anchorMin         (Vector2( 0.0f,  0.0f));
        m_ValueText->RTransform()->set_anchorMax         (Vector2( 1.0f,  1.0f));
        m_ValueText->RTransform()->set_anchoredPosition  (Vector2(-2.5f,  0.0f));
        m_ValueText->RTransform()->set_sizeDelta         (Vector2(-5.0f,  0.0f));

        m_Button = get_gameObject()->AddComponent<Button*>();
        m_Button->set_targetGraphic (m_BG.Ptr());
        m_Button->set_transition    (Selectable::Transition::ColorTint);
        //m_Button->get_onClick()->RemoveAllListeners();
        m_Button->get_onClick()->AddListener(MakeUnityAction(std::bind(&DefaultCTextInput::Button_OnClick, this)));

        auto l_Colors = m_Button->get_colors();
        l_Colors.set_normalColor        (ColorU::Convert(Color32(0, 255, 255, 255, 127)));
        l_Colors.set_highlightedColor   (ColorU::Convert(Color32(0, 255, 255, 255, 255)));
        l_Colors.set_pressedColor       (ColorU::Convert(Color32(0, 200, 200, 200, 255)));
        l_Colors.set_selectedColor      (l_Colors.get_normalColor());
        l_Colors.set_disabledColor      (ColorU::Convert(Color32(0, 255, 255, 255,  48)));
        l_Colors.set_fadeDuration       (0.05f);
        m_Button->set_colors(l_Colors);

        Refresh();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCTextInput::OnValueChanged_Impl(_v::CActionRef<std::u16string_view> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnValueChangedEvent += p_Functor;
        else        m_OnValueChangedEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get text
    std::u16string_view DefaultCTextInput::GetValue_Impl()
    {
        return m_Value;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set button interactable state
    /// @param p_Interactable New state
    void DefaultCTextInput::SetInteractable_Impl(bool p_Interactable)
    {
        m_Button->set_interactable(p_Interactable);

        m_ValueText->SetColor(p_Interactable ? UISystem::TextColor : UISystem::TextColorDisabled);
        m_Icon->SetColor(ColorU::WithAlpha(Color::get_white(), p_Interactable ? 1.0f : 0.5f));
    }
    /// @brief Set is password
    /// @param p_IsPassword Is password?
    void DefaultCTextInput::SetIsPassword_Impl(bool p_IsPassword)
    {
        m_IsPassword = p_IsPassword;
        Refresh();
    }
    /// @brief Set place holder
    /// @param p_PlaceHolder New place holder
    void DefaultCTextInput::SetPlaceHolder_Impl(std::u16string_view p_PlaceHolder)
    {
        m_PlaceHolder = p_PlaceHolder;
        Refresh();
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    void DefaultCTextInput::SetValue_Impl(std::u16string_view p_Value, bool p_Notify)
    {
        m_Value = p_Value;
        Refresh();

        if (p_Notify)
            Notify();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Refresh displayed value
    void DefaultCTextInput::Refresh()
    {
        m_Icon->SetAlign(TMPro::TextAlignmentOptions::Midline);

        if (m_Value.empty())
        {
            m_ValueText->TMProUGUI()->set_text("<i><alpha=#AA>" + m_PlaceHolder);
            return;
        }

        if (m_IsPassword)
            m_ValueText->TMProUGUI()->set_text("******");
        else
            m_ValueText->TMProUGUI()->set_text(m_Value);
    }
    /// @brief Notify
    void DefaultCTextInput::Notify()
    {
        try { m_OnValueChangedEvent(m_Value); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCTextInput.Notify] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief On click unity callback
    void DefaultCTextInput::Button_OnClick()
    {
        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);

        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        l_ViewController->ShowKeyboardModal(m_Value, [this](std::u16string_view x) -> void {
            m_Value = x;
            Refresh();

            Notify();
        });
    }

}   ///< namespace CP_SDK::UI::DefaultComponents