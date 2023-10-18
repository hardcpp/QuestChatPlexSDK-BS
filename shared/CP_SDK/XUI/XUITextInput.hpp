#pragma once

#include "../UI/Components/CTextInput.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CTextInput XUI Element
    class XUITextInput :
        public IXUIElement,
        public IXUIElementReady<XUITextInput, _v::CTextInput>,
        public IXUIBindable<XUITextInput>,
        public std::enable_shared_from_this<XUITextInput>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUITextInput);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CTextInput> m_Element;

            _v::Event<_v::CTextInput*> m_OnReady;

        public:
            std::shared_ptr<XUITextInput> AsShared() { return std::enable_shared_from_this<XUITextInput>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CTextInput> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::TextInput;
            using Ptr = std::shared_ptr<XUITextInput>;

            XUITextInput(PRIVATE_TAG_ARG(), std::u16string_view p_Name, std::u16string_view p_PlaceHolder)
                : IXUIElement(p_Name, TypeID)
            {
                SetPlaceHolder(std::u16string(p_PlaceHolder));
            }

        public:
            /// @brief Constructor
            /// @param p_PlaceHolder Place holder
            static Ptr Make(std::u16string_view p_PlaceHolder)
            {
                return std::make_shared<XUITextInput>(PRIVATE_TAG_VAL(), u"XUITextInput", p_PlaceHolder);
            }
            /// @brief Constructor
            /// @param p_Name        Element name
            /// @param p_PlaceHolder Place holder
            static Ptr Make(std::u16string_view p_Name, std::u16string_view p_PlaceHolder)
            {
                return std::make_shared<XUITextInput>(PRIVATE_TAG_VAL(), p_Name, p_PlaceHolder);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::TextInputFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUITextInput.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUITextInput* OnReady(_v::CActionRef<_v::CTextInput*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUITextInput* Bind(std::shared_ptr<XUITextInput>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUITextInput>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUITextInput* OnValueChanged(_v::Delegate<void(std::u16string_view)> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CTextInput, { x->OnValueChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUITextInput* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CTextInput, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set button interable
            /// @param p_Interactable New state
            XUITextInput* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL(_v::CTextInput, { x->SetInteractable(p_Interactable); })
            /// @brief Set is password
            /// @param p_IsPassword Is password?
            XUITextInput* SetIsPassword(bool p_IsPassword) XUI_PROXY_IMPL(_v::CTextInput, { x->SetIsPassword(p_IsPassword); })
            /// @brief Set place holder
            /// @param p_PlaceHolder New place holder
            XUITextInput* SetPlaceHolder(std::u16string p_PlaceHolder) XUI_PROXY_IMPL(_v::CTextInput, { x->SetPlaceHolder(p_PlaceHolder); })
            /// @brief Set value
            /// @param p_Value New value
            /// @param p_Notify Should notify?
            XUITextInput* SetValue(std::u16string p_Value, bool p_Notify = true) XUI_PROXY_IMPL(_v::CTextInput, { x->SetValue(p_Value, p_Notify); })

    };

}   ///< namespace CP_SDK::XUI