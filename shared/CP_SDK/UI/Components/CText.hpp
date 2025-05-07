#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>
#include <TMPro/TextMeshProUGUI.hpp>
#include <TMPro/TextAlignmentOptions.hpp>

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

    /// @brief CText component
    class CP_SDK_EXPORT CText : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CText, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CText);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CText);

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            UIFieldRefDel<_u::TextMeshProUGUI>  TMProUGUI;

        public:
            /// @brief Get text
            std::u16string GetText();

        public:
            /// @brief Set align
            /// @param p_Align New align
            CText* SetAlign(_u::TextAlignmentOptions p_Align);
            /// @brief Set alpha
            /// @param p_Alpha New alpha
            CText* SetAlpha(float p_Alpha);
            /// @brief Set color
            /// @param p_Color New color
            CText* SetColor(_u::Color p_Color);
            /// @brief Set font size
            /// @param p_Size New size
            CText* SetFontSize(float p_Size);
            /// @brief Set font sizes
            /// @param p_MinSize New size
            /// @param p_MaxSize New size
            CText* SetFontSizes(float p_MinSize, float p_MaxSize);
            /// @brief Set margins
            /// @param p_Left Left margin
            /// @param p_Top Top margin
            /// @param p_Right Right margin
            /// @param p_Bottom Bottom margin
            CText* SetMargins(float p_Left, float p_Top, float p_Right, float p_Bottom);
            /// @brief Set overflow mode
            /// @param p_OverflowMode New overflow mdoe
            CText* SetOverflowMode(_u::TextOverflowModes p_OverflowMode);
            /// @brief Set style
            /// @param p_Style New style
            CText* SetStyle(_u::FontStyles p_Style);
            /// @brief Set button text
            /// @param p_Text New text
            CText* SetText(std::u16string_view p_Text);
            /// @brief Set wrapping
            /// @param p_Wrapping New state
            CText* SetWrapping(bool p_Wrapping);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CText);