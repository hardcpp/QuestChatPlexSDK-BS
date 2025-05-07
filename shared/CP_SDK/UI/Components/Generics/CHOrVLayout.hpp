#pragma once

#include "../../UIIl2cpp.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/ContentSizeFitter.hpp>
#include <UnityEngine/UI/HorizontalOrVerticalLayoutGroup.hpp>
#include <UnityEngine/UI/Image.hpp>
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

    /// @brief Horizontal or vertical layout base component
    class CP_SDK_EXPORT CHOrVLayout : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CHOrVLayout, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CHOrVLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CHOrVLayout);

        protected:
            _v::MonoPtr<_u::Image>                           m_Background;

        public:
            UIFieldRefDel<_u::RectTransform>                    RTransform;
            UIFieldRefDel<_u::ContentSizeFitter>                CSizeFitter;
            UIFieldRefDel<_u::LayoutElement>                    LElement;
            UIFieldRefDel<_u::HorizontalOrVerticalLayoutGroup>  HOrVLayoutGroup;

        public:
            /// @brief Get background fill amount
            float GetBackgroundFillAmount();

        public:
            /// @brief Set background state
            /// @param p_Enabled       Is enabled?
            /// @param p_Color         Optional color, default to black
            /// @param p_RaycastTarget Should raycast target
            CHOrVLayout* SetBackground(bool p_Enabled, std::optional<_u::Color> p_Color = std::nullopt, bool p_RaycastTarget = false);
            /// @brief Set background color
            /// @param p_Color New background color
            CHOrVLayout* SetBackgroundColor(_u::Color p_Color);
            /// @brief Set background fill method
            /// @param p_FillMethod Fill method
            CHOrVLayout* SetBackgroundFillMethod(_u::Image::FillMethod p_FillMethod);
            /// @brief Set background fill amount
            /// @param p_FillAmount Fill amount
            CHOrVLayout* SetBackgroundFillAmount(float p_FillAmount);
            /// @brief Set background sprite
            /// @param p_Sprite New sprite
            /// @param p_Type   Image type
            CHOrVLayout* SetBackgroundSprite(_u::Sprite* p_Sprite, _u::Image::Type p_Type = _u::Image::Type::Simple);

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            CHOrVLayout* SetWidth(float p_Width);
            /// @brief Set preferred height
            /// @param p_Height Height
            CHOrVLayout* SetHeight(float p_Height);

        public:
            /// @brief Set min width
            /// @param p_Width Width
            CHOrVLayout* SetMinWidth(float p_Width);
            /// @brief Set min height
            /// @param p_Height Height
            CHOrVLayout* SetMinHeight(float p_Height);

        public:
            /// @brief Set padding
            /// @param p_Padding New padding
            CHOrVLayout* SetPadding(_u::RectOffset* p_Padding);
            /// @brief Set padding
            /// @param p_Top    Top padding
            /// @param p_Right  Right padding
            /// @param p_Bottom Bottom padding
            /// @param p_Left   Left padding
            CHOrVLayout* SetPadding(int p_Top, int p_Right, int p_Bottom, int p_Left);
            /// @brief Set padding
            /// @param p_Padding New padding
            CHOrVLayout* SetPadding(int p_Padding);

        public:
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            CHOrVLayout* SetSpacing(float p_Spacing);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CHOrVLayout);