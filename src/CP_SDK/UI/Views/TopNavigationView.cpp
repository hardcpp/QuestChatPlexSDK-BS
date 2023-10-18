#include "CP_SDK/UI/Views/TopNavigationView.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(TopNavigationView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(TopNavigationView, ViewController)
    {
        OnViewCreation = {this, &TopNavigationView::OnViewCreation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(TopNavigationView, ViewController)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void TopNavigationView::OnViewCreation_Impl()
    {
        XUIHLayout::Make({
            XUIPrimaryButton::Make(u"", {this, &TopNavigationView::OnBackButtonPressed})
                ->SetWidth(12.0f)->SetHeight(8.0f)
                ->SetBackgroundSprite(UISystem::GetUIRoundRectLeftBGSprite().Ptr())
                ->SetIconSprite(UISystem::GetUIDownArrowSprite().Ptr())
                ->OnReady([](UI::Components::CPrimaryButton* x) -> void {
                    x->IconImageC()->get_rectTransform()->set_localEulerAngles(Vector3(0.0f, 0.0f, -90.0f));
                    x->IconImageC()->get_rectTransform()->set_localScale      (Vector3(0.4f, 0.4f,   0.6f));
                })
                ->AsShared(),

            XUIHLayout::Make({
                XUIText::Make(u"super test title!")
                    ->SetStyle(TMPro::FontStyles::UpperCase | TMPro::FontStyles::Bold)
                    ->SetFontSize(4.5f)
                    ->Bind(&m_Title)
                    ->AsShared()
            })
            ->SetPadding(0, 0, 0, -12)->SetSpacing(0)
            ->SetBackground(true, UISystem::NavigationBarBGColor, true)
            ->SetBackgroundSprite(UISystem::GetUIRoundRectRightBGSprite().Ptr(), UnityEngine::UI::Image::Type::Sliced)
            ->OnReady([](UI::Components::CHLayout* x) -> void {
                x->LElement()->set_flexibleWidth(5000.0f);
                x->CSizeFitter()->set_enabled(false);
                x->HOrVLayoutGroup()->set_childForceExpandHeight(true);
            })
            ->AsShared()
        })
        ->SetPadding(0)->SetSpacing(0)
        ->OnReady([](UI::Components::CHLayout* x) -> void {
            x->CSizeFitter()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::Unconstrained);
            x->CSizeFitter()->set_enabled(false);
            x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
            x->HOrVLayoutGroup()->set_childForceExpandHeight(true);
        })
        ->BuildUI(get_transform());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set title
    /// @param p_Title New title
    void TopNavigationView::SetTitle(std::u16string_view p_Title)
    {
        m_Title->Element()->SetText(p_Title);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On back button pressed
    void TopNavigationView::OnBackButtonPressed()
    {
        try { OnBackButton(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Views][TopNavigationView.OnBackButtonPressed] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI::Views