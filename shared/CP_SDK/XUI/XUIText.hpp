#pragma once

#include "../UI/Components/CText.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CText XUI Element
    class XUIText :
        public IXUIElement,
        public IXUIElementReady<XUIText, _v::CText>,
        public IXUIBindable<XUIText>,
        public std::enable_shared_from_this<XUIText>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIText);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CText> m_Element;

            _v::Event<_v::CText*> m_OnReady;

        public:
            std::shared_ptr<XUIText> AsShared() { return std::enable_shared_from_this<XUIText>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CText> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::Text;
            using Ptr = std::shared_ptr<XUIText>;

            XUIText(PRIVATE_TAG_ARG(), std::u16string_view p_Name, std::u16string_view p_Text)
                : IXUIElement(p_Name, TypeID)
            {
                SetText(std::u16string(p_Text));
            }

        public:
            /// @brief Constructor
            /// @param p_Spacing Spacing
            static Ptr Make(std::u16string_view p_Text)
            {
                return std::make_shared<XUIText>(PRIVATE_TAG_VAL(), u"XUIText", p_Text);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_Text Text
            static Ptr Make(std::u16string_view p_Name, std::u16string_view p_Text)
            {
                return std::make_shared<XUIText>(PRIVATE_TAG_VAL(), p_Name, p_Text);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::TextFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIText.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIText* OnReady(_v::CActionRef<_v::CText*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIText* Bind(std::shared_ptr<XUIText>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIText>::shared_from_this();
                return this;
            }

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIText* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CText, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Set align
            /// @param p_Align New align
            XUIText* SetAlign(_u::TextAlignmentOptions p_Align) XUI_PROXY_IMPL(_v::CText, { x->SetAlign(p_Align); })
            /// @brief Set alpha
            /// @param p_Alpha New alpha
            XUIText* SetAlpha(float p_Alpha) XUI_PROXY_IMPL(_v::CText, { x->SetAlpha(p_Alpha); })
            /// @brief Set color
            /// @param p_Color New color
            XUIText* SetColor(Color p_Color) XUI_PROXY_IMPL(_v::CText, { x->SetColor(p_Color); })
            /// @brief Set font size
            /// @param p_Size New size
            XUIText* SetFontSize(float p_Size) XUI_PROXY_IMPL(_v::CText, { x->SetFontSize(p_Size); })
            /// @brief Set font size
            /// @param p_MinSize New size
            /// @param p_MaxSize New size
            XUIText* SetFontSizes(float p_MinSize, float p_MaxSize) XUI_PROXY_IMPL(_v::CText, { x->SetFontSizes(p_MinSize, p_MaxSize); })
            /// @brief Set margins
            /// @param p_Left Left margin
            /// @param p_Top Top margin
            /// @param p_Right Right margin
            /// @param p_Bottom Bottom margin
            XUIText* SetMargins(float p_Left, float p_Top, float p_Right, float p_Bottom) XUI_PROXY_IMPL(_v::CText, { x->SetMargins(p_Left, p_Top, p_Right, p_Bottom); })
            /// @brief Set overflow mode
            /// @param p_OverflowMode New overflow mdoe
            XUIText* SetOverflowMode(_u::TextOverflowModes p_OverflowMode) XUI_PROXY_IMPL(_v::CText, { x->SetOverflowMode(p_OverflowMode); })
            /// @brief Set style
            /// @param p_Style New style
            XUIText* SetStyle(_u::FontStyles p_Style) XUI_PROXY_IMPL(_v::CText, { x->SetStyle(p_Style); })
            /// @brief Set button text
            /// @param p_Text New text
            XUIText* SetText(std::u16string p_Text) XUI_PROXY_IMPL(_v::CText, { x->SetText(p_Text); })
            /// @brief Set wrapping
            /// @param p_Wrapping New state
            XUIText* SetWrapping(bool p_Wrapping) XUI_PROXY_IMPL(_v::CText, { x->SetWrapping(p_Wrapping); })

    };

}   ///< namespace CP_SDK::XUI