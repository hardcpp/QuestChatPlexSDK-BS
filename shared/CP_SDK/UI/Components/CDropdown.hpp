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

    /// @brief CDropdown component
    class CDropdown : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CDropdown, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CDropdown);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CDropdown);

        protected:
            _v::Action<_v::CActionRef<int, std::u16string_view>, bool> m_OnValueChanged;

            _v::Func<std::u16string_view> m_GetValue;

            _v::Action<bool>                                 m_SetInteractable;
            _v::Action<const std::vector<std::u16string>&>   m_SetOptions;
            _v::Action<std::u16string_view, bool>            m_SetValue;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CDropdown* OnValueChanged(_v::CActionRef<int, std::u16string_view> p_Functor, bool p_Add = true);

        public:
            /// @brief Get value
            std::u16string_view GetValue();

        public:
            /// @brief Set interactable state
            /// @param p_Interactable New state
            CDropdown* SetInteractable(bool p_Interactable);
            /// @brief Set available options
            /// @param p_Options New options list
            CDropdown* SetOptions(const std::vector<std::u16string>& p_Options);
            /// @brief Set value
            /// @param p_Value  New value
            /// @param p_Notify Should notify?
            CDropdown* SetValue(std::u16string_view p_Value, bool p_Notify = true);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CDropdown);