#include "CP_SDK/UI/Data/TextListItem.hpp"
#include "CP_SDK/UI/Data/TextListCell.hpp"

namespace CP_SDK::UI::Data {

    /// @brief Constructor
    /// @param p_Text    Text
    /// @param p_Tooltip On hover tooltip
    /// @param p_Align   Text alignment
    TextListItem::TextListItem(std::u16string_view p_Text, std::u16string_view p_Tooltip, TMPro::TextAlignmentOptions p_Align)
    {
        if (!p_Text.empty())
            Text = p_Text;

        if (!p_Tooltip.empty())
            Tooltip = p_Tooltip;

        Align = p_Align;
    }
    /// @brief Destructor
    TextListItem::~TextListItem()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On show
    void TextListItem::OnShow()
    {
        auto l_TextListCell = GetCellAsClassOf<TextListCell*>();
        if (!l_TextListCell)
            return;

        l_TextListCell->Text()->SetText(Text)->SetAlign(Align);
        l_TextListCell->Tooltip = Tooltip;
    }

}   ///< namespace CP_SDK::UI::Data