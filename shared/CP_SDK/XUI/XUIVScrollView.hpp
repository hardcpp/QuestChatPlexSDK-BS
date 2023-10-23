#pragma once

#include "../UI/Components/CVScrollView.hpp"
#include "Generics/IXUIElementWithChilds.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CVScrollView XUI Element
    class XUIVScrollView :
        public IXUIElementWithChilds<XUIVScrollView>,
        public IXUIElementReady<XUIVScrollView, _v::CVScrollView>,
        public IXUIBindable<XUIVScrollView>,
        public std::enable_shared_from_this<XUIVScrollView>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIVScrollView);
        CP_SDK_PRIV_TAG();

        private:
            _v::MonoPtr<_v::CVScrollView> m_Element;

            _v::Event<_v::CVScrollView*> m_OnReady;

        public:
            std::shared_ptr<XUIVScrollView> AsShared() { return std::enable_shared_from_this<XUIVScrollView>::shared_from_this(); }

            virtual _v::UIFieldRef<RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CVScrollView> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::VScrollView;
            using Ptr = std::shared_ptr<XUIVScrollView>;

            XUIVScrollView(CP_SDK_PRIV_TAG_ARG(), const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : XUIVScrollView(CP_SDK_PRIV_TAG_FWD(), u"XUIVLayout", p_Childs)
            { }
            XUIVScrollView(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : IXUIElementWithChilds<XUIVScrollView>(p_Name, TypeID, p_Childs)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                static std::vector<std::shared_ptr<IXUIElement>> s_Empty(0);
                return std::make_shared<XUIVScrollView>(CP_SDK_PRIV_TAG_VAL(), s_Empty);
            }
            /// @brief Constructor
            /// @param p_Childs Child XUI Elements
            static Ptr Make(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIVScrollView>(CP_SDK_PRIV_TAG_VAL(), u"XUIVScrollView", p_Childs);
            }
            /// @brief Constructor
            /// @param p_Name   Element name
            /// @param p_Childs Child XUI Elements
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIVScrollView>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Childs);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::VScrollViewFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                BuildUIChilds(m_Element->Container().Ptr());

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIVScrollView.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIVScrollView* OnReady(_v::CActionRef<_v::CVScrollView*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIVScrollView* Bind(std::shared_ptr<XUIVScrollView>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIVScrollView>::shared_from_this();
                return this;
            }

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIVScrollView* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CVScrollView, { x->get_gameObject()->SetActive(p_Active); })

    };

}   ///< namespace CP_SDK::XUI