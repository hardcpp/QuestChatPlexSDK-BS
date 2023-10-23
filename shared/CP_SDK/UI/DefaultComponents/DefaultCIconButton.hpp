#pragma once

#include "../Components/CIconButton.hpp"
#include "../../Utils/Event.hpp"
#include "../../Utils/Delegate.hpp"

#include <custom-types/shared/coroutine.hpp>
#include <UnityEngine/EventSystems/IEventSystemHandler.hpp>
#include <UnityEngine/EventSystems/IPointerEnterHandler.hpp>
#include <UnityEngine/EventSystems/IPointerExitHandler.hpp>
#include <UnityEngine/EventSystems/PointerEventData.hpp>

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
        using namespace UnityEngine::EventSystems;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CIconButton component
    class DefaultCIconButton : public Components::CIconButton
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCIconButton, Components::CIconButton, CP_SDK_IL2CPP_INTERFACES(
            _u::IEventSystemHandler,
            _u::IPointerEnterHandler,
            _u::IPointerExitHandler
        ));
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCIconButton);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCIconButton);

        private:
            _v::MonoPtr<_u::RectTransform>  m_RTransform;
            _v::MonoPtr<_u::LayoutElement>  m_LElement;
            _v::MonoPtr<_u::Button>         m_Button;
            _v::MonoPtr<_u::Image>          m_IconImage;

            _v::Event<>                     m_OnClickEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCIconButton();

        private:
            /// @brief On click event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add Should add
            void OnClick_Impl(_v::CActionRef<> p_Functor, bool p_Add = true);

        private:
            /// @brief On click unity callback
            void Button_OnClick();

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerEnterHandler, void, OnPointerEnter, _u::PointerEventData*);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerExitHandler,  void, OnPointerExit,  _u::PointerEventData*);

        private:
            /// @brief Coroutine animate scale
            /// @param p_Target Target scale
            /// @param p_Time   Animation time
            static custom_types::Helpers::Coroutine Coroutine_AnimateScale(DefaultCIconButton* p_Self, _u::Vector3 p_Target, float p_Time);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCIconButton);