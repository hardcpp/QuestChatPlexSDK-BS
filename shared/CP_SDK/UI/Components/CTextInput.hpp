#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>

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

    /// @brief CTextInput component
    class CTextInput : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CTextInput, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CTextInput);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CTextInput);

        protected:
            _v::Action<_v::CActionRef<std::u16string_view>, bool> m_OnValueChanged;

            _v::Func<std::u16string_view> m_GetValue;

            _v::Action<bool>                         m_SetInteractable;
            _v::Action<bool>                         m_SetIsPassword;
            _v::Action<std::u16string_view>          m_SetPlaceHolder;
            _v::Action<std::u16string_view, bool>    m_SetValue;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CTextInput* OnValueChanged(_v::CActionRef<std::u16string_view> p_Functor, bool p_Add = true);

        public:
            /// @brief Get value
            std::u16string_view GetValue();

        public:
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            CTextInput* SetInteractable(bool p_Interactable);
            /// @brief Set is password
            /// @param p_IsPassword Is password?
            CTextInput* SetIsPassword(bool p_IsPassword);
            /// @brief Set place holder
            /// @param p_PlaceHolder New place holder
            CTextInput* SetPlaceHolder(std::u16string_view p_PlaceHolder);
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            CTextInput* SetValue(std::u16string_view p_Value, bool p_Notify = true);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CTextInput);