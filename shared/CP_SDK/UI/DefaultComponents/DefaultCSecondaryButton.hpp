#pragma once

#include "Subs/SubStackLayoutGroup.hpp"
#include "../Components/CSecondaryButton.hpp"
#include "../Components/CText.hpp"
#include "../../Utils/Event.hpp"
#include "../../Utils/Delegate.hpp"

#include <UnityEngine/EventSystems/IEventSystemHandler.hpp>
#include <UnityEngine/EventSystems/IPointerEnterHandler.hpp>
#include <UnityEngine/EventSystems/IPointerExitHandler.hpp>
#include <UnityEngine/EventSystems/PointerEventData.hpp>

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace TMPro;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
        using namespace UnityEngine::EventSystems;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CSecondaryButton component
    class DefaultCSecondaryButton : public Components::CSecondaryButton
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCSecondaryButton, Components::CSecondaryButton, CP_SDK_IL2CPP_INTERFACES(
            _u::IEventSystemHandler,
            _u::IPointerEnterHandler,
            _u::IPointerExitHandler
        ));
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCSecondaryButton);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCSecondaryButton);

        private:
            _v::MonoPtr<_u::RectTransform>      m_RTransform;
            _v::MonoPtr<_u::ContentSizeFitter>  m_CSizeFitter;
            _v::MonoPtr<_u::LayoutGroup>        m_StackLayoutGroup;
            _v::MonoPtr<_u::LayoutElement>      m_LElement;
            _v::MonoPtr<_u::Button>             m_Button;
            _v::MonoPtr<_u::Image>              m_BackgroundImage;
            _v::MonoPtr<_u::Image>              m_IconImage;
            _v::MonoPtr<Components::CText>      m_Label;

            std::u16string                      m_Tooltip;
            _v::Event<>                         m_OnClickEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCSecondaryButton();

        private:
            /// @brief On click event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add Should add
            void OnClick_Impl(_v::CActionRef<> p_Functor, bool p_Add = true);

        private:
            /// @brief Set tooltip
            /// @param p_Tooltip New tooltip
            void SetTooltip_Impl(std::u16string_view p_Tooltip);

        private:
            /// @brief On click unity callback
            void Button_OnClick();

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerEnterHandler, void, OnPointerEnter, _u::PointerEventData*);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerExitHandler,  void, OnPointerExit,  _u::PointerEventData*);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCSecondaryButton);