#include "CP_SDK/UI/Components/Generics/CHOrVLayout.hpp"

#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/Image.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CHOrVLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CHOrVLayout)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CHOrVLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get background fill amount
    float CHOrVLayout::GetBackgroundFillAmount()
    {
        return m_Background ? m_Background->get_fillAmount() : 0.0f;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set background state
    /// @param p_Enabled       Is enabled?
    /// @param p_Color         Optional color, default to black
    /// @param p_RaycastTarget Should raycast target
    CHOrVLayout* CHOrVLayout::SetBackground(bool p_Enabled, std::optional<Color> p_Color, bool p_RaycastTarget)
    {
        if (p_Enabled)
        {
            if (!m_Background)
            {
                m_Background = get_gameObject()->AddComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
                m_Background->set_material(UISystem::Override_GetUIMaterial());
                m_Background->set_raycastTarget(p_RaycastTarget);
            }

            m_Background->set_pixelsPerUnitMultiplier(1);
            m_Background->set_type(Image::Type::Sliced);
            m_Background->set_sprite(UISystem::GetUIRoundBGSprite().Ptr());
            m_Background->set_color(p_Color.has_value() ? p_Color.value() : UISystem::DefaultBGColor);
        }
        else if (m_Background)
        {
            Destroy(m_Background.Ptr());
            m_Background = nullptr;
        }

        return this;
    }
    /// @brief Set background color
    /// @param p_Color New background color
    CHOrVLayout* CHOrVLayout::SetBackgroundColor(Color p_Color)
    {
        if (!m_Background) return this;
        m_Background->set_color(p_Color);

        return this;
    }
    /// @brief Set background fill method
    /// @param p_FillMethod Fill method
    CHOrVLayout* CHOrVLayout::SetBackgroundFillMethod(Image::FillMethod p_FillMethod)
    {
        if (!m_Background) return this;
        m_Background->set_fillMethod(p_FillMethod);

        return this;
    }
    /// @brief Set background fill amount
    /// @param p_FillAmount Fill amount
    CHOrVLayout* CHOrVLayout::SetBackgroundFillAmount(float p_FillAmount)
    {
        if (!m_Background) return this;
        m_Background->set_fillAmount(p_FillAmount);

        return this;
    }
    /// @brief Set background sprite
    /// @param p_Sprite New sprite
    /// @param p_Type   Image type
    CHOrVLayout* CHOrVLayout::SetBackgroundSprite(Sprite* p_Sprite, Image::Type p_Type)
    {
        if (!m_Background) return this;
        m_Background->set_type(p_Type);
        m_Background->set_sprite(p_Sprite);

        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set preferred width
    /// @param p_Width Width
    CHOrVLayout* CHOrVLayout::SetWidth(float p_Width)
    {
        RTransform()->set_sizeDelta(Vector2(p_Width, RTransform()->get_sizeDelta().y));
        LElement()->set_preferredWidth(p_Width);
        return this;
    }
    /// @brief Set preferred height
    /// @param p_Height Height
    CHOrVLayout* CHOrVLayout::SetHeight(float p_Height)
    {
        RTransform()->set_sizeDelta(Vector2(RTransform()->get_sizeDelta().x, p_Height));
        LElement()->set_preferredHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set min width
    /// @param p_Width Width
    CHOrVLayout* CHOrVLayout::SetMinWidth(float p_Width)
    {
        LElement()->set_minWidth(p_Width);
        return this;
    }
    /// @brief Set min height
    /// @param p_Height Height
    CHOrVLayout* CHOrVLayout::SetMinHeight(float p_Height)
    {
        LElement()->set_minHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set padding
    /// @param p_Padding New padding
    CHOrVLayout* CHOrVLayout::SetPadding(RectOffset* p_Padding)
    {
        HOrVLayoutGroup()->set_padding(p_Padding);
        return this;
    }
    /// @brief Set padding
    /// @param p_Top    Top padding
    /// @param p_Right  Right padding
    /// @param p_Bottom Bottom padding
    /// @param p_Left   Left padding
    CHOrVLayout* CHOrVLayout::SetPadding(int p_Top, int p_Right, int p_Bottom, int p_Left)
    {
        HOrVLayoutGroup()->set_padding(RectOffset::New_ctor(p_Left, p_Right, p_Top, p_Bottom));
        return this;
    }
    /// @brief Set padding
    /// @param p_Padding New padding
    CHOrVLayout* CHOrVLayout::SetPadding(int p_Padding)
    {
        HOrVLayoutGroup()->set_padding(RectOffset::New_ctor(p_Padding, p_Padding, p_Padding, p_Padding));
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set spacing between elements
    /// @param p_Spacing New spacing
    CHOrVLayout* CHOrVLayout::SetSpacing(float p_Spacing)
    {
        HOrVLayoutGroup()->set_spacing(p_Spacing);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components