#pragma once

#include "../Components/CColorInput.hpp"
#include "../Components/CPrimaryButton.hpp"
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

    /// @brief Default CColorInput component
    class CP_SDK_EXPORT DefaultCColorInput : public Components::CColorInput
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCColorInput, Components::CColorInput);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCColorInput);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCColorInput);

        private:
            _v::MonoPtr<_u::RectTransform>          m_RTransform;
            _v::MonoPtr<_u::LayoutElement>          m_LElement;
            _v::MonoPtr<_u::Image>                  m_BG;
            _v::MonoPtr<Components::CPrimaryButton> m_Icon;
            _v::MonoPtr<_u::Button>                 m_Button;
            _u::Color                               m_Value;
            _u::Color                               m_OriginalValue;
            bool                                    m_AlphaSupport;

            _v::Event<_u::Color> m_OnChange;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCColorInput();

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnValueChanged_Impl(_v::CActionRef<_u::Color> p_Functor, bool p_Add = true);

        public:
            /// @brief Get value
            _u::Color GetValue_Impl();

        public:
            /// @brief Set alpha support
            /// @param p_Support New state
            void SetAlphaSupport_Impl(bool p_Support);
            /// @brief Set interactable state
            /// @param p_Interactable New state
            void SetInteractable_Impl(bool p_Interactable);
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            void SetValue_Impl(_u::Color p_Value, bool p_Notify = true);

        private:
            /// @brief Notify
            void Notify();
            /// @brief On click unity callback
            void Button_OnClick();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCColorInput);