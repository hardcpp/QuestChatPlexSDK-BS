#pragma once

#include "DefaultCVScrollView.hpp"
#include "../Components/CVVList.hpp"
#include "../../Pool/ObjectPool.hpp"
#include "../../Utils/Event.hpp"

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CVVList component
    class DefaultCVVList : public Components::CVVList
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.DefaultComponents", DefaultCVVList, Components::CVVList);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(DefaultCVVList, Components::CVVList);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(DefaultCVVList, Components::CVVList);

        using t_CellMemoryPool = Pool::ObjectPool<_v::MonoPtr<Data::IListCell>>;

        private:
            _v::MonoPtr<_u::RectTransform>                  m_RTransform;
            _v::MonoPtr<DefaultCVScrollView>                m_ScrollView;

            _v::MonoPtr<Data::IListCell>                    m_ListCellTemplate;
            t_CellMemoryPool::Ptr                           m_ListCellPool;
            std::vector<_v::MonoPtr<Data::IListCell>>       m_VisibleListCells;
            std::vector<Data::IListItem::Ptr>               m_ListItems;
            Data::IListItem::Ptr                            m_SelectedListItem;
            bool                                            m_Dirty;

            _v::Event<const Data::IListItem::Ptr&>          m_OnListItemSelectedEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCVVList();

        public:
            /// @brief On list item selected event
            /// @param p_Functor Functor to add/remove</param>
            /// @param p_Add Should add
            void OnListItemSelected_Impl(_v::CActionRef<const Data::IListItem::Ptr&> p_Functor, bool p_Add = true);

        public:
            /// @brief Get selected item
            const Data::IListItem::Ptr& GetSelectedItem_Impl();

        public:
            /// @brief Scroll to position
            /// @param p_TargetPosition New target position</param>
            /// @param p_Animated Is animated?</param>
            void ScrollTo_Impl(float p_TargetPosition, bool p_Animated);

        public:
            /// @brief Set list cell prefab
            /// @param p_Prefab New prefab</param>
            void SetListCellPrefab_Impl(Data::IListCell* p_Prefab);
            /// @brief Set list items
            /// @param p_ListItems New items
            void SetListItems_Impl(const std::vector<Data::IListItem::Ptr>& p_ListItems);
            /// @brief Set selected list item
            /// @param p_ListItem Selected list item</param>
            /// @param p_Notify Should notify?
            void SetSelectedListItem_Impl(const Data::IListItem::Ptr& p_ListItem, bool p_Notify = true);

        public:
            /// @brief Add a list item
            /// @param p_ListItem Item to add
            void AddListItem_Impl(const Data::IListItem::Ptr& p_ListItem);
            /// @brief Sort list items by a functor
            /// @param p_Functor </param>
            void SortListItems_Impl(_v::CFuncRef<int, const Data::IListItem::Ptr&, const Data::IListItem::Ptr&> p_Functor);
            /// @brief Remove a list item
            /// @param p_ListItem Item to remove
            void RemoveListItem_Impl(const Data::IListItem::Ptr& p_ListItem);

        public:
            /// @brief On list cell clicked
            /// @param p_ListCell Clicked list cell</param>
            void OnListCellClicked_Impl(Data::IListCell* p_ListCell);

        private:
            /// @brief On frame
            DECLARE_INSTANCE_METHOD(void, Update);

        private:
            /// @brief Clear
            /// @param p_ScrollToTop Should scroll back to top after clearing?
            void ClearVisibles(bool p_ScrollToTop);
            /// @brief Get list cell per page
            float GetListCellPerPage();
            /// @brief Update for current scroll
            void UpdateForCurrentScroll();

        private:
            /// @brief On rect transform dimensions changed
            DECLARE_INSTANCE_METHOD(void, OnRectTransformDimensionsChange);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCVVList);