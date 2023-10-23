#include "CP_SDK/UI/Views/MainLeftView.hpp"
#include "CP_SDK/UI/FlowCoordinators/MainFlowCoordinator.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"

#include <UnityEngine/Application.hpp>

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(MainLeftView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(MainLeftView)
    {
        OnViewCreation = {this, &MainLeftView::OnViewCreation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(MainLeftView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void MainLeftView::OnViewCreation_Impl()
    {
        Templates::FullRectLayout({
            Templates::TitleBar(u"Information"),

            Templates::ScrollableInfos(50, {
                XUIText::Make(u"<b>Welcome to " + FlowCoordinators::MainFlowCoordinator::Instance()->Title() + u"</b>\nMade by HardCPP!")
                    ->SetAlign(TMPro::TextAlignmentOptions::CaplineLeft)
                    ->AsShared()
            }),

            Templates::ExpandedButtonsLine({
                XUIPrimaryButton::Make(u"Documentation", {this, &MainLeftView::OnDocumentationButton})->AsShared(),
                XUIPrimaryButton::Make(u"Discord", {this, &MainLeftView::OnDiscordButton})->AsShared()
            }),
            Templates::ExpandedButtonsLine({
                XUISecondaryButton::Make(u"Donate - Patreon", {this, &MainLeftView::OnDonateButton})->AsShared()
            })
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Documentation button
    void MainLeftView::OnDocumentationButton()
    {
        ShowMessageModal(u"URL opened in your web browser.");
        Application::OpenURL("https://github.com/hardcpp/BeatSaberPlus/wiki");
    }
    /// @brief Go to discord
    void MainLeftView::OnDiscordButton()
    {
        ShowMessageModal(u"URL opened in your web browser.");
        Application::OpenURL("https://discord.chatplex.org");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Go to donate
    void MainLeftView::OnDonateButton()
    {
        ShowMessageModal(u"URL opened in your web browser.");
        Application::OpenURL("https://donate.chatplex.org");
    }

}   ///< namespace CP_SDK::UI::Views