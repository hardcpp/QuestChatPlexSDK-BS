#include "CP_SDK/UI/DefaultComponents/DefaultCHLayout.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCHLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCHLayout)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,        m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(CSizeFitter,       m_CSizeFitter);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,          m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(HOrVLayoutGroup,   m_HOrVLayoutGroup);
        CP_SDK_UI_IL2CPP_BIND_FIELD(HLayoutGroup,      m_HLayoutGroup);

        Init += {this, &DefaultCHLayout::Init_DefaultCHLayout};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCHLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCHLayout::Init_DefaultCHLayout()
    {
        if (m_RTransform)
            return;

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);
        m_RTransform->set_anchorMin(Vector2(0.0f, 0.0f));
        m_RTransform->set_anchorMax(Vector2(1.0f, 1.0f));
        m_RTransform->set_sizeDelta(Vector2(0.0f, 0.0f));

        m_CSizeFitter = get_gameObject()->AddComponent<ContentSizeFitter*>();
        m_CSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        m_CSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();

        m_HLayoutGroup = get_gameObject()->AddComponent<HorizontalLayoutGroup*>();
        m_HLayoutGroup->set_childForceExpandWidth (false);
        m_HLayoutGroup->set_childForceExpandHeight(false);
        m_HLayoutGroup->set_childScaleWidth       (false);
        m_HLayoutGroup->set_childScaleHeight      (false);
        m_HLayoutGroup->set_childControlWidth     (true);
        m_HLayoutGroup->set_childControlHeight    (true);
        m_HLayoutGroup->set_childAlignment        (TextAnchor::UpperCenter);

        /// IL2CPP specifics
        m_HOrVLayoutGroup = m_HLayoutGroup.Ptr();
    }

}   ///< namespace CP_SDK::UI::DefaultComponents