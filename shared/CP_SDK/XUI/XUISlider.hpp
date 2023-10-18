#pragma once

#include "../UI/Components/CSlider.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CSlider XUI Element
    class XUISlider :
        public IXUIElement,
        public IXUIElementReady<XUISlider, _v::CSlider>,
        public IXUIBindable<XUISlider>,
        public std::enable_shared_from_this<XUISlider>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUISlider);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CSlider> m_Element;

            _v::Event<_v::CSlider*> m_OnReady;

        public:
            std::shared_ptr<XUISlider> AsShared() { return std::enable_shared_from_this<XUISlider>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CSlider> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::Slider;
            using Ptr = std::shared_ptr<XUISlider>;

            XUISlider(PRIVATE_TAG_ARG(), std::u16string_view p_Name)
                : IXUIElement(p_Name, TypeID)
            {

            }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                return std::make_shared<XUISlider>(PRIVATE_TAG_VAL(), u"XUISlider");
            }
            /// @brief Constructor
            /// @param p_Name Element name
            static Ptr Make(std::u16string_view p_Name)
            {
                return std::make_shared<XUISlider>(PRIVATE_TAG_VAL(), p_Name);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::SliderFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUISlider.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUISlider* OnReady(_v::CActionRef<_v::CSlider*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUISlider* Bind(std::shared_ptr<XUISlider>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUISlider>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUISlider* OnValueChanged(_v::Action<float> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CSlider, { x->OnValueChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUISlider* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CSlider, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set theme color
            /// @param p_Color New color
            XUISlider* SetColor(_u::Color p_Color) XUI_PROXY_IMPL(_v::CSlider, { x->SetColor(p_Color);  })
            /// @brief Set value formatter
            /// @param p_CustomFormatter Custom value formatter
            XUISlider* SetFormatter(_v::Func<std::u16string, float> p_CustomFormatter) XUI_PROXY_IMPL(_v::CSlider, { x->SetFormatter(p_CustomFormatter); })
            /// @brief Set integer mode
            /// @param p_IsInteger Is integer?
            XUISlider* SetInteger(bool p_IsInteger) XUI_PROXY_IMPL(_v::CSlider, { x->SetInteger(p_IsInteger); })
            /// @brief Set interactable state
            /// @param p_Interactable New state
            XUISlider* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL(_v::CSlider, { x->SetInteractable(p_Interactable); })
            /// @brief Set min value
            /// @param p_MinValue New value
            XUISlider* SetMinValue(float p_MinValue) XUI_PROXY_IMPL(_v::CSlider, { x->SetMinValue(p_MinValue); })
            /// @brief Set max value
            /// @param p_MaxValue New value
            XUISlider* SetMaxValue(float p_MaxValue) XUI_PROXY_IMPL(_v::CSlider, { x->SetMaxValue(p_MaxValue); })
            /// @brief Set increments
            /// @param p_Increments New value
            XUISlider* SetIncrements(float p_Increments) XUI_PROXY_IMPL(_v::CSlider, { x->SetIncrements(p_Increments); })
            /// @brief Set value
            /// @param p_Value Value
            /// @param p_Notify Notify?
            XUISlider* SetValue(float p_Value, bool p_Notify = true) XUI_PROXY_IMPL(_v::CSlider, { x->SetValue(p_Value, p_Notify); })

        public:
            /// @brief Switch to color mode
            /// @param p_H Is Hue mode?
            /// @param p_S Is saturation mode?
            /// @param p_V Is value mode?
            /// @param p_O Is opacity mode?
            XUISlider* SwitchToColorMode(bool p_H, bool p_S, bool p_V, bool p_O) XUI_PROXY_IMPL(_v::CSlider, { x->SwitchToColorMode(p_H, p_S, p_V, p_O); })
            /// @brief Color mode set H
            /// @param p_H Is Hue mode?
            XUISlider* ColorModeSetHue(float p_H) XUI_PROXY_IMPL(_v::CSlider, { x->ColorModeSetHue(p_H); })

    };

}   ///< namespace CP_SDK::XUI