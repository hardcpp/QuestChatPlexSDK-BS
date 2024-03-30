#include "CP_SDK/UI/DefaultComponents/DefaultCSecondaryButton.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Rect.hpp>
#include <UnityEngine/UI/Button.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::EventSystems;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCSecondaryButton);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCSecondaryButton)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,         m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(CSizeFitter,        m_CSizeFitter);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LayoutGroupC,       m_StackLayoutGroup);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,           m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(ButtonC,            m_Button);
        CP_SDK_UI_IL2CPP_BIND_FIELD(BackgroundImageC,   m_BackgroundImage);
        CP_SDK_UI_IL2CPP_BIND_FIELD(IconImageC,         m_IconImage);
        CP_SDK_UI_IL2CPP_BIND_FIELD(TextC,              m_Label);

        m_OnClick       = {this, &DefaultCSecondaryButton::OnClick_Impl};
        m_SetTooltip    = {this, &DefaultCSecondaryButton::SetTooltip_Impl};

        Init += {this, &DefaultCSecondaryButton::Init_DefaultCSecondaryButton};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCSecondaryButton)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCSecondaryButton::Init_DefaultCSecondaryButton()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);

        m_CSizeFitter = get_gameObject()->AddComponent<ContentSizeFitter*>();
        m_CSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        m_CSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minHeight(5.0f);

        m_BackgroundImage = GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        m_BackgroundImage->get_gameObject()->set_layer(UISystem::UILayer);
        m_BackgroundImage->get_rectTransform()->SetParent           (get_transform(), false);
        m_BackgroundImage->get_rectTransform()->set_anchorMin       (Vector2::get_zero());
        m_BackgroundImage->get_rectTransform()->set_anchorMax       (Vector2::get_one());
        m_BackgroundImage->get_rectTransform()->set_sizeDelta       (Vector2::get_zero());
        m_BackgroundImage->get_rectTransform()->set_localPosition   (Vector3::get_zero());
        m_BackgroundImage->set_material                     (UISystem::Override_GetUIMaterial());
        m_BackgroundImage->set_color                        (UISystem::SecondaryColor);
        m_BackgroundImage->set_type                         (Image::Type::Sliced);
        m_BackgroundImage->set_pixelsPerUnitMultiplier      (1);
        m_BackgroundImage->set_sprite                       (UISystem::GetUIButtonSprite().Ptr());

        m_Label = UISystem::TextFactory->Create(u"Label", get_transform());
        m_Label->SetMargins(2.0f, 0.0f, 2.0f, 0.0f);
        m_Label->SetAlign(TMPro::TextAlignmentOptions::Capline);
        m_Label->SetStyle(FontStyles::Bold);

        m_IconImage = GameObject::New_ctor("Icon", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        m_IconImage->get_gameObject()->set_layer(UISystem::UILayer);
        m_IconImage->get_rectTransform()->SetParent           (get_transform(), false);
        m_IconImage->get_rectTransform()->set_anchorMin       (Vector2::get_zero());
        m_IconImage->get_rectTransform()->set_anchorMax       (Vector2::get_one());
        m_IconImage->get_rectTransform()->set_sizeDelta       (Vector2::get_zero());
        m_IconImage->get_rectTransform()->set_localPosition   (Vector3::get_zero());
        m_IconImage->set_material                     (UISystem::Override_GetUIMaterial());
        m_IconImage->set_type                         (Image::Type::Simple);
        m_IconImage->set_pixelsPerUnitMultiplier      (1);
        m_IconImage->set_preserveAspect               (true);
        m_IconImage->get_gameObject()->set_active(false);

        m_Button = get_gameObject()->AddComponent<Button*>();
        m_Button->set_targetGraphic (m_BackgroundImage.Ptr());
        m_Button->set_transition    (Selectable::Transition::ColorTint);
        //m_Button->get_onClick()->RemoveAllListeners();
        m_Button->get_onClick()->AddListener(MakeUnityAction(std::bind(&DefaultCSecondaryButton::Button_OnClick, this)));

        auto l_Colors = m_Button->get_colors();
        l_Colors.set_normalColor        (ColorU::Convert(Color32(0, 230, 230, 230, 180)));
        l_Colors.set_highlightedColor   (ColorU::Convert(Color32(0, 255, 255, 255, 255)));
        l_Colors.set_pressedColor       (ColorU::Convert(Color32(0, 200, 200, 200, 255)));
        l_Colors.set_selectedColor      (l_Colors.get_normalColor());
        l_Colors.set_disabledColor      (ColorU::Convert(Color32(0, 255, 255, 255,  68)));
        l_Colors.set_fadeDuration       (0.05f);
        m_Button->set_colors(l_Colors);

        m_StackLayoutGroup = get_gameObject()->AddComponent<Subs::SubStackLayoutGroup*>();
        reinterpret_cast<Subs::SubStackLayoutGroup*>(m_StackLayoutGroup.Ptr())->SetChildForceExpandWidth (true);
        reinterpret_cast<Subs::SubStackLayoutGroup*>(m_StackLayoutGroup.Ptr())->SetChildForceExpandHeight(true);
        m_StackLayoutGroup->set_childAlignment(TextAnchor::MiddleCenter);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On click event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add Should add
    void DefaultCSecondaryButton::OnClick_Impl(_v::CActionRef<> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnClickEvent += p_Functor;
        else        m_OnClickEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set tooltip
    /// @param p_Tooltip New tooltip
    void DefaultCSecondaryButton::SetTooltip_Impl(std::u16string_view p_Tooltip)
    {
        m_Tooltip = p_Tooltip;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On click unity callback
    void DefaultCSecondaryButton::Button_OnClick()
    {
        try { m_OnClickEvent(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCSecondaryButton.Button_OnClick] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On pointer enter
    /// @param p_EventData Event data
    void DefaultCSecondaryButton::IPointerEnterHandler__OnPointerEnter(PointerEventData* p_EventData)
    {
        if (m_Tooltip.size() == 0)
            return;

        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        auto l_Rect = RTransform()->get_rect();
        auto l_RPos = Vector3(l_Rect.get_x() + l_Rect.get_width() / 2.0f, l_Rect.get_y() + l_Rect.get_height(), 0.0f);
        auto l_Pos  = RTransform()->TransformPoint(l_RPos);
        l_ViewController->ShowTooltip(l_Pos, m_Tooltip);
    }
    /// @brief On pointer exit
    /// @param p_EventData Event data
    void DefaultCSecondaryButton::IPointerExitHandler__OnPointerExit(PointerEventData* p_EventData)
    {
        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        l_ViewController->HideTooltip();
    }

}   ///< namespace CP_SDK::UI::DefaultComponents