#include "CP_SDK/UI/Modals/ColorPicker.hpp"
#include "CP_SDK/UI/ValueFormatters.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"
#include "CP_SDK/XUI/Templates.hpp"

using namespace CP_SDK::Unity::Extensions;
using namespace CP_SDK::XUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Modals {

    CP_SDK_IL2CPP_INHERIT_INIT( ColorPicker);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(ColorPicker)
    {
        OnShow  = {this, &ColorPicker::Impl_OnShow};
        OnClose = {this, &ColorPicker::Impl_OnClose};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(ColorPicker)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On modal show
    void ColorPicker::Impl_OnShow()
    {
        if (m_H)
            return;

        Templates::ModalRectLayout({
            XUIHLayout::Make({
                XUIVLayout::Make({
                    XUIText::Make(u"H")->AsShared(),
                    XUIText::Make(u"S")->AsShared(),
                    XUIText::Make(u"V")->AsShared(),
                    XUIText::Make(u"O")->AsShared()
                })
                ->SetPadding(0)
                ->SetSpacing(1)
                ->ForEachDirect<XUIText>([](XUIText* x) -> void {
                    x->SetAlign(TMPro::TextAlignmentOptions::Center);
                })
                ->AsShared(),

                XUIVLayout::Make({
                    XUISlider::Make()->SwitchToColorMode(true,  false, false, false)->Bind(&m_H)->AsShared(),
                    XUISlider::Make()->SwitchToColorMode(false , true, false, false)->Bind(&m_S)->AsShared(),
                    XUISlider::Make()->SwitchToColorMode(false, false,  true, false)->Bind(&m_V)->AsShared(),
                    XUISlider::Make()->SwitchToColorMode(false, false, false,  true)->Bind(&m_O)->AsShared()
                })
                ->SetPadding(0)
                ->SetSpacing(1)
                ->SetWidth(40.0f)
                ->ForEachDirect<XUISlider>([this](XUISlider* x) -> void {
                    x->SetFormatter(ValueFormatters::Percentage);
                    x->OnValueChanged([this](float) -> void { OnColorChanged(); });
                })
                ->AsShared(),

                XUIVLayout::Make({
                    XUIText::Make(u"")->Bind(&m_HLabel)->AsShared(),
                    XUIText::Make(u"")->Bind(&m_SLabel)->AsShared(),
                    XUIText::Make(u"")->Bind(&m_VLabel)->AsShared(),
                    XUIText::Make(u"")->Bind(&m_OLabel)->AsShared()
                })
                ->SetPadding(0)
                ->SetSpacing(1)
                ->SetWidth(10.0f)
                ->ForEachDirect<XUIText>([](XUIText* x) -> void {
                    x->SetAlign(TMPro::TextAlignmentOptions::CaplineRight);
                })
                ->OnReady([](Components::CVLayout* x) -> void {
                    x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                })
                ->AsShared(),

                XUIVLayout::Make({
                    XUIImage::Make()
                        ->SetWidth(20 + 3)->SetHeight(20 + 3)
                        ->SetType(UnityEngine::UI::Image::Type::Sliced)
                        ->SetSprite(UISystem::GetUIRoundBGSprite().Ptr())
                        ->Bind(&m_Image)
                        ->AsShared()
                })
                ->SetPadding(0)
                ->SetSpacing(1)
                ->AsShared()
            })->AsShared(),

            XUIHLayout::Make({
                XUISecondaryButton::Make(u"Cancel", {this, &ColorPicker::OnCancelButton})->SetWidth(30.0f)->AsShared(),
                XUIPrimaryButton::Make  (u"Apply",  {this, &ColorPicker::OnApplyButton })->SetWidth(30.0f)->AsShared()
            })
            ->SetPadding(0)
            ->AsShared()
        })
        ->BuildUI(get_transform());
    }
    /// @brief On modal close
    void ColorPicker::Impl_OnClose()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    /// @param p_Value          Initial value
    /// @param p_Opacity        If opacity supported?
    /// @param p_Callback       Callback
    /// @param p_CancelCallback On cancel callback
    void ColorPicker::Init(Color p_Value, bool p_Opacity, _v::CActionRef<Color> p_Callback, _v::CActionRef<> p_CancelCallback)
    {
        m_Callback          = nullptr;
        m_CancelCallback    = nullptr;

        float l_H, l_S, l_V;
        Color::RGBToHSV(p_Value, l_H, l_S, l_V);

        m_H->SetValue(l_H, false);
        m_S->SetValue(l_S, false);
        m_V->SetValue(l_V, false);
        m_O->SetValue(p_Opacity ? p_Value.a : 1.0f, false);
        m_O->SetInteractable(p_Opacity);

        OnColorChanged();

        m_Callback          = p_Callback;
        m_CancelCallback    = p_CancelCallback;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On color changed
    void ColorPicker::OnColorChanged()
    {
        auto l_Color = ColorU::WithAlpha(Color::HSVToRGB(m_H->Element()->GetValue(), m_S->Element()->GetValue(), m_V->Element()->GetValue()), m_O->Element()->GetValue());

        m_Image->SetColor(l_Color);
        m_S->ColorModeSetHue(m_H->Element()->GetValue());
        m_V->ColorModeSetHue(m_H->Element()->GetValue());

        m_HLabel->SetText(ValueFormatters::Percentage(m_H->Element()->GetValue()));
        m_SLabel->SetText(ValueFormatters::Percentage(m_S->Element()->GetValue()));
        m_VLabel->SetText(ValueFormatters::Percentage(m_V->Element()->GetValue()));
        m_OLabel->SetText(ValueFormatters::Percentage(m_O->Element()->GetValue()));

        m_Callback(l_Color);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On cancel button
    void ColorPicker::OnCancelButton()
    {
        VController->CloseModal(this);

        try { m_CancelCallback(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][ColorPicker.OnCancelButton] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief On apply button
    void ColorPicker::OnApplyButton()
    {
        VController->CloseModal(this);

        try { m_Callback(m_Image->Element()->ImageC()->get_color()); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][ColorPicker.OnApplyButton] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI::Modals