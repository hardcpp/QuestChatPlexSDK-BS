#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/ContentSizeFitter.hpp>
#include <UnityEngine/UI/GridLayoutGroup.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Grid layout group
    class CP_SDK_EXPORT CGLayout : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CGLayout, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CGLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CGLayout);

        public:
            UIFieldRefDel<_u::RectTransform>        RTransform;
            UIFieldRefDel<_u::ContentSizeFitter>    CSizeFitter;
            UIFieldRefDel<_u::LayoutElement>        LElement;
            UIFieldRefDel<_u::GridLayoutGroup>      GLayoutGroup;

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            CGLayout* SetWidth(float p_Width);
            /// @brief Set preferred height
            /// @param p_Height Height
            CGLayout* SetHeight(float p_Height);

        public:
            /// @brief Set min width
            /// @param p_Width Width
            CGLayout* SetMinWidth(float p_Width);
            /// @brief Set min height
            /// @param p_Height Height
            CGLayout* SetMinHeight(float p_Height);

        public:
            /// @brief Set padding
            /// @param p_Padding New padding
            CGLayout* SetPadding(_u::RectOffset* p_Padding);
            /// @brief Set padding
            /// @param p_Top    Top padding
            /// @param p_Right  Right padding
            /// @param p_Bottom Bottom padding
            /// @param p_Left   Left padding
            CGLayout* SetPadding(int p_Top, int p_Right, int p_Bottom, int p_Left);
            /// @brief Set padding
            /// @param p_Padding New padding
            CGLayout* SetPadding(int p_Padding);

        public:
            /// @brief Set cell size
            /// @param p_CellSize New size
            CGLayout* SetCellSize(_u::Vector2 p_CellSize);
            /// @brief Set child alignment
            /// @param p_ChildAlign New alignment
            CGLayout* SetChildAlign(_u::TextAnchor p_ChildAlign);
            /// @brief Set layout constraint
            /// @param p_Constraint New value
            CGLayout* SetConstraint(_u::GridLayoutGroup::Constraint p_Constraint);
            /// @brief Set layout constraint count
            /// @param p_ConstraintCount New value
            CGLayout* SetConstraintCount(int p_ConstraintCount);
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            CGLayout* SetSpacing(_u::Vector2 p_Spacing);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CGLayout);