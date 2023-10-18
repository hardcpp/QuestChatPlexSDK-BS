#include "CP_SDK/UI/Components/Generics/CVXList.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CVXList);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CVXList)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CVXList)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On list item selected event
    /// @param p_Functor Functor to add/remove</param>
    /// @param p_Add Should add
    CVXList* CVXList::OnListItemSelected(_v::CActionRef<const Data::IListItem::Ptr&> p_Functor, bool p_Add)
    {
        m_OnListItemSelected(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get selected item
    const Data::IListItem::Ptr& CVXList::GetSelectedItem()
    {
        return m_GetSelectedItem();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Scroll to position
    /// @param p_TargetPosition New target position</param>
    /// @param p_Animated Is animated?</param>
    CVXList* CVXList::ScrollTo(float p_TargetPosition, bool p_Animated)
    {
        m_ScrollTo(p_TargetPosition, p_Animated);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set list cell prefab
    /// @param p_Prefab New prefab</param>
    CVXList* CVXList::SetListCellPrefab(Data::IListCell* p_Prefab)
    {
        m_SetListCellPrefab(p_Prefab);
        return this;
    }
    /// @brief Set list items
    /// @param p_ListItems New items
    CVXList* CVXList::SetListItems(const std::vector<Data::IListItem::Ptr>& p_ListItems)
    {
        m_SetListItems(p_ListItems);
        return this;
    }
    /// @brief Set selected list item
    /// @param p_ListItem Selected list item</param>
    /// @param p_Notify Should notify?
    CVXList* CVXList::SetSelectedListItem(const Data::IListItem::Ptr& p_ListItem, bool p_Notify)
    {
        m_SetSelectedListItem(p_ListItem, p_Notify);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Add a list item
    /// @param p_ListItem Item to add
    CVXList* CVXList::AddListItem(const Data::IListItem::Ptr& p_ListItem)
    {
        m_AddListItem(p_ListItem);
        return this;
    }
    /// @brief Sort list items by a functor
    /// @param p_Functor </param>
    CVXList* CVXList::SortListItems(_v::CFuncRef<int, const Data::IListItem::Ptr&, const Data::IListItem::Ptr&> p_Functor)
    {
        m_SortListItems(p_Functor);
        return this;
    }
    /// @brief Remove a list item
    /// @param p_ListItem Item to remove
    CVXList* CVXList::RemoveListItem(const Data::IListItem::Ptr& p_ListItem)
    {
        m_RemoveListItem(p_ListItem);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On list cell clicked
    /// @param p_ListCell Clicked list cell</param>
    void CVXList::OnListCellClicked(Data::IListCell* p_ListCell)
    {
        m_OnListCellClicked(p_ListCell);
    }

}   ///< namespace CP_SDK::UI::Components