#include "CP_SDK/UI/Modals/Confirmation.hpp"
#include "CP_SDK/XUI/Templates.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Modals {

    CP_SDK_IL2CPP_INHERIT_INIT(Confirmation);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(Confirmation, IModal)
    {
        OnShow  = {this, &Confirmation::Impl_OnShow};
        OnClose = {this, &Confirmation::Impl_OnClose};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(Confirmation, IModal)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On modal show
    void Confirmation::Impl_OnShow()
    {
        if (m_Message)
            return;

        Templates::ModalRectLayout({
            XUIHLayout::Make({
                XUIText::Make(u"Message...")
                    ->SetAlign(TMPro::TextAlignmentOptions::Top)
                    ->SetColor(Color::get_yellow())
                    ->Bind(&m_Message)
                    ->AsShared()
            })
            ->OnReady([](Components::CHLayout* x) -> void {
                x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
            })
            ->AsShared(),

            XUIHLayout::Make({
                XUIPrimaryButton::Make(u"Yes", {this, &Confirmation::OnYesButton})->SetWidth(20.0f)->AsShared(),
                XUISecondaryButton::Make(u"No", {this, &Confirmation::OnNoButton})->SetWidth(20.0f)->AsShared()
            })
            ->SetPadding(0)
            ->AsShared()
        })
        ->BuildUI(get_transform());
    }
    /// @brief On modal close
    void Confirmation::Impl_OnClose()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    /// @param p_Message  Value
    /// @param p_Callback Callback
    void Confirmation::Init(std::u16string_view p_Message, _v::CActionRef<bool> p_Callback)
    {
        m_Message->Element()->SetText(p_Message);
        m_Callback = p_Callback;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On button "Yes" pressed
    void Confirmation::OnYesButton()
    {
        VController->CloseModal(this);

        try { m_Callback(true); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][Confirmation.OnYesButton] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief On button "No" pressed
    void Confirmation::OnNoButton()
    {
        VController->CloseModal(this);

        try { m_Callback(false); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][Confirmation.OnNoButton] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI::Modals