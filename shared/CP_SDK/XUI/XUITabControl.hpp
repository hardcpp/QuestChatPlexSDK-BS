#pragma once

#include "../UI/Components/CTabControl.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CTabControl XUI Element
    class XUITabControl :
        public IXUIElement,
        public IXUIElementReady<XUITabControl, _v::CTabControl>,
        public IXUIBindable<XUITabControl>,
        public std::enable_shared_from_this<XUITabControl>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUITabControl);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CTabControl> m_Element;

            _v::Event<_v::CTabControl*> m_OnReady;

        public:
            using t_TabDefinition = std::tuple<std::u16string, std::shared_ptr<IXUIElement>>;

            std::shared_ptr<XUITabControl> AsShared() { return std::enable_shared_from_this<XUITabControl>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CTabControl> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::TabControl;
            using Ptr = std::shared_ptr<XUITabControl>;

            XUITabControl(PRIVATE_TAG_ARG(), std::u16string_view p_Name, const std::vector<t_TabDefinition>& p_Tabs)
                : IXUIElement(p_Name, TypeID)
            {
                SetTabs(p_Tabs);
            }

        public:
            /// @brief Constructor
            /// @param p_Spacing Spacing
            static Ptr Make(const std::vector<t_TabDefinition>& p_Tabs)
            {
                return std::make_shared<XUITabControl>(PRIVATE_TAG_VAL(), u"XUITabControl", p_Tabs);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_TabControl TabControl
            static Ptr Make(std::u16string_view p_Name, const std::vector<t_TabDefinition>& p_Tabs)
            {
                return std::make_shared<XUITabControl>(PRIVATE_TAG_VAL(), p_Name, p_Tabs);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::TabControlFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUITabControl.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUITabControl* OnReady(_v::CActionRef<_v::CTabControl*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUITabControl* Bind(std::shared_ptr<XUITabControl>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUITabControl>::shared_from_this();
                return this;
            }

        public:
            /// @brief On value changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUITabControl* OnActiveChanged(_v::Action<int> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CTabControl, { x->OnActiveChanged(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUITabControl* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CTabControl, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set active tab
            /// @param p_Index New active index
            /// @param p_Notify Should notify?
            XUITabControl* SetActiveTab(int p_Index, bool p_Notify = true) XUI_PROXY_IMPL(_v::CTabControl, { x->SetActiveTab(p_Index, p_Notify); })
            /// @brief Set tabs
            /// @param p_Tabs New tabs
            XUITabControl* SetTabs(std::vector<t_TabDefinition> p_Tabs) XUI_PROXY_IMPL(_v::CTabControl, {
                auto l_Converted = std::vector<_v::CTabControl::t_TabDefinition>();
                for (auto l_I = 0; l_I < p_Tabs.size(); ++l_I)
                {
                    auto& l_IXUIElement = std::get<1>(p_Tabs[l_I]);
                    if (!l_IXUIElement->RTransform())
                        l_IXUIElement->BuildUI(nullptr);

                    auto l_RTransform = l_IXUIElement->RTransform().Ptr();
                    l_Converted.push_back(std::make_tuple(std::get<0>(p_Tabs[l_I]), l_RTransform));
                }

                x->SetTabs(l_Converted);
            })

    };

}   ///< namespace CP_SDK::XUI