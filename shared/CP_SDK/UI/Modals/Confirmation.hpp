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

    /// @brief Confirmation modal
    class Confirmation : public IModal
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.Modals", Confirmation, IModal);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(Confirmation, IModal);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(Confirmation, IModal);

        private:
            _v::XUIText::Ptr    m_Message;
            _v::Action<bool>    m_Callback;

        private:
            /// @brief On modal show
            void Impl_OnShow();
            /// @brief On modal close
            void Impl_OnClose();

        public:
            /// @brief Init
            /// @param p_Message  Value
            /// @param p_Callback Callback
            void Init(std::u16string_view p_Message, _v::CActionRef<bool> p_Callback);

        private:
            /// @brief On button "Yes" pressed
            void OnYesButton();
            /// @brief On button "No" pressed
            void OnNoButton();

    };

}   ///< namespace CP_SDK::UI::Modals

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Modals::Confirmation);