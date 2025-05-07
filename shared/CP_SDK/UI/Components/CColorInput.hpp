#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>

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

    /// @brief CColorInput component
    class CP_SDK_EXPORT CColorInput : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CColorInput, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CColorInput);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CColorInput);

        protected:
            _v::Action<_v::CActionRef<_u::Color>, bool> m_OnValueChanged;

            _v::Func<_u::Color> m_GetValue;

            _v::Action<bool>            m_SetAlphaSupport;
            _v::Action<bool>            m_SetInteractable;
            _v::Action<_u::Color, bool> m_SetValue;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CColorInput* OnValueChanged(_v::CActionRef<_u::Color> p_Functor, bool p_Add = true);

        public:
            /// @brief Get value
            _u::Color GetValue();

        public:
            /// @brief Set alpha support
            /// @param p_Support New state
            CColorInput* SetAlphaSupport(bool p_Support);
            /// @brief Set interactable state
            /// @param p_Interactable New state
            CColorInput* SetInteractable(bool p_Interactable);
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            CColorInput* SetValue(_u::Color p_Value, bool p_Notify = true);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CColorInput);