#pragma once

#include "../../UIIl2cpp.hpp"
#include "../../Data/IListCell.hpp"
#include "../../Data/IListItem.hpp"
#include "../../Data/ListCellPrefabs.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/ContentSizeFitter.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>

#include <string>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Generic virtual list interface
    class CP_SDK_EXPORT CVXList : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CVXList, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CVXList);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CVXList);

        protected:
            _v::Action<_v::CActionRef<const Data::IListItem::Ptr&>, bool>  m_OnListItemSelected;

            _v::Func<const Data::IListItem::Ptr&>                        m_GetSelectedItem;

            _v::Action<float, bool>                                      m_ScrollTo;

            _v::Action<Data::IListCell*>                                 m_SetListCellPrefab;
            _v::Action<const std::vector<Data::IListItem::Ptr>&>         m_SetListItems;
            _v::Action<const Data::IListItem::Ptr&, bool>                m_SetSelectedListItem;

            _v::Action<const Data::IListItem::Ptr&>                                                 m_AddListItem;
            _v::Action<_v::CFuncRef<int, const Data::IListItem::Ptr&, const Data::IListItem::Ptr&>> m_SortListItems;
            _v::Action<const Data::IListItem::Ptr&>                                                 m_RemoveListItem;

            _v::Action<Data::IListCell*> m_OnListCellClicked;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            _v::Delegate<float()>               ScrollPosition;

        public:
            /// @brief On list item selected event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add Should add
            CVXList* OnListItemSelected(_v::CActionRef<const Data::IListItem::Ptr&> p_Functor, bool p_Add = true);

        public:
            /// @brief Get selected item
            const Data::IListItem::Ptr& GetSelectedItem();

        public:
            /// @brief Scroll to position
            /// @param p_TargetPosition New target position
            /// @param p_Animated Is animated?
            CVXList* ScrollTo(float p_TargetPosition, bool p_Animated);

        public:
            /// @brief Set list cell prefab
            /// @param p_Prefab New prefab
            CVXList* SetListCellPrefab(Data::IListCell* p_Prefab);
            /// @brief Set list items
            /// @param p_ListItems New items
            CVXList* SetListItems(const std::vector<Data::IListItem::Ptr>& p_ListItems);
            /// @brief Set selected list item
            /// @param p_ListItem Selected list item
            /// @param p_Notify Should notify?
            CVXList* SetSelectedListItem(const Data::IListItem::Ptr& p_ListItem, bool p_Notify = true);

        public:
            /// @brief Add a list item
            /// @param p_ListItem Item to add
            CVXList* AddListItem(const Data::IListItem::Ptr& p_ListItem);
            /// @brief Sort list items by a functor
            /// @param p_Functor
            CVXList* SortListItems(_v::CFuncRef<int, const Data::IListItem::Ptr&, const Data::IListItem::Ptr&> p_Functor);
            /// @brief Remove a list item
            /// @param p_ListItem Item to remove
            CVXList* RemoveListItem(const Data::IListItem::Ptr& p_ListItem);

        public:
            /// @brief On list cell clicked
            /// @param p_ListCell Clicked list cell
            void OnListCellClicked(Data::IListCell* p_ListCell);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CVXList);