#include "CP_SDK/UI/Components/CGLayout.hpp"

#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CGLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CGLayout)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CGLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set preferred width
    /// @param p_Width Width
    CGLayout* CGLayout::SetWidth(float p_Width)
    {
        RTransform()->set_sizeDelta(Vector2(p_Width, RTransform()->get_sizeDelta().y));
        LElement()->set_preferredWidth(p_Width);
        return this;
    }
    /// @brief Set preferred height
    /// @param p_Height Height
    CGLayout* CGLayout::SetHeight(float p_Height)
    {
        RTransform()->set_sizeDelta(Vector2(RTransform()->get_sizeDelta().x, p_Height));
        LElement()->set_preferredHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set min width
    /// @param p_Width Width
    CGLayout* CGLayout::SetMinWidth(float p_Width)
    {
        LElement()->set_minWidth(p_Width);
        return this;
    }
    /// @brief Set min height
    /// @param p_Height Height
    CGLayout* CGLayout::SetMinHeight(float p_Height)
    {
        LElement()->set_minHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set cell size
    /// @param p_CellSize New size
    CGLayout* CGLayout::SetCellSize(Vector2 p_CellSize)
    {
        GLayoutGroup()->set_cellSize(p_CellSize);
        return this;
    }
    /// @brief Set child alignment
    /// @param p_ChildAlign New alignment
    CGLayout* CGLayout::SetChildAlign(TextAnchor p_ChildAlign)
    {
        GLayoutGroup()->set_childAlignment(p_ChildAlign);
        return this;
    }
    /// @brief Set layout constraint
    /// @param p_Constraint New value
    CGLayout* CGLayout::SetConstraint(GridLayoutGroup::Constraint p_Constraint)
    {
        GLayoutGroup()->set_constraint(p_Constraint);
        return this;
    }
    /// @brief Set layout constraint count
    /// @param p_ConstraintCount New value
    CGLayout* CGLayout::SetConstraintCount(int p_ConstraintCount)
    {
        GLayoutGroup()->set_constraintCount(p_ConstraintCount);
        return this;
    }
    /// @brief Set spacing between elements
    /// @param p_Spacing New spacing
    CGLayout* CGLayout::SetSpacing(Vector2 p_Spacing)
    {
        GLayoutGroup()->set_spacing(p_Spacing);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components