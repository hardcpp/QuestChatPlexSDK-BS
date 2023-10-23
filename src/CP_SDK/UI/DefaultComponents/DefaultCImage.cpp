#include "CP_SDK/UI/DefaultComponents/DefaultCImage.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCImage);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCImage)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,   m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(ImageC,     m_ImageC);

        Init += {this, &DefaultCImage::Init_DefaultCImage};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCImage)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCImage::Init_DefaultCImage()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_sizeDelta(Vector2(5.0f, 5.0f));

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();

        m_ImageC = reinterpret_cast<Image*>(get_gameObject()->AddComponent(UISystem::Override_UnityComponent_Image.ptr()));
        m_ImageC->set_material                  (UISystem::Override_GetUIMaterial());
        m_ImageC->set_type                      (Image::Type::Simple);
        m_ImageC->set_pixelsPerUnitMultiplier   (1);
        m_ImageC->set_sprite                    (UISystem::GetUIRectBGSprite().Ptr());
        m_ImageC->set_preserveAspect            (true);
        m_ImageC->set_raycastTarget             (false);
    }

}   ///< namespace CP_SDK::UI::DefaultComponents