#include "CP_SDK/UI/DefaultComponents/Subs/SubVScrollIndicator.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <cfloat>
#include <cmath>

#include <UnityEngine/Rect.hpp>
#include <UnityEngine/Vector2.hpp>

using namespace UnityEngine;

namespace CP_SDK::UI::DefaultComponents::Subs {

    CP_SDK_IL2CPP_INHERIT_INIT(SubVScrollIndicator);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SubVScrollIndicator)
    {
        m_Padding               = 0.25f;
        m_NormalizedPageHeight  = 1.00f;
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SubVScrollIndicator)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Handle transform
    RectTransform* SubVScrollIndicator::Handle()
    {
        return m_Handle.Ptr(false);
    }
    RectTransform* SubVScrollIndicator::Handle(RectTransform* p_Handle)
    {
        if (m_Handle.Ptr(false) == p_Handle)
            return m_Handle.Ptr(false);

        m_Handle = p_Handle;
        RefreshHandle();
        return m_Handle.Ptr(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Progress
    float SubVScrollIndicator::Progress()
    {
        return m_Progress;
    }
    float SubVScrollIndicator::Progress(float p_Progress)
    {
        if (p_Progress == m_Progress)
            return m_Progress;

        m_Progress = std::clamp<float>(p_Progress, 0.0f, 1.0f);
        RefreshHandle();
        return m_Progress;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Normalized page height
    float SubVScrollIndicator::NormalizedPageHeight()
    {
        return m_NormalizedPageHeight;
    }
    float SubVScrollIndicator::NormalizedPageHeight(float p_NormalizedPageHeight)
    {
        if (p_NormalizedPageHeight == m_NormalizedPageHeight)
            return m_NormalizedPageHeight;

        m_NormalizedPageHeight = std::clamp<float>(p_NormalizedPageHeight, 0.0f, 1.0f);
        RefreshHandle();
        return m_NormalizedPageHeight;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Refresh handle
    void SubVScrollIndicator::RefreshHandle()
    {
        auto l_Progress  = get_transform().try_cast<RectTransform>().value_or(nullptr)->get_rect().get_size().y - 2.0f * m_Padding;
        auto l_PosY      = (0.0f - m_Progress) * (1.0f - m_NormalizedPageHeight) * l_Progress - m_Padding;

        if (std::isnan(l_PosY))
            l_PosY = 0.0f;

        m_Handle->set_sizeDelta       (Vector2(0.0f, m_NormalizedPageHeight * l_Progress));
        m_Handle->set_anchoredPosition(Vector2(0.0f, l_PosY));
    }

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs