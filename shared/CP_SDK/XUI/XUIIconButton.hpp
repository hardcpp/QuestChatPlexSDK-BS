#pragma once

#include "../UI/Components/CIconButton.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CIconButton XUI Element
    class XUIIconButton :
        public IXUIElement,
        public IXUIElementReady<XUIIconButton, _v::CIconButton>,
        public IXUIBindable<XUIIconButton>,
        public std::enable_shared_from_this<XUIIconButton>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIIconButton);
        CP_SDK_PRIV_TAG();

        private:
            _v::MonoPtr<_v::CIconButton> m_Element;

            _v::Event<_v::CIconButton*> m_OnReady;

        public:
            std::shared_ptr<XUIIconButton> AsShared() { return std::enable_shared_from_this<XUIIconButton>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CIconButton> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::IconButton;
            using Ptr = std::shared_ptr<XUIIconButton>;

            XUIIconButton(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, _u::Sprite* p_Sprite, _v::CActionRef<> p_OnClick)
                : IXUIElement(p_Name, TypeID)
            {
                if (p_Sprite)
                    SetSprite(p_Sprite);

                if (p_OnClick.IsValid())
                    OnClick(p_OnClick);
            }

        public:
            /// @brief Constructor
            /// @param p_Sprite  Sprite
            /// @param p_OnClick On click callback
            static Ptr Make(_v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUIIconButton>(CP_SDK_PRIV_TAG_VAL(), u"XUIIconButton", nullptr, p_OnClick);
            }
            /// @brief Constructor
            /// @param p_Sprite  Sprite
            /// @param p_OnClick On click callback
            static Ptr Make(_u::Sprite* p_Sprite, _v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUIIconButton>(CP_SDK_PRIV_TAG_VAL(), u"XUIIconButton", p_Sprite, p_OnClick);
            }
            /// @brief Constructor
            /// @param p_Name    Element name
            /// @param p_Sprite  Sprite
            /// @param p_OnClick On click callback
            static Ptr Make(std::u16string_view p_Name, _u::Sprite* p_Sprite, _v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUIIconButton>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Sprite, p_OnClick);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::IconButtonFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIIconButton.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIIconButton* OnReady(_v::CActionRef<_v::CIconButton*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIIconButton* Bind(std::shared_ptr<XUIIconButton>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIIconButton>::shared_from_this();
                return this;
            }

        public:
            /// @brief On click event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            XUIIconButton* OnClick(_v::Action<> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CIconButton, { x->OnClick(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIIconButton* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CIconButton, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            XUIIconButton* SetWidth(float p_Width) XUI_PROXY_IMPL(_v::CIconButton, { x->SetWidth(p_Width); })
            /// @brief Set preferred height
            /// @param p_Height Height
            XUIIconButton* SetHeight(float p_Height) XUI_PROXY_IMPL(_v::CIconButton, { x->SetHeight(p_Height); })

        public:
            /// @brief Set color
            /// @param p_Color New color
            XUIIconButton* SetColor(_u::Color p_Color) XUI_PROXY_IMPL(_v::CIconButton, { x->SetColor(p_Color); })
            /// @brief Set button interactable state
            /// @param p_Interactable New state
            XUIIconButton* SetInteractable(bool p_Interactable) XUI_PROXY_IMPL(_v::CIconButton, { x->SetInteractable(p_Interactable); })
            /// @brief Set sprite
            /// @param p_Sprite New sprite
            XUIIconButton* SetSprite(_u::Sprite* p_Sprite) XUI_PROXY_IMPL(_v::CIconButton, { x->SetSprite(p_Sprite); })

    };

}   ///< namespace CP_SDK::XUI