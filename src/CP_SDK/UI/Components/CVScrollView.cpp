#include "CP_SDK/UI/Components/CVScrollView.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CVScrollView);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CVScrollView)
    {
        ScrollType              = EScrollType::PageSize;
        FixedCellSize           = 10.0f;
        PageStepNormalizedSize  = 0.7f;
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CVScrollView)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On scroll changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CVScrollView* CVScrollView::OnScrollChanged(_v::CActionRef<float> p_Functor, bool p_Add)
    {
        m_OnScrollChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief
    CVScrollView* CVScrollView::UpdateContentSize()
    {
        m_UpdateContentSize();
        return this;
    }
    /// @brief
    /// @param p_ContentSize
    CVScrollView* CVScrollView::SetContentSize(float p_ContentSize)
    {
        m_SetContentSize(p_ContentSize);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Scroll to position
    /// @param p_TargetPosition New target position
    /// @param p_Animated       Is animated?
    CVScrollView* CVScrollView::ScrollTo(float p_TargetPosition, bool p_Animated)
    {
        m_ScrollTo(p_TargetPosition, p_Animated);
        return this;
    }
    /// @brief Scroll to end
    /// @param p_Animated Is animated?
    CVScrollView* CVScrollView::ScrollToEnd(bool p_Animated)
    {
        m_ScrollToEnd(p_Animated);
        return this;
    }
    /// @brief Refresh scroll buttons
    CVScrollView* CVScrollView::RefreshScrollButtons()
    {
        m_RefreshScrollButtons();
        return this;
    }

}   ///< namespace CP_SDK::UI::Components