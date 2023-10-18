#include "CP_SDK/UI/DefaultComponents/DefaultCVVList.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Mathf.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCVVList);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(DefaultCVVList, Components::CVVList)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,     m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,       m_ScrollView->LElement());

        ScrollPosition = [this]() -> float { return m_ScrollView->Position(); };

        m_OnListItemSelected    = {this, &DefaultCVVList::OnListItemSelected_Impl};

        m_GetSelectedItem       = {this, &DefaultCVVList::GetSelectedItem_Impl};

        m_ScrollTo              = {this, &DefaultCVVList::ScrollTo_Impl};

        m_SetListCellPrefab     = {this, &DefaultCVVList::SetListCellPrefab_Impl};
        m_SetListItems          = {this, &DefaultCVVList::SetListItems_Impl};
        m_SetSelectedListItem   = {this, &DefaultCVVList::SetSelectedListItem_Impl};

        m_AddListItem           = {this, &DefaultCVVList::AddListItem_Impl};
        m_SortListItems         = {this, &DefaultCVVList::SortListItems_Impl};
        m_RemoveListItem        = {this, &DefaultCVVList::RemoveListItem_Impl};

        m_OnListCellClicked     = {this, &DefaultCVVList::OnListCellClicked_Impl};

        Init += {this, &DefaultCVVList::Init_DefaultCVVList};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(DefaultCVVList, Components::CVVList)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCVVList::Init_DefaultCVVList()
    {
        if (m_RTransform)
            return;

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());

        m_ScrollView = get_gameObject()->AddComponent<DefaultCVScrollView*>();
        m_ScrollView->Init();
        m_ScrollView->ScrollType = Components::CVScrollView::EScrollType::FixedCellSize;
        m_ScrollView->OnScrollChanged([this](float x) { UpdateForCurrentScroll(); });

        GameObject::DestroyImmediate(m_ScrollView->Container()->GetComponent<VerticalLayoutGroup*>());
        GameObject::DestroyImmediate(m_ScrollView->Container()->get_parent()->GetComponent<VerticalLayoutGroup*>());
        GameObject::DestroyImmediate(m_ScrollView->Container()->get_parent()->GetComponent<ContentSizeFitter*>());

        m_ScrollView->Container()->set_anchorMin(Vector2(0.0f, 1.0f));
        m_ScrollView->Container()->set_anchorMax(Vector2(1.0f, 1.0f));
        m_ScrollView->Container()->set_pivot    (Vector2(0.5f, 1.0f));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On list item selected event
    /// @param p_Functor Functor to add/remove</param>
    /// @param p_Add Should add
    void DefaultCVVList::OnListItemSelected_Impl(_v::CActionRef<const Data::IListItem::Ptr&> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnListItemSelectedEvent += p_Functor;
        else        m_OnListItemSelectedEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get selected item
    const Data::IListItem::Ptr& DefaultCVVList::GetSelectedItem_Impl()
    {
        return m_SelectedListItem;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Scroll to position
    /// @param p_TargetPosition New target position</param>
    /// @param p_Animated Is animated?</param>
    void DefaultCVVList::ScrollTo_Impl(float p_TargetPosition, bool p_Animated)
    {
        m_ScrollView->ScrollTo(p_TargetPosition, p_Animated);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set list cell prefab
    /// @param p_Prefab New prefab</param>
    void DefaultCVVList::SetListCellPrefab_Impl(Data::IListCell* p_Prefab)
    {
        ClearVisibles(true);

        if (m_ListCellPool != nullptr)
        {
            m_ListCellPool->Clear();
            m_ListCellPool = nullptr;
        }

        GameObject::DontDestroyOnLoad(p_Prefab->get_gameObject());
        p_Prefab->get_gameObject()->SetActive(false);

        m_ListCellTemplate = p_Prefab;

        m_ListCellPool = t_CellMemoryPool::Make(
            [this]() -> _v::MonoPtr<Data::IListCell> {
                auto x = m_ListCellTemplate->Create(m_ScrollView->Container().Ptr());
                x->RTransform()->set_anchorMin(Vector2(0.0f, 1.0f));
                x->RTransform()->set_anchorMax(Vector2(1.0f, 1.0f));
                x->RTransform()->set_pivot    (Vector2(0.5f, 1.0f));
                x->RTransform()->set_sizeDelta(Vector2(0.0f, x->GetCellHeight()));

                return _v::MonoPtr(x);
            },
            [](_v::MonoPtr<Data::IListCell>& x) { x->get_gameObject()->set_active(true); },
            [](_v::MonoPtr<Data::IListCell>& x) { x->get_gameObject()->set_active(false); },
            [](_v::MonoPtr<Data::IListCell>& x) { GameObject::Destroy(x->get_gameObject()); },
            true
        );

        m_ScrollView->FixedCellSize = m_ListCellTemplate->GetCellHeight();

        UpdateForCurrentScroll();
    }
    /// @brief Set list items
    /// @param p_ListItems New items
    void DefaultCVVList::SetListItems_Impl(const std::vector<Data::IListItem::Ptr>& p_ListItems)
    {
        m_ListItems.clear();
        if (p_ListItems.size() > 0)
            m_ListItems.insert(m_ListItems.end(), p_ListItems.begin(), p_ListItems.end());

        m_ScrollView->Container()->set_sizeDelta(Vector2(0.0f, m_ListItems.size() * m_ListCellTemplate->GetCellHeight()));
        m_ScrollView->SetContentSize(m_ScrollView->Container()->get_sizeDelta().y);
        m_ScrollView->ScrollTo(0.0f, false);

        ClearVisibles(true);
        SetSelectedListItem(nullptr);
        UpdateForCurrentScroll();
    }
    /// @brief Set selected list item
    /// @param p_ListItem Selected list item</param>
    /// @param p_Notify Should notify?
    void DefaultCVVList::SetSelectedListItem_Impl(const Data::IListItem::Ptr& p_ListItem, bool p_Notify)
    {
        auto l_NewSelectListItem = p_ListItem;
        const auto& l_It = std::find(m_ListItems.begin(), m_ListItems.end(), p_ListItem);
        if (l_It == m_ListItems.end())
            l_NewSelectListItem = nullptr;

        try { if (m_SelectedListItem) m_SelectedListItem->OnUnselect(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCVVList.SetSelectedListItem] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        m_SelectedListItem = l_NewSelectListItem;

        if (m_SelectedListItem != nullptr)
        {
            auto l_PackIndex         = l_It - m_ListItems.begin();
            auto l_TargetHeight      = l_PackIndex * m_ListCellTemplate->GetCellHeight();
            auto l_CenteredHeight    = l_TargetHeight - ((GetListCellPerPage() / 2) * m_ListCellTemplate->GetCellHeight());
            auto l_MaxHeight         = std::max<float>(0, m_ScrollView->ContentSize() - m_ScrollView->ScrollPageSize());

            l_CenteredHeight = std::max<float>(0, l_CenteredHeight);
            if (l_CenteredHeight > l_MaxHeight)
                l_CenteredHeight = l_MaxHeight;

            if (m_ScrollView->ContentSize() < m_ScrollView->ScrollPageSize())
                l_CenteredHeight = 0.0f;

            m_ScrollView->ScrollTo(l_CenteredHeight, true);
            UpdateForCurrentScroll();

            try { if (m_SelectedListItem) m_SelectedListItem->OnSelect(); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCVVList.SetSelectedListItem] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }

        if (p_Notify)
        {
            try { m_OnListItemSelectedEvent(m_SelectedListItem); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCVVList.SetSelectedListItem] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Add a list item
    /// @param p_ListItem Item to add
    void DefaultCVVList::AddListItem_Impl(const Data::IListItem::Ptr& p_ListItem)
    {
        m_ListItems.push_back(p_ListItem);

        m_ScrollView->Container()->set_sizeDelta(Vector2(0.0f, m_ListItems.size() * m_ListCellTemplate->GetCellHeight()));
        m_ScrollView->SetContentSize(m_ScrollView->Container()->get_sizeDelta().y);

        ClearVisibles(false);
        UpdateForCurrentScroll();
    }
    /// @brief Sort list items by a functor
    /// @param p_Functor </param>
    void DefaultCVVList::SortListItems_Impl(_v::CFuncRef<int, const Data::IListItem::Ptr&, const Data::IListItem::Ptr&> p_Functor)
    {
        std::sort(m_ListItems.begin(), m_ListItems.end(), [&](const Data::IListItem::Ptr& p_A, const Data::IListItem::Ptr& p_B) -> int {
            return p_Functor(p_A, p_B) < 0;
        });

        ClearVisibles(false);
        UpdateForCurrentScroll();
    }
    /// @brief Remove a list item
    /// @param p_ListItem Item to remove
    void DefaultCVVList::RemoveListItem_Impl(const Data::IListItem::Ptr& p_ListItem)
    {
        const auto& l_It = std::find(m_ListItems.begin(), m_ListItems.end(), p_ListItem);
        if (l_It == m_ListItems.end())
            return;

        m_ListItems.erase(l_It);

        m_ScrollView->Container()->set_sizeDelta(Vector2(0.0f, m_ListItems.size() * m_ListCellTemplate->GetCellHeight()));
        m_ScrollView->SetContentSize(m_ScrollView->Container()->get_sizeDelta().y);

        ClearVisibles(false);
        UpdateForCurrentScroll();

        if (m_SelectedListItem == p_ListItem)
            SetSelectedListItem(nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On list cell clicked
    /// @param p_ListCell Clicked list cell</param>
    void DefaultCVVList::OnListCellClicked_Impl(Data::IListCell* p_ListCell)
    {
        if (!p_ListCell || p_ListCell->OwnerList().Ptr() != this || std::find(m_VisibleListCells.begin(), m_VisibleListCells.end(), p_ListCell) == m_VisibleListCells.end())
            return;

        SetSelectedListItem(p_ListCell->ListItem());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame
    void DefaultCVVList::Update()
    {
        if (!m_Dirty)
            return;

        UpdateForCurrentScroll();
        m_Dirty = false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Clear
    /// @param p_ScrollToTop Should scroll back to top after clearing?
    void DefaultCVVList::ClearVisibles(bool p_ScrollToTop)
    {
        for (int l_I = 0; l_I < m_VisibleListCells.size(); ++l_I)
        {
            auto& l_ExistingCell = m_VisibleListCells[l_I];

            try { l_ExistingCell->ListItem()->OnHide(); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCVVList.ClearVisibles] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            l_ExistingCell->ListItem()->SetCell(nullptr);
            l_ExistingCell->Bind(nullptr, -1, nullptr);
            m_ListCellPool->Release(l_ExistingCell);
        }

        m_VisibleListCells.clear();

        if (p_ScrollToTop)
            m_ScrollView->ScrollTo(0.0f, false);
    }
    /// @brief Get list cell per page
    float DefaultCVVList::GetListCellPerPage()
    {
        return m_ScrollView->ScrollPageSize() / m_ListCellTemplate->GetCellHeight();
    }
    /// @brief Update for current scroll
    void DefaultCVVList::UpdateForCurrentScroll()
    {
        if (!m_ListCellTemplate)
            return;

        auto l_ScrollPosY    = std::clamp<float>(m_ScrollView->Position(), 0.0f, m_ScrollView->ContentSize());
        auto l_StartItem     = std::max<float>(0, Mathf::FloorToInt(l_ScrollPosY / m_ListCellTemplate->GetCellHeight()));
        auto l_EndItem       = std::min<float>(Mathf::CeilToInt(l_StartItem + GetListCellPerPage()), m_ListItems.size());

        for (int l_I = 0; l_I < m_VisibleListCells.size(); ++l_I)
        {
            auto& l_ExistingCell = m_VisibleListCells[l_I];
            if (l_ExistingCell->Index() < l_StartItem || l_ExistingCell->Index() >= l_EndItem)
            {
                try { l_ExistingCell->ListItem()->OnHide(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCVVList.UpdateForCurrentScroll] OnHide Error:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }

                l_ExistingCell->ListItem()->SetCell(nullptr);
                l_ExistingCell->Bind(nullptr, -1, nullptr);
                m_ListCellPool->Release(l_ExistingCell);

                m_VisibleListCells.erase(std::next(m_VisibleListCells.begin(), l_I));
                l_I--;
            }
        }

        for (int l_I = l_StartItem; l_I < l_EndItem; ++l_I)
        {
            if (std::count_if(m_VisibleListCells.begin(), m_VisibleListCells.end(), [&](const _v::MonoPtr<Data::IListCell>& x) -> bool { return x->Index() == l_I; }))
                continue;

            auto l_NewCell = m_ListCellPool->Get();
            l_NewCell->RTransform()->set_anchoredPosition(Vector2(0, -(l_I * m_ListCellTemplate->GetCellHeight())));
            l_NewCell->Bind(this, l_I, m_ListItems[l_I]);
            l_NewCell->ListItem()->SetCell(l_NewCell.Ptr());

            try { l_NewCell->ListItem()->OnShow(); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCVVList.UpdateForCurrentScroll] OnShow Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            m_VisibleListCells.push_back(l_NewCell);
        }

        for (int l_I = 0; l_I < m_VisibleListCells.size(); ++l_I)
        {
            auto& l_Object = m_VisibleListCells[l_I];
            l_Object->SetState(l_Object->ListItem() == m_SelectedListItem);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On rect transform dimensions changed
    void DefaultCVVList::OnRectTransformDimensionsChange()
    {
        m_Dirty = true;
        SetSelectedListItem(m_SelectedListItem, false);
    }

}   ///< namespace CP_SDK::UI::DefaultComponents