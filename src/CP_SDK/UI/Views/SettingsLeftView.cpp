#include "CP_SDK/UI/Views/SettingsLeftView.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include <optional>
#include <string>

using namespace CP_SDK::XUI;
using namespace UnityEngine;

#include "assets.hpp"

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(SettingsLeftView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SettingsLeftView)
    {
        OnViewCreation      = {this, &SettingsLeftView::OnViewCreation_Impl};
        OnViewDeactivation  = {this, &SettingsLeftView::OnViewDeactivation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SettingsLeftView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void SettingsLeftView::OnViewCreation_Impl()
    {
        auto l_Sprite = Unity::SpriteU::CreateFromRaw(Assets::ChatPlexLogoTransparent_png);

        Templates::FullRectLayout({
                Templates::TitleBar(u"ChatPlex Account"),

                XUIPrimaryButton::Make(u"")
                    ->SetBackgroundSprite(nullptr)
                    ->SetIconSprite(l_Sprite)
                    ->SetWidth(52)
                    ->SetHeight(52)
                    ->AsShared(),

                XUIText::Make(u"Not connected")
                    ->Bind(&m_StatusText)
                    ->AsShared(),

                XUIText::Make(u" ")
                    ->Bind(&m_SubscriptionText)
                    ->AsShared(),

                XUIVLayout::Make({
                    XUIPrimaryButton::Make(u"Connect", {this, &SettingsLeftView::OnPrimaryButtonPressed})
                        ->Bind(&m_PrimaryButton)
                        ->AsShared(),
                    XUISecondaryButton::Make(u"Disconnect", {this, &SettingsLeftView::OnSecondaryButtonPressed})
                        ->Bind(&m_SecondaryButton)
                        ->AsShared()
                })
                ->SetWidth(60.0f)
                ->SetPadding(0)
                ->ForEachDirect<XUIPrimaryButton>([](XUIPrimaryButton* y) -> void
                {
                    y->SetHeight(8.0f);
                    y->OnReady([](Components::CPrimaryButton* x) -> void
                    {
                        x->CSizeFitter()->horizontalFit = ContentSizeFitter::FitMode::Unconstrained;
                    });
                })
                ->ForEachDirect<XUISecondaryButton>([](XUISecondaryButton* y ) -> void
                {
                    y->SetHeight(8.0f);
                    y->OnReady([](Components::CSecondaryButton* x) -> void
                    {
                        x->CSizeFitter()->horizontalFit = ContentSizeFitter::FitMode::Unconstrained;
                    });
                })
                ->AsShared()
            })
            ->SetBackground(true, std::nullopt, true)
            ->BuildUI(get_transform());

        ChatPlexService::StateChanged += {this, &SettingsLeftView::ChatPlexService_StateChanged};

        ChatPlexService_StateChanged(ChatPlexService::State(), ChatPlexService::State());
    }
    /// @brief On view deactivation
    void SettingsLeftView::OnViewDeactivation_Impl()
    {
        ChatPlexService::StateChanged -= {this, &SettingsLeftView::ChatPlexService_StateChanged};
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On primary button pressed
    void SettingsLeftView::OnPrimaryButtonPressed()
    {
        if (ChatPlexService::State() == ChatPlexService::EState::Disconnected)
        {
            m_IsLinking = true;
            ChatPlexService::StartLinking();
            ShowLoadingModal(u"Loading...", true, {this, &SettingsLeftView::OnLoadingCancel});
        }
        else if (ChatPlexService::State() == ChatPlexService::EState::Error || ChatPlexService::State() == ChatPlexService::EState::Connected)
        {
            ChatPlexService::Refresh();
        }
    }
    /// @brief On secondary button pressed
    void SettingsLeftView::OnSecondaryButtonPressed()
    {
        ChatPlexService::Disconnect();
    }
    /// @brief On Loading cancel
    void SettingsLeftView::OnLoadingCancel()
    {
        if (ChatPlexService::State() == ChatPlexService::EState::LinkRequest || ChatPlexService::State() == ChatPlexService::EState::LinkWait)
        {
            m_IsLinking = false;
            ChatPlexService::StopLinking();
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On ChatPlex service state change
    /// @param oldState Old state
    /// @param newState New state
    void SettingsLeftView::ChatPlexService_StateChanged(ChatPlexService::EState oldState, ChatPlexService::EState newState)
    {
        Unity::MTMainThreadInvoker::Enqueue([this, oldState, newState]() -> void
        {
            if (m_IsLinking)
            {
                if (newState == ChatPlexService::EState::LinkRequest)
                    ShowLoadingModal(u"Creating link request...", true, {this, &SettingsLeftView::OnLoadingCancel});
                else if (newState == ChatPlexService::EState::LinkWait)
                    ShowLoadingModal(
                        std::u16string(u"Go to https://chatplex.org/link and the input following code\n") + ChatPlexService::LinkCode(),
                        true,
                        {this, &SettingsLeftView::OnLoadingCancel}
                    );
                else if (newState == ChatPlexService::EState::Error)
                {
                    m_IsLinking = false;

                    CloseLoadingModal();
                    ShowMessageModal(u"Error: " + ChatPlexService::LastError());
                }
                else
                {
                    m_IsLinking = false;
                    CloseLoadingModal();
                }
            }

            switch (newState)
            {
                case ChatPlexService::EState::Disconnected:
                    m_StatusText->SetColor(Color::get_red());
                    m_StatusText->SetText(u"Disconected!");
                    m_PrimaryButton->SetInteractable(true);
                    m_PrimaryButton->SetText(u"Connect");
                    m_SecondaryButton->SetInteractable(false);
                    break;

                case ChatPlexService::EState::Error:
                    m_StatusText->SetColor(Color::get_red());
                    m_StatusText->SetText(u"Disconected, error!");
                    m_PrimaryButton->SetInteractable(true);
                    m_PrimaryButton->SetText(u"Connect");
                    m_SecondaryButton->SetInteractable(false);
                    break;

                case ChatPlexService::EState::Connecting:
                    m_StatusText->SetColor(Color::get_blue());
                    m_StatusText->SetText(u"Connecting...");
                    m_PrimaryButton->SetInteractable(false);
                    m_PrimaryButton->SetText(u"Connect");
                    m_SecondaryButton->SetInteractable(false);
                    break;

                case ChatPlexService::EState::LinkRequest:
                case ChatPlexService::EState::LinkWait:
                    m_StatusText->SetColor(Color::get_blue());
                    m_StatusText->SetText(u"Linking account...");
                    m_PrimaryButton->SetInteractable(false);
                    m_PrimaryButton->SetText(u"Connect");
                    m_SecondaryButton->SetInteractable(false);
                    break;

                case ChatPlexService::EState::Connected:
                    m_StatusText->SetColor(Color::get_green());
                    m_StatusText->SetText(u"Connected!");
                    m_PrimaryButton->SetInteractable(true);
                    m_PrimaryButton->SetText(u"Refresh");
                    m_SecondaryButton->SetInteractable(true);
                    break;
            }

            m_SubscriptionText->Element()->SetText(ChatPlexService::ActiveSubscription());
        });
    }

}   ///< namespace CP_SDK::UI::Views