#include "CP_SDK/UI/DefaultComponents/DefaultCDropdown.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/Button.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCDropdown);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCDropdown)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);

        m_Value = -1;

        m_OnValueChanged    = {this, &DefaultCDropdown::OnValueChanged_Impl};

        m_GetValue          = {this, &DefaultCDropdown::GetValue_Impl};

        m_SetInteractable   = {this, &DefaultCDropdown::SetInteractable_Impl};
        m_SetOptions        = {this, &DefaultCDropdown::SetOptions_Impl};
        m_SetValue          = {this, &DefaultCDropdown::SetValue_Impl};

        Init += [this]() { Init_DefaultCDropdown(); };
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCDropdown)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCDropdown::Init_DefaultCDropdown()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);
        m_RTransform->set_sizeDelta(Vector2(60.0f, 5.5f));

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minWidth       ( 40.0f);
        m_LElement->set_minHeight      (  5.0f);
        m_LElement->set_preferredHeight(  5.0f);
        m_LElement->set_flexibleWidth  (150.0f);

        m_BG = GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        m_BG->get_gameObject()->set_layer(UISystem::UILayer);
        m_BG->get_rectTransform()->SetParent(get_transform(), false);
        m_BG->get_rectTransform()->set_pivot           (Vector2(0.50f,  0.50f));
        m_BG->get_rectTransform()->set_anchorMin       (Vector2(0.00f,  0.00f));
        m_BG->get_rectTransform()->set_anchorMax       (Vector2(1.00f,  1.00f));
        m_BG->get_rectTransform()->set_anchoredPosition(Vector2(0.00f,  0.00f));
        m_BG->get_rectTransform()->set_sizeDelta       (Vector2(0.00f,  0.00f));
        m_BG->set_material               (UISystem::Override_GetUIMaterial());
        m_BG->set_color                  (UISystem::PrimaryColor);
        m_BG->set_type                   (Image::Type::Sliced);
        m_BG->set_pixelsPerUnitMultiplier(1);
        m_BG->set_sprite                 (UISystem::GetUIRoundBGSprite().Ptr());

        m_Icon = UISystem::IconButtonFactory->Create(u"Icon", get_transform());
        m_Icon->RTransform()->set_pivot           (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchorMin       (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchorMax       (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchoredPosition(Vector2(-0.5f,  0.0f));
        m_Icon->RTransform()->set_sizeDelta       (Vector2( 4.0f,  4.0f));
        m_Icon->SetSprite(UISystem::GetUIDownArrowSprite().Ptr());
        m_Icon->OnClick({this, &DefaultCDropdown::Button_OnClick});

        m_ValueText = UISystem::TextFactory->Create(u"Value", get_transform());
        m_ValueText->RTransform()->set_anchorMin       (Vector2::get_zero());
        m_ValueText->RTransform()->set_anchorMax       (Vector2::get_one());
        m_ValueText->RTransform()->set_anchoredPosition(Vector2(-2.5f, 0));
        m_ValueText->RTransform()->set_sizeDelta       (Vector2(-5.0f, 0));
        m_ValueText->SetMargins(1, 0, 1, 0);
        m_ValueText->SetOverflowMode(TMPro::TextOverflowModes::Ellipsis);
        m_ValueText->SetAlign(TMPro::TextAlignmentOptions::MidlineLeft);

        m_Button = get_gameObject()->AddComponent<Button*>();
        m_Button->set_targetGraphic(m_BG.Ptr());
        m_Button->set_transition   (Selectable::Transition::ColorTint);
        //m_Button->onClick.RemoveAllListeners();
        m_Button->get_onClick()->AddListener(MakeUnityAction(std::bind(&DefaultCDropdown::Button_OnClick, this)));

        auto l_Colors = m_Button->get_colors();
        l_Colors.set_normalColor        (ColorU::Convert(Color32(0, 230, 230, 230, 127)));
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
    void DefaultCDropdown::OnValueChanged_Impl(_v::CActionRef<int, std::u16string_view> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnValueChangeEvent += p_Functor;
        else        m_OnValueChangeEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get value
    std::u16string_view DefaultCDropdown::GetValue_Impl()
    {
        static constexpr std::u16string_view _default = u"<i>None</i>";

        if (m_Value >= 0 && m_Value < m_Options.size())
            return m_Options[m_Value];
        else
            return _default;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set interactable state
    /// @param p_Interactable New state
    void DefaultCDropdown::SetInteractable_Impl(bool p_Interactable)
    {
        m_Button->set_interactable(p_Interactable);

        m_ValueText->SetColor(p_Interactable ? UISystem::TextColor : UISystem::TextColorDisabled);
        m_Icon->SetInteractable(p_Interactable);
    }
    /// @brief Set available options
    /// @param p_Options New options list
    void DefaultCDropdown::SetOptions_Impl(const std::vector<std::u16string>& p_Options)
    {
        m_Options.clear();

        if (!p_Options.empty())
        {
            m_Options.reserve(p_Options.size());
            m_Options.insert(m_Options.begin(), p_Options.begin(), p_Options.end());
        }

        if (m_Value > m_Options.size())
        {
            m_Value = -1;
            Refresh();
            Notify();
        }
        else
            Refresh();
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    void DefaultCDropdown::SetValue_Impl(std::u16string_view p_Value, bool p_Notify)
    {
        auto l_It       = std::find(m_Options.begin(), m_Options.end(), p_Value);
        auto l_NewValue = l_It != m_Options.end() ? l_It - m_Options.begin() : -1;

        m_Value = l_NewValue;
        Refresh();

        if (p_Notify)
            Notify();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Refresh displayed value
    void DefaultCDropdown::Refresh()
    {
        if (m_Value >= 0 && m_Value < m_Options.size())
            m_ValueText->TMProUGUI()->set_text(m_Options[m_Value]);
        else
            m_ValueText->TMProUGUI()->set_text("<i>None</i>");
    }
    /// @brief Notify
    void DefaultCDropdown::Notify()
    {
        static constexpr std::u16string_view _default = u"";

        try { m_OnValueChangeEvent(m_Value, (m_Value >= 0 && m_Value < m_Options.size()) ? m_Options[m_Value] : _default); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCDropdown.Notify] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief On click unity callback
    void DefaultCDropdown::Button_OnClick()
    {
        static std::vector<std::u16string> _default = { u"<i>None</i>" };

        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);

        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        l_ViewController->ShowDropdownModal(m_Options.size() != 0 ? m_Options : _default, GetValue_Impl(), [this](std::u16string_view x) {
            auto l_It       = std::find(m_Options.begin(), m_Options.end(), x);
            auto l_NewValue = l_It != m_Options.end() ? l_It - m_Options.begin() : -1;
            if (l_NewValue != m_Value)
            {
                m_Value = l_NewValue;
                Refresh();
                Notify();
            }
        });
    }

}   ///< namespace CP_SDK::UI::DefaultComponents