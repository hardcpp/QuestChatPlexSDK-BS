#include "CP_SDK/UI/DefaultComponents/DefaultCSlider.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <System/Math.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/RectTransformUtility.hpp>
#include <UnityEngine/Time.hpp>

#include <sstream>

using namespace CP_SDK::Unity::Extensions;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::EventSystems;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCSlider);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(DefaultCSlider, Components::CSlider)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(DecButton,  m_DecButton);
        CP_SDK_UI_IL2CPP_BIND_FIELD(IncButton,  m_IncButton);

        m_OnColor           = UISystem::PrimaryColor;
        m_OffColor          = UISystem::SecondaryColor;
        m_ColorMode         = EColorMode::None;
        m_EnableDragging    = true;
        m_IsInteger         = false;
        m_HandleSize        = 1.5f;
        m_ValueSize         = 20.0f;
        m_SeparatorSize     = 0.50f;
        m_NumberOfSteps     = 100;
        m_NormalizedValue   = 0.5f;
        m_MinValue          = 0.0f;
        m_MaxValue          = 1.0f;
        m_Increments        = 0.1f;
        m_DragSmoothing     = 1.0f;

        m_OnValueChanged    = {this, &DefaultCSlider::OnValueChanged_Impl};

        m_GetMinValue       = {this, &DefaultCSlider::GetMinValue_Impl};
        m_GetMaxValue       = {this, &DefaultCSlider::GetMaxValue_Impl};
        m_GetIncrements     = {this, &DefaultCSlider::GetIncrements_Impl};
        m_GetValue          = {this, &DefaultCSlider::GetValue_Impl};

        m_SetColor          = {this, &DefaultCSlider::SetColor_Impl};
        m_SetFormatter      = {this, &DefaultCSlider::SetFormatter_Impl};
        m_SetInteger        = {this, &DefaultCSlider::SetInteger_Impl};
        m_SetInteractable   = {this, &DefaultCSlider::SetInteractable_Impl};
        m_SetMinValue       = {this, &DefaultCSlider::SetMinValue_Impl};
        m_SetMaxValue       = {this, &DefaultCSlider::SetMaxValue_Impl};
        m_SetIncrements     = {this, &DefaultCSlider::SetIncrements_Impl};
        m_SetValue          = {this, &DefaultCSlider::SetValue_Impl};

        m_SwitchToColorMode = {this, &DefaultCSlider::SwitchToColorMode_Impl};
        m_ColorModeSetHue   = {this, &DefaultCSlider::ColorModeSetHue_Impl};

        m_SetValue          = {this, &DefaultCSlider::SetValue_Impl};

        Init += {this, &DefaultCSlider::Init_DefaultCSlider};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(DefaultCSlider, Components::CSlider)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCSlider::Init_DefaultCSlider()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_sizeDelta(Vector2(60.0f, 5.5f));

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minWidth       (30.0f);
        m_LElement->set_minHeight      (5.0f);
        m_LElement->set_preferredHeight(5.0f);
        m_LElement->set_flexibleWidth  (150.0f);

        m_BG = reinterpret_cast<Image*>(GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_BG->get_gameObject()->get_gameObject()->set_layer(UISystem::UILayer);
        m_BG->get_rectTransform()->SetParent(get_transform(), false);
        m_BG->get_rectTransform()->set_pivot           (Vector2(  0.50f,  0.50f));
        m_BG->get_rectTransform()->set_anchorMin       (Vector2(  0.00f,  0.00f));
        m_BG->get_rectTransform()->set_anchorMax       (Vector2(  1.00f,  1.00f));
        m_BG->get_rectTransform()->set_anchoredPosition(Vector2(  0.00f,  0.00f));
        m_BG->get_rectTransform()->set_sizeDelta       (Vector2(-10.00f,  0.00f));
        m_BG->set_material               (UISystem::Override_GetUIMaterial());
        m_BG->set_color                  (ColorU::WithAlpha(m_OnColor, 110.0f / 255.0f));
        m_BG->set_type                   (Image::Type::Sliced);
        m_BG->set_pixelsPerUnitMultiplier(1);
        m_BG->set_sprite                 (UISystem::GetUISliderBGSprite().Ptr());

        m_DecButton = UISystem::PrimaryButtonFactory->Create(u"Dec", m_RTransform.Ptr());
        m_DecButton->RTransform()->set_pivot           (Vector2(0.0f,  0.5f));
        m_DecButton->RTransform()->set_anchorMin       (Vector2(0.0f,  0.0f));
        m_DecButton->RTransform()->set_anchorMax       (Vector2(0.0f,  1.0f));
        m_DecButton->RTransform()->set_anchoredPosition(Vector2(0.0f,  0.0f));
        m_DecButton->RTransform()->set_sizeDelta       (Vector2(6.0f, -1.0f));
        m_DecButton->IconImageC()->get_rectTransform()->set_localEulerAngles(Vector3(0.0f,  0.0f, -90.0f));
        m_DecButton->IconImageC()->get_rectTransform()->set_localScale      (Vector3(0.6f,  0.4f,   0.6f));
        m_DecButton->LElement()->set_minWidth       (5.0f);
        m_DecButton->LElement()->set_minHeight      (5.0f);
        m_DecButton->LElement()->set_preferredWidth (5.0f);
        m_DecButton->LElement()->set_preferredHeight(5.0f);
        m_DecButton->SetText(u"")->SetBackgroundSprite(UISystem::GetUIRoundRectLeftBGSprite().Ptr())->SetIconSprite(UISystem::GetUIDownArrowSprite().Ptr())
                   ->OnClick([this]() -> void {
                        SetNormalizedValue(GetSteppedNormalizedValue() - ((m_NumberOfSteps > 0) ? (1.0f / (float)m_NumberOfSteps) : 0.1f));
                    });

        m_IncButton = UISystem::PrimaryButtonFactory->Create(u"Inc", m_RTransform.Ptr());
        m_IncButton->RTransform()->set_pivot           (Vector2( 1.0f,  0.5f));
        m_IncButton->RTransform()->set_anchorMin       (Vector2( 1.0f,  0.0f));
        m_IncButton->RTransform()->set_anchorMax       (Vector2( 1.0f,  1.0f));
        m_IncButton->RTransform()->set_anchoredPosition(Vector2(-0.0f,  0.0f));
        m_IncButton->RTransform()->set_sizeDelta       (Vector2( 6.0f, -1.0f));
        m_IncButton->IconImageC()->get_rectTransform()->set_localEulerAngles(Vector3( 0.0f,  0.0f,  90.0f));
        m_IncButton->IconImageC()->get_rectTransform()->set_localScale      (Vector3( 0.6f,  0.4f,   0.6f));
        m_IncButton->LElement()->set_minWidth       (5.0f);
        m_IncButton->LElement()->set_minHeight      (5.0f);
        m_IncButton->LElement()->set_preferredWidth (5.0f);
        m_IncButton->LElement()->set_preferredHeight(5.0f);
        m_IncButton->SetText(u"")->SetBackgroundSprite(UISystem::GetUIRoundRectRightBGSprite().Ptr())->SetIconSprite(UISystem::GetUIDownArrowSprite().Ptr())
                   ->OnClick([this]() -> void {
                        SetNormalizedValue(GetSteppedNormalizedValue() + ((m_NumberOfSteps > 0) ? (1.0f / (float)m_NumberOfSteps) : 0.1f));
                    });

        m_SlidingArea = GameObject::New_ctor("SlidingArea", ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)) }))->GetComponent<RectTransform*>();
        m_SlidingArea->get_gameObject()->set_layer(UISystem::UILayer);
        m_SlidingArea->SetParent(get_transform(), false);
        m_SlidingArea->set_pivot           (Vector2(  0.5f,  0.5f));
        m_SlidingArea->set_anchorMin       (Vector2(  0.0f,  0.0f));
        m_SlidingArea->set_anchorMax       (Vector2(  1.0f,  1.0f));
        m_SlidingArea->set_anchoredPosition(Vector2(  0.0f,  0.0f));
        m_SlidingArea->set_sizeDelta       (Vector2(-11.5f, -1.0f));

        m_Handle = reinterpret_cast<Image*>(GameObject::New_ctor("Handle", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_Handle->get_gameObject()->set_layer(UISystem::UILayer);
        m_Handle->get_rectTransform()->SetParent(m_SlidingArea.Ptr(), false);
        m_Handle->get_rectTransform()->set_pivot           (Vector2(0.5f,  0.5f));
        m_Handle->get_rectTransform()->set_anchorMin       (Vector2(0.0f,  0.0f));
        m_Handle->get_rectTransform()->set_anchorMax       (Vector2(0.0f,  1.0f));
        m_Handle->get_rectTransform()->set_anchoredPosition(Vector2(0.0f,  0.0f));
        m_Handle->get_rectTransform()->set_sizeDelta       (Vector2(1.5f, -1.0f));
        m_Handle->set_material               (UISystem::Override_GetUIMaterial());
        m_Handle->set_color                  (ColorU::Convert(Color32(255, 255, 255, 210)));
        m_Handle->set_type                   (Image::Type::Simple);
        m_Handle->set_pixelsPerUnitMultiplier(15);
        m_Handle->set_sprite                 (UISystem::GetUISliderHandleSprite().Ptr());

        m_ValueText = UISystem::TextFactory->Create(u"Value", m_SlidingArea.Ptr());

        SetIncrements(0.01f);
        UpdateVisuals();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCSlider::OnValueChanged_Impl(_v::CActionRef<float> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnChange += p_Functor;
        else        m_OnChange -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get min value
    float DefaultCSlider::GetMinValue_Impl()
    {
        return m_MinValue;
    }
    /// @brief Get max value
    float DefaultCSlider::GetMaxValue_Impl()
    {
        return m_MaxValue;
    }
    /// @brief Get increments
    float DefaultCSlider::GetIncrements_Impl()
    {
        return m_Increments;
    }
    /// @brief Get value
    float DefaultCSlider::GetValue_Impl()
    {
        return ConvertFromSteppedNormalizedValue(GetSteppedNormalizedValue());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set theme color
    /// @param p_Color New color
    void DefaultCSlider::SetColor_Impl(Color p_Color)
    {
        m_OnColor   = p_Color;
        m_OffColor  = p_Color;

        m_DecButton->SetColor(p_Color);
        m_IncButton->SetColor(p_Color);

        UpdateStyle();
    }
    /// @brief Set value formatter
    /// @param p_CustomFormatter Custom value formatter
    void DefaultCSlider::SetFormatter_Impl(_v::CFuncRef<std::u16string, float> p_CustomFormatter)
    {
        m_CustomFormatter = p_CustomFormatter;
        UpdateVisuals();
    }
    /// @brief Set integer mode
    /// @param p_IsInteger Is integer?
    void DefaultCSlider::SetInteger_Impl(bool p_IsInteger)
    {
        m_IsInteger = p_IsInteger;
        UpdateVisuals();
    }
    /// @brief Set button interactable state
    /// @param p_Interactable New state
    void DefaultCSlider::SetInteractable_Impl(bool p_Interactable)
    {
        /// Handled in the super method
        //base.SetInteractable(p_Interactable);
        UpdateStyle();
    }
    /// @brief Set min value
    /// @param p_MinValue New value
    void DefaultCSlider::SetMinValue_Impl(float p_MinValue)
    {
        if (m_MinValue != p_MinValue)
        {
            m_MinValue = p_MinValue;
            UpdateVisuals();
        }
    }
    /// @brief Set max value
    /// @param p_MaxValue New value
    void DefaultCSlider::SetMaxValue_Impl(float p_MaxValue)
    {
        if (m_MaxValue != p_MaxValue)
        {
            m_MaxValue = p_MaxValue;
            UpdateVisuals();
        }
    }
    /// @brief Set increments
    /// @param p_Increments New value
    void DefaultCSlider::SetIncrements_Impl(float p_Increments)
    {
        m_Increments    = p_Increments;
        m_NumberOfSteps = (int)System::Math::Round((m_MaxValue - m_MinValue) / m_Increments) + 1;
        UpdateVisuals();
    }
    /// @brief Set value
    /// @param p_Value Value
    /// @param p_Notify Notify?
    void DefaultCSlider::SetValue_Impl(float p_Value, bool p_Notify)
    {
        p_Value = m_IsInteger ? Mathf::Round((int)p_Value) : p_Value;
        SetNormalizedValue((p_Value - m_MinValue) / (m_MaxValue - m_MinValue), p_Notify);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Switch to color mode
    /// @param p_H Is Hue mode?
    /// @param p_S Is saturation mode?
    /// @param p_V Is value mode?
    /// @param p_O Is opacity mode?
    void DefaultCSlider::SwitchToColorMode_Impl(bool p_H, bool p_S, bool p_V, bool p_O)
    {
        if (m_ColorMode != EColorMode::None) return;

        if (!p_O)
        {
            m_BGSub2 = reinterpret_cast<Image*>(GameObject::New_ctor("BGSub2", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
            m_BGSub2->get_rectTransform()->SetParent(m_BG->get_transform(), false);
            m_BGSub2->get_rectTransform()->set_pivot           (Vector2(0.50f, 0.50f));
            m_BGSub2->get_rectTransform()->set_anchorMin       (Vector2(0.00f, 0.00f));
            m_BGSub2->get_rectTransform()->set_anchorMax       (Vector2(1.00f, 1.00f));
            m_BGSub2->get_rectTransform()->set_anchoredPosition(Vector2(0.00f, 0.00f));
            m_BGSub2->get_rectTransform()->set_sizeDelta       (Vector2(0.00f, 0.00f));
            m_BGSub2->set_material               (UISystem::Override_GetUIMaterial());
            m_BGSub2->set_color                  (Color::get_white());
            m_BGSub2->set_type                   (Image::Type::Simple);
            m_BGSub2->set_pixelsPerUnitMultiplier(1);

            m_BG->set_enabled(false);
        }

        m_ValueText->TMProUGUI()->set_enabled(false);

        if (p_H)
        {
            m_BGSub2->set_sprite(UISystem::GetUIColorPickerHBGSprite().Ptr());
            m_ColorMode = EColorMode::H;
        }
        else if (p_S)
        {
            m_BGSub1 = reinterpret_cast<Image*>(GameObject::New_ctor("BGSub1", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
            m_BGSub1->get_rectTransform()->SetParent(m_BG->get_transform(), false);
            m_BGSub1->get_rectTransform()->set_pivot           (Vector2(0.50f, 0.50f));
            m_BGSub1->get_rectTransform()->set_anchorMin       (Vector2(0.00f, 0.00f));
            m_BGSub1->get_rectTransform()->set_anchorMax       (Vector2(1.00f, 1.00f));
            m_BGSub1->get_rectTransform()->set_anchoredPosition(Vector2(0.00f, 0.00f));
            m_BGSub1->get_rectTransform()->set_sizeDelta       (Vector2(0.00f, 0.00f));
            m_BGSub1->set_material               (UISystem::Override_GetUIMaterial());
            m_BGSub1->set_color                  (Color::get_white());
            m_BGSub1->set_type                   (Image::Type::Simple);
            m_BGSub1->set_pixelsPerUnitMultiplier(1);
            m_BGSub1->set_sprite                 (UISystem::GetUIColorPickerFBGSprite().Ptr());
            m_BGSub1->get_transform()->SetAsFirstSibling();

            m_BGSub2->set_sprite(UISystem::GetUIColorPickerSBGSprite().Ptr());
            m_ColorMode = EColorMode::S;
        }
        else if (p_V)
        {
            m_BGSub2->set_sprite(UISystem::GetUIColorPickerVBGSprite().Ptr());
            m_ColorMode = EColorMode::V;
        }
        else
            m_ColorMode = EColorMode::O;

        UpdateStyle();
    }
    /// @brief Color mode set H
    /// @param p_H Is Hue mode?
    void DefaultCSlider::ColorModeSetHue_Impl(float p_H)
    {
        if (m_ColorMode == EColorMode::None) return;

        if (m_ColorMode == EColorMode::S) m_BGSub1->set_color(Color::HSVToRGB(p_H, 1.0f, 1.0f));
        if (m_ColorMode == EColorMode::V) m_BGSub2->set_color(Color::HSVToRGB(p_H, 1.0f, 1.0f));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component enable
    void DefaultCSlider::OnEnable()
    {
        CP_SDK_IL2CPP_CALL_BASE_METHOD(Selectable, "OnEnable");

        SetNormalizedValue(m_NormalizedValue, false);
        UpdateVisuals();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame (late)
    void DefaultCSlider::LateUpdate()
    {
        if (!m_LeftMouseButtonPressed)
            return;

        auto l_Delta        = m_DragSmoothing * Time::get_deltaTime() * Mathf::Clamp01((Time::get_time() - m_DragStartTime) / 2.0f);
        m_DragCurrentValue  = Mathf::Lerp(m_DragCurrentValue, m_DragTargetValue, l_Delta);

        SetNormalizedValue(m_DragCurrentValue, true);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief May drag based on pointer event data
    /// @param p_EventData Event data
    bool DefaultCSlider::MayDrag(PointerEventData* p_EventData)
    {
        if (!IsActive() || !IsInteractable())
            return false;

        return p_EventData->get_button() == PointerEventData::InputButton::Left;
    }
    /// @brief Initialize a potential drag
    /// @param p_EventData Event data
    void DefaultCSlider::IInitializePotentialDragHandler__OnInitializePotentialDrag(PointerEventData* p_EventData)
    {
        p_EventData->set_useDragThreshold(false);
    }
    /// @brief On drag start
    /// @param p_EventData Event data
    void DefaultCSlider::IBeginDragHandler__OnBeginDrag(PointerEventData* p_EventData)
    {
        if (!MayDrag(p_EventData) || !m_EnableDragging)
            return;
    }
    /// @brief On mouse drag
    /// @param p_EventData Event data
    void DefaultCSlider::IDragHandler__OnDrag(PointerEventData* p_EventData)
    {
        if (!MayDrag(p_EventData) || !m_EnableDragging || !m_SlidingArea)
            return;

        UpdateDrag(p_EventData);
    }
    /// @brief Update drag
    /// @param p_EventData Event data
    void DefaultCSlider::UpdateDrag(PointerEventData* p_EventData)
    {
        Vector2 l_LocalPoint;
        if (p_EventData->button != PointerEventData::InputButton::Left
            || !m_SlidingArea
            || p_EventData->hovered->get_Count() == 0
            || !RectTransformUtility::ScreenPointToLocalPointInRectangle(m_SlidingArea.Ptr(), p_EventData->position, p_EventData->get_pressEventCamera(), /*OUT*/l_LocalPoint)
            || std::isnan(l_LocalPoint.x)
            || std::isnan(l_LocalPoint.y))
            return;

        auto l_HandleRectTransform   = m_Handle->get_rectTransform();
        auto l_HandleRect            = l_HandleRectTransform->get_rect();
        auto l_SlidingRect           = m_SlidingArea->get_rect();

        auto l_Point = l_LocalPoint - l_SlidingRect.get_position() - Vector2(l_HandleRect.m_Width * 0.5f, 0.0f) - (l_HandleRect.get_size() - l_HandleRectTransform->get_sizeDelta()) * 0.5f;
        auto l_Value = l_SlidingRect.m_Width * (1.0f - m_HandleSize / l_SlidingRect.m_Width);

        m_DragTargetValue = (l_Point.x / l_Value);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On pointer enter
    /// @param p_EventData Event data
    void DefaultCSlider::IPointerEnterHandler__OnPointerEnter(PointerEventData* p_EventData)
    {
        CP_SDK_IL2CPP_CALL_BASE_METHOD(Selectable, "OnPointerEnter", p_EventData);

        if (IsInteractable())
        {
            m_LastPointerEvent = p_EventData;
            UpdateStyle();
        }
    }
    /// @brief On pointer exit
    /// @param p_EventData Event data
    void DefaultCSlider::IPointerExitHandler__OnPointerExit(PointerEventData* p_EventData)
    {
        CP_SDK_IL2CPP_CALL_BASE_METHOD(Selectable, "OnPointerExit", p_EventData);

        m_LastPointerEvent = nullptr;
        UpdateStyle();
    }
    /// @brief On pointer button down
    /// @param p_EventData Event data
    void DefaultCSlider::IPointerDownHandler__OnPointerDown(PointerEventData* p_EventData)
    {
        if (!MayDrag(p_EventData))
            return;

        CP_SDK_IL2CPP_CALL_BASE_METHOD(Selectable, "OnPointerDown", p_EventData);

        m_DragCurrentValue          = m_DragTargetValue = GetSteppedNormalizedValue();
        m_LeftMouseButtonPressed    = true;
        m_DragStartTime             = Time::get_time();

        UpdateDrag(p_EventData);
        UpdateStyle();
    }
    /// @brief On pointer button up
    /// @param p_EventData Event data
    void DefaultCSlider::IPointerUpHandler__OnPointerUp(PointerEventData* p_EventData)
    {
        CP_SDK_IL2CPP_CALL_BASE_METHOD(Selectable, "OnPointerUp", p_EventData);

        if (m_LeftMouseButtonPressed)
        {
            m_LeftMouseButtonPressed = false;
            SetNormalizedValue(m_DragCurrentValue, true);
        }

        UpdateStyle();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set normalized value
    /// @param p_Value  Normalized value
    /// @param p_Notify Should notify?
    void DefaultCSlider::SetNormalizedValue(float p_Value, bool p_Notify)
    {
        auto l_Original = m_NormalizedValue;
        m_NormalizedValue = Mathf::Clamp01(p_Value);

        if (l_Original == GetSteppedNormalizedValue())
            return;

        UpdateVisuals();

        if (p_Notify)
        {
            try { m_OnChange(ConvertFromSteppedNormalizedValue(GetSteppedNormalizedValue())); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCSlider.SetNormalizedValue] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On rect transform dimensions changed
    void DefaultCSlider::UIBehaviour__OnRectTransformDimensionsChange()
    {
        if (IsActive())
            UpdateVisuals();
    }
    /// @brief Update visual style
    void DefaultCSlider::UpdateStyle()
    {
        auto l_IsInteractable = IsInteractable();
        m_DecButton->SetColor(l_IsInteractable ? m_OnColor : m_OffColor);
        m_IncButton->SetColor(l_IsInteractable ? m_OnColor : m_OffColor);

        if (m_LeftMouseButtonPressed || m_LastPointerEvent)
        {
            m_DecButton->SetInteractable(!m_LeftMouseButtonPressed);
            m_IncButton->SetInteractable(!m_LeftMouseButtonPressed);
            m_BG->set_color(ColorU::WithAlpha(l_IsInteractable ? m_OnColor : m_OffColor, l_IsInteractable ? 200.0f / 255.0f : 50.0f / 255.0f));
        }
        else
        {
            m_DecButton->SetInteractable(l_IsInteractable);
            m_IncButton->SetInteractable(l_IsInteractable);
            m_BG->set_color(ColorU::WithAlpha(l_IsInteractable ? m_OnColor : m_OffColor, l_IsInteractable ? 110.0f / 255.0f : 50.0f / 255.0f));
        }
    }
    /// @brief Update visuals
    void DefaultCSlider::UpdateVisuals()
    {
        if (!m_Handle)
            return;

        auto l_HandleRectTransform   = m_Handle->get_rectTransform();
        auto l_ValueRectTransform    = m_ValueText->RTransform();

        auto l_Width     = m_SlidingArea->get_rect().m_Width;
        auto l_PosX      = GetSteppedNormalizedValue() * (l_Width - m_HandleSize);
        auto l_AnchorMin = Vector2(0.0f, 0.0f);
        auto l_AnchorMax = Vector2(0.0f, 1.0f);

        l_HandleRectTransform->set_anchorMin       (l_AnchorMin);
        l_HandleRectTransform->set_anchorMax       (l_AnchorMax);
        l_HandleRectTransform->set_sizeDelta       (Vector2(m_HandleSize, 0.0f));
        l_HandleRectTransform->set_pivot           (Vector2(0.0f, 0.5f));
        l_HandleRectTransform->set_anchoredPosition(Vector2(l_PosX, 0.0f));

        l_ValueRectTransform->set_anchorMin(l_AnchorMin);
        l_ValueRectTransform->set_anchorMax(l_AnchorMax);
        l_ValueRectTransform->set_sizeDelta(Vector2(m_ValueSize, 0.0f));

        if (GetSteppedNormalizedValue() > 0.5f)
        {
            l_ValueRectTransform->set_pivot           (Vector2(1.0f, 0.5f));
            l_ValueRectTransform->set_anchoredPosition(Vector2(l_PosX - m_SeparatorSize, 0.0f));
            m_ValueText->SetAlign(TextAlignmentOptions::CaplineRight);
        }
        else
        {
            l_ValueRectTransform->set_pivot           (Vector2(0.0f, 0.5f));
            l_ValueRectTransform->set_anchoredPosition(Vector2(l_PosX + m_HandleSize + m_SeparatorSize, 0.0f));
            m_ValueText->SetAlign(TextAlignmentOptions::CaplineLeft);
        }

        m_ValueText->SetText(GetTextValue(ConvertFromSteppedNormalizedValue(GetSteppedNormalizedValue())));
    }
    /// @brief On state transition
    /// @param p_State   New state
    /// @param p_Instant Is instant
    void DefaultCSlider::Selectable__DoStateTransition(Selectable::SelectionState p_State, bool p_Instant)
    {
        if (m_DecButton) m_DecButton->SetInteractable(get_interactable());
        if (m_IncButton) m_IncButton->SetInteractable(get_interactable());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get transform
    Transform* DefaultCSlider::ICanvasElement__get_transform()
    {
        return get_transform();
    }
    /// @brief On canvas rebuild
    /// @param p_Executing Executing update
    void DefaultCSlider::ICanvasElement__Rebuild(CanvasUpdate p_Executing)
    {

    }
    /// @brief On layout rebuild
    void DefaultCSlider::ICanvasElement__LayoutComplete()
    {

    }
    /// @brief On graphic update complete
    void DefaultCSlider::ICanvasElement__GraphicUpdateComplete()
    {

    }
    /// @brief Is destroyed
    bool DefaultCSlider::ICanvasElement__IsDestroyed()
    {
        return UIBehaviour::IsDestroyed();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get stepped rounded normalized value
    float DefaultCSlider::GetSteppedNormalizedValue()
    {
        auto l_Result = m_NormalizedValue;
        if (m_NumberOfSteps > 1)
            l_Result = Mathf::Round(l_Result * (float)(m_NumberOfSteps - 1)) / (float)(m_NumberOfSteps - 1);

        return l_Result;
    }
    /// @brief Convert stepped normalized value to value
    /// @param p_NormalizedValue Normalized value
    float DefaultCSlider::ConvertFromSteppedNormalizedValue(float p_NormalizedValue)
    {
        auto l_Value = p_NormalizedValue * (m_MaxValue - m_MinValue) + m_MinValue;
        return m_IsInteger ? (int)l_Value : l_Value;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get text for value
    /// @param p_ConvertedValue Value to convert
    std::u16string DefaultCSlider::GetTextValue(float p_ConvertedValue)
    {
        if (m_CustomFormatter.IsValid())
            return m_CustomFormatter(m_IsInteger ? (int)p_ConvertedValue : p_ConvertedValue);

        if (m_IsInteger)
            return _v::StrToU16Str(std::to_string(static_cast<int>(p_ConvertedValue)));

        std::ostringstream l_Converted;
        l_Converted.precision(2);
        l_Converted << std::fixed << p_ConvertedValue;

        return Utils::StrToU16Str(l_Converted.str());
    }

}   ///< namespace CP_SDK::UI::DefaultComponents