#pragma once

#include "../UI/Components/CDropdown.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CDropdown XUI Element
    class XUIDropdown :
        public IXUIElement,
        public IXUIElementReady<XUIDropdown, _v::CDropdown>,
        public IXUIBindable<XUIDropdown>,
        public std::enable_shared_from_this<XUIDropdown>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIDropdown);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CDropdown> m_Element;

            _v::Event<_v::CDropdown*> m_OnReady;

        public:
            std::shared_ptr<XUIDropdown> AsShared() { return std::enable_shared_from_this<XUIDropdown>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CDropdown> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::Dropdown;
            using Ptr = std::shared_ptr<XUIDropdown>;

            XUIDropdown(PRIVATE_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::u16string>& p_Options)
                : IXUIElement(p_Name, TypeID)
            {
                if (!p_Options.empty())
                    SetOptions(p_Options);
            }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                static std::vector<std::u16string> s_Empty(0);
                return std::make_shared<XUIDropdown>(PRIVATE_TAG_VAL(), u"XUIDropdown", s_Empty);
            }
            /// @brief Constructor
            static Ptr Make(const std::vector<std::u16string>& p_Options)
            {
                return std::make_shared<XUIDropdown>(PRIVATE_TAG_VAL(), u"XUIDropdown", p_Options);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::u16string>& p_Options)
            {
                return std::make_shared<XUIDropdown>(PRIVATE_TAG_VAL(), p_Name, p_Options);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::DropdownFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIDropdown.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIDropdown* OnReady(_v::CActionRef<_v::CDropdown*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIDropdown* Bind(std::shared_ptr<XUIDropdown>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIDropdown>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUIDropdown* OnValueChanged(_v::Action<int, std::u16string_view> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CDropdown, { x->OnValueChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIDropdown* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CDropdown, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set interable
            /// @param p_Interactable New state
            XUIDropdown* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL(_v::CDropdown, { x->SetInteractable(p_Interactable); })
            /// @brief Set available options
            /// @param p_Options New options list
            XUIDropdown* SetOptions(std::vector<std::u16string> p_Options) XUI_PROXY_IMPL(_v::CDropdown, { x->SetOptions(p_Options); })
            /// @brief Set value
            /// @param p_Value New value
            /// @param p_Notify Should notify?
            XUIDropdown* SetValue(std::u16string_view p_Value, bool p_Notify = true) XUI_PROXY_IMPL(_v::CDropdown, { x->SetValue(p_Value, p_Notify); })

    };

}   ///< namespace CP_SDK::XUI