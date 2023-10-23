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

    /// @brief Loading modal
    class Loading : public IModal
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Modals", Loading, IModal);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(Loading);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(Loading);

        private:
            _v::XUIText::Ptr            m_Message;
            _v::XUISecondaryButton::Ptr m_CancelButton;

            _v::Action<>                m_CancelCallback;

        private:
            /// @brief On modal show
            void Impl_OnShow();
            /// @brief On modal close
            void Impl_OnClose();

        public:
            /// @brief Init
            /// @param p_Message        Message to display
            /// @param p_CancelButton   Show cancel button
            /// @param p_CancelCallback On cancel callback
            void Init(std::u16string_view p_Message, bool p_CancelButton = false, _v::CActionRef<> p_CancelCallback = nullptr);

        public:
            /// @brief Init
            /// @param p_Message New message
            void SetMessage(std::u16string_view p_Message);

        private:
            /// @brief On cancel button
            void OnCancelButton();

    };

}   ///< namespace CP_SDK::UI::Modals

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Modals::Loading);