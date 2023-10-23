#pragma once

#include "../UI/Components/CImage.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    using namespace UnityEngine;
    using namespace UnityEngine::UI;

    /// @brief CImage XUI Element
    class XUIImage :
        public IXUIElement,
        public IXUIElementReady<XUIImage, _v::CImage>,
        public IXUIBindable<XUIImage>,
        public std::enable_shared_from_this<XUIImage>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIImage);
        CP_SDK_PRIV_TAG();

        private:
            _v::MonoPtr<_v::CImage> m_Element;

            _v::Event<_v::CImage*> m_OnReady;

        public:
            std::shared_ptr<XUIImage> AsShared() { return std::enable_shared_from_this<XUIImage>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CImage> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::Image;
            using Ptr = std::shared_ptr<XUIImage>;

            XUIImage(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, _u::Sprite* p_Sprite)
                : IXUIElement(p_Name, TypeID)
            {
                if (p_Sprite)
                    SetSprite(p_Sprite);
            }

        public:
            /// @brief Constructor
            /// @param p_Sprite Sprite
            static Ptr Make(_u::Sprite* p_Sprite = nullptr)
            {
                return std::make_shared<XUIImage>(CP_SDK_PRIV_TAG_VAL(), u"XUIImage", p_Sprite);
            }
            /// @brief Constructor
            /// @param p_Name   Element name
            /// @param p_Sprite Sprite
            static Ptr Make(std::u16string_view p_Name, _u::Sprite* p_Sprite = nullptr)
            {
                return std::make_shared<XUIImage>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Sprite);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::ImageFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIImage.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIImage* OnReady(_v::CActionRef<_v::CImage*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIImage* Bind(std::shared_ptr<XUIImage>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIImage>::shared_from_this();
                return this;
            }

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIImage* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CImage, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            XUIImage* SetWidth(float p_Width) XUI_PROXY_IMPL(_v::CImage, { x->SetWidth(p_Width); })
            /// @brief Set preferred height
            /// @param p_Height Height
            XUIImage* SetHeight(float p_Height) XUI_PROXY_IMPL(_v::CImage, { x->SetHeight(p_Height); })

        public:
            /// @brief Set min width
            /// @param p_Width Width
            XUIImage* SetMinWidth(float p_Width) XUI_PROXY_IMPL(_v::CImage, { x->SetMinWidth(p_Width); })
            /// @brief Set min height
            /// @param p_Height Height
            XUIImage* SetMinHeight(float p_Height) XUI_PROXY_IMPL(_v::CImage, { x->SetMinHeight(p_Height); })

        public:
            /// @brief Set color
            /// @param p_Color New color
            XUIImage* SetColor(_u::Color p_Color) XUI_PROXY_IMPL(_v::CImage, { x->SetColor(p_Color); })
            /// @brief Set enhanced image
            /// @param p_EnhancedImage New enhanced image
            XUIImage* SetEnhancedImage(Unity::EnhancedImage::Ptr p_EnhancedImage) XUI_PROXY_IMPL(_v::CImage, { x->SetEnhancedImage(p_EnhancedImage); })
            /// @brief Set pixels per unit multiplier
            /// @param p_Multiplier New multiplier
            XUIImage* SetPixelsPerUnitMultiplier(float p_Multiplier) XUI_PROXY_IMPL(_v::CImage, { x->SetPixelsPerUnitMultiplier(p_Multiplier); })
            /// @brief Set sprite
            /// @param p_Sprite New sprite
            XUIImage* SetSprite(_u::Sprite* p_Sprite) XUI_PROXY_IMPL(_v::CImage, { x->SetSprite(p_Sprite); })
            /// @brief Set type
            /// @param p_Type Image type
            XUIImage* SetType(_u::Image::Type p_Type) XUI_PROXY_IMPL(_v::CImage, { x->SetType(p_Type); })

    };

}   ///< namespace CP_SDK::XUI