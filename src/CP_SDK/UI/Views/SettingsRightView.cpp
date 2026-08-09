#include "CP_SDK/UI/Views/SettingsRightView.hpp"
#include "CP_SDK/CPConfig.hpp"
#include "CP_SDK/UI/Components/CVLayout.hpp"

using namespace CP_SDK::UI::Components;
using namespace CP_SDK::XUI;
using namespace UnityEngine;

namespace CP_SDK::UI::Views {

    CP_SDK_IL2CPP_INHERIT_INIT(SettingsRightView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SettingsRightView)
    {
        OnViewCreation = {this, &SettingsRightView::OnViewCreation_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SettingsRightView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On view creation
    void SettingsRightView::OnViewCreation_Impl()
    {
        Templates::FullRectLayout({
            Templates::TitleBar(u"Other settings"),

            XUITabControl::Make({
                {u"Misc", BuildMiscTab()}
            })
            ->Bind(&m_TabControl)
            ->AsShared()
        })
        ->SetBackground(true, std::nullopt, true)
        ->BuildUI(get_transform());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Build misc tab
    std::shared_ptr<IXUIElement> SettingsRightView::BuildMiscTab()
    {
        auto l_CPConfig = CPConfig::Instance();

        return XUIVLayout::Make({
            XUIHLayout::Make({
                XUIVLayout::Make({
                    XUIText::Make(u"Event specials (Require game restart)")->AsShared()
                })
                ->OnReady([](CVLayout* x) { x->HOrVLayoutGroup()->childForceExpandWidth = true; })
                ->ForEachDirect<XUIText>([](XUIText* x) { x->SetAlign(TMPro::TextAlignmentOptions::CaplineLeft); })
                ->AsShared(),

                XUIVLayout::Make({
                    XUIToggle::Make()
                        ->SetValue(l_CPConfig->EventSpecials)
                        ->Bind(&m_MiscTab_EventSpecialsEnabled)
                        ->AsShared()
                })
                ->ForEachDirect<XUIToggle>([this](XUIToggle* x) { x->OnValueChanged([this](bool) { OnValueChanged(); }); })
                ->AsShared()
            })
        })->AsShared();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On setting changed
    void SettingsRightView::OnValueChanged()
    {
        auto l_CPConfig = CPConfig::Instance();
        l_CPConfig->EventSpecials = m_MiscTab_EventSpecialsEnabled->Element()->GetValue();
    }

}   ///< namespace CP_SDK::UI::Views