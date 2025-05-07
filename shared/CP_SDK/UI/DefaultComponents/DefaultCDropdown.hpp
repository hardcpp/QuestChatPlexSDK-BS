#pragma once

#include "../Components/CDropdown.hpp"
#include "../Components/CIconButton.hpp"
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

    /// @brief Default CDropdown component
    class CP_SDK_EXPORT DefaultCDropdown : public Components::CDropdown
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCDropdown, Components::CDropdown);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCDropdown);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCDropdown);

        private:
            _v::MonoPtr<_u::RectTransform>          m_RTransform;
            _v::MonoPtr<_u::LayoutElement>          m_LElement;
            _v::MonoPtr<_u::Image>                  m_BG;
            _v::MonoPtr<Components::CIconButton>    m_Icon;
            _v::MonoPtr<Components::CText>          m_ValueText;
            _v::MonoPtr<_u::Button>                 m_Button;
            int                                     m_Value;
            std::vector<std::u16string>             m_Options;

            _v::Event<int, std::u16string_view> m_OnValueChangeEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCDropdown();

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnValueChanged_Impl(_v::CActionRef<int, std::u16string_view> p_Functor, bool p_Add = true);

        public:
            /// @brief Get value
            std::u16string_view GetValue_Impl();

        public:
            /// @brief Set interactable state
            /// @param p_Interactable New state
            void SetInteractable_Impl(bool p_Interactable);
            /// @brief Set available options
            /// @param p_Options New options list
            void SetOptions_Impl(const std::vector<std::u16string>& p_Options);
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

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCDropdown);