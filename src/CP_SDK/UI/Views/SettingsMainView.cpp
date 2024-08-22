#include "CP_SDK/UI/Views/SettingsMainView.hpp"
#include "CP_SDK/UI/FlowCoordinator.hpp"
#include "CP_SDK/UI/FlowCoordinators/MainFlowCoordinator.hpp"
#include "CP_SDK/Chat/Service.hpp"
#include "CP_SDK/CPConfig.hpp"
#include "CP_SDK/ModuleBase.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(SettingsMainView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SettingsMainView)
    {
        OnViewCreation      = {this, &SettingsMainView::OnViewCreation_Impl};
        OnViewActivation    = {this, &SettingsMainView::OnViewActivation_Impl};
    };
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SettingsMainView)
    {

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void SettingsMainView::OnViewCreation_Impl()
    {
        auto l_Modules = ChatPlexSDK::GetModules();
        auto l_Toggles = std::vector<std::shared_ptr<IXUIElement>>();

        for (auto l_Module : l_Modules)
        {
            if (l_Module->Type() != EIModuleBaseType::Integrated)
                continue;

            l_Toggles.push_back(
                XUIVLayout::Make({
                    XUIHLayout::Make({
                        XUIHLayout::Make({
                            XUIText::Make(l_Module->FancyName())
                                ->SetAlign(TMPro::TextAlignmentOptions::CaplineLeft)
                                ->AsShared()
                        })
                        ->SetWidth(42.5f)
                        ->SetPadding(0)
                        ->OnReady([](Components::CHLayout* x) -> void { x->HLayoutGroup()->set_childForceExpandWidth(true); })
                        ->AsShared(),

                        XUIHLayout::Make({
                            XUIToggle::Make()
                                ->SetValue(l_Module->IsEnabled())
                                ->OnValueChanged([this, l_Module](bool x) -> void { OnModuleToggled(l_Module, x); })
                                ->OnReady([this, l_Module](Components::CToggle* x) -> void { m_ModulesToggles[l_Module] = x; })
                                ->AsShared()
                        })
                        ->SetPadding(0)
                        ->AsShared()
                    })
                    ->SetPadding(0)
                    ->SetSpacing(0)
                    ->AsShared(),

                    XUIVLayout::Make({
                        XUIText::Make(l_Module->Description())
                            ->SetColor(Color::get_gray())
                            ->SetAlign(TMPro::TextAlignmentOptions::CaplineGeoAligned)
                            ->SetFontSize(2.8f)
                            ->SetOverflowMode(TMPro::TextOverflowModes::Ellipsis)
                            ->AsShared(),

                        XUIPrimaryButton::Make(u"Documentation", [this, l_Module]() -> void { OnDocumentationButton(l_Module); })
                            ->SetInteractable(!l_Module->DocumentationURL().empty())
                            ->AsShared()
                    })
                    ->SetWidth(60.0f)
                    ->SetHeight(10.0f)
                    ->SetPadding(0)
                    ->SetSpacing(0)
                    ->OnReady([](Components::CVLayout* x) -> void {
                        x->VLayoutGroup()->set_childAlignment(TextAnchor::MiddleCenter);
                        x->VLayoutGroup()->set_childForceExpandWidth(true);
                    })
                    ->AsShared()
                })
                ->SetWidth(65.0f)
                ->SetPadding(1)
                ->SetSpacing(0)
                ->SetBackground(true)
                ->AsShared()
            );
        }

        Templates::FullRectLayout({
            Templates::TitleBar(u"Modules"),

            XUIHLayout::Make({
                XUIVScrollView::Make({
                    XUIGLayout::Make(
                        l_Toggles
                    )
                    ->SetConstraint(GridLayoutGroup::Constraint::FixedColumnCount)
                    ->SetConstraintCount(2)
                    ->SetCellSize(Vector2(65.0f, 18.0f))
                    ->SetSpacing(Vector2(2.0f, 0.0f))
                    ->AsShared()
                })
                ->AsShared()
            })
            ->SetHeight(65)
            ->SetSpacing(0)
            ->SetPadding(0)
            ->SetBackground(true)
            ->OnReady([](Components::CHLayout* x) -> void {
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                x->CSizeFitter()->set_verticalFit(ContentSizeFitter::FitMode::Unconstrained);
                x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                x->HOrVLayoutGroup()->set_childForceExpandHeight(true);
            })
            ->AsShared()
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }
    /// @brief On view creation
    void SettingsMainView::OnViewActivation_Impl()
    {
        /// Refresh button states
        for (auto[l_Key, l_Value] : m_ModulesToggles)
            l_Value->SetValue(l_Key->IsEnabled(), false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On module toggled
    /// @param p_Module  Module instance
    /// @param p_Enabled Is enabled
    void SettingsMainView::OnModuleToggled(IModuleBase* p_Module, bool p_Enabled)
    {
        try
        {
            p_Module->SetEnabled(p_Enabled);
            CheckChatTutorial(p_Module);
        }
        catch (const std::exception& p_InitException)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Views][SettingsMainView.OnModuleToggled] Error on module \"" + p_Module->Name() + "\" init");
            ChatPlexSDK::Logger()->Error(p_InitException);
        }
    }
    /// @brief On documentation button
    /// @param p_Module Module instance
    void SettingsMainView::OnDocumentationButton(IModuleBase* p_Module)
    {
        ShowMessageModal(u"URL opened in your web browser.");
        ChatPlexSDK::OpenURL(p_Module->DocumentationURL());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Check for chat tutorial
    /// @param p_Module Module instance
    void SettingsMainView::CheckChatTutorial(IModuleBase* p_Module)
    {
        if (p_Module->UseChatFeatures() && CPConfig::Instance()->FirstChatServiceRun)
        {
            ShowMessageModal(u"Hey it's seems that this is the first time\nyou use a chat module!\n<b><color=yellow>The configuration page has been opened in your browser!</color></b>");

            Chat::Service::OpenWebConfiguration();

            CPConfig::Instance()->FirstChatServiceRun = false;
            CPConfig::Instance()->Save();
        }
    }

}   ///< namespace CP_SDK::UI::Views