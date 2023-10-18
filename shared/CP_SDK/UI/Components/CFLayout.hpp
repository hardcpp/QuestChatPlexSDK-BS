#pragma once

#include "../UIIl2cpp.hpp"
#include "../../Utils/Il2cpp.hpp"

#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/TextAnchor.hpp>
#include <UnityEngine/UI/LayoutGroup.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>
#include <UnityEngine/Vector2.hpp>

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

    /// @brief Flow layout group
    class CFLayout : public _u::LayoutGroup
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CFLayout, _u::LayoutGroup);
        CP_SDK_IL2CPP_DECLARE_CTOR(CFLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CFLayout);

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;

        public:
            enum class EAxis
            {
                Horizontal,
                Vertical
            };

            _v::Func<EAxis>      GetStartAxis;
            _v::Func<bool>       GetChildForceExpandWidth;
            _v::Func<bool>       GetChildForceExpandHeight;
            _v::Func<bool>       GetExpandHorizontalSpacing;
            _v::Func<float>      GetSpacingX;
            _v::Func<float>      GetSpacingY;
            _v::Func<bool>       GetInvertOrder;

            _v::Action<EAxis>    SetStartAxis;
            _v::Action<bool>     SetChildForceExpandWidth;
            _v::Action<bool>     SetChildForceExpandHeight;
            _v::Action<bool>     SetExpandHorizontalSpacing;
            _v::Action<float>    SetSpacingX;
            _v::Action<float>    SetSpacingY;
            _v::Action<bool>     SetInvertOrder;

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            CFLayout* SetWidth(float p_Width);
            /// @brief Set preferred height
            /// @param p_Height Height
            CFLayout* SetHeight(float p_Height);

        public:
            /// @brief Set min width
            /// @param p_Width Width
            CFLayout* SetMinWidth(float p_Width);
            /// @brief Set min height
            /// @param p_Height Height
            CFLayout* SetMinHeight(float p_Height);

        public:
            /// @brief Set child alignment
            /// @param p_Spacing New alignment
            CFLayout* SetChildAlign(_u::TextAnchor p_Spacing);
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            CFLayout* SetSpacing(_u::Vector2 p_Spacing);

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, LayoutGroup,    void, CalculateLayoutInputHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, ILayoutElement, void, CalculateLayoutInputHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, LayoutGroup,    void, CalculateLayoutInputVertical);
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, ILayoutElement, void, CalculateLayoutInputVertical);

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, LayoutGroup,          void, SetLayoutHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, ILayoutController,    void, SetLayoutHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, LayoutGroup,          void, SetLayoutVertical);
            CP_SDK_IL2CPP_OVERRIDE_METHOD2(_u, ILayoutController,    void, SetLayoutVertical);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CFLayout);