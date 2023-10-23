#include "CP_SDK/UI/Views/SettingsLeftView.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"

using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(SettingsLeftView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SettingsLeftView)
    {
        OnViewCreation = {this, &SettingsLeftView::OnViewCreation_Impl};
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
        Templates::FullRectLayout({
            Templates::TitleBar(u"Tools"),

            XUIText::Make(u"No available tools at the moment!")->AsShared()
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }

}   ///< namespace CP_SDK::UI::Views