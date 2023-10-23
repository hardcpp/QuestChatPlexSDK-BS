#pragma once

#include "../UI/Components/CTextSegmentedControl.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CTextSegmentedControl XUI Element
    class XUITextSegmentedControl :
        public IXUIElement,
        public IXUIElementReady<XUITextSegmentedControl, _v::CTextSegmentedControl>,
        public IXUIBindable<XUITextSegmentedControl>,
        public std::enable_shared_from_this<XUITextSegmentedControl>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUITextSegmentedControl);
        CP_SDK_PRIV_TAG();

        private:
            _v::MonoPtr<_v::CTextSegmentedControl> m_Element;

            _v::Event<_v::CTextSegmentedControl*> m_OnReady;

        public:
            std::shared_ptr<XUITextSegmentedControl> AsShared() { return std::enable_shared_from_this<XUITextSegmentedControl>::shared_from_this(); }

            virtual _v::UIFieldRef<RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CTextSegmentedControl> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::TextSegmentedControl;
            using Ptr = std::shared_ptr<XUITextSegmentedControl>;

            XUITextSegmentedControl(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::u16string>& p_Texts)
                : IXUIElement(p_Name, TypeID)
            {
                SetTexts(p_Texts);
            }

        public:
            /// @brief Constructor
            /// @param p_Spacing Spacing
            static Ptr Make(const std::vector<std::u16string>& p_Texts)
            {
                return std::make_shared<XUITextSegmentedControl>(CP_SDK_PRIV_TAG_VAL(), u"XUITextSegmentedControl", p_Texts);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_TextSegmentedControl TextSegmentedControl
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::u16string>& p_Texts)
            {
                return std::make_shared<XUITextSegmentedControl>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Texts);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::TextSegmentedControlFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUITextSegmentedControl.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUITextSegmentedControl* OnReady(_v::CActionRef<_v::CTextSegmentedControl*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUITextSegmentedControl* Bind(std::shared_ptr<XUITextSegmentedControl>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUITextSegmentedControl>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUITextSegmentedControl* OnActiveChanged(_v::Action<int> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CTextSegmentedControl, { x->OnActiveChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUITextSegmentedControl* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CTextSegmentedControl, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set active text
            /// @param p_Index New active index
            /// @param p_Notify Should notify?
            XUITextSegmentedControl* SetActiveText(int p_Index, bool p_Notify = true) XUI_PROXY_IMPL(_v::CTextSegmentedControl, { x->SetActiveText(p_Index, p_Notify); })
            /// @brief Set texts
            /// @param p_Texts New texts
            XUITextSegmentedControl* SetTexts(std::vector<std::u16string> p_Texts) XUI_PROXY_IMPL(_v::CTextSegmentedControl, { x->SetTexts(p_Texts); })

    };

}   ///< namespace CP_SDK::XUI