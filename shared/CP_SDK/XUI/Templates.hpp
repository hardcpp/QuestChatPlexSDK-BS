#pragma once

#include "XUI.hpp"

namespace CP_SDK::XUI {

    /// @brief XUI templates
    class Templates
    {
        public:
            /// @brief Modal rect layout
            /// @param p_Childs Childs
            static XUIVLayout::Ptr ModalRectLayout(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);
            /// @brief Full rect layout for screens template
            /// @param p_Childs Childs
            static XUIVLayout::Ptr FullRectLayout(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);
            /// @brief Full rect layout for screens template
            /// @param p_Childs Childs
            static XUIVLayout::Ptr FullRectLayoutMainView(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);
            /// @brief Title bar template
            /// @param p_Text Title
            static XUIHLayout::Ptr TitleBar(std::u16string_view p_Text);
            /// @brief Scrollable infos widget template
            /// @param p_Height Height
            /// @param p_Childs Childs
            static XUIHLayout::Ptr ScrollableInfos(float p_Height, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);
            /// @brief Expanded buttons line
            /// @param p_Childs Childs
            static XUIHLayout::Ptr ExpandedButtonsLine(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);
            /// @brief Settings group
            /// @param p_Title  Title
            /// @param p_Childs Childs
            static XUIVLayout::Ptr SettingsHGroup(std::u16string_view p_Title, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);
            /// @brief Settings group
            /// @param p_Title  Title
            /// @param p_Childs Childs
            static XUIHLayout::Ptr SettingsVGroup(std::u16string_view p_Title, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs);

    };

}   ///< namespace CP_SDK::XUI