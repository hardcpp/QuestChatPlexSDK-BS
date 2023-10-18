#pragma once

#include "../../UI/Components/CHLayout.hpp"
#include "../../UI/Components/CVLayout.hpp"
#include "IXUIElementWithChilds.hpp"
#include "IXUIBindable.hpp"
#include "IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CHOrVLayout abstract XUI Element
    /// @tparam t_Base Return type for daisy chaining
    /// @tparam t_Component Element type
    template<class t_Base, class t_Component>
    class _XUIHOrVLayout
        : public IXUIElementWithChilds<t_Base>,
          public IXUIElementReady<t_Base, t_Component>,
          public IXUIBindable<t_Base>,
          public std::enable_shared_from_this<t_Base>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(_XUIHOrVLayout);

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
            /// @param p_Name   Element name
            /// @param p_TypeID Type ID
            /// @param p_Childs Child XUI Elements
            _XUIHOrVLayout(std::u16string_view p_Name, IXUIElement::ETypeID p_TypeID, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : IXUIElementWithChilds<t_Base>(p_Name, p_TypeID, p_Childs)
            { }
            /// @brief Destructor
            virtual ~_XUIHOrVLayout() = default;

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                if constexpr (std::is_same_v<t_Component, _v::CHLayout>)
                    m_Element = _v::UISystem::HLayoutFactory->Create(IXUIElement::GetInitialName(), p_Parent);
                else
                    m_Element = _v::UISystem::VLayoutFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                m_Element->SetSpacing(2.0f);
                m_Element->SetPadding(_u::RectOffset::New_ctor(2, 2, 2, 2));

                IXUIElementWithChilds<t_Base>::BuildUIChilds(m_Element->get_transform());

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    if constexpr (std::is_same_v<t_Component, _v::CHLayout>)
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][_XUIHOrVLayout<CHLayout>.BuildUI] Error OnReady:");
                    else
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][_XUIHOrVLayout<CVLayout}>.BuildUI] Error OnReady:");
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
            /// Set background state
            /// @param p_Enabled Is enabled?
            /// @param p_Color   Optional color, default to black
            /// @param p_RaycastTarget Should raycast target
            t_Base* SetBackground(bool p_Enabled, std::optional<_u::Color> p_Color = std::nullopt, bool p_RaycastTarget = false) XUI_PROXY_IMPL_EX(t_Component, { x->SetBackground(p_Enabled, p_Color, p_RaycastTarget); })
            /// @brief Set background color
            /// @param p_Color New background color
            t_Base* SetBackgroundColor(float p_Color) XUI_PROXY_IMPL_EX(t_Component, { x->SetBackgroundColor(p_Color); })

            /// @brief Set background fill method
            /// @param p_FillMethod New background color
            t_Base* SetBackgroundFillMethod(_u::Image::FillMethod p_FillMethod) XUI_PROXY_IMPL_EX(t_Component, { x->SetBackgroundFillMethod(p_FillMethod); })
            /// @brief Set background fill amount
            /// @param p_FillAmount Fill amount
            t_Base* SetBackgroundFillAmount(float p_FillAmount) XUI_PROXY_IMPL_EX(t_Component, { x->SetBackgroundFillAmount(p_FillAmount); })
            /// @brief Set background sprite
            /// @param p_Sprite New sprite
            /// @param p_Type   Image type
            t_Base* SetBackgroundSprite(_u::Sprite* p_Sprite, _u::Image::Type p_Type = _u::Image::Type::Simple) XUI_PROXY_IMPL_EX(t_Component, { x->SetBackgroundSprite(p_Sprite, p_Type); })

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            t_Base* SetWidth(float p_Width) XUI_PROXY_IMPL_EX(t_Component, { x->SetWidth(p_Width); })
            /// @brief Set preferred height
            /// @param p_Height Height
            t_Base* SetHeight(float p_Height) XUI_PROXY_IMPL_EX(t_Component, { x->SetHeight(p_Height); })

        public:
            /// @brief Set min width
            /// @param p_Width Width
            t_Base* SetMinWidth(float p_Width) XUI_PROXY_IMPL_EX(t_Component, { x->SetMinWidth(p_Width); })
            /// @brief Set min height
            /// @param p_Height Height
            t_Base* SetMinHeight(float p_Height) XUI_PROXY_IMPL_EX(t_Component, { x->SetMinHeight(p_Height); })

        public:
            /// @brief Set padding
            /// @param p_Padding New padding
            t_Base* SetPadding(_u::RectOffset* p_Padding) XUI_PROXY_IMPL_EX(t_Component, { x->SetPadding(p_Padding); })
            /// @brief Set padding
            /// @param p_Top    Top padding
            /// @param p_Right  Right padding
            /// @param p_Bottom Bottom padding
            /// @param p_Left   Left padding
            t_Base* SetPadding(int p_Top, int p_Right, int p_Bottom, int p_Left) XUI_PROXY_IMPL_EX(t_Component, { x->SetPadding(p_Top, p_Right, p_Bottom, p_Left); })
            /// @brief Set padding
            /// @param p_Padding New padding
            t_Base* SetPadding(int p_Padding) XUI_PROXY_IMPL_EX(t_Component, { x->SetPadding(p_Padding); })

        public:
            /// @brief Set spacing between elements
            /// @param p_Spacing New spacing
            t_Base* SetSpacing(float p_Spacing) XUI_PROXY_IMPL_EX(t_Component, { x->SetSpacing(p_Spacing); })

    };

}   ///< namespace CP_SDK::XUI