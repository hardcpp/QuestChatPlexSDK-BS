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

    /// @brief Message modal
    class Message : public IModal
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Modals", Message, IModal);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(Message);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(Message);

        private:
            _v::XUIText::Ptr    m_Message;
            _v::Action<>        m_Callback;

        private:
            /// @brief On modal show
            void Impl_OnShow();
            /// @brief On modal close
            void Impl_OnClose();

        public:
            /// @brief Init
            /// @param p_Message  Value
            /// @param p_Callback Callback
            void Init(std::u16string_view p_Message, _v::CActionRef<> p_Callback);

        private:
            /// @brief On button "OK" pressed
            void OnOKButton();

    };

}   ///< namespace CP_SDK::UI::Modals

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Modals::Message);