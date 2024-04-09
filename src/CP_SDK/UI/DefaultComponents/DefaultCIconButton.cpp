#include "CP_SDK/UI/DefaultComponents/DefaultCIconButton.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "CP_SDK/Unity/Operators.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Rect.hpp>
#include <UnityEngine/Time.hpp>
#include <UnityEngine/UI/Button.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::EventSystems;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCIconButton);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCIconButton)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,         m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,           m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(ButtonC,            m_Button);
        CP_SDK_UI_IL2CPP_BIND_FIELD(IconImageC,         m_IconImage);

        m_OnClick = {this, &DefaultCIconButton::OnClick_Impl};

        Init += {this, &DefaultCIconButton::Init_DefaultCIconButton };
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCIconButton)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCIconButton::Init_DefaultCIconButton()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();

        m_IconImage = GameObject::New_ctor(u"Icon", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        m_IconImage->get_gameObject()->set_layer(UISystem::UILayer);
        m_IconImage->get_rectTransform()->SetParent           (get_transform(), false);
        m_IconImage->get_rectTransform()->set_anchorMin       (Vector2::get_zero());
        m_IconImage->get_rectTransform()->set_anchorMax       (Vector2::get_one());
        m_IconImage->get_rectTransform()->set_sizeDelta       (Vector2::get_zero());
        m_IconImage->get_rectTransform()->set_localPosition   (Vector3::get_zero());
        m_IconImage->set_material                     (UISystem::Override_GetUIMaterial());
        m_IconImage->set_type                         (Image::Type::Simple);
        m_IconImage->set_pixelsPerUnitMultiplier      (1);
        m_IconImage->set_sprite                       (UISystem::GetUIDownArrowSprite().Ptr());
        m_IconImage->set_preserveAspect               (true);
        m_IconImage->set_raycastTarget                (false);

        m_Button = get_gameObject()->AddComponent<Button*>();
        m_Button->set_targetGraphic (m_IconImage.Ptr());
        m_Button->set_transition    (Selectable::Transition::ColorTint);
        //m_Button->get_onClick()->RemoveAllListeners();
        m_Button->get_onClick()->AddListener(MakeUnityAction(std::bind(&DefaultCIconButton::Button_OnClick, this)));

        auto l_FakeBg = get_gameObject()->AddComponent(UISystem::Override_UnityComponent_Image.ptr()).try_cast<Image>().value_or(nullptr);
        l_FakeBg->set_material               (UISystem::Override_GetUIMaterial());
        l_FakeBg->set_type                   (Image::Type::Simple);
        l_FakeBg->set_pixelsPerUnitMultiplier(1);
        l_FakeBg->set_sprite                 (UISystem::GetUIRectBGSprite().Ptr());
        l_FakeBg->set_color                  (ColorU::WithAlpha(Color::get_black(), 0.01f));

        auto l_Colors = m_Button->get_colors();
        l_Colors.set_normalColor        (ColorU::Convert(Color32(0, 255, 255, 255, 150)));
        l_Colors.set_highlightedColor   (ColorU::Convert(Color32(0, 255, 255, 255, 255)));
        l_Colors.set_pressedColor       (ColorU::Convert(Color32(0, 255, 255, 255, 255)));
        l_Colors.set_selectedColor      (l_Colors.get_normalColor());
        l_Colors.set_disabledColor      (ColorU::Convert(Color32(0, 127, 127, 127, 150)));
        l_Colors.set_fadeDuration       (0.05f);
        m_Button->set_colors(l_Colors);

        SetWidth(3.6f);
        SetHeight(2.3f);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On click event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add Should add
    void DefaultCIconButton::OnClick_Impl(const _v::Delegate<void()>& p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnClickEvent += p_Functor;
        else        m_OnClickEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On click unity callback
    void DefaultCIconButton::Button_OnClick()
    {
        try { m_OnClickEvent(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCIconButton.Button_OnClick] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On pointer enter
    /// @param p_EventData Event data
    void DefaultCIconButton::IPointerEnterHandler__OnPointerEnter(PointerEventData* p_EventData)
    {
        if (!m_Button->get_interactable())
            return;

        StopAllCoroutines();
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(Coroutine_AnimateScale(this, Vector3::get_one() * 1.25f, 0.075f)));
    }
    /// @brief On pointer exit
    /// @param p_EventData Event data
    void DefaultCIconButton::IPointerExitHandler__OnPointerExit(PointerEventData* p_EventData)
    {
        StopAllCoroutines();
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(Coroutine_AnimateScale(this, Vector3::get_one(), 0.075f)));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Coroutine animate scale
    /// @param p_Target Target scale
    /// @param p_Time   Animation time
    custom_types::Helpers::Coroutine DefaultCIconButton::Coroutine_AnimateScale(DefaultCIconButton* p_Self, Vector3 p_Target, float p_Time)
    {
        auto l_DeltaT = 0.0f;

        while (l_DeltaT < p_Time)
        {
            l_DeltaT += Time::get_deltaTime();
            p_Self->m_IconImage->get_rectTransform()->set_localScale(Vector3::Lerp(p_Self->m_IconImage->get_rectTransform()->get_localScale(), p_Target, l_DeltaT));

            co_yield nullptr;
        }

        p_Self->m_IconImage->get_rectTransform()->set_localScale(p_Target);
    }

}   ///< namespace CP_SDK::UI::DefaultComponents