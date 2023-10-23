#include "CP_SDK/UI/Views/ModMenuView.hpp"
#include "CP_SDK/UI/ModMenu.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"

#include <UnityEngine/Application.hpp>

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(ModMenuView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(ModMenuView)
    {
        OnViewCreation      = {this, &ModMenuView::OnViewCreation_Impl};
        OnViewActivation    = {this, &ModMenuView::OnViewActivation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(ModMenuView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void ModMenuView::OnViewCreation_Impl()
    {
        Templates::FullRectLayout({
            Templates::TitleBar(u"Mods"),

            XUIHLayout::Make({
                XUIVScrollView::Make({
                    XUIGLayout::Make({

                    })
                    ->SetCellSize(Vector2(34, 9))
                    ->SetChildAlign(TextAnchor::UpperLeft)
                    ->SetConstraint(GridLayoutGroup::Constraint::FixedColumnCount)
                    ->SetConstraintCount(3)
                    ->SetSpacing(Vector2(2, 0))
                    ->Bind(&m_GLayout)
                    ->AsShared()
                })
                ->AsShared()
            })
            ->SetHeight(60)
            ->SetSpacing(0)
            ->SetPadding(0)
            ->SetBackground(true, std::nullopt, true)
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
    /// @brief On view activation
    void ModMenuView::OnViewActivation_Impl()
    {
        Refresh();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Refresh buttons
    void ModMenuView::Refresh()
    {
        auto&                       l_ModButtons    = ModMenu::ModButtons();
        std::vector<ModButton::Ptr> l_Existing;

        for(auto const& l_Pair : m_Buttons)
            l_Existing.push_back(l_Pair.first);

        /// Deletion
        for (auto l_I = 0; l_I < l_Existing.size(); ++l_I)
        {
            if (std::find(l_ModButtons.begin(), l_ModButtons.end(), l_Existing[l_I]) != l_ModButtons.end())
                continue;

            auto& l_ModButton = l_Existing[l_I];
            GameObject::Destroy(m_Buttons[l_ModButton]->get_gameObject());
            m_Buttons.erase(l_ModButton);
        }

        /// Creation
        for (auto l_I = 0; l_I < l_ModButtons.size(); ++l_I)
        {
            if (m_Buttons.contains(l_ModButtons[l_I]))
                continue;

            auto& l_ModButton = l_ModButtons[l_I];
            auto  l_Button    = UISystem::PrimaryButtonFactory->Create(u"ModButton", m_GLayout->RTransform().Ptr());
            l_Button->SetWidth(34.0f);
            l_Button->SetHeight(7.0f);
            l_Button->SetFontSize(3.44f);
            l_Button->SetOverflowMode(TMPro::TextOverflowModes::Ellipsis);
            l_Button->OnClick([=]() -> void { l_ModButton->FireOnClick(); });

            m_Buttons[l_ModButton] = l_Button;
        }

        /// Sorting / Update
        for (auto l_I = 0; l_I < l_ModButtons.size(); ++l_I)
        {
            auto& l_ModButton = l_ModButtons[l_I];
            auto& l_Button    = m_Buttons[l_ModButton];

            l_Button->RTransform()->SetAsLastSibling();
            l_Button->SetText(l_ModButton->Text());
            l_Button->SetTooltip(l_ModButton->Tooltip());
            l_Button->SetInteractable(l_ModButton->Interactable());
        }
    }

}   ///< namespace CP_SDK::UI::Views