#include "CP_SDK/UI/Views/MainRightView.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"

#include "assets.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(MainRightView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(MainRightView)
    {
        OnViewCreation = {this, &MainRightView::OnViewCreation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(MainRightView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void MainRightView::OnViewCreation_Impl()
    {
        auto l_Sprite = Unity::SpriteU::CreateFromRaw(IncludedAssets::ChatPlexLogoTransparent_png.Raw());

        Templates::FullRectLayout({
            Templates::TitleBar(u"Powered By"),

            XUIPrimaryButton::Make(u"")
                ->SetBackgroundSprite(nullptr)
                ->SetIconSprite(l_Sprite)
                ->SetWidth(52)
                ->SetHeight(52)
                ->AsShared()
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }

}   ///< namespace CP_SDK::UI::Views