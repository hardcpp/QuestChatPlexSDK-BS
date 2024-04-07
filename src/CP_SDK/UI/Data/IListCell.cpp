#include "CP_SDK/UI/Data/IListCell.hpp"
#include "CP_SDK/UI/Components/Generics/CVXList.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/UI/Button.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::EventSystems;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Data {

    CP_SDK_IL2CPP_INHERIT_INIT(IListCell);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(IListCell)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(OwnerList,  m_OwnerList);
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(IListCell)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    int IListCell::Index() const
    {
        return m_Index;
    }
    const std::shared_ptr<IListItem>& IListCell::ListItem()
    {
        return m_ListItem;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create cell instance
    /// @param p_Parent Parent
    IListCell* IListCell::Create(RectTransform* p_Parent)
    {
        auto l_NewCell = m_AddSelfComponent(GameObject::New_ctor("ListCell", ArrayW<System::Type*>({
            UISystem::Override_UnityComponent_Image.ptr() ,
            reinterpret_cast<System::Type*>(csTypeOf(RectTransform*).convert()),
            reinterpret_cast<System::Type*>(csTypeOf(Button*).convert())
        })));
        l_NewCell->get_transform()->SetParent(p_Parent, false);
        l_NewCell->get_gameObject()->SetActive(false);
        l_NewCell->Build();

        return l_NewCell;
    }
    /// @brief Bind to list
    /// @param p_OwnerList Owner list
    /// @param p_Index List item index
    /// @param p_ListItem List item instance
    void IListCell::Bind(_v::CMonoPtrRef<Components::CVXList> p_OwnerList, int p_Index, const std::shared_ptr<IListItem>& p_ListItem)
    {
        m_OwnerList = p_OwnerList;
        m_Index     = p_Index;
        m_ListItem  = p_ListItem;

        SetState(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Build cell
    void IListCell::Build()
    {
        if (m_RTransform)
            return;

        m_RTransform = GetComponent<RectTransform*>();
        if (!m_RTransform)
            m_RTransform = get_gameObject()->AddComponent<RectTransform*>();

        m_Image = GetComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        m_Image->set_material               (UISystem::Override_GetUIMaterial());
        m_Image->set_type                   (Image::Type::Sliced);
        m_Image->set_pixelsPerUnitMultiplier(1);
        m_Image->set_sprite                 (UISystem::GetUIRoundBGSprite().Ptr());
        m_Image->set_preserveAspect         (false);

        m_Button = GetComponent<Button*>();
        m_Button->set_targetGraphic(m_Image.Ptr());
        m_Button->set_transition   (Selectable::Transition::ColorTint);
        //m_Button->onClick.RemoveAllListeners();

        m_Button->get_onClick()->AddListener(MakeUnityAction(std::bind(&IListCell::Button_OnClick, this)));

        SetState(false);

        m_Build();
    }
    /// @brief Set list cell state
    /// @param p_State New state
    void IListCell::SetState(bool p_State)
    {
        if (!m_RTransform)
            return;

        if (m_Stateless)
            p_State = false;

        auto l_IsOdd  = ((Index() & 1) != 0) ? true : false;
        auto l_Colors = m_Button->get_colors();
        l_Colors.set_normalColor     (ColorU::Convert(Color32(0, 255, 255, 255, p_State ? (uint8_t)100 : (l_IsOdd ? (uint8_t)15 : (uint8_t)0))));
        l_Colors.set_highlightedColor(ColorU::Convert(Color32(0, 255, 255, 255, p_State ? (uint8_t)100 : (uint8_t)75)));
        l_Colors.set_pressedColor    (ColorU::Convert(Color32(0, 255, 255, 255, p_State ? (uint8_t)100 : (uint8_t)75)));
        l_Colors.set_selectedColor   (l_Colors.get_normalColor());
        l_Colors.set_disabledColor   (ColorU::Convert(Color32(0, 127, 127, 127, p_State ? (uint8_t)100 : (uint8_t)75)));
        l_Colors.set_fadeDuration    (0.05f);
        m_Button->set_colors(l_Colors);
    }
    /// @brief Set is stateless
    /// @param p_Stateless New value
    void IListCell::SetStateless(bool p_Stateless)
    {
        m_Stateless = p_Stateless;

        if (p_Stateless)
            SetState(false);
    }
    /// @brief Set is selectable
    /// @param p_Selectable New state
    void IListCell::SetSelectable(bool p_Selectable)
    {
        if (!m_Button || !m_Image)
            return;

        m_Image->set_enabled (p_Selectable);
        m_Button->set_enabled(p_Selectable);
    }
    /// @brief Get cell height
    float IListCell::GetCellHeight()
    {
        return m_GetCellHeight();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On cell click
    void IListCell::Button_OnClick()
    {
        if (m_OwnerList)
            m_OwnerList->OnListCellClicked(this);

        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On pointer enter
    /// @param p_EventData Event data
    void IListCell::IPointerEnterHandler__OnPointerEnter(PointerEventData* p_EventData)
    {
        if (Tooltip.size() == 0)
            return;

        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        auto l_Rect = RTransform()->get_rect();
        auto l_RPos = Vector3(l_Rect.get_x() + l_Rect.get_width() / 2.0f, l_Rect.get_y() + l_Rect.get_height(), 0.0f);
        auto l_Pos  = RTransform()->TransformPoint(l_RPos);
        l_ViewController->ShowTooltip(l_Pos, Tooltip);
    }
    /// @brief On pointer exit
    /// @param p_EventData Event data
    void IListCell::IPointerExitHandler__OnPointerExit(PointerEventData* p_EventData)
    {
        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        l_ViewController->HideTooltip();
    }

}   ///< namespace CP_SDK::UI::Data