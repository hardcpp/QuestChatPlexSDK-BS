#pragma once

#include "../UI/Components/CGLayout.hpp"
#include "Generics/IXUIElementWithChilds.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CGLayout XUI Element
    class XUIGLayout
        : public IXUIElementWithChilds<XUIGLayout>,
          public IXUIElementReady<XUIGLayout, _v::CGLayout>,
          public IXUIBindable<XUIGLayout>,
          public std::enable_shared_from_this<XUIGLayout>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIGLayout);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CGLayout> m_Element;

            _v::Event<_v::CGLayout*> m_OnReady;

        public:
            std::shared_ptr<XUIGLayout> AsShared() { return std::enable_shared_from_this<XUIGLayout>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CGLayout> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::GLayout;
            using Ptr = std::shared_ptr<XUIGLayout>;

            XUIGLayout(PRIVATE_TAG_ARG(), const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : XUIGLayout(PRIVATE_TAG_FWD(), u"XUIGLayout", p_Childs)
            { }
            XUIGLayout(PRIVATE_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : IXUIElementWithChilds<XUIGLayout>(p_Name, TypeID, p_Childs)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                static std::vector<std::shared_ptr<IXUIElement>> s_Empty(0);
                return std::make_shared<XUIGLayout>(PRIVATE_TAG_VAL(), s_Empty);
            }
            /// @brief Constructor
            /// @param p_Childs Child XUI Elements
            static Ptr Make(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIGLayout>(PRIVATE_TAG_VAL(), p_Childs);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_Childs Child XUI Elements
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIGLayout>(PRIVATE_TAG_VAL(), p_Name, p_Childs);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::GLayoutFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                IXUIElementWithChilds<XUIGLayout>::BuildUIChilds(m_Element->get_transform());

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIGLayout.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIGLayout* OnReady(_v::CActionRef<_v::CGLayout*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return reinterpret_cast<XUIGLayout*>(this);
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIGLayout* Bind(std::shared_ptr<XUIGLayout>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIGLayout>::shared_from_this();
                return reinterpret_cast<XUIGLayout*>(this);
            }

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIGLayout* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CGLayout, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            XUIGLayout* SetWidth(float p_Width) XUI_PROXY_IMPL(_v::CGLayout, { x->SetWidth(p_Width); })
            /// @brief Set preferred height
            /// @param p_Height Height
            XUIGLayout* SetHeight(float p_Height) XUI_PROXY_IMPL(_v::CGLayout, { x->SetHeight(p_Height); })

        public:
            /// @brief Set min width
            /// @param p_Width Width
            XUIGLayout* SetMinWidth(float p_Width) XUI_PROXY_IMPL(_v::CGLayout, { x->SetMinWidth(p_Width); })
            /// @brief Set min height
            /// @param p_Height Height
            XUIGLayout* SetMinHeight(float p_Height) XUI_PROXY_IMPL(_v::CGLayout, { x->SetMinHeight(p_Height); })

        public:
            /// @brief Set cell size
            /// @param p_CellSize New size
            XUIGLayout* SetCellSize(_u::Vector2 p_CellSize) XUI_PROXY_IMPL(_v::CGLayout, { x->SetCellSize(p_CellSize); })
            /// @brief Set child alignment
            /// @param p_ChildAlign New alignment
            XUIGLayout* SetChildAlign(_u::TextAnchor p_ChildAlign) XUI_PROXY_IMPL(_v::CGLayout, { x->SetChildAlign(p_ChildAlign); })
            /// @brief Set layout constraint
            /// @param p_Constraint New value
            XUIGLayout* SetConstraint(_u::GridLayoutGroup::Constraint p_Constraint) XUI_PROXY_IMPL(_v::CGLayout, { x->SetConstraint(p_Constraint); })
            /// @brief Set layout constraint count
            /// @param p_ConstraintCount New value
            XUIGLayout* SetConstraintCount(int p_ConstraintCount) XUI_PROXY_IMPL(_v::CGLayout, { x->SetConstraintCount(p_ConstraintCount); })
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            XUIGLayout* SetSpacing(_u::Vector2 p_Spacing) XUI_PROXY_IMPL(_v::CGLayout, { x->SetSpacing(p_Spacing); })

    };

}   ///< namespace CP_SDK::XUI