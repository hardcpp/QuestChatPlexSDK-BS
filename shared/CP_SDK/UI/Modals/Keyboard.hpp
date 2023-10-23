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

    /// @brief Keyboard modal
    class Keyboard : public IModal
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Modals", Keyboard, IModal);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(Keyboard);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(Keyboard);

        private:
            static std::u16string                       m_CopyBuffer;

            _v::XUIText::Ptr                            m_Text;
            _v::XUIFLayout::Ptr                         m_CustomKeyLayout;
            std::vector<Components::CPrimaryButton*>    m_CustomKeys;
            std::vector<_v::XUIPrimaryButton::Ptr>      m_SecondaryButtons;
            std::u16string                              m_Value;
            _v::Action<std::u16string_view>             m_Callback;
            _v::Action<>                                m_CancelCallback;
            bool                                        m_IsCaps;
            bool                                        m_IsTempCaps;

        private:
            /// @brief On modal show
            void Impl_OnShow();
            /// @brief On modal close
            void Impl_OnClose();

        public:
            /// @brief Init
            /// @param p_Value          Value
            /// @param p_Callback       Callback
            /// @param p_CancelCallback On cancel callback
            /// @param p_CustomKeys     Custom keys
            void Init(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback, KeyboardCustomKeys* p_CustomKeys);

        public:
            /// @brief Get current value
            std::u16string_view GetValue();
            /// @brief Set value
            /// @param p_Value
            void SetValue(std::u16string_view p_Value);
            /// @brief Append
            /// @param p_ToAppend
            void Append(std::u16string_view p_ToAppend);

        private:
            /// @brief On copy button
            void OnCopyButton();
            /// @brief On paste button
            void OnPasteButton();
            /// @brief On key press
            /// @param p_Text Text
            void OnKeyPress(std::u16string p_Text);

        private:
            /// @brief Build a key row
            /// @param p_ForcePrimary Force as primary
            /// @param p_Keys         Keys to build
            _v::XUIHLayout::Ptr BuildKeyRow(bool p_ForcePrimary, std::initializer_list<std::u16string> p_Keys);
            /// @brief Switch caps
            void SwitchCaps();

    };

}   ///< namespace CP_SDK::UI::Modals

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Modals::Keyboard);