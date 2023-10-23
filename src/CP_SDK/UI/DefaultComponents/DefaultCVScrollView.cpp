#include "CP_SDK/UI/DefaultComponents/DefaultCVScrollView.hpp"
#include "CP_SDK/UI/DefaultComponents/Subs/SubVScrollViewContent.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/Time.hpp>
#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector3.hpp>
#include <UnityEngine/UI/RectMask2D.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCVScrollView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCVScrollView)
    {
        m_Smooth            =  8.0f;
        m_ScrollBarWidth    =  6.0f;
        m_LastContentSize   = -1.0f;

        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,     m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,       m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(Container,      m_Container);

        Position        = [this]() { return m_VScrollViewContent->get_anchoredPosition().y; };
        ViewPortWidth   = [this]() { return m_ViewPort->get_rect().m_Width; };
        ScrollableSize  = [this]() { return std::max<float>(ContentSize() - ScrollPageSize(), 0.0f); };
        ScrollPageSize  = [this]() { return m_ViewPort->get_rect().m_Height; };
        ContentSize     = [this]() { return m_Container->get_rect().m_Height; };

        m_OnScrollChanged = {this, &DefaultCVScrollView::OnScrollChanged_Impl};

        m_UpdateContentSize = {this, &DefaultCVScrollView::UpdateContentSize_Impl};
        m_SetContentSize    = {this, &DefaultCVScrollView::SetContentSize_Impl};

        m_ScrollTo              = {this, &DefaultCVScrollView::ScrollTo_Impl};
        m_ScrollToEnd           = {this, &DefaultCVScrollView::ScrollToEnd_Impl};
        m_RefreshScrollButtons  = {this, &DefaultCVScrollView::RefreshScrollButtons_Impl};

        Init += {this, &DefaultCVScrollView::Init_DefaultCVScrollView};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCVScrollView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCVScrollView::Init_DefaultCVScrollView()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_anchorMin    (Vector2(0.0f, 1.0f));
        m_RTransform->set_anchorMax    (Vector2(1.0f, 1.0f));
        m_RTransform->set_sizeDelta    (Vector2::get_zero());
        m_RTransform->set_localPosition(Vector3::get_zero());

        ////////////////////////////////////////////////////////////////////////////

        auto l_ScrollBar = reinterpret_cast<RectTransform*>(GameObject::New_ctor("ScrollBar", ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)) }))->get_transform());
        l_ScrollBar->get_gameObject()->set_layer(UISystem::UILayer);
        l_ScrollBar->SetParent(get_transform(), false);
        l_ScrollBar->set_anchorMin       (Vector2(                      1.0f, 0.0f));
        l_ScrollBar->set_anchorMax       (Vector2(                      1.0f, 1.0f));
        l_ScrollBar->set_sizeDelta       (Vector2(          m_ScrollBarWidth, 0.0f));
        l_ScrollBar->set_anchoredPosition(Vector2(-(m_ScrollBarWidth / 2.0f), 0.0f));

        auto l_ScrollBarBG = reinterpret_cast<Image*>(l_ScrollBar->get_gameObject()->AddComponent(UISystem::Override_UnityComponent_Image.ptr()));
        l_ScrollBarBG->set_material               (UISystem::Override_GetUIMaterial());
        l_ScrollBarBG->set_color                  (ColorU::WithAlpha("#202020", 0.7f));
        l_ScrollBarBG->set_pixelsPerUnitMultiplier(1);
        l_ScrollBarBG->set_type                   (Image::Type::Sliced);
        l_ScrollBarBG->set_raycastTarget          (false);
        l_ScrollBarBG->set_sprite                 (UISystem::GetUIRoundBGSprite().Ptr());

        m_UpButton = UISystem::IconButtonFactory->Create(u"Up", l_ScrollBar);
        m_UpButton->LElement()->set_enabled           (false);
        m_UpButton->RTransform()->set_pivot           (Vector2(0.5f, 0.5f));
        m_UpButton->RTransform()->set_anchorMin       (Vector2(0.0f, 0.5f));
        m_UpButton->RTransform()->set_anchorMax       (Vector2(1.0f, 1.0f));
        m_UpButton->RTransform()->set_sizeDelta       (Vector2::get_zero());
        m_UpButton->RTransform()->set_anchoredPosition(Vector2::get_zero());
        m_UpButton->IconImageC()->get_rectTransform()->set_localEulerAngles(Vector3( 0.0f,  0.0f, 180.0f));
        m_UpButton->IconImageC()->get_rectTransform()->set_pivot           (Vector2( 0.5f,  0.0f));
        m_UpButton->IconImageC()->get_rectTransform()->set_anchorMin       (Vector2( 0.5f,  1.0f));
        m_UpButton->IconImageC()->get_rectTransform()->set_anchorMax       (Vector2( 0.5f,  1.0f));
        m_UpButton->IconImageC()->get_rectTransform()->set_anchoredPosition(Vector2( 0.0f, -2.0f));
        m_UpButton->IconImageC()->get_rectTransform()->set_sizeDelta       (Vector2( 4.0f,  2.0f));
        m_UpButton->SetSprite(UISystem::GetUIDownArrowSprite().Ptr())->OnClick({this, &DefaultCVScrollView::OnUpButton});

        m_DownButton = UISystem::IconButtonFactory->Create(u"Down", l_ScrollBar);
        m_DownButton->LElement()->set_enabled           (false);
        m_DownButton->RTransform()->set_pivot           (Vector2(0.5f, 0.5f));
        m_DownButton->RTransform()->set_anchorMin       (Vector2(0.0f, 0.0f));
        m_DownButton->RTransform()->set_anchorMax       (Vector2(1.0f, 0.5f));
        m_DownButton->RTransform()->set_sizeDelta       (Vector2::get_zero());
        m_DownButton->RTransform()->set_anchoredPosition(Vector2::get_zero());
        m_DownButton->IconImageC()->get_rectTransform()->set_pivot           (Vector2( 0.5f,  0.0f));
        m_DownButton->IconImageC()->get_rectTransform()->set_anchorMin       (Vector2( 0.5f,  0.0f));
        m_DownButton->IconImageC()->get_rectTransform()->set_anchorMax       (Vector2( 0.5f,  0.0f));
        m_DownButton->IconImageC()->get_rectTransform()->set_anchoredPosition(Vector2( 0.0f,  2.0f));
        m_DownButton->IconImageC()->get_rectTransform()->set_sizeDelta       (Vector2( 4.0f,  2.0f));
        m_DownButton->SetSprite(UISystem::GetUIDownArrowSprite().Ptr())->OnClick({this, &DefaultCVScrollView::OnDownButton});

        auto l_ScrollIndicator = reinterpret_cast<RectTransform*>(GameObject::New_ctor("ScrollIndicator", ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)) }))->get_transform());
        l_ScrollIndicator->get_gameObject()->set_layer(UISystem::UILayer);
        l_ScrollIndicator->SetParent(l_ScrollBar->get_transform(), false);
        l_ScrollIndicator->set_anchorMin(Vector2(0.5f,   0.0f));
        l_ScrollIndicator->set_anchorMax(Vector2(0.5f,   1.0f));
        l_ScrollIndicator->set_sizeDelta(Vector2(1.6f, -12.0f));

        auto l_ScrollIndicatorImage = reinterpret_cast<Image*>(l_ScrollIndicator->get_gameObject()->AddComponent(UISystem::Override_UnityComponent_Image.ptr()));
        l_ScrollIndicatorImage->set_sprite       (UISystem::GetUIRoundBGSprite().Ptr());
        l_ScrollIndicatorImage->set_color        (Color(0.0f, 0.0f, 0.0f, 0.5f));
        l_ScrollIndicatorImage->set_type         (Image::Type::Sliced);
        l_ScrollIndicatorImage->set_material     (UISystem::Override_GetUIMaterial());
        l_ScrollIndicatorImage->set_raycastTarget(false);

        m_Handle = reinterpret_cast<Image*>(GameObject::New_ctor("Handle", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_Handle->get_gameObject()->set_layer(UISystem::UILayer);
        m_Handle->get_rectTransform()->SetParent(l_ScrollIndicator->get_transform(), false);
        m_Handle->get_rectTransform()->set_pivot    (Vector2(0.5f, 1.0f));
        m_Handle->get_rectTransform()->set_anchorMin(Vector2(0.0f, 1.0f));
        m_Handle->get_rectTransform()->set_anchorMax(Vector2(1.0f, 1.0f));
        m_Handle->get_rectTransform()->set_sizeDelta(Vector2::get_zero());
        m_Handle->set_sprite       (UISystem::GetUIRoundBGSprite().Ptr());
        m_Handle->set_color        (Color(1.0f, 1.0f, 1.0f, 0.5f));
        m_Handle->set_type         (Image::Type::Sliced);
        m_Handle->set_material     (UISystem::Override_GetUIMaterial());
        m_Handle->set_raycastTarget(false);

        ////////////////////////////////////////////////////////////////////////////

        m_ViewPort = reinterpret_cast<RectTransform*>(GameObject::New_ctor("ViewPort", ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)),
            reinterpret_cast<System::Type*>(csTypeOf(RectMask2D*))
        }))->get_transform());
        m_ViewPort->get_gameObject()->set_layer(UISystem::UILayer);
        m_ViewPort->SetParent(get_transform(), false);
        m_ViewPort->set_anchorMin    (Vector2(                      0.0f, 0.0f));
        m_ViewPort->set_anchorMax    (Vector2(                      1.0f, 1.0f));
        m_ViewPort->set_sizeDelta    (Vector2(         -m_ScrollBarWidth, 0.0f));
        m_ViewPort->set_localPosition(Vector3(-(m_ScrollBarWidth / 2.0f), 0.0f, 0.0f));
        m_ViewPort->GetComponent<RectMask2D*>()->set_padding(Vector4(0.25f, 0.25f, 0.25f, 0.25f));

        m_VScrollViewContent = reinterpret_cast<RectTransform*>(GameObject::New_ctor("ScrollViewContent", ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)),
            reinterpret_cast<System::Type*>(csTypeOf(ContentSizeFitter*)),
            reinterpret_cast<System::Type*>(csTypeOf(VerticalLayoutGroup*))
        }))->get_transform());
        m_VScrollViewContent->get_gameObject()->set_layer(UISystem::UILayer);
        m_VScrollViewContent->SetParent(m_ViewPort.Ptr(), false);
        m_VScrollViewContent->set_anchorMin(Vector2(0.0f, 1.0f));
        m_VScrollViewContent->set_anchorMax(Vector2(1.0f, 1.0f));
        m_VScrollViewContent->set_sizeDelta(Vector2::get_zero());
        m_VScrollViewContent->set_pivot    (Vector2(0.5f, 1.0f));

        auto l_VScrollViewContent_ContentSizeFitter = m_VScrollViewContent->GetComponent<ContentSizeFitter*>();
        l_VScrollViewContent_ContentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
        l_VScrollViewContent_ContentSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        auto l_VScrollViewContent_VerticalLayoutGroup = m_VScrollViewContent->GetComponent<VerticalLayoutGroup*>();
        l_VScrollViewContent_VerticalLayoutGroup->set_childForceExpandWidth (false);
        l_VScrollViewContent_VerticalLayoutGroup->set_childForceExpandHeight(false);
        l_VScrollViewContent_VerticalLayoutGroup->set_childControlWidth     (true);
        l_VScrollViewContent_VerticalLayoutGroup->set_childControlHeight    (true);
        l_VScrollViewContent_VerticalLayoutGroup->set_childScaleWidth       (false);
        l_VScrollViewContent_VerticalLayoutGroup->set_childScaleHeight      (false);
        l_VScrollViewContent_VerticalLayoutGroup->set_childAlignment        (TextAnchor::UpperCenter);

        ////////////////////////////////////////////////////////////////////////////

        m_Container = reinterpret_cast<RectTransform*>(GameObject::New_ctor("Container", ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)),
            reinterpret_cast<System::Type*>(csTypeOf(VerticalLayoutGroup*)),
            reinterpret_cast<System::Type*>(csTypeOf(LayoutElement*))
        }))->get_transform());
        m_Container->get_gameObject()->set_layer(UISystem::UILayer);
        m_Container->SetParent(m_VScrollViewContent.Ptr(), false);
        m_Container->set_anchorMin(Vector2(0.0f, 1.0f));
        m_Container->set_anchorMax(Vector2(0.0f, 1.0f));

        m_VLayoutGroup = m_Container->GetComponent<VerticalLayoutGroup*>();
        m_VLayoutGroup->set_childForceExpandWidth (true);
        m_VLayoutGroup->set_childForceExpandHeight(false);
        m_VLayoutGroup->set_childControlWidth     (true);
        m_VLayoutGroup->set_childControlHeight    (true);
        m_VLayoutGroup->set_childScaleWidth       (false);
        m_VLayoutGroup->set_childScaleHeight      (false);
        m_VLayoutGroup->set_childAlignment        (TextAnchor::LowerCenter);
        m_VLayoutGroup->set_padding               (RectOffset::New_ctor(2, 2, 2, 2));
        m_VLayoutGroup->set_spacing               (0);

        m_Container->set_sizeDelta(Vector2(0.0f, -1.0f));

        ////////////////////////////////////////////////////////////////////////////

        set_enabled(false);

        m_VScrollIndicator = l_ScrollIndicator->get_gameObject()->AddComponent<Subs::SubVScrollIndicator*>();

        m_VScrollViewContent->get_gameObject()->AddComponent<Subs::SubVScrollViewContent*>()->VScrollView = this;
        m_VScrollIndicator->Handle(m_Handle->get_rectTransform());

        UpdateContentSize();
        RefreshScrollButtons();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame
    void DefaultCVScrollView::Update()
    {
        if (m_ContentSizeChanged)
        {
            SetDestinationScrollPos(m_DestinationPos);
            RefreshScrollButtons();
            m_VScrollIndicator->NormalizedPageHeight(m_ViewPort->get_rect().m_Height / m_VScrollViewContent->get_sizeDelta().y);

            m_ContentSizeChanged = false;
        }

        auto l_AnchoredPosition  = m_VScrollViewContent->get_anchoredPosition();
        auto l_VPos              = Mathf::Lerp(l_AnchoredPosition.y, m_DestinationPos, Time::get_deltaTime() * m_Smooth);

        if (std::abs(l_AnchoredPosition.y - m_DestinationPos) < 0.01f)
        {
            l_VPos = m_DestinationPos;
            set_enabled(false);
        }

        m_VScrollViewContent->set_anchoredPosition(Vector2(0.0f, l_VPos));
        m_ScrollPositionChangedEvent(l_VPos);

        UpdateVScrollIndicator(std::abs(l_VPos));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On scroll changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCVScrollView::OnScrollChanged_Impl(_v::CActionRef<float> p_Functor, bool p_Add)
    {
        if (p_Add)  m_ScrollPositionChangedEvent += p_Functor;
        else        m_ScrollPositionChangedEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief
    void DefaultCVScrollView::UpdateContentSize_Impl()
    {
        SetContentSize(m_VScrollViewContent->get_rect().m_Height);
        ScrollTo(0.0f, false);
    }
    /// @brief
    /// @param p_ContentSize
    void DefaultCVScrollView::SetContentSize_Impl(float p_ContentSize)
    {
        if (std::abs(m_LastContentSize - p_ContentSize) < 0.01f)
            return;

        m_VScrollViewContent->set_sizeDelta(Vector2(m_VScrollViewContent->get_sizeDelta().x, p_ContentSize));

        set_enabled(true);

        m_LastContentSize       = p_ContentSize;
        m_ContentSizeChanged    = true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Scroll to position
    /// @param p_TargetPosition New target position
    /// @param p_Animated       Is animated?
    void DefaultCVScrollView::ScrollTo_Impl(float p_TargetPosition, bool p_Animated)
    {
        SetDestinationScrollPos(p_TargetPosition);

        if (!p_Animated)
        {
            m_VScrollViewContent->set_anchoredPosition(Vector2(0.0f, m_DestinationPos));
            m_ScrollPositionChangedEvent(m_DestinationPos);
        }

        RefreshScrollButtons();
        set_enabled(true);
    }
    /// @brief Scroll to end
    /// @param p_Animated Is animated?
    void DefaultCVScrollView::ScrollToEnd_Impl(bool p_Animated)
    {
        ScrollTo(ContentSize() - ScrollPageSize(), p_Animated);
    }
    /// @brief Refresh scroll buttons
    void DefaultCVScrollView::RefreshScrollButtons_Impl()
    {
        m_UpButton->SetInteractable(m_DestinationPos > 0.001f);
        m_DownButton->SetInteractable(m_DestinationPos < ContentSize() - ScrollPageSize() - 0.001f);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set destination scroll position
    /// @param p_Value New destination
    void DefaultCVScrollView::SetDestinationScrollPos(float p_Value)
    {
        float l_Target = ContentSize() - ScrollPageSize();
        if (l_Target < 0.0f)  m_DestinationPos = 0.0f;
        else                  m_DestinationPos = std::clamp<float>(p_Value, 0.0f, l_Target);
    }
    /// @brief Update the vertical scroll indicator
    /// @param p_Position Current position
    void DefaultCVScrollView::UpdateVScrollIndicator(float p_Position)
    {
        m_VScrollIndicator->Progress(p_Position / (ContentSize() - ScrollPageSize()));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On up button pressed
    void DefaultCVScrollView::OnUpButton()
    {
        auto l_Target = m_DestinationPos;
        switch (ScrollType)
        {
            case EScrollType::FixedCellSize:
                l_Target -= FixedCellSize * (float)(Mathf::RoundToInt(ScrollPageSize() / FixedCellSize) - 1);
                l_Target  = Mathf::FloorToInt(l_Target / FixedCellSize) * FixedCellSize;
                break;

            case EScrollType::PageSize:
                l_Target -= PageStepNormalizedSize * ScrollPageSize();
                break;
        }

        SetDestinationScrollPos(l_Target);
        RefreshScrollButtons();

        set_enabled(true);
    }
    /// @brief On down button pressed
    void DefaultCVScrollView::OnDownButton()
    {
        auto l_Target = m_DestinationPos;
        switch (ScrollType)
        {
            case EScrollType::FixedCellSize:
                l_Target += FixedCellSize * (float)(Mathf::RoundToInt(ScrollPageSize() / FixedCellSize) - 1);
                l_Target  = Mathf::CeilToInt(l_Target / FixedCellSize) * FixedCellSize;
                break;

            case EScrollType::PageSize:
                l_Target += PageStepNormalizedSize * ScrollPageSize();
                break;
        }

        SetDestinationScrollPos(l_Target);
        RefreshScrollButtons();

        set_enabled(true);
    }

}   ///< namespace CP_SDK::UI::DefaultComponents