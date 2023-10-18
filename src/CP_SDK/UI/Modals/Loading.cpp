#include "CP_SDK/UI/Modals/Loading.hpp"
#include "CP_SDK/XUI/Templates.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Modals {

    CP_SDK_IL2CPP_INHERIT_INIT(Loading);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(Loading, IModal)
    {
        OnShow  = {this, &Loading::Impl_OnShow};
        OnClose = {this, &Loading::Impl_OnClose};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(Loading, IModal)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On modal show
    void Loading::Impl_OnShow()
    {
        if (m_Message)
            return;

        Templates::ModalRectLayout({
            XUIImage::Make()
                ->SetWidth(20.0f)->SetHeight(20.0f)
                ->SetEnhancedImage(UISystem::GetLoadingAnimation())
                ->AsShared(),

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

            XUISecondaryButton::Make(u"Cancel", {this, &Loading::OnCancelButton})
                ->SetWidth(40.0f)
                ->Bind(&m_CancelButton)
                ->AsShared()
        })
        ->BuildUI(get_transform());
    }
    /// @brief On modal close
    void Loading::Impl_OnClose()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    /// @param p_Message        Message to display
    /// @param p_CancelButton   Show cancel button
    /// @param p_CancelCallback On cancel callback
    void Loading::Init(std::u16string_view p_Message, bool p_CancelButton, _v::CActionRef<> p_CancelCallback)
    {
        m_Message->Element()->SetText(p_Message);
        m_CancelButton->SetActive(p_CancelButton);

        m_CancelCallback = p_CancelCallback;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    /// @param p_Message New message
    void Loading::SetMessage(std::u16string_view p_Message)
    {
        m_Message->Element()->SetText(p_Message);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On cancel button
    void Loading::OnCancelButton()
    {
        VController->CloseModal(this);

        try { m_CancelCallback(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][Loading.OnCancelButton] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI::Modals