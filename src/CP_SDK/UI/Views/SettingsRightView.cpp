#include "CP_SDK/UI/Views/SettingsRightView.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(SettingsRightView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(SettingsRightView, ViewController)
    {
        OnViewCreation = {this, &SettingsRightView::OnViewCreation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(SettingsRightView, ViewController)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void SettingsRightView::OnViewCreation_Impl()
    {
        Templates::FullRectLayout({
            Templates::TitleBar(u"Other settings"),

            XUIText::Make(u"No available other settings at the moment!")->AsShared()
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }

}   ///< namespace CP_SDK::UI::Views