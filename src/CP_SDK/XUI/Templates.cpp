#include "CP_SDK/XUI/Templates.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

using namespace CP_SDK::UI::Components;
using namespace CP_SDK::XUI;
using namespace CP_SDK::Unity::Extensions;

namespace CP_SDK::XUI {

    /// @brief Modal rect layout
    /// @param p_Childs Childs
    XUIVLayout::Ptr Templates::ModalRectLayout(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        auto l_FinalChilds = std::vector<std::shared_ptr<IXUIElement>>({
            XUIImage::Make(UI::UISystem::GetUIRoundSmoothFrameSprite().Ptr())
                ->SetType(Image::Type::Sliced)
                ->SetColor(ColorU::WithAlpha(Color::get_white(), 0.80f))
                ->OnReady([](CImage* x) -> void {
                    x->LElement()->set_ignoreLayout(true);
                    x->RTransform()->set_anchorMin       (Vector2::get_zero());
                    x->RTransform()->set_anchorMax       (Vector2::get_one());
                    x->RTransform()->set_anchoredPosition(Vector2::get_zero());
                    x->RTransform()->set_sizeDelta       (Vector2::get_zero());
                })
                ->AsShared()
        });

        l_FinalChilds.insert(l_FinalChilds.end(), p_Childs.begin(), p_Childs.end());

        return XUIVLayout::Make(u"ModalRectLayout", l_FinalChilds)
            ->SetBackground(true, UI::UISystem::ModalBGColor, true)
            ->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Full rect layout for screens template
    /// @param p_Childs Childs
    XUIVLayout::Ptr Templates::FullRectLayout(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        return  XUIVLayout::Make(u"FullRectLayout",
                    p_Childs
                )
                ->OnReady([](CVLayout* x) -> void {
                    x->CSizeFitter()->set_enabled(false);
                })
                ->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Full rect layout for screens template
    /// @param p_Childs Childs
    XUIVLayout::Ptr Templates::FullRectLayoutMainView(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        return  XUIVLayout::Make(u"FullRectLayoutMainView",
                    p_Childs
                )
                ->SetWidth(150)
                ->SetHeight(80)
                ->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Title bar template
    /// @param p_Text Title
    XUIHLayout::Ptr Templates::TitleBar(std::u16string_view p_Text)
    {
        return  XUIHLayout::Make(u"TitleBar", {
                    XUIText::Make(p_Text)
                        ->SetAlign(TMPro::TextAlignmentOptions::CaplineGeoAligned)
                        ->SetFontSize(4.0f)
                        ->AsShared()
                })
                ->SetSpacing(0)
                ->SetPadding(0, 8, 0, 8)
                ->SetBackground(true, UI::UISystem::TitleBlockBGColor)
                ->OnReady([](CHLayout* x) -> void {
                    x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
                })
                ->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Scrollable infos widget template
    /// @param p_Height Height
    /// @param p_Childs Childs
    XUIHLayout::Ptr Templates::ScrollableInfos(float p_Height, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        return XUIHLayout::Make(u"ScrollableInfos", {
                XUIVScrollView::Make(u"ScrollView",
                    p_Childs
                )
            })
            ->SetHeight(p_Height)
            ->SetSpacing(0)
            ->SetPadding(0)
            ->SetBackground(true)
            ->OnReady([](CHLayout* x) -> void {
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                x->CSizeFitter()->set_verticalFit(ContentSizeFitter::FitMode::Unconstrained);
                x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                x->HOrVLayoutGroup()->set_childForceExpandHeight(true);
            })->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Expanded buttons line
    /// @param p_Childs Childs
    XUIHLayout::Ptr Templates::ExpandedButtonsLine(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        return  XUIHLayout::Make(u"ExpandedButtonsLine",
                    p_Childs
                )
                ->SetPadding(0)
                ->OnReady([](CHLayout* x) -> void { x->CSizeFitter()->set_enabled(false); })
                ->ForEachDirect<XUIPrimaryButton>  ([](XUIPrimaryButton* y) -> void {
                    y->SetHeight(8.0f);
                    y->OnReady([](CPrimaryButton* x) -> void {  x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained); } );
                })
                ->ForEachDirect<XUISecondaryButton>([](XUISecondaryButton* y) -> void {
                    y->SetHeight(8.0f);
                    y->OnReady([](CSecondaryButton* x) -> void {  x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained); } );
                })
                ->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Settings group
    /// @param p_Title  Title
    /// @param p_Childs Childs
    XUIVLayout::Ptr Templates::SettingsHGroup(std::u16string_view p_Title, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        return  XUIVLayout::Make({
                    XUIText::Make(p_Title)
                        ->SetAlign(TMPro::TextAlignmentOptions::CaplineGeoAligned)
                        ->SetColor(Color::get_yellow())
                        ->AsShared(),

                    XUIHLayout::Make(
                        p_Childs
                    )
                    ->SetPadding(0)
                    ->OnReady([](CHLayout* x) -> void {
                        x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                        x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                    })
                    ->AsShared()
                })
                ->SetBackground(true)
                ->OnReady([](CVLayout* x) -> void {
                    x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                    x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                })
                ->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Settings group
    /// @param p_Title  Title
    /// @param p_Childs Childs
    XUIHLayout::Ptr Templates::SettingsVGroup(std::u16string_view p_Title, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
    {
        return  XUIHLayout::Make({
                    XUIText::Make(p_Title)
                        ->SetAlign(TMPro::TextAlignmentOptions::CaplineGeoAligned)
                        ->SetColor(Color::get_yellow())
                        ->AsShared(),

                    XUIHLayout::Make(
                        p_Childs
                    )
                    ->SetPadding(0)
                    ->OnReady([](CHLayout* x) -> void {
                        x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                        x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                    })
                    ->AsShared()
                })
                ->SetBackground(true)
                ->OnReady([](CHLayout* x) -> void {
                    x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                    x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                })
                ->AsShared();
    }

}   ///< namespace CP_SDK::XUI