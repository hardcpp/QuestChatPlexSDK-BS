#pragma once

#include "../../UI/Components/CHLayout.hpp"
#include "../../UI/Components/CVLayout.hpp"
#include "IXUIElement.hpp"
#include "IXUIBindable.hpp"
#include "IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief IHOrVLayout abstract XUI Element
    /// @tparam t_Base Return type for daisy chaining
    /// @tparam t_Component Element type
    template<class t_Base, class t_Component>
    class _XUIHOrVSpacer
        : public IXUIElement,
          public IXUIElementReady<t_Base, t_Component>,
          public IXUIBindable<t_Base>,
          public std::enable_shared_from_this<t_Base>
    {
        CP_SDK_NO_COPYMOVE_CTORS(_XUIHOrVSpacer);

        private:
            _v::MonoPtr<t_Component> m_Element;

            _v::Event<t_Component*> m_OnReady;

        public:
            std::shared_ptr<t_Base> AsShared() { return std::enable_shared_from_this<t_Base>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<t_Component> Element()
            {
                return m_Element;
            }

        protected:
            /// @brief Constructor
            /// @param p_Name    Element name
            /// @param p_TypeID  Type ID
            /// @param p_Spacing Spacing
            _XUIHOrVSpacer(std::u16string_view p_Name, IXUIElement::ETypeID p_TypeID, float p_Spacing)
                : IXUIElement(p_Name, p_TypeID)
            {
                SetSpacing(p_Spacing);
            }
            /// @brief Destructor
            virtual ~_XUIHOrVSpacer() = default;

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                if constexpr (std::is_same_v<t_Component, _v::CHLayout>)
                    m_Element = _v::UISystem::HLayoutFactory->Create(IXUIElement::GetInitialName(), p_Parent);
                else
                    m_Element = _v::UISystem::VLayoutFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][_XUIHOrVSpacer<{typeof(t_Component).Name}>.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual t_Base* OnReady(_v::CActionRef<t_Component*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return reinterpret_cast<t_Base*>(this);
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual t_Base* Bind(std::shared_ptr<t_Base>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<t_Base>::shared_from_this();
                return reinterpret_cast<t_Base*>(this);
            }

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            t_Base* SetActive(bool p_Active) XUI_PROXY_IMPL_EX(t_Component, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            t_Base* SetSpacing(float p_Spacing) XUI_PROXY_IMPL_EX(t_Component, {
                if constexpr (std::is_same_v<t_Component, _v::CHLayout>)
                { x->SetMinWidth(p_Spacing); x->SetWidth(p_Spacing); }
                else
                { x->SetMinHeight(p_Spacing); x->SetHeight(p_Spacing); }
            })

    };

}   ///< namespace CP_SDK::XUI