#include "CP_SDK/UI/DefaultComponents/Subs/SubStackLayoutGroup.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/Mathf.hpp>
#include <UnityEngine/Rect.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/LayoutUtility.hpp>
#include <UnityEngine/UI/Image.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents::Subs {

    CP_SDK_IL2CPP_INHERIT_INIT(SubStackLayoutGroup);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SubStackLayoutGroup)
    {
        m_ChildForceExpandWidth     = true;
        m_ChildForceExpandHeight    = true;
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SubStackLayoutGroup)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get child force expand width
    bool SubStackLayoutGroup::GetChildForceExpandWidth() const
    {
        return m_ChildForceExpandWidth;
    }
    /// @brief Get child force expand height
    bool SubStackLayoutGroup::GetChildForceExpandHeight() const
    {
        return m_ChildForceExpandHeight;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set child force expand width
    /// @param p_Value New value
    void SubStackLayoutGroup::SetChildForceExpandWidth(bool p_Value)
    {
        if (m_ChildForceExpandWidth == p_Value)
            return;

        m_ChildForceExpandWidth = p_Value;
        SetDirty();
    }
    /// @brief Set child force expand height
    /// @param p_Value New value
    void SubStackLayoutGroup::SetChildForceExpandHeight(bool p_Value)
    {
        if (m_ChildForceExpandHeight == p_Value)
            return;

        m_ChildForceExpandHeight = p_Value;
        SetDirty();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Calculate the layout input for horizontal axis
    void SubStackLayoutGroup::LayoutGroup__CalculateLayoutInputHorizontal()
    {
        ILayoutElement__CalculateLayoutInputHorizontal();
    }
    /// @brief Calculate the layout input for horizontal axis
    void SubStackLayoutGroup::ILayoutElement__CalculateLayoutInputHorizontal()
    {
        CP_SDK_IL2CPP_CALL_BASE_METHOD(LayoutGroup, "CalculateLayoutInputHorizontal");
        UpdateForAxis(0);
    }
    /// @brief Calculate the layout input for vertical axis
    void SubStackLayoutGroup::LayoutGroup__CalculateLayoutInputVertical()
    {
        ILayoutElement__CalculateLayoutInputVertical();
    }
    /// @brief Calculate the layout input for vertical axis
    void SubStackLayoutGroup::ILayoutElement__CalculateLayoutInputVertical()
    {
        UpdateForAxis(1);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// Set layout horizontal
    /// </summary>
    void SubStackLayoutGroup::LayoutGroup__SetLayoutHorizontal()
    {
        ILayoutController__SetLayoutHorizontal();
    }
    void SubStackLayoutGroup::ILayoutController__SetLayoutHorizontal()
    {
        SetChildrensAlongAxis(0);
    }
    /// <summary>
    /// Set layout vertical
    /// </summary>
    void SubStackLayoutGroup::LayoutGroup__SetLayoutVertical()
    {
        ILayoutController__SetLayoutVertical();
    }
    void SubStackLayoutGroup::ILayoutController__SetLayoutVertical()
    {
        SetChildrensAlongAxis(1);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Update the layout group for given axis
    /// @param p_Axis 0 Horizontal 1 vertical
    void SubStackLayoutGroup::UpdateForAxis(int p_Axis)
    {
        auto l_Padding   = p_Axis == 0 ? get_padding()->get_horizontal() : get_padding()->get_vertical();
        auto l_AxisMin   = 0.0f;
        auto l_AxisPref  = 0.0f;
        auto l_AxisFlex  = 0.0f;

        auto rectChildren = get_rectChildren();
        for (auto l_I = 0; l_I < rectChildren->get_Count(); ++l_I)
        {
            auto l_RectTransform = rectChildren->get_Item(l_I);

            if (l_RectTransform->GetComponent<Image*>())
                continue;

            auto l_MinSize       = LayoutUtility::GetMinSize(         l_RectTransform, p_Axis);
            auto l_PreferredSize = LayoutUtility::GetPreferredSize(   l_RectTransform, p_Axis);
            auto l_FlexibleSize  = LayoutUtility::GetFlexibleSize(    l_RectTransform, p_Axis);

            if (p_Axis == 0 ? m_ChildForceExpandWidth : m_ChildForceExpandHeight)
                l_FlexibleSize = std::max<float>(l_FlexibleSize, 1.0f);

            l_AxisMin   = std::max<float>(l_MinSize + l_Padding, l_AxisMin);
            l_AxisPref  = std::max<float>(l_PreferredSize + l_Padding, l_AxisPref);
            l_AxisFlex  = std::max<float>(l_FlexibleSize, l_AxisFlex);
        }

        l_AxisPref = std::max<float>(l_AxisMin, l_AxisPref);

        SetLayoutInputForAxis(l_AxisMin, l_AxisPref, l_AxisFlex, p_Axis);
    }
    /// @brief Set childrens along axis for given axis
    /// @param p_Axis 0 Horizontal 1 vertical
    void SubStackLayoutGroup::SetChildrensAlongAxis(int p_Axis)
    {
        auto l_AxisSize      = p_Axis == 0 ? get_rectTransform()->get_rect().get_size().x : get_rectTransform()->get_rect().get_size().y;
        auto l_NewAxisSize   = l_AxisSize - (p_Axis == 0 ? get_padding()->get_horizontal() : get_padding()->get_vertical());

        auto rectChildren = get_rectChildren();
        for (auto l_I = 0; l_I < rectChildren->get_Count(); ++l_I)
        {
            auto l_RectTransform = rectChildren->get_Item(l_I);
            auto l_MinSize       = LayoutUtility::GetMinSize(         l_RectTransform, p_Axis);
            auto l_PreferredSize = LayoutUtility::GetPreferredSize(   l_RectTransform, p_Axis);
            auto l_FlexibleSize  = LayoutUtility::GetFlexibleSize(    l_RectTransform, p_Axis);

            if ((p_Axis == 0) ? m_ChildForceExpandWidth : m_ChildForceExpandHeight)
                l_FlexibleSize = std::max<float>(l_FlexibleSize, 1.0f);

            auto l_Size      = Mathf::Clamp(l_NewAxisSize, l_MinSize, (l_FlexibleSize > 0.0f) ? l_AxisSize : l_PreferredSize);
            auto l_Position  = GetStartOffset(p_Axis, l_Size);

            SetChildAlongAxis(l_RectTransform, p_Axis, l_Position, l_Size);
        }
    }

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs