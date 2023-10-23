#pragma once

#include "../UI/Components/CColorInput.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CColorInput XUI Element
    class XUIColorInput :
        public IXUIElement,
        public IXUIElementReady<XUIColorInput, _v::CColorInput>,
        public IXUIBindable<XUIColorInput>,
        public std::enable_shared_from_this<XUIColorInput>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIColorInput);
        CP_SDK_PRIV_TAG();

        private:
            _v::MonoPtr<_v::CColorInput> m_Element;

            _v::Event<_v::CColorInput*> m_OnReady;

        public:
            std::shared_ptr<XUIColorInput> AsShared() { return std::enable_shared_from_this<XUIColorInput>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CColorInput> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::ColorInput;
            using Ptr = std::shared_ptr<XUIColorInput>;

            XUIColorInput(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name)
                : IXUIElement(p_Name, TypeID)
            {

            }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                return std::make_shared<XUIColorInput>(CP_SDK_PRIV_TAG_VAL(), u"XUIColorInput");
            }
            /// @brief Constructor
            /// @param p_Name Element name
            static Ptr Make(std::u16string_view p_Name)
            {
                return std::make_shared<XUIColorInput>(CP_SDK_PRIV_TAG_VAL(), p_Name);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::ColorInputFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIColorInput.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIColorInput* OnReady(_v::CActionRef<_v::CColorInput*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIColorInput* Bind(std::shared_ptr<XUIColorInput>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIColorInput>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUIColorInput* OnValueChanged(_v::Action<_u::Color> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CColorInput, { x->OnValueChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIColorInput* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CColorInput, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set alpha support
            /// @param p_Support New state
            XUIColorInput* SetAlphaSupport(bool p_Support) XUI_PROXY_IMPL(_v::CColorInput, { x->SetAlphaSupport(p_Support); })
            /// @brief Set interable
            /// @param p_Interactable New state
            XUIColorInput* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL(_v::CColorInput, { x->SetInteractable(p_Interactable); })
            /// @brief Set value
            /// @param p_Value New value
            /// @param p_Notify Should notify?
            XUIColorInput* SetValue(_u::Color p_Value, bool p_Notify = true) XUI_PROXY_IMPL(_v::CColorInput, { x->SetValue(p_Value, p_Notify); })

    };

}   ///< namespace CP_SDK::XUI