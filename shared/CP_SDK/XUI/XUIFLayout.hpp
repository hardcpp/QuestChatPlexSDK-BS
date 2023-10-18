#pragma once

#include "../UI/Components/CFLayout.hpp"
#include "Generics/IXUIElementWithChilds.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CFLayout XUI Element
    class XUIFLayout
        : public IXUIElementWithChilds<XUIFLayout>,
          public IXUIElementReady<XUIFLayout, _v::CFLayout>,
          public IXUIBindable<XUIFLayout>,
          public std::enable_shared_from_this<XUIFLayout>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIFLayout);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CFLayout> m_Element;

            _v::Event<_v::CFLayout*> m_OnReady;

        public:
            std::shared_ptr<XUIFLayout> AsShared() { return std::enable_shared_from_this<XUIFLayout>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CFLayout> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::FLayout;
            using Ptr = std::shared_ptr<XUIFLayout>;

            XUIFLayout(PRIVATE_TAG_ARG(), const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : XUIFLayout(PRIVATE_TAG_FWD(), u"XUIFLayout", p_Childs)
            { }
            XUIFLayout(PRIVATE_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : IXUIElementWithChilds<XUIFLayout>(p_Name, TypeID, p_Childs)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                static std::vector<std::shared_ptr<IXUIElement>> s_Empty(0);
                return std::make_shared<XUIFLayout>(PRIVATE_TAG_VAL(), s_Empty);
            }
            /// @brief Constructor
            /// @param p_Childs Child XUI Elements
            static Ptr Make(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIFLayout>(PRIVATE_TAG_VAL(), p_Childs);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_Childs Child XUI Elements
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIFLayout>(PRIVATE_TAG_VAL(), p_Name, p_Childs);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::FLayoutFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                IXUIElementWithChilds<XUIFLayout>::BuildUIChilds(m_Element->get_transform());

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIFLayout.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIFLayout* OnReady(_v::CActionRef<_v::CFLayout*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return reinterpret_cast<XUIFLayout*>(this);
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIFLayout* Bind(std::shared_ptr<XUIFLayout>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIFLayout>::shared_from_this();
                return reinterpret_cast<XUIFLayout*>(this);
            }

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIFLayout* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CFLayout, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            XUIFLayout* SetWidth(float p_Width) XUI_PROXY_IMPL(_v::CFLayout, { x->SetWidth(p_Width); })
            /// @brief Set preferred height
            /// @param p_Height Height
            XUIFLayout* SetHeight(float p_Height) XUI_PROXY_IMPL(_v::CFLayout, { x->SetHeight(p_Height); })

        public:
            /// @brief Set min width
            /// @param p_Width Width
            XUIFLayout* SetMinWidth(float p_Width) XUI_PROXY_IMPL(_v::CFLayout, { x->SetMinWidth(p_Width); })
            /// @brief Set min height
            /// @param p_Height Height
            XUIFLayout* SetMinHeight(float p_Height) XUI_PROXY_IMPL(_v::CFLayout, { x->SetMinHeight(p_Height); })

        public:
            /// @brief Set child alignment
            /// @param p_ChildAlign New alignment
            XUIFLayout* SetChildAlign(_u::TextAnchor p_ChildAlign) XUI_PROXY_IMPL(_v::CFLayout, { x->SetChildAlign(p_ChildAlign); })
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            XUIFLayout* SetSpacing(_u::Vector2 p_Spacing) XUI_PROXY_IMPL(_v::CFLayout, { x->SetSpacing(p_Spacing); })

    };

}   ///< namespace CP_SDK::XUI