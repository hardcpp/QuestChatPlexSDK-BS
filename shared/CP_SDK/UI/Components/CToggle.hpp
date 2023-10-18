#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>
#include <UnityEngine/UI/Toggle.hpp>

#include <string>

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

    /// @brief CToggle component
    class CToggle : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CToggle, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CToggle);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CToggle);

        protected:
            _v::Action<_v::CActionRef<bool>, bool> m_OnValueChanged;

            _v::Func<bool>           m_GetValue;

            _v::Action<bool, bool>   m_SetValue;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            UIFieldRefDel<_u::Toggle>           Toggle;

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CToggle* OnValueChanged(_v::CActionRef<bool> p_Functor, bool p_Add = true);

        public:
            /// @brief Get toggle value
            bool GetValue();

        public:
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            CToggle* SetInteractable(bool p_Interactable);
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            CToggle* SetValue(bool p_Value, bool p_Notify = true);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CToggle);