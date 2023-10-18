#include "CP_SDK/UI/Components/Generics/CPOrSButton.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CPOrSButton);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CPOrSButton)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CPOrSButton)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On click event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CPOrSButton* CPOrSButton::OnClick(_v::CActionRef<> p_Functor, bool p_Add)
    {
        m_OnClick(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Get text
    std::u16string CPOrSButton::GetText()
    {
        return TextC()->GetText();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set preferred width
    /// @param p_Width Width
    CPOrSButton* CPOrSButton::SetWidth(float p_Width)
    {
        RTransform()->set_sizeDelta(Vector2(p_Width, RTransform()->get_sizeDelta().y));
        LElement()->set_preferredWidth(p_Width);
        return this;
    }
    /// @brief Set preferred height
    /// @param p_Height Height
    CPOrSButton* CPOrSButton::SetHeight(float p_Height)
    {
        RTransform()->set_sizeDelta(Vector2(RTransform()->get_sizeDelta().x, p_Height));
        LElement()->set_preferredHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set background color
    /// @param p_Color New background color
    CPOrSButton* CPOrSButton::SetBackgroundColor(Color p_Color)
    {
        BackgroundImageC()->set_color(p_Color);
        return this;
    }
    /// @brief Set background sprite
    /// @param p_Sprite New sprite
    CPOrSButton* CPOrSButton::SetBackgroundSprite(Sprite* p_Sprite)
    {
        BackgroundImageC()->set_sprite(p_Sprite);
        BackgroundImageC()->get_gameObject()->SetActive(p_Sprite != nullptr);
        return this;
    }
    /// @brief Set font size
    /// @param p_Size New size
    CPOrSButton* CPOrSButton::SetFontSize(float p_Size)
    {
        TextC()->SetFontSize(p_Size);
        return this;
    }
    /// @brief Set theme color
    /// @param p_Color New color
    CPOrSButton* CPOrSButton::SetColor(Color p_Color)
    {
        BackgroundImageC()->set_color(p_Color);
        return this;
    }
    /// @brief Set button icon sprite
    /// @param p_Sprite New sprite
    CPOrSButton* CPOrSButton::SetIconSprite(Sprite* p_Sprite)
    {
        IconImageC()->set_sprite(p_Sprite);
        IconImageC()->get_gameObject()->SetActive(p_Sprite != nullptr);
        return this;
    }
    /// @brief Set button interactable state
    /// @param p_Interactable New state
    CPOrSButton* CPOrSButton::SetInteractable(bool p_Interactable)
    {
        ButtonC()->set_interactable(p_Interactable);
        return this;
    }
    /// @brief Set overflow mode
    /// @param p_OverflowMode New overflow mdoe
    CPOrSButton* CPOrSButton::SetOverflowMode(TextOverflowModes p_OverflowMode)
    {
        TextC()->SetOverflowMode(p_OverflowMode);
        return this;
    }
    /// @brief Set button text
    /// @param p_Text New text
    CPOrSButton* CPOrSButton::SetText(std::u16string_view p_Text)
    {
        TextC()->SetText(p_Text);
        return this;
    }
    /// @brief Set tooltip
    /// @param p_Tooltip New tooltip
    CPOrSButton* CPOrSButton::SetTooltip(std::u16string_view p_Tooltip)
    {
        m_SetTooltip(p_Tooltip);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components