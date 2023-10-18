#pragma once

#include "../../UI/Components/CPrimaryButton.hpp"
#include "../../UI/Components/CSecondaryButton.hpp"
#include "IXUIElement.hpp"
#include "IXUIBindable.hpp"
#include "IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CPOrSButton abstract XUI Element
    /// @tparam t_Base Return type for daisy chaining
    /// @tparam t_Component Element type
    template<class t_Base, class t_Component>
    class _XUIPOrSButton
        : public IXUIElement,
          public IXUIElementReady<t_Base, t_Component>,
          public IXUIBindable<t_Base>,
          public std::enable_shared_from_this<t_Base>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(_XUIPOrSButton);

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
            /// @param p_Label   Button label
            /// @param p_OnClick On click
            _XUIPOrSButton(std::u16string_view p_Name, IXUIElement::ETypeID p_TypeID, std::u16string_view p_Label, _v::CActionRef<> p_OnClick)
                : IXUIElement(p_Name, p_TypeID)
            {
                if (p_OnClick.IsValid())
                    OnClick(p_OnClick);

                SetText(std::u16string(p_Label));
            }
            /// @brief Destructor
            virtual ~_XUIPOrSButton() = default;

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                if constexpr (std::is_same_v<t_Component, _v::CPrimaryButton>)
                    m_Element = _v::UISystem::PrimaryButtonFactory->Create(IXUIElement::GetInitialName(), p_Parent);
                else
                    m_Element = _v::UISystem::SecondaryButtonFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    if constexpr (std::is_same_v<t_Component, _v::CPrimaryButton>)
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][_XUIPOrSButton<CPrimaryButton>.BuildUI] Error OnReady:");
                    else
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][_XUIPOrSButton<CSecondaryButton>.BuildUI] Error OnReady:");
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
            /// @brief On click event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            t_Base* OnClick(_v::Action<> p_Functor, bool p_Add = true) XUI_PROXY_IMPL_EX(t_Component, { x->OnClick(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            t_Base* SetActive(bool p_Active) XUI_PROXY_IMPL_EX(t_Component, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            t_Base* SetWidth(float p_Width) XUI_PROXY_IMPL_EX(t_Component, { x->SetWidth(p_Width); })
            /// @brief Set preferred height
            /// @param p_Height Height
            t_Base* SetHeight(float p_Height) XUI_PROXY_IMPL_EX(t_Component, { x->SetHeight(p_Height); })

        public:
            /// @brief Set button background sprite
            /// @param p_Sprite New sprite
            t_Base* SetBackgroundSprite(_u::Sprite* p_Sprite) XUI_PROXY_IMPL_EX(t_Component, { x->SetBackgroundSprite(p_Sprite); })
            /// @brief Set font size
            /// @param p_Size New size
            t_Base* SetFontSize(float p_Size) XUI_PROXY_IMPL_EX(t_Component, { x->SetFontSize(p_Size); })
            /// @brief Set button icon sprite
            /// @param p_Sprite New sprite
            t_Base* SetIconSprite(_u::Sprite* p_Sprite) XUI_PROXY_IMPL_EX(t_Component, { x->SetIconSprite(p_Sprite); })
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            t_Base* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL_EX(t_Component, { x->SetInteractable(p_Interactable); })
            /// @brief Set overflow mode
            /// @param p_OverflowMode New overflow mdoe
            t_Base* SetOverflowMode(_u::TextOverflowModes p_OverflowMode) XUI_PROXY_IMPL_EX(t_Component, { x->SetOverflowMode(p_OverflowMode); })
            /// @brief Set button text
            /// @param p_Text New text
            t_Base* SetText(std::u16string p_Text) XUI_PROXY_IMPL_EX(t_Component, { x->SetText(p_Text); })
            /// @brief Set tooltip
            /// @param p_Tooltip New tooltip
            t_Base* SetTooltip(std::u16string p_Tooltip) XUI_PROXY_IMPL_EX(t_Component, { x->SetTooltip(p_Tooltip); })

    };

}   ///< namespace CP_SDK::XUI