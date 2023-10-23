#pragma once

#include "../UIIl2cpp.hpp"
#include "Generics/CPOrSButton.hpp"

#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>
#include <UnityEngine/UI/Selectable.hpp>
#include <UnityEngine/Color.hpp>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief CSlider component
    class CSlider : public _u::Selectable
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CSlider, _u::Selectable);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(CSlider);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(CSlider);
        CP_SDK_IL2CPP_CTOR_FIELD();
        CP_SDK_IL2CPP_DTOR_FIELD();

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            UIFieldRefDel<CPOrSButton>          DecButton;
            UIFieldRefDel<CPOrSButton>          IncButton;

        protected:
            _v::Action<_v::CActionRef<float>, bool> m_OnValueChanged;

            _v::Func<float> m_GetMinValue;
            _v::Func<float> m_GetMaxValue;
            _v::Func<float> m_GetIncrements;
            _v::Func<float> m_GetValue;

            _v::Action<_u::Color>                           m_SetColor;
            _v::Action<_v::CFuncRef<std::u16string, float>> m_SetFormatter;
            _v::Action<bool>                                m_SetInteger;
            _v::Action<bool>                                m_SetInteractable;
            _v::Action<float>                               m_SetMinValue;
            _v::Action<float>                               m_SetMaxValue;
            _v::Action<float>                               m_SetIncrements;
            _v::Action<float, bool>                         m_SetValue;

            _v::Action<bool, bool, bool, bool>   m_SwitchToColorMode;
            _v::Action<float>                    m_ColorModeSetHue;

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CSlider* OnValueChanged(_v::CActionRef<float> p_Functor, bool p_Add = true);

        public:
            /// @brief Get min value
            float GetMinValue();
            /// @brief Get max value
            float GetMaxValue();
            /// @brief Get increments
            float GetIncrements();
            /// @brief Get value
            float GetValue();

        public:
            /// @brief Set theme color
            /// @param p_Color New color
            CSlider* SetColor(_u::Color p_Color);
            /// @brief Set value formatter
            /// @param p_CustomFormatter Custom value formatter
            CSlider* SetFormatter(_v::CFuncRef<std::u16string, float> p_CustomFormatter);
            /// @brief Set integer mode
            /// @param p_IsInteger Is integer?
            CSlider* SetInteger(bool p_IsInteger);
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            CSlider* SetInteractable(bool p_Interactable);
            /// @brief Set min value
            /// @param p_MinValue New value
            CSlider* SetMinValue(float p_MinValue);
            /// @brief Set max value
            /// @param p_MaxValue New value
            CSlider* SetMaxValue(float p_MaxValue);
            /// @brief Set increments
            /// @param p_Increments New value
            CSlider* SetIncrements(float p_Increments);
            /// @brief Set value
            /// @param p_Value Value
            /// @param p_Notify Notify?
            CSlider* SetValue(float p_Value, bool p_Notify = true);

        public:
            /// @brief Switch to color mode
            /// @param p_H Is Hue mode?
            /// @param p_S Is saturation mode?
            /// @param p_V Is value mode?
            /// @param p_O Is opacity mode?
            CSlider* SwitchToColorMode(bool p_H, bool p_S, bool p_V, bool p_O);
            /// @brief Color mode set H
            /// @param p_H Is Hue mode?
            CSlider* ColorModeSetHue(float p_H);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CSlider);