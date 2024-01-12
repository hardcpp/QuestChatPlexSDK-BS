#include "CP_SDK/UI/DefaultComponents/DefaultCColorInput.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/Button.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCColorInput);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCColorInput)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);

        m_Value         = Color::get_red();
        m_AlphaSupport  = false;

        m_OnValueChanged    = {this, &DefaultCColorInput::OnValueChanged_Impl};

        m_GetValue          = {this, &DefaultCColorInput::GetValue_Impl};

        m_SetAlphaSupport   = {this, &DefaultCColorInput::SetAlphaSupport_Impl};
        m_SetInteractable   = {this, &DefaultCColorInput::SetInteractable_Impl};
        m_SetValue          = {this, &DefaultCColorInput::SetValue_Impl};

        Init += [this]() { Init_DefaultCColorInput(); };
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCColorInput)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCColorInput::Init_DefaultCColorInput()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_sizeDelta(Vector2(15.0f, 5.0f));

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_preferredWidth (15.0f);
        m_LElement->set_preferredHeight( 5.0f);
        m_LElement->set_minWidth       (15.0f);
        m_LElement->set_minHeight      ( 5.0f);

        auto l_View  = GameObject::New_ctor("View", ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*).convert()) }))->GetComponent<RectTransform*>();
        l_View->get_gameObject()->set_layer(UISystem::UILayer);
        l_View->SetParent(get_transform(), false);
        l_View->set_anchorMin(Vector2::op_Multiply(Vector2::get_one(), 0.5f));
        l_View->set_anchorMax(Vector2::op_Multiply(Vector2::get_one(), 0.5f));
        l_View->set_sizeDelta(Vector2(15.0f, 5.0f));

        m_BG = reinterpret_cast<Image*>(GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_BG->get_gameObject()->set_layer(UISystem::UILayer);
        m_BG->get_rectTransform()->SetParent(l_View, false);
        m_BG->get_rectTransform()->set_pivot           (Vector2(  0.50f,  0.50f));
        m_BG->get_rectTransform()->set_anchorMin       (Vector2(  0.00f,  0.00f));
        m_BG->get_rectTransform()->set_anchorMax       (Vector2(  1.00f,  1.00f));
        m_BG->get_rectTransform()->set_anchoredPosition(Vector2( -2.50f,  0.00f));
        m_BG->get_rectTransform()->set_sizeDelta       (Vector2( -5.00f,  0.00f));
        m_BG->set_material               (UISystem::Override_GetUIMaterial());
        m_BG->set_color                  (m_Value);
        m_BG->set_type                   (Image::Type::Sliced);
        m_BG->set_pixelsPerUnitMultiplier(1);
        m_BG->set_sprite                 (UISystem::GetUIRoundRectLeftBGSprite().Ptr());

        m_Icon = UISystem::PrimaryButtonFactory->Create(u"Inc", l_View);
        m_Icon->RTransform()->set_pivot           (Vector2( 1.0f,  0.5f));
        m_Icon->RTransform()->set_anchorMin       (Vector2( 1.0f,  0.0f));
        m_Icon->RTransform()->set_anchorMax       (Vector2( 1.0f,  1.0f));
        m_Icon->RTransform()->set_anchoredPosition(Vector2(-0.0f,  0.0f));
        m_Icon->RTransform()->set_sizeDelta       (Vector2( 6.0f, -1.0f));
        m_Icon->LElement()->set_minWidth       (5.0f);
        m_Icon->LElement()->set_minHeight      (5.0f);
        m_Icon->LElement()->set_preferredWidth (5.0f);
        m_Icon->LElement()->set_preferredHeight(5.0f);
        m_Icon->SetText(u"🖌");
        m_Icon->SetBackgroundSprite(UISystem::GetUIRoundRectRightBGSprite().Ptr());
        m_Icon->OnClick({this, &DefaultCColorInput::Button_OnClick});

        m_Button = l_View->get_gameObject()->AddComponent<Button*>();
        m_Button->set_targetGraphic(m_BG.Ptr());
        m_Button->set_transition   (Selectable::Transition::ColorTint);
        //m_Button->onClick.RemoveAllListeners();
        m_Button->get_onClick()->AddListener(MakeUnityAction(std::bind(&DefaultCColorInput::Button_OnClick, this)));

        auto l_Colors = m_Button->get_colors();
        l_Colors.set_normalColor     (ColorU::Convert(Color32(0, 255, 255, 255, 255)));
        l_Colors.set_highlightedColor(ColorU::Convert(Color32(0, 255, 255, 255, 127)));
        l_Colors.set_pressedColor    (ColorU::Convert(Color32(0, 255, 255, 255, 255)));
        l_Colors.set_selectedColor   (l_Colors.get_normalColor());
        l_Colors.set_disabledColor   (ColorU::Convert(Color32(0, 255, 255, 255,  48)));
        l_Colors.set_fadeDuration    (0.05f);
        m_Button->set_colors(l_Colors);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCColorInput::OnValueChanged_Impl(_v::CActionRef<Color> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnChange += p_Functor;
        else        m_OnChange -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get text
    Color DefaultCColorInput::GetValue_Impl()
    {
        return m_Value;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set alpha support
    /// @param p_Support New state
    void DefaultCColorInput::SetAlphaSupport_Impl(bool p_Support)
    {
        m_AlphaSupport = p_Support;
    }
    /// @brief Set interactable state
    /// @param p_Interactable New state
    void DefaultCColorInput::SetInteractable_Impl(bool p_Interactable)
    {
        m_Button->set_interactable(p_Interactable);
        m_Icon->SetInteractable(p_Interactable);
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    void DefaultCColorInput::SetValue_Impl(Color p_Value, bool p_Notify)
    {
        m_Value = p_Value;
        m_BG->set_color(ColorU::WithAlpha(p_Value, 1.0f));

        if (p_Notify)
            Notify();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Notify
    void DefaultCColorInput::Notify()
    {
        try { m_OnChange(m_Value); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCColorInput.Notify] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief On click unity callback
    void DefaultCColorInput::Button_OnClick()
    {
        if (UISystem::Override_OnClick.IsValid())
            UISystem::Override_OnClick(this);

        auto l_ViewController = get_gameObject()->GetComponentInParent<IViewController*>();
        if (!l_ViewController)
            return;

        m_OriginalValue = m_Value;
        l_ViewController->ShowColorPickerModal(m_Value, m_AlphaSupport, [this](Color x) -> void {
            SetValue(x, true);
        }, [this]() -> void {
            SetValue(m_OriginalValue, true);
        });
    }

}   ///< namespace CP_SDK::UI::DefaultComponents