#pragma once

#include "../Components/CTextInput.hpp"
#include "../Components/CText.hpp"
#include "../../Utils/Event.hpp"

#include <UnityEngine/UI/Image.hpp>
#include <UnityEngine/UI/Button.hpp>

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CTextInput component
    class DefaultCTextInput : public Components::CTextInput
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCTextInput, Components::CTextInput);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCTextInput);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCTextInput);

        private:
            _v::MonoPtr<_u::RectTransform>      m_RTransform;
            _v::MonoPtr<_u::LayoutElement>      m_LElement;
            _v::MonoPtr<_u::Image>              m_BG;
            _v::MonoPtr<Components::CText>      m_Icon;
            _v::MonoPtr<Components::CText>      m_ValueText;
            _v::MonoPtr<_u::Button>             m_Button;
            bool                                m_IsPassword;
            std::u16string                      m_PlaceHolder;
            std::u16string                      m_Value;

            _v::Event<std::u16string_view>   m_OnValueChangedEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCTextInput();

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnValueChanged_Impl(_v::CActionRef<std::u16string_view> p_Functor, bool p_Add = true);

        public:
            /// @brief Get text
            std::u16string_view GetValue_Impl();

        public:
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            void SetInteractable_Impl(bool p_Interactable);
            /// @brief Set is password
            /// @param p_IsPassword Is password?
            void SetIsPassword_Impl(bool p_IsPassword);
            /// @brief Set place holder
            /// @param p_PlaceHolder New place holder
            void SetPlaceHolder_Impl(std::u16string_view p_PlaceHolder);
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            void SetValue_Impl(std::u16string_view p_Value, bool p_Notify = true);

        private:
            /// @brief Refresh displayed value
            void Refresh();
            /// @brief Notify
            void Notify();
            /// @brief On click unity callback
            void Button_OnClick();
    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCTextInput);