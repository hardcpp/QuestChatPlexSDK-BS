#include "CP_SDK/UI/DefaultComponents/DefaultCGLayout.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCGLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCGLayout)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,     m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(CSizeFitter,    m_CSizeFitter);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,       m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(GLayoutGroup,   m_GLayoutGroup);

        Init += {this, &DefaultCGLayout::Init_DefaultCGLayout};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCGLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCGLayout::Init_DefaultCGLayout()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);
        m_RTransform->set_anchorMin(Vector2(0.0f, 0.0f));
        m_RTransform->set_anchorMax(Vector2(1.0f, 1.0f));
        m_RTransform->set_sizeDelta(Vector2(5.0f, 5.0f));

        m_GLayoutGroup = get_gameObject()->AddComponent<GridLayoutGroup*>();
        m_GLayoutGroup->set_childAlignment(TextAnchor::UpperCenter);

        m_CSizeFitter = get_gameObject()->AddComponent<ContentSizeFitter*>();
        m_CSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        m_CSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
    }

}   ///< namespace CP_SDK::UI::DefaultComponents