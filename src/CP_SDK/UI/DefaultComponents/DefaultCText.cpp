#include "CP_SDK/UI/DefaultComponents/DefaultCText.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCText);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCText)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(TMProUGUI,  m_TMProUGUI);

        Init += {this, &DefaultCText::Init_DefaultCText};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCText)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCText::Init_DefaultCText()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minHeight(5.0f);

        m_TMProUGUI = reinterpret_cast<TextMeshProUGUI*>(get_gameObject()->AddComponent(UISystem::Override_UnityComponent_TextMeshProUGUI.ptr()));
        m_TMProUGUI->set_font               (UISystem::Override_GetUIFont()                 ? UISystem::Override_GetUIFont()                : m_TMProUGUI->get_font());
        m_TMProUGUI->set_fontSharedMaterial (UISystem::Override_GetUIFontSharedMaterial()   ? UISystem::Override_GetUIFontSharedMaterial()  : m_TMProUGUI->get_fontSharedMaterial());
        m_TMProUGUI->set_margin             (Vector4(0.0f, 0.0f, 0.0f, 0.0f));
        m_TMProUGUI->set_fontSize           (3.4f * UISystem::FontScale);
        m_TMProUGUI->set_color              (UISystem::TextColor);
        m_TMProUGUI->set_raycastTarget      (false);

        m_TMProUGUI->get_rectTransform()->set_anchorMin(Vector2(0.5f, 0.5f));
        m_TMProUGUI->get_rectTransform()->set_anchorMax(Vector2(0.5f, 0.5f));

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());

        SetAlign(TextAlignmentOptions::Left);
        SetText(u"Default Text");
    }

}   ///< namespace CP_SDK::UI::DefaultComponents