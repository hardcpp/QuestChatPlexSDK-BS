#include "CP_SDK/UI/Views/MainMainView.hpp"
#include "CP_SDK/UI/FlowCoordinators/MainFlowCoordinator.hpp"
#include "CP_SDK/CPConfig.hpp"
#include "CP_SDK/ModuleBase.hpp"

#include <UnityEngine/Application.hpp>

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(MainMainView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(MainMainView, ViewController)
    {
        OnViewCreation      = {this, &MainMainView::OnViewCreation_Impl};
        OnViewActivation    = {this, &MainMainView::OnViewActivation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(MainMainView, ViewController)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void MainMainView::OnViewCreation_Impl()
    {
        auto l_Modules = ChatPlexSDK::GetModules();
        auto l_Buttons = std::vector<std::shared_ptr<IXUIElement>>();

        for (auto& l_Module : l_Modules)
        {
            if (l_Module->Type() != EIModuleBaseType::Integrated)
                continue;

            l_Buttons.push_back(
                XUISecondaryButton::Make(l_Module->FancyName(), [=]() -> void
                {
                    auto l_Items = l_Module->GetSettingsViewControllers();
                    FlowCoordinators::MainFlowCoordinator::Instance()->ChangeViewControllers(l_Items.Main.Ptr(false), l_Items.Left.Ptr(false), l_Items.Right.Ptr(false));
                })
                ->SetFontSize(3.44f)
                ->SetWidth(40.0f)
                ->SetHeight(7.0f)
                ->SetTooltip(std::u16string(l_Module->Description()))
                ->OnReady([this, l_Module](Components::CSecondaryButton* x) -> void {
                    m_ModulesButton[l_Module] = x;
                })
                ->AsShared()
            );
        }

        Templates::FullRectLayout({
            Templates::TitleBar(u"Modules"),

            XUIGLayout::Make(
                l_Buttons
            )
            ->SetCellSize(Vector2(40, 9))
            ->SetChildAlign(TextAnchor::UpperCenter)
            ->SetConstraint(GridLayoutGroup::Constraint::FixedColumnCount)
            ->SetConstraintCount(3)
            ->SetSpacing(Vector2(2, 0))
            ->SetWidth(124)
            ->SetHeight(55)
            ->AsShared(),

            Templates::ExpandedButtonsLine({
                XUIPrimaryButton::Make(u"Settings", {this, &MainMainView::OnSettingsPressed})->AsShared()
            })
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }
    /// @brief On view creation
    void MainMainView::OnViewActivation_Impl()
    {
        /// Refresh button states
        for (auto[l_Key, l_Value] : m_ModulesButton)
            l_Value->SetInteractable(l_Key->IsEnabled());

        /// Show welcome message
        if (CPConfig::Instance()->FirstRun)
        {
            ShowMessageModal(u"<color=yellow><b>Welcome to " + FlowCoordinators::MainFlowCoordinator::Instance()->Title() + u"!</b></color>\nBy default most modules are disabled, you can enable/disable them\nany time by clicking the <b>Settings</b> button below");
            CPConfig::Instance()->FirstRun = false;
            CPConfig::Instance()->Save();
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Go to settings
    void MainMainView::OnSettingsPressed()
    {
        FlowCoordinators::MainFlowCoordinator::Instance()->SwitchToSettingsView();
    }

}   ///< namespace CP_SDK::UI::Views