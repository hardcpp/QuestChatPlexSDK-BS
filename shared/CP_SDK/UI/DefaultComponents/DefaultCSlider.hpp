#pragma once

#include "../Components/CSlider.hpp"
#include "../Components/CText.hpp"
#include "../Components/Generics/CPOrSButton.hpp"
#include "../../Utils/Event.hpp"

#include <UnityEngine/EventSystems/IPointerDownHandler.hpp>
#include <UnityEngine/EventSystems/IPointerUpHandler.hpp>
#include <UnityEngine/EventSystems/IPointerEnterHandler.hpp>
#include <UnityEngine/EventSystems/IPointerExitHandler.hpp>
#include <UnityEngine/EventSystems/IBeginDragHandler.hpp>
#include <UnityEngine/EventSystems/IDragHandler.hpp>
#include <UnityEngine/EventSystems/IEventSystemHandler.hpp>
#include <UnityEngine/EventSystems/IInitializePotentialDragHandler.hpp>
#include <UnityEngine/EventSystems/PointerEventData.hpp>
#include <UnityEngine/UI/CanvasUpdate.hpp>
#include <UnityEngine/UI/ICanvasElement.hpp>
#include <UnityEngine/UI/Selectable.hpp>

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

    /// @brief Default CSlider component
    class CP_SDK_EXPORT DefaultCSlider : public Components::CSlider
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCSlider, Components::CSlider, CP_SDK_IL2CPP_INTERFACES(
            _u::IEventSystemHandler,
            _u::IBeginDragHandler,
            _u::IDragHandler,
            _u::IInitializePotentialDragHandler,
            _u::ICanvasElement
        ));
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCSlider);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCSlider);

        private:
            enum class EColorMode
            {
                None,
                H,
                S,
                V,
                O
            };

        private:
            _v::MonoPtr<_u::RectTransform>          m_RTransform;
            _v::MonoPtr<_u::LayoutElement>          m_LElement;
            _v::MonoPtr<_u::Image>                  m_BG;
            _v::MonoPtr<_u::Image>                  m_BGSub1;
            _v::MonoPtr<_u::Image>                  m_BGSub2;
            _v::MonoPtr<Components::CPOrSButton>    m_DecButton;
            _v::MonoPtr<Components::CPOrSButton>    m_IncButton;
            _v::MonoPtr<_u::RectTransform>          m_SlidingArea;
            _v::MonoPtr<_u::Image>                  m_Handle;
            _v::MonoPtr<Components::CText>          m_ValueText;
            _u::Color                               m_OnColor;
            _u::Color                               m_OffColor;
            EColorMode                              m_ColorMode;
            bool                                    m_EnableDragging;
            bool                                    m_IsInteger;
            float                                   m_HandleSize;
            float                                   m_ValueSize;
            float                                   m_SeparatorSize;
            int                                     m_NumberOfSteps;
            float                                   m_NormalizedValue;
            float                                   m_MinValue;
            float                                   m_MaxValue;
            float                                   m_Increments;
            float                                   m_DragSmoothing;
            bool                                    m_LeftMouseButtonPressed;
            float                                   m_DragStartTime;
            float                                   m_DragTargetValue;
            float                                   m_DragCurrentValue;
            _v::MonoPtr<_u::PointerEventData>       m_LastPointerEvent;

            _v::Func<std::u16string, float>      m_CustomFormatter;
            _v::Event<float>                     m_OnChange;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCSlider();

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnValueChanged_Impl(_v::CActionRef<float> p_Functor, bool p_Add = true);

        public:
            /// @brief Get min value
            float GetMinValue_Impl();
            /// @brief Get max value
            float GetMaxValue_Impl();
            /// @brief Get increments
            float GetIncrements_Impl();
            /// @brief Get value
            float GetValue_Impl();

        public:
            /// @brief Set theme color
            /// @param p_Color New color
            void SetColor_Impl(_u::Color p_Color);
            /// @brief Set value formatter
            /// @param p_CustomFormatter Custom value formatter
            void SetFormatter_Impl(_v::CFuncRef<std::u16string, float> p_CustomFormatter);
            /// @brief Set integer mode
            /// @param p_IsInteger Is integer?
            void SetInteger_Impl(bool p_IsInteger);
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            void SetInteractable_Impl(bool p_Interactable);
            /// @brief Set min value
            /// @param p_MinValue New value
            void SetMinValue_Impl(float p_MinValue);
            /// @brief Set max value
            /// @param p_MaxValue New value
            void SetMaxValue_Impl(float p_MaxValue);
            /// @brief Set increments
            /// @param p_Increments New value
            void SetIncrements_Impl(float p_Increments);
            /// @brief Set value
            /// @param p_Value Value
            /// @param p_Notify Notify?
            void SetValue_Impl(float p_Value, bool p_Notify = true);

        public:
            /// @brief Switch to color mode
            /// @param p_H Is Hue mode?
            /// @param p_S Is saturation mode?
            /// @param p_V Is value mode?
            /// @param p_O Is opacity mode?
            void SwitchToColorMode_Impl(bool p_H, bool p_S, bool p_V, bool p_O);
            /// @brief Color mode set H
            /// @param p_H Is Hue mode?
            void ColorModeSetHue_Impl(float p_H);

        protected:
            /// @brief On component enable
            DECLARE_INSTANCE_METHOD(void, OnEnable);

        private:
            /// @brief On frame (late)
            DECLARE_INSTANCE_METHOD(void, LateUpdate);

        private:
            /// @brief May drag based on pointer event data
            /// @param p_EventData Event data
            bool MayDrag(_u::PointerEventData* p_EventData);
            /// @brief Initialize a potential drag
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IInitializePotentialDragHandler, void, OnInitializePotentialDrag, _u::PointerEventData*);
            /// @brief On drag start
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IBeginDragHandler, void, OnBeginDrag, _u::PointerEventData*);
            /// @brief On mouse drag
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IDragHandler, void, OnDrag, _u::PointerEventData*);
            /// @brief Update drag
            /// @param p_EventData Event data
            void UpdateDrag(_u::PointerEventData* p_EventData);

        public:
            /// @brief On pointer enter
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerEnterHandler, void, OnPointerEnter, _u::PointerEventData*);
            /// @brief On pointer exit
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerExitHandler, void, OnPointerExit, _u::PointerEventData*);
            /// @brief On pointer button down
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerDownHandler, void, OnPointerDown, _u::PointerEventData*);
            /// @brief On pointer button up
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerUpHandler, void, OnPointerUp, _u::PointerEventData*);

        public:
            /// @brief Set normalized value
            /// @param p_Value  Normalized value
            /// @param p_Notify Should notify?
            void SetNormalizedValue(float p_Value, bool p_Notify = true);

        protected:
            /// @brief On rect transform dimensions changed
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, UIBehaviour, void, OnRectTransformDimensionsChange);
            /// @brief Update visual style
            void UpdateStyle();
            /// @brief Update visuals
            void UpdateVisuals();
            /// @brief On state transition
            /// @param p_State   New state
            /// @param p_Instant Is instant
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, Selectable, void, DoStateTransition, _u::Selectable::SelectionState, bool);

        public:
            /// @brief Get transform
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ICanvasElement, _u::Transform*, get_transform);
            /// @brief On canvas rebuild
            /// @param p_Executing Executing update
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ICanvasElement, void, Rebuild, _u::CanvasUpdate);
            /// @brief On layout rebuild
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ICanvasElement, void, LayoutComplete);
            /// @brief On graphic update complete
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ICanvasElement, void, GraphicUpdateComplete);
            /// @brief Is destroyed
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ICanvasElement, bool, IsDestroyed);

        private:
            /// @brief Get stepped rounded normalized value
            float GetSteppedNormalizedValue();
            /// @brief Convert stepped normalized value to value
            /// @param p_NormalizedValue Normalized value
            float ConvertFromSteppedNormalizedValue(float p_NormalizedValue);

        private:
            /// @brief Get text for value
            /// @param p_ConvertedValue Value to convert
            std::u16string GetTextValue(float p_ConvertedValue);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCSlider);