#pragma once

#include "../IModal.hpp"
#include "../../XUI/XUI.hpp"

namespace CP_SDK::UI::Modals {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
        using namespace CP_SDK::XUI;
    }

    /// @brief Dropdown modal
    class CP_SDK_EXPORT Dropdown : public IModal
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Modals", Dropdown, IModal);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(Dropdown);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(Dropdown);

        private:
            _v::XUIVVList::Ptr m_List;

            _v::Action<std::u16string_view> m_Callback;

        private:
            /// @brief On modal show
            void Impl_OnShow();
            /// @brief On modal close
            void Impl_OnClose();

        public:
            /// @brief Init
            /// @param p_Options  Options to display
            /// @param p_Selected Selected option value
            /// @param p_Callback Callback
            void Init(const std::vector<std::u16string>& p_Options, std::u16string_view p_Selected, _v::CActionRef<std::u16string_view> p_Callback);

        private:
            /// @brief On cancel button
            void OnCancelButton();
            /// @brief On list item selected
            /// @param p_SelectedItem Selected list item
            void OnListItemSelect(const UI::Data::IListItem::Ptr& p_SelectedItem);

    };

}   ///< namespace CP_SDK::UI::Modals

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Modals::Dropdown);