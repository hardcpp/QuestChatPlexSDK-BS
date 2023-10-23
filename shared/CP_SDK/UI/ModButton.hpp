#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/Delegate.hpp"

#include <memory>
#include <string>

namespace CP_SDK::UI {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Mod button
    class ModButton
    {
        CP_SDK_NO_DEF_CTORS(ModButton);
        CP_SDK_PRIV_TAG();

        static constexpr std::u16string_view _default_Tooltip = u"";

        private:
            std::u16string  m_Text;
            std::u16string  m_Tooltip;
            bool            m_Interactable;
            _v::Action<> m_OnClick;

        public:
            std::u16string_view Text();
            std::u16string_view Tooltip();
            bool                Interactable();

        public:
            using Ptr = std::shared_ptr<ModButton>;

            /// @brief Constructor
            /// @param p_Text         Button text
            /// @param p_OnClick      Action when clicked
            /// @param p_Tooltip      Tooltip text
            /// @param p_Interactable Is interactable?
            ModButton(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Text, _v::CActionRef<> p_OnClick, std::u16string_view p_Tooltip = _default_Tooltip, bool p_Interactable = true);
            /// @brief Destructor
            ~ModButton();

        public:
            /// @brief Constructor
            /// @param p_Text         Button text
            /// @param p_OnClick      Action when clicked
            /// @param p_Tooltip      Tooltip text
            /// @param p_Interactable Is interactable?
            static Ptr Make(std::u16string_view p_Text, _v::CActionRef<> p_OnClick, std::u16string_view p_Tooltip = _default_Tooltip, bool p_Interactable = true);

        public:
            /// @brief Fire on click event
            void FireOnClick();

    };

}   ///< namespace CP_SDK::UI