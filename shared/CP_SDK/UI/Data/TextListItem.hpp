#pragma once

#include "IListItem.hpp"

#include <TMPro/TextAlignmentOptions.hpp>

namespace CP_SDK::UI::Data {

    /// @brief Abstract List Item
    class CP_SDK_EXPORT_VISIBILITY TextListItem : public IListItem
    {
        CP_SDK_NO_DEF_CTORS(TextListItem);

        static constexpr std::u16string_view _default_Text    = u"Default...";
        static constexpr std::u16string_view _default_Tooltip = u"";

        public:
            std::u16string              Text;
            std::u16string              Tooltip;
            TMPro::TextAlignmentOptions Align;

        public:
            /// @brief Constructor
            /// @param p_Text    Text
            /// @param p_Tooltip On hover tooltip
            /// @param p_Align   Text alignment
            TextListItem(std::u16string_view p_Text = _default_Text, std::u16string_view p_Tooltip = _default_Tooltip, TMPro::TextAlignmentOptions p_Align = TMPro::TextAlignmentOptions::CaplineLeft);
            /// @brief Destructor
            virtual ~TextListItem();

        public:
            /// @brief On show
            virtual void OnShow() override;

    };

}   ///< namespace CP_SDK::UI::Data