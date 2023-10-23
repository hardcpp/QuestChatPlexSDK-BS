#include "CP_SDK/UI/Modals/Keyboard.hpp"
#include "CP_SDK/XUI/Templates.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Modals {

    CP_SDK_IL2CPP_INHERIT_INIT(Keyboard);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string Keyboard::m_CopyBuffer;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(Keyboard)
    {
        OnShow  = {this, &Keyboard::Impl_OnShow};
        OnClose = {this, &Keyboard::Impl_OnClose};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(Keyboard)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On modal show
    void Keyboard::Impl_OnShow()
    {
        if (m_SecondaryButtons.size() != 0)
            return;

        Templates::ModalRectLayout({
            XUIHLayout::Make({
                XUIPrimaryButton::Make(u"Copy",  {this, &Keyboard::OnCopyButton}),
                XUIPrimaryButton::Make(u"Paste", {this, &Keyboard::OnPasteButton})
            })
            ->SetPadding(0)
            ->OnReady([](Components::CHLayout* x) -> void {
                x->HOrVLayoutGroup()->set_childControlWidth(false);
                x->HOrVLayoutGroup()->set_childControlHeight(false);
                x->HOrVLayoutGroup()->set_childAlignment(TextAnchor::MiddleRight);
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
            })
            ->AsShared(),

            XUIHLayout::Make({
                XUIText::Make(u"")
                    ->SetAlign(TMPro::TextAlignmentOptions::Center)
                    ->SetWrapping(true)
                    ->Bind(&m_Text)
                    ->AsShared()
            })
            ->SetPadding(2)
            ->SetBackground(true, UISystem::KeyboardTextBGColor)
            ->OnReady([](Components::CHLayout* x) -> void {
                x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
            })
            ->AsShared(),

            XUIFLayout::Make()
                ->Bind(&m_CustomKeyLayout)
                ->AsShared(),

            XUIHLayout::Make({
                XUIVLayout::Make({
                   BuildKeyRow(true,  {u"/",                u".", u",", u"!", u"?", u"@", u"#", u":", u"[", u"]", u"$", u"=",            u"_"  }),
                   BuildKeyRow(false, {u"TAB",              u"q", u"w", u"e", u"r", u"t", u"y", u"u", u"i", u"o", u"p",                  u"🔙"}),
                   BuildKeyRow(false, {u"CAPS",             u"a", u"s", u"d", u"f", u"g", u"h", u"j", u"k", u"l", u"<color=green>ENTER"        }),
                   BuildKeyRow(false, {u"🔼",               u"z", u"x", u"c", u"v", u"b", u"n", u"m", u"🔼"                                   }),
                   BuildKeyRow(false, {u"<color=red>Clear",                      u" ",                            u"Cancel"                    })
                })
                ->SetPadding(0)->SetSpacing(1.5f)
                ->SetWidth(90.0f)
                ->AsShared(),

                XUIVLayout::Make({
                    BuildKeyRow(true,  { u"*", u"-", u"+" }),
                    BuildKeyRow(false, { u"7", u"8", u"9" }),
                    BuildKeyRow(false, { u"4", u"5", u"6" }),
                    BuildKeyRow(false, { u"1", u"2", u"3" }),
                    BuildKeyRow(false, { u"0"             })
                })
                ->SetPadding(0)->SetSpacing(1.5f)
                ->SetWidth(20.0f)
                ->AsShared()
            })
            ->SetPadding(0)->SetSpacing(2)
            ->AsShared()
        })
        ->SetWidth(115.0f)
        ->OnReady([](Components::CVLayout* x) -> void {
            x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
        })
        ->BuildUI(get_transform());
    }
    /// @brief On modal close
    void Keyboard::Impl_OnClose()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    /// @param p_Value          Value
    /// @param p_Callback       Callback
    /// @param p_CancelCallback On cancel callback
    /// @param p_CustomKeys     Custom keys
    void Keyboard::Init(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback, KeyboardCustomKeys* p_CustomKeys)
    {
        m_Value             = p_Value;
        m_Callback          = p_Callback;
        m_CancelCallback    = p_CancelCallback;

        m_Text->SetText(m_Value);

        for (int l_I = 0; l_I < m_CustomKeys.size(); ++l_I)
            GameObject::Destroy(m_CustomKeys[l_I]->get_gameObject());

        m_CustomKeys.clear();

        if (p_CustomKeys != nullptr)
        {
            for (int l_I = 0; l_I < p_CustomKeys->size(); ++l_I)
            {
                auto l_Text   = std::get<2>((*p_CustomKeys)[l_I]).empty() ? std::get<0>((*p_CustomKeys)[l_I]) : (u"<color=" + std::get<2>((*p_CustomKeys)[l_I]) + u">" + std::get<0>((*p_CustomKeys)[l_I]));
                auto l_Button = UISystem::PrimaryButtonFactory->Create(u"CustomKey", m_CustomKeyLayout->RTransform().Ptr());
                l_Button->SetText(l_Text);
                l_Button->OnClick(std::get<1>((*p_CustomKeys)[l_I]));

                m_CustomKeys.push_back(l_Button);
            }
        }

        m_CustomKeyLayout->SetActive(m_CustomKeys.size() != 0);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get current value
    std::u16string_view Keyboard::GetValue()
    {
        return m_Value;
    }
    /// @brief Set value
    /// @param p_Value
    void Keyboard::SetValue(std::u16string_view p_Value)
    {
        m_Value = p_Value;
        m_Text->SetText(m_Value);
    }
    /// @brief Append
    /// @param p_ToAppend
    void Keyboard::Append(std::u16string_view p_ToAppend)
    {
        m_Value += p_ToAppend;
        m_Text->SetText(m_Value);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On copy button
    void Keyboard::OnCopyButton()
    {
        m_CopyBuffer = m_Value;
    }
    /// @brief On paste button
    void Keyboard::OnPasteButton()
    {
        if (m_CopyBuffer.size())
        {
            m_Value += m_CopyBuffer;
            m_Text->SetText(m_Value);
        }
    }
    /// @brief On key press
    /// @param p_Text Text
    void Keyboard::OnKeyPress(std::u16string p_Text)
    {
        if (p_Text == u"🔙")
        {
            if (m_Value.size() > 0) m_Value = m_Value.substr(0, m_Value.size() - 1);
            m_Text->SetText(m_Value);
        }
        else if (p_Text == u"TAB")
        {
            m_Value += u"\t";
            m_Text->SetText(m_Value);
        }
        else if (p_Text == u"CAPS")
        {
            m_IsTempCaps = false;
            SwitchCaps();
        }
        else if (p_Text == u"🔼")
        {
            if (m_IsCaps && !m_IsTempCaps)
            {
                SwitchCaps();
                return;
            }

            m_IsTempCaps = !m_IsTempCaps;
            SwitchCaps();
        }
        else if (p_Text == u"<color=green>ENTER")
        {
            VController->CloseModal(this);

            try { m_Callback(m_Value); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][Keyboard.OnKeyPress] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
        else if (p_Text == u"<color=red>Clear")
        {
            m_Value = u"";
            m_Text->SetText(m_Value);
        }
        else if (p_Text == u"Cancel")
        {
            VController->CloseModal(this);

            try { m_CancelCallback(); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][Keyboard.OnKeyPress] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
        else
        {
            m_Value += p_Text;
            m_Text->SetText(m_Value);

            if (m_IsTempCaps)
            {
                m_IsTempCaps = false;
                SwitchCaps();
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Build a key row
    /// @param p_ForcePrimary Force as primary
    /// @param p_Keys         Keys to build
    XUIHLayout::Ptr Keyboard::BuildKeyRow(bool p_ForcePrimary, std::initializer_list<std::u16string> p_Keys)
    {
        auto l_Keys = std::vector<std::shared_ptr<IXUIElement>>();
        l_Keys.reserve(p_Keys.size());

        for (auto& l_Text : p_Keys)
        {
            if (p_ForcePrimary || (l_Text[0] >= 0xD800 && l_Text[0] <= 0xDBFF) || l_Text.size() > 1)
            {
                auto l_Key = XUISecondaryButton::Make(l_Text);
                l_Key->SetWidth(6.0f);
                l_Key->OnClick([this, l_Key]() -> void { OnKeyPress(l_Key->Element()->GetText()); });
                l_Key->OnReady([](Components::CSecondaryButton* x) -> void {
                    x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                    x->LElement()->set_flexibleWidth(1000.0f);
                });
                l_Keys.push_back(l_Key->AsShared());
            }
            else
            {
                auto l_Key = XUIPrimaryButton::Make(l_Text);
                l_Key->SetWidth(6.0f);
                l_Key->OnClick([this, l_Key]() -> void { OnKeyPress(l_Key->Element()->GetText()); });

                if (l_Text == u" " || l_Text == u"0")
                {
                    l_Key->OnReady([](Components::CPrimaryButton* x) -> void {
                        x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                        x->LElement()->set_flexibleWidth(4000.0f);
                    });
                }

                l_Keys.push_back(l_Key->AsShared());
                m_SecondaryButtons.push_back(l_Key->AsShared());
            }
        }

        return XUIHLayout::Make(l_Keys)
            ->SetPadding(0)->SetSpacing(1.0f)
            ->OnReady([](Components::CHLayout* x) -> void {
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
            })
            ->AsShared();
    }
    /// @brief Switch caps
    void Keyboard::SwitchCaps()
    {
        m_IsCaps = !m_IsCaps;

        for (int l_I = 0; l_I < m_SecondaryButtons.size(); ++l_I)
        {
            auto& l_Button = m_SecondaryButtons[l_I];
            auto  l_Text   = l_Button->Element()->GetText();

            std::transform(l_Text.begin(), l_Text.end(), l_Text.begin(), m_IsCaps ? std::towupper : std::towlower);

            l_Button->SetText(l_Text);
        }
    }

}   ///< namespace CP_SDK::UI::Modals