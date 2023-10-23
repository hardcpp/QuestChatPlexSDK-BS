#include "CP_SDK/UI/Modals/Dropdown.hpp"
#include "CP_SDK/UI/Data/TextListCell.hpp"
#include "CP_SDK/UI/Data/TextListItem.hpp"
#include "CP_SDK/XUI/Templates.hpp"

using namespace CP_SDK::UI::Data;
using namespace CP_SDK::XUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Modals {

    CP_SDK_IL2CPP_INHERIT_INIT(Dropdown);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(Dropdown)
    {
        OnShow  = {this, &Dropdown::Impl_OnShow};
        OnClose = {this, &Dropdown::Impl_OnClose};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(Dropdown)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On modal show
    void Dropdown::Impl_OnShow()
    {
        if (m_List)
            return;

        Templates::ModalRectLayout({
            XUIHLayout::Make({
                XUIVVList::Make()
                    ->SetListCellPrefab(ListCellPrefabs<TextListCell>::Get())
                    ->Bind(&m_List)
                    ->AsShared()
            })
            ->SetHeight(50)
            ->SetSpacing(0)
            ->SetPadding(0)
            ->SetBackground(true, UISystem::ListBGColor)
            ->OnReady([](Components::CHLayout* x) -> void {
                x->HOrVLayoutGroup()->set_childForceExpandWidth(true);
                x->HOrVLayoutGroup()->set_childForceExpandHeight(true);
                x->CSizeFitter()->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
                x->CSizeFitter()->set_verticalFit(ContentSizeFitter::FitMode::Unconstrained);
            })
            ->AsShared(),

            XUIHLayout::Make({
                XUISecondaryButton::Make(u"Cancel", {this, &Dropdown::OnCancelButton})->SetWidth(30.0f)->AsShared()
            })
            ->SetPadding(0)
            ->AsShared()
        })
        ->SetWidth(80.0f)->SetHeight(55.0f)
        ->BuildUI(get_transform());
    }
    /// @brief On modal close
    void Dropdown::Impl_OnClose()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    /// @param p_Options  Options to display
    /// @param p_Selected Selected option value
    /// @param p_Callback Callback
    void Dropdown::Init(const std::vector<std::u16string>& p_Options, std::u16string_view p_Selected, _v::CActionRef<std::u16string_view> p_Callback)
    {
        m_List->OnListItemSelected({this, &Dropdown::OnListItemSelect}, false);

        auto l_Items = std::vector<IListItem::Ptr>();
        l_Items.reserve(p_Options.size());

        IListItem::Ptr l_SelectedItem;
        for (auto& l_Option : p_Options)
        {
            auto l_New = std::make_shared<TextListItem>(l_Option);
            l_Items.push_back(l_New);

            if (l_Option == p_Selected)
                l_SelectedItem = l_New;
        }

        m_List->SetListItems(l_Items);

        if (l_SelectedItem)
            m_List->SetSelectedListItem(l_SelectedItem);

        m_List->OnListItemSelected({this, &Dropdown::OnListItemSelect}, true);

        m_Callback = p_Callback;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On cancel button
    void Dropdown::OnCancelButton()
    {
        VController->CloseModal(this);
    }
    /// @brief On list item selected
    /// @param p_SelectedItem Selected list item
    void Dropdown::OnListItemSelect(const UI::Data::IListItem::Ptr& p_SelectedItem)
    {
        VController->CloseModal(this);

        if (!p_SelectedItem)
            return;

        try { m_Callback(reinterpret_cast<TextListItem*>(p_SelectedItem.get())->Text); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.Modals][Dropdown.OnListItemSelect] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI::Modals