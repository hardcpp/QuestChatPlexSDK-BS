#include "CP_SDK/UI/Components/CIconButton.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Vector2.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CIconButton);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CIconButton)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CIconButton)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On click event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CIconButton* CIconButton::OnClick(_v::CActionRef<> p_Functor, bool p_Add)
    {
        m_OnClick(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set preferred width
    /// @param p_Width Width
    CIconButton* CIconButton::SetWidth(float p_Width)
    {
        RTransform()->set_sizeDelta(Vector2(p_Width, RTransform()->get_sizeDelta().y));
        LElement()->set_preferredWidth(p_Width);
        return this;
    }
    /// @brief Set preferred height
    /// @param p_Height Height
    CIconButton* CIconButton::SetHeight(float p_Height)
    {
        RTransform()->set_sizeDelta(Vector2(RTransform()->get_sizeDelta().x, p_Height));
        LElement()->set_preferredHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set theme color
    /// @param p_Color New color
    CIconButton* CIconButton::SetColor(Color p_Color)
    {
        IconImageC()->set_color(p_Color);
        return this;
    }
    /// @brief Set button interactable state
    /// @param p_Interactable New state
    CIconButton* CIconButton::SetInteractable(bool p_Interactable)
    {
        ButtonC()->set_interactable(p_Interactable);
        return this;
    }
    /// @brief Set button sprite
    /// @param p_Sprite New sprite
    CIconButton* CIconButton::SetSprite(Sprite* p_Sprite)
    {
        IconImageC()->set_sprite(p_Sprite);
        IconImageC()->get_gameObject()->SetActive(p_Sprite != nullptr);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components