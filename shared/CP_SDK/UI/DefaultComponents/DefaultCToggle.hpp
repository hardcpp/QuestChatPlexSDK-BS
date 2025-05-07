#pragma once

#include "Subs/SubToggleWithCallbacks.hpp"
#include "../Components/CToggle.hpp"
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

    /// @brief Default CToggle component
    class CP_SDK_EXPORT DefaultCToggle : public Components::CToggle
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCToggle, Components::CToggle);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCToggle);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCToggle);

        private:
            enum AnimationState : uint32_t
            {
                Idle            = 0 << 0,
                SwitchingOn     = 1 << 0,
                SwitchingOff    = 1 << 1,
                HighlightingOn  = 1 << 2,
                HighlightingOff = 1 << 3,
                DisablingOn     = 1 << 4,
                DisablingOff    = 1 << 5
            };

            struct ColorBlock
            {
                _u::Color KnobColor;
                _u::Color BackgroundColor;

                ColorBlock() : KnobColor(_u::Color::get_white()), BackgroundColor(_u::Color::get_white()) {}
                ColorBlock(_u::Color p_KnobColor, _u::Color p_BackgroundColor) : KnobColor(p_KnobColor), BackgroundColor(p_BackgroundColor) {}
            };

        private:
            _v::MonoPtr<_u::RectTransform>               m_RTransform;
            _v::MonoPtr<_u::LayoutElement>               m_LElement;
            _v::MonoPtr<Subs::SubToggleWithCallbacks>    m_Toggle;
            _v::MonoPtr<_u::Image>                       m_BackgroundImage;
            _v::MonoPtr<Components::CText>               m_OffText;
            _v::MonoPtr<Components::CText>               m_OnText;
            _v::MonoPtr<_u::Image>                       m_KnobImage;
            bool                                         m_PreventChange;
            _v::Event<bool>                              m_OnChange;

            float           m_SwitchAnimationSmooth;
            float           m_HorizontalStretchAmount;
            float           m_VerticalStretchAmount;

            uint32_t        m_AnimationState;
            float           m_SwitchAmount;
            float           m_HighlightAmount;
            float           m_DisabledAmount;
            float           m_OriginalKnobWidth;
            float           m_OriginalKnobHeight;

            ColorBlock      m_OnColors;
            ColorBlock      m_OffColors;
            ColorBlock      m_OnHighlightedColors;
            ColorBlock      m_OffHighlightedColors;
            ColorBlock      m_DisabledColors;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCToggle();

        public:
            /// @brief Component first frame
            DECLARE_INSTANCE_METHOD(void, Start);

        public:
            /// @brief On frame
            DECLARE_INSTANCE_METHOD(void, Update);

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnValueChanged_Impl(_v::CActionRef<bool> p_Functor, bool p_Add = true);

        public:
            /// @brief Get value
            bool GetValue_Impl();

        public:
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            void SetValue_Impl(bool p_Value, bool p_Notify = true);

        private:
            /// @brief Toggle event onValueChanged
            /// @param p_Value
            DECLARE_INSTANCE_METHOD(void, Toggle_onValueChanged, bool p_Value);
            /// @brief Toggle event stateDidChangeEvent
            /// @param p_SelectionState New state
            void Toggle_StateDidChange(Subs::SubToggleWithCallbacks::ESelectionState p_SelectionState);

        private:
            /// @brief Lerp position
            /// @param p_SwitchAmount Switch amount
            void LerpPosition(float p_SwitchAmount);
            /// @brief Lerp stretching
            /// @param p_SwitchAmount Switch amount
            void LerpStretch(float p_SwitchAmount);
            /// @brief Lerp all colors
            /// @param p_SwitchAmount    Switch amount
            /// @param p_HighlightAmount Highlight amount
            /// @param p_DisabledAmount  Disabled amount
            void LerpColors(float p_SwitchAmount, float p_HighlightAmount, float p_DisabledAmount);
            /// @brief Lerp specific color
            /// @param p_SwitchAmount    Switch amount
            /// @param p_HighlightAmount Highlight amount
            /// @param p_DisabledAmount  Disabled amount
            /// @param p_Delegate        Get specific sub color delegate
            _u::Color LerpColor(float p_SwitchAmount, float p_HighlightAmount, float p_DisabledAmount, const _v::Delegate<_u::Color(ColorBlock&)>& p_Delegate);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCToggle);