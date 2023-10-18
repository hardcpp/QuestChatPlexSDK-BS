#include "CP_SDK/UI/Components/CText.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CText);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CText)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CText)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get text
    std::u16string CText::GetText()
    {
        return TMProUGUI()->get_text();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set align
    /// @param p_Align New align
    CText* CText::SetAlign(TextAlignmentOptions p_Align)
    {
        TMProUGUI()->set_alignment(p_Align);
        return this;
    }
    /// @brief Set alpha
    /// @param p_Alpha New alpha
    CText* CText::SetAlpha(float p_Alpha)
    {
        TMProUGUI()->set_alpha(p_Alpha);
        return this;
    }
    /// @brief Set color
    /// @param p_Color New color
    CText* CText::SetColor(Color p_Color)
    {
        TMProUGUI()->set_color(p_Color);
        return this;
    }
    /// @brief Set font size
    /// @param p_Size New size
    CText* CText::SetFontSize(float p_Size)
    {
        TMProUGUI()->set_fontSize(p_Size * UISystem::FontScale);
        return this;
    }
    /// @brief Set font sizes
    /// @param p_MinSize New size
    /// @param p_MaxSize New size
    CText* CText::SetFontSizes(float p_MinSize, float p_MaxSize)
    {
        TMProUGUI()->set_fontSizeMin(p_MinSize * UISystem::FontScale);
        TMProUGUI()->set_fontSizeMax(p_MaxSize * UISystem::FontScale);
        return this;
    }
    /// @brief Set margins
    /// @param p_Left Left margin
    /// @param p_Top Top margin
    /// @param p_Right Right margin
    /// @param p_Bottom Bottom margin
    CText* CText::SetMargins(float p_Left, float p_Top, float p_Right, float p_Bottom)
    {
        TMProUGUI()->set_margin(Vector4(p_Left, p_Top, p_Right, p_Bottom));
        return this;
    }
    /// @brief Set overflow mode
    /// @param p_OverflowMode New overflow mdoe
    CText* CText::SetOverflowMode(TextOverflowModes p_OverflowMode)
    {
        TMProUGUI()->set_overflowMode(p_OverflowMode);
        return this;
    }
    /// @brief Set style
    /// @param p_Style New style
    CText* CText::SetStyle(FontStyles p_Style)
    {
        TMProUGUI()->set_fontStyle(p_Style);
        return this;
    }
    /// @brief Set button text
    /// @param p_Text New text
    CText* CText::SetText(std::u16string_view p_Text)
    {
        TMProUGUI()->set_text(p_Text);
        return this;
    }
    /// @brief Set wrapping
    /// @param p_Wrapping New state
    CText* CText::SetWrapping(bool p_Wrapping)
    {
        TMProUGUI()->set_enableWordWrapping(p_Wrapping);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components