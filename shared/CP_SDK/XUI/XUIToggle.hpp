#pragma once

#include "../UI/Components/CToggle.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CToggle XUI Element
    class XUIToggle :
        public IXUIElement,
        public IXUIElementReady<XUIToggle, _v::CToggle>,
        public IXUIBindable<XUIToggle>,
        public std::enable_shared_from_this<XUIToggle>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIToggle);
        CP_SDK_PRIV_TAG();

        private:
            _v::MonoPtr<_v::CToggle> m_Element;

            _v::Event<_v::CToggle*> m_OnReady;

        public:
            std::shared_ptr<XUIToggle> AsShared() { return std::enable_shared_from_this<XUIToggle>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CToggle> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::Toggle;
            using Ptr = std::shared_ptr<XUIToggle>;

            XUIToggle(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name)
                : IXUIElement(p_Name, TypeID)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                return std::make_shared<XUIToggle>(CP_SDK_PRIV_TAG_VAL(), u"XUIToggle");
            }
            /// @brief Constructor
            /// @param p_Name Element name
            static Ptr Make(std::u16string_view p_Name)
            {
                return std::make_shared<XUIToggle>(CP_SDK_PRIV_TAG_VAL(), p_Name);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::ToggleFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIToggle.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIToggle* OnReady(_v::CActionRef<_v::CToggle*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIToggle* Bind(std::shared_ptr<XUIToggle>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIToggle>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUIToggle* OnValueChanged(_v::Action<bool> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CToggle, { x->OnValueChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIToggle* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CToggle, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            XUIToggle* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL(_v::CToggle, { x->SetInteractable(p_Interactable); })
            /// @brief Set value
            /// @param p_Value  Value
            /// @param p_Notify Notify?
            XUIToggle* SetValue(bool p_Value, bool p_Notify = true) XUI_PROXY_IMPL(_v::CToggle, { x->SetValue(p_Value, p_Notify); })

    };

}   ///< namespace CP_SDK::XUI