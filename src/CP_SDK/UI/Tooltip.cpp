#include "CP_SDK/UI/Tooltip.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(Tooltip);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(Tooltip)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(Tooltip)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create a tooltip
    /// @param p_Parent Parent container
    Tooltip* Tooltip::Create(RectTransform* p_Parent)
    {
        auto l_Tooltip = GameObject::New_ctor("Tooltip")->AddComponent<Tooltip*>();
        l_Tooltip->m_RTransform = l_Tooltip->get_gameObject()->AddComponent<RectTransform*>();
        l_Tooltip->m_RTransform->SetParent(p_Parent, false);
        l_Tooltip->m_RTransform->set_localRotation(Quaternion::get_identity());
        l_Tooltip->m_RTransform->set_localScale   (Vector3::get_one());
        l_Tooltip->m_RTransform->set_anchorMin    (Vector2(0.5f, 0.0f));
        l_Tooltip->m_RTransform->set_anchorMax    (Vector2(0.5f, 0.0f));
        l_Tooltip->m_RTransform->set_pivot        (Vector2(0.5f, 0.0f));

        l_Tooltip->m_HorizontalLayoutGroup = l_Tooltip->get_gameObject()->AddComponent<HorizontalLayoutGroup*>();
        l_Tooltip->m_HorizontalLayoutGroup->set_padding(RectOffset::New_ctor(2, 2, 2, 2));

        l_Tooltip->m_ContentSizeFitter = l_Tooltip->get_gameObject()->AddComponent<ContentSizeFitter*>();
        l_Tooltip->m_ContentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        l_Tooltip->m_ContentSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        l_Tooltip->m_Image = reinterpret_cast<Image*>(l_Tooltip->get_gameObject()->AddComponent(UISystem::Override_UnityComponent_Image.ptr()));
        l_Tooltip->m_Image->set_material               (UISystem::Override_GetUIMaterial());
        l_Tooltip->m_Image->set_type                   (Image::Type::Sliced);
        l_Tooltip->m_Image->set_pixelsPerUnitMultiplier(1);
        l_Tooltip->m_Image->set_sprite                 (UISystem::GetUIRoundBGSprite().Ptr());
        l_Tooltip->m_Image->set_raycastTarget          (false);
        l_Tooltip->m_Image->set_color                  (UISystem::TooltipBGColor);
        l_Tooltip->m_Image->set_maskable               (false);

        l_Tooltip->m_Border = UISystem::ImageFactory->Create(u"Text", l_Tooltip->m_RTransform.Ptr());
        l_Tooltip->m_Border->SetSprite(UISystem::GetUIRoundSmoothFrameSprite().Ptr());
        l_Tooltip->m_Border->SetColor(ColorU::WithAlpha(Color::get_white(), 0.80f));
        l_Tooltip->m_Border->SetType(Image::Type::Sliced);
        l_Tooltip->m_Border->LElement()->set_ignoreLayout(true);
        l_Tooltip->m_Border->RTransform()->set_anchorMin       (Vector2::get_zero());
        l_Tooltip->m_Border->RTransform()->set_anchorMax       (Vector2::get_one());
        l_Tooltip->m_Border->RTransform()->set_anchoredPosition(Vector2::get_zero());
        l_Tooltip->m_Border->RTransform()->set_sizeDelta       (Vector2::get_zero());

        l_Tooltip->m_Text = UISystem::TextFactory->Create(u"Text", l_Tooltip->m_RTransform.Ptr());
        l_Tooltip->m_Text->SetText(u"Tooltip");
        l_Tooltip->m_Text->SetFontSize(3.8f);
        l_Tooltip->m_Text->SetColor(Color::get_white());

        l_Tooltip->get_gameObject()->SetActive(false);

        return l_Tooltip;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show the tooltip
    /// @param p_Position World position
    /// @param p_Text     Tooltip text
    void Tooltip::Show(Vector3 p_Position, std::u16string_view p_Text)
    {
        m_Text->SetText(p_Text);
        m_RTransform->set_position(p_Position);

        get_gameObject()->SetActive(true);
    }
    /// @brief Hide the tooltip
    void Tooltip::Hide()
    {
        get_gameObject()->SetActive(false);
    }

}   ///< namespace CP_SDK::UI