#pragma once

#include "../UIIl2cpp.hpp"
#include "../../Utils/Il2cpp.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/Sprite.hpp>
#include <UnityEngine/UI/Button.hpp>
#include <UnityEngine/UI/Image.hpp>
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

    /// @brief Icon button component
    class CP_SDK_EXPORT CIconButton : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CIconButton, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CIconButton);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CIconButton);

        protected:
            _v::Action<_v::CActionRef<>, bool> m_OnClick;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            UIFieldRefDel<_u::Button>           ButtonC;
            UIFieldRefDel<_u::Image>            IconImageC;

        public:
            /// @brief On click event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CIconButton* OnClick(_v::CActionRef<> p_Functor, bool p_Add = true);

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            CIconButton* SetWidth(float p_Width);
            /// @brief Set preferred height
            /// @param p_Height Height
            CIconButton* SetHeight(float p_Height);

        public:
            /// @brief Set theme color
            /// @param p_Color New color
            CIconButton* SetColor(_u::Color p_Color);
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            CIconButton* SetInteractable(bool p_Interactable);
            /// @brief Set button sprite
            /// @param p_Sprite New sprite
            CIconButton* SetSprite(_u::Sprite* p_Sprite);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CIconButton);