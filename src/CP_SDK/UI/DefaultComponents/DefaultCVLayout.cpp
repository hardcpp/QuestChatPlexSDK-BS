#include "CP_SDK/UI/DefaultComponents/DefaultCVLayout.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCVLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCVLayout)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,        m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(CSizeFitter,       m_CSizeFitter);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,          m_LElement);
        CP_SDK_UI_IL2CPP_BIND_FIELD(HOrVLayoutGroup,   m_HOrVLayoutGroup);
        CP_SDK_UI_IL2CPP_BIND_FIELD(VLayoutGroup,      m_VLayoutGroup);

        Init += {this, &DefaultCVLayout::Init_DefaultCVLayout};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCVLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCVLayout::Init_DefaultCVLayout()
    {
        if (m_RTransform)
            return;

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_anchorMin(Vector2(0.0f, 0.0f));
        m_RTransform->set_anchorMax(Vector2(1.0f, 1.0f));
        m_RTransform->set_sizeDelta(Vector2(0.0f, 0.0f));

        m_CSizeFitter = get_gameObject()->AddComponent<ContentSizeFitter*>();
        m_CSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        m_CSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();

        m_VLayoutGroup = get_gameObject()->AddComponent<VerticalLayoutGroup*>();
        m_VLayoutGroup->set_childForceExpandWidth (false);
        m_VLayoutGroup->set_childForceExpandHeight(false);
        m_VLayoutGroup->set_childScaleWidth       (false);
        m_VLayoutGroup->set_childScaleHeight      (false);
        m_VLayoutGroup->set_childControlWidth     (true);
        m_VLayoutGroup->set_childControlHeight    (true);
        m_VLayoutGroup->set_childAlignment        (TextAnchor::UpperCenter);

        /// IL2CPP specifics
        m_HOrVLayoutGroup = m_VLayoutGroup.Ptr();
    }

}   ///< namespace CP_SDK::UI::DefaultComponents