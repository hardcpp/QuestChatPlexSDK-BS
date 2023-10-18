#pragma once

#include "../CText.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/Sprite.hpp>
#include <UnityEngine/UI/ContentSizeFitter.hpp>
#include <UnityEngine/UI/Button.hpp>
#include <UnityEngine/UI/Image.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>
#include <UnityEngine/UI/LayoutGroup.hpp>
#include <TMPro/TextOverflowModes.hpp>

#include <string>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace TMPro;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Primary or Secondary button component
    class CPOrSButton : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CPOrSButton, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CPOrSButton);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CPOrSButton);

        protected:
            _v::Action<std::u16string_view>     m_SetTooltip;
            _v::Action<_v::CActionRef<>, bool>  m_OnClick;

        public:
            UIFieldRefDel<_u::RectTransform>        RTransform;
            UIFieldRefDel<_u::ContentSizeFitter>    CSizeFitter;
            UIFieldRefDel<_u::LayoutGroup>          LayoutGroupC;
            UIFieldRefDel<_u::LayoutElement>        LElement;
            UIFieldRefDel<_u::Button>               ButtonC;
            UIFieldRefDel<_u::Image>                BackgroundImageC;
            UIFieldRefDel<_u::Image>                IconImageC;
            UIFieldRefDel<CText>                    TextC;

        public:
            /// @brief On click event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CPOrSButton* OnClick(_v::CActionRef<> p_Functor, bool p_Add = true);

        public:
            /// Get text
            std::u16string GetText();

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            CPOrSButton* SetWidth(float p_Width);
            /// @brief Set preferred height
            /// @param p_Height Height
            CPOrSButton* SetHeight(float p_Height);

        public:
            /// @brief Set background color
            /// @param p_Color New background color
            CPOrSButton* SetBackgroundColor(_u::Color p_Color);
            /// @brief Set background sprite
            /// @param p_Sprite New sprite
            CPOrSButton* SetBackgroundSprite(_u::Sprite* p_Sprite);
            /// @brief Set font size
            /// @param p_Size New size
            CPOrSButton* SetFontSize(float p_Size);
            /// @brief Set theme color
            /// @param p_Color New color
            CPOrSButton* SetColor(_u::Color p_Color);
            /// @brief Set button icon sprite
            /// @param p_Sprite New sprite
            CPOrSButton* SetIconSprite(_u::Sprite* p_Sprite);
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            CPOrSButton* SetInteractable(bool p_Interactable);
            /// @brief Set overflow mode
            /// @param p_OverflowMode New overflow mdoe
            CPOrSButton* SetOverflowMode(_u::TextOverflowModes p_OverflowMode);
            /// @brief Set button text
            /// @param p_Text New text
            CPOrSButton* SetText(std::u16string_view p_Text);
            /// @brief Set tooltip
            /// @param p_Tooltip New tooltip
            CPOrSButton* SetTooltip(std::u16string_view p_Tooltip);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CPOrSButton);