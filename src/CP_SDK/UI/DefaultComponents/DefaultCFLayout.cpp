#include "CP_SDK/UI/DefaultComponents/DefaultCFLayout.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/LayoutUtility.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCFLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(DefaultCFLayout)
    {
        m_StartAxis                 = EAxis::Horizontal;
        m_ChildForceExpandWidth     = false;
        m_ChildForceExpandHeight    = false;
        m_ExpandHorizontalSpacing   = false;
        m_SpacingX                  = 0.5f;
        m_SpacingY                  = 0.5f;
        m_InvertOrder               = false;
        m_LayoutWidth               = 0.0f;
        m_LayoutHeight              = 0.0f;

        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,     m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,       m_LElement);

        GetStartAxis                = [this]() { return m_StartAxis;                };
        GetChildForceExpandWidth    = [this]() { return m_ChildForceExpandWidth;    };
        GetChildForceExpandHeight   = [this]() { return m_ChildForceExpandHeight;   };
        GetExpandHorizontalSpacing  = [this]() { return m_ExpandHorizontalSpacing;  };
        GetSpacingX                 = [this]() { return m_SpacingX;                 };
        GetSpacingY                 = [this]() { return m_SpacingY;                 };
        GetInvertOrder              = [this]() { return m_InvertOrder;              };

        SetStartAxis                = [this](EAxis value) { SetPropertyCustom(m_StartAxis,               value); };
        SetChildForceExpandWidth    = [this](bool  value) { SetPropertyCustom(m_ChildForceExpandWidth,   value); };
        SetChildForceExpandHeight   = [this](bool  value) { SetPropertyCustom(m_ChildForceExpandHeight,  value); };
        SetExpandHorizontalSpacing  = [this](bool  value) { SetPropertyCustom(m_ExpandHorizontalSpacing, value); };
        SetSpacingX                 = [this](float value) { SetPropertyCustom(m_SpacingX,                value); };
        SetSpacingY                 = [this](float value) { SetPropertyCustom(m_SpacingY,                value); };
        SetInvertOrder              = [this](bool  value) { SetPropertyCustom(m_InvertOrder,             value); };

        Init += [this]() { Init_DefaultCFLayout(); };
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(DefaultCFLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    bool DefaultCFLayout::IsCenterAlign()
    {
        auto l_ChildAlignment = get_childAlignment();
        return l_ChildAlignment == TextAnchor::LowerCenter || l_ChildAlignment == TextAnchor::MiddleCenter || l_ChildAlignment == TextAnchor::UpperCenter;
    }
    bool DefaultCFLayout::IsRightAlign()
    {
        auto l_ChildAlignment = get_childAlignment();
        return l_ChildAlignment == TextAnchor::LowerRight  || l_ChildAlignment == TextAnchor::MiddleRight  || l_ChildAlignment == TextAnchor::UpperRight;
    }
    bool DefaultCFLayout::IsMiddleAlign()
    {
        auto l_ChildAlignment = get_childAlignment();
        return l_ChildAlignment == TextAnchor::MiddleLeft  || l_ChildAlignment == TextAnchor::MiddleRight  || l_ChildAlignment == TextAnchor::MiddleCenter;
    }
    bool DefaultCFLayout::IsLowerAlign()
    {
        auto l_ChildAlignment = get_childAlignment();
        return l_ChildAlignment == TextAnchor::LowerLeft   || l_ChildAlignment == TextAnchor::LowerRight   || l_ChildAlignment == TextAnchor::LowerCenter;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCFLayout::Init_DefaultCFLayout()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());
        m_RTransform->set_anchorMin(Vector2(0.0f, 0.0f));
        m_RTransform->set_anchorMax(Vector2(1.0f, 1.0f));
        m_RTransform->set_sizeDelta(Vector2(0.0f, 0.0f));

        set_childAlignment(TextAnchor::MiddleCenter);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_flexibleWidth(1.0f);
        m_LElement->set_flexibleHeight(1.0f);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Calculate the layout input for horizontal axis
    void DefaultCFLayout::LayoutGroup__CalculateLayoutInputHorizontal()
    {
        ILayoutElement__CalculateLayoutInputHorizontal();
    }
    void DefaultCFLayout::ILayoutElement__CalculateLayoutInputHorizontal()
    {
        if (m_StartAxis == EAxis::Horizontal)
        {
            CP_SDK_IL2CPP_CALL_BASE_METHOD(LayoutGroup, "CalculateLayoutInputHorizontal");

            auto l_Padding  = get_padding();
            auto l_MinWidth = GetGreatestMinimumChildWidth() + l_Padding->get_left() + l_Padding->get_right();
            SetLayoutInputForAxis(l_MinWidth, -1, -1, 0);
        }
        else
            m_LayoutWidth = SetLayout(0, true);
    }
    /// @brief Calculate the layout input for vertical axis
    void DefaultCFLayout::LayoutGroup__CalculateLayoutInputVertical()
    {
        ILayoutElement__CalculateLayoutInputVertical();
    }
    void DefaultCFLayout::ILayoutElement__CalculateLayoutInputVertical()
    {
        if (m_StartAxis == EAxis::Horizontal)
            m_LayoutHeight = SetLayout(1, true);
        else
        {
            CP_SDK_IL2CPP_CALL_BASE_METHOD(LayoutGroup, "CalculateLayoutInputHorizontal");

            auto l_Padding   = get_padding();
            auto l_MinHeight = GetGreatestMinimumChildHeigth() + l_Padding->get_bottom() + l_Padding->get_top();
            SetLayoutInputForAxis(l_MinHeight, -1, -1, 1);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set layout horizontal
    void DefaultCFLayout::LayoutGroup__SetLayoutHorizontal()
    {
        ILayoutController__SetLayoutHorizontal();
    }
    void DefaultCFLayout::ILayoutController__SetLayoutHorizontal()
    {
        SetLayout(0, false);
    }
    /// @brief Set layout vertical
    void DefaultCFLayout::LayoutGroup__SetLayoutVertical()
    {
        ILayoutController__SetLayoutVertical();
    }
    void DefaultCFLayout::ILayoutController__SetLayoutVertical()
    {
        SetLayout(1, false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get greatest minimum child width
    float DefaultCFLayout::GetGreatestMinimumChildWidth()
    {
        auto l_RectChildrens    = get_rectChildren();
        auto l_Max              = 0.0f;

        for (auto l_I = 0; l_I < l_RectChildrens->get_Count(); ++l_I)
            l_Max = std::max<float>(LayoutUtility::GetMinWidth(l_RectChildrens->get_Item(l_I)), l_Max);

        return l_Max;
    }
    /// @brief Get greatest minimum child height
    float DefaultCFLayout::GetGreatestMinimumChildHeigth()
    {
        auto l_RectChildrens    = get_rectChildren();
        auto l_Max              = 0.0f;

        for (auto l_I = 0; l_I < l_RectChildrens->get_Count(); ++l_I)
            l_Max = std::max<float>(LayoutUtility::GetMinHeight(l_RectChildrens->get_Item(l_I)), l_Max);

        return l_Max;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Main layout method
    /// @param p_Axis 0 for horizontal axis, 1 for vertical
    /// @param p_LayoutInput If true, sets the layout input for the axis. If false, sets child position for axis
    float DefaultCFLayout::SetLayout(int p_Axis, bool p_LayoutInput)
    {
        auto l_GroupHeight  = get_rectTransform()->get_rect().m_Height;
        auto l_GroupWidth   = get_rectTransform()->get_rect().m_Width;
        auto l_Padding      = get_padding();

        float l_SpacingBetweenBars      = 0;
        float l_SpacingBetweenElements  = 0;
        float l_Offset                  = 0;
        float l_CounterOffset           = 0;
        float l_GroupSize               = 0;
        float l_WorkingSize             = 0;

        if (m_StartAxis == EAxis::Horizontal)
        {
            l_GroupSize                 = l_GroupHeight;
            l_WorkingSize               = l_GroupWidth - l_Padding->get_left() - l_Padding->get_right();
            l_Offset                    = IsLowerAlign() ? (float)l_Padding->get_bottom() : (float)l_Padding->get_top();
            l_CounterOffset             = IsLowerAlign() ? (float)l_Padding->get_top()    : (float)l_Padding->get_bottom();
            l_SpacingBetweenBars        = m_SpacingY;
            l_SpacingBetweenElements    = m_SpacingX;
        }
        else if (m_StartAxis == EAxis::Vertical)
        {
            l_GroupSize                 = l_GroupWidth;
            l_WorkingSize               = l_GroupHeight - l_Padding->get_top() - l_Padding->get_bottom();
            l_Offset                    = IsRightAlign() ? (float)l_Padding->get_right() : (float)l_Padding->get_left();
            l_CounterOffset             = IsRightAlign() ? (float)l_Padding->get_left()  : (float)l_Padding->get_right();
            l_SpacingBetweenBars        = m_SpacingX;
            l_SpacingBetweenElements    = m_SpacingY;
        }

        auto l_CurrentBarSize  = 0.0f;
        auto l_CurrentBarSpace = 0.0f;

        auto l_RectChildrens = get_rectChildren();
        for (auto l_I = 0; l_I < l_RectChildrens->get_Count(); ++l_I)
        {
            auto    l_Index             = l_I;
            auto    l_Child             = l_RectChildrens->get_Item(l_Index);
            float   l_ChildSize         = 0;
            float   l_ChildOtherSize    = 0;

            if (m_StartAxis == EAxis::Horizontal)
            {
                if (m_InvertOrder)
                    l_Index = IsLowerAlign() ? l_RectChildrens->get_Count() - 1 - l_I : l_I;

                l_Child             = l_RectChildrens->get_Item(l_Index);
                l_ChildSize         = LayoutUtility::GetPreferredSize(l_Child, 0);
                l_ChildSize         = std::min<float>(l_ChildSize, l_WorkingSize);
                l_ChildOtherSize    = LayoutUtility::GetPreferredSize(l_Child, 1);
                l_ChildOtherSize    = std::min<float>(l_ChildOtherSize, l_WorkingSize);
            }
            else if (m_StartAxis == EAxis::Vertical)
            {
                if (m_InvertOrder)
                    l_Index = IsRightAlign() ? l_RectChildrens->get_Count() - 1 - l_I : l_I;

                l_Child             = l_RectChildrens->get_Item(l_Index);
                l_ChildSize         = LayoutUtility::GetPreferredSize(l_Child, 1);
                l_ChildSize         = std::min<float>(l_ChildSize, l_WorkingSize);
                l_ChildOtherSize    = LayoutUtility::GetPreferredSize(l_Child, 0);
                l_ChildOtherSize    = std::min<float>(l_ChildOtherSize, l_WorkingSize);
            }

            /// If adding this element would exceed the bounds of the container, go to a new bar after processing the current bar
            if (l_CurrentBarSize + l_ChildSize > l_WorkingSize)
            {
                l_CurrentBarSize -= l_SpacingBetweenElements;
                if (!p_LayoutInput) /// Process current bar elements positioning
                {
                    if (m_StartAxis == EAxis::Horizontal)
                    {
                        float newOffset = CalculateRowVerticalOffset(l_GroupSize, l_Offset, l_CurrentBarSpace);
                        LayoutRow(l_CurrentBarSize, l_CurrentBarSpace, l_WorkingSize, l_Padding->get_left(), newOffset, p_Axis);
                    }
                    else if (m_StartAxis == EAxis::Vertical)
                    {
                        float newOffset = CalculateColHorizontalOffset(l_GroupSize, l_Offset, l_CurrentBarSpace);
                        LayoutCol(l_CurrentBarSpace, l_CurrentBarSize, l_WorkingSize, newOffset, l_Padding->get_top(), p_Axis);
                    }
                }

                m_ItemList.clear();

                /// Add the current bar space to total barSpace accumulator, and reset to 0 for the next row
                l_Offset += l_CurrentBarSpace;
                l_Offset += l_SpacingBetweenBars;

                l_CurrentBarSpace = 0;
                l_CurrentBarSize  = 0;
            }

            l_CurrentBarSize += l_ChildSize;
            m_ItemList.push_back(l_Child);

            /// We need the largest element height to determine the starting position of the next line
            if (l_ChildOtherSize > l_CurrentBarSpace)   l_CurrentBarSpace = l_ChildOtherSize;
            /// Don't do this for the last one
            if (l_I < l_RectChildrens->get_Count() - 1) l_CurrentBarSize += l_SpacingBetweenElements;
        }

        if (!p_LayoutInput) /// Layout the final bar
        {
            if (m_StartAxis == EAxis::Horizontal)
            {
                float l_NewOffset = CalculateRowVerticalOffset(l_GroupHeight, l_Offset, l_CurrentBarSpace);
                l_CurrentBarSize -= l_SpacingBetweenElements;
                LayoutRow(l_CurrentBarSize, l_CurrentBarSpace, l_WorkingSize - (m_ChildForceExpandWidth ? 0 : l_SpacingBetweenElements), l_Padding->get_left(), l_NewOffset, p_Axis);
            }
            else if (m_StartAxis == EAxis::Vertical)
            {
                float l_NewOffset = CalculateColHorizontalOffset(l_GroupWidth, l_Offset, l_CurrentBarSpace);
                l_CurrentBarSize -= l_SpacingBetweenElements;
                LayoutCol(l_CurrentBarSpace, l_CurrentBarSize, l_WorkingSize - (m_ChildForceExpandHeight ? 0 : l_SpacingBetweenElements), l_NewOffset, l_Padding->get_top(), p_Axis);
            }
        }

        m_ItemList.clear();

        /// Add the last bar space to the barSpace accumulator
        l_Offset += l_CurrentBarSpace;
        l_Offset += l_CounterOffset;

        if (p_LayoutInput) SetLayoutInputForAxis(l_Offset, l_Offset, -1, p_Axis);

        return l_Offset;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Calculate row vertical offset
    /// @param p_GroupHeight      Group height
    /// @param p_YOffset          Y offset
    /// @param p_CurrentRowHeight Current row height
    float DefaultCFLayout::CalculateRowVerticalOffset(float p_GroupHeight, float p_YOffset, float p_CurrentRowHeight)
    {
             if (IsLowerAlign())  return p_GroupHeight - p_YOffset - p_CurrentRowHeight;
        else if (IsMiddleAlign()) return p_GroupHeight * 0.5f - m_LayoutHeight * 0.5f + p_YOffset;

        return p_YOffset;
    }
    /// @brief Calculate column horizontal offset
    /// @param p_GroupWidth      Group width
    /// @param p_XOffset         X Offset
    /// @param p_CurrentColWidth Current column width
    float DefaultCFLayout::CalculateColHorizontalOffset(float p_GroupWidth, float p_XOffset, float p_CurrentColWidth)
    {
             if (IsRightAlign())  return p_GroupWidth - p_XOffset - p_CurrentColWidth;
        else if (IsCenterAlign()) return p_GroupWidth * 0.5f - m_LayoutWidth * 0.5f + p_XOffset;

        return p_XOffset;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Layout row
    /// @param p_RowWidth  Row width
    /// @param p_RowHeight Row height
    /// @param p_MaxWidth  Max width
    /// @param p_XOffset   X offset
    /// @param p_YOffset   Y offset
    /// @param p_Axis      Axis
    void DefaultCFLayout::LayoutRow(float p_RowWidth, float p_RowHeight, float p_MaxWidth, float p_XOffset, float p_YOffset, int p_Axis)
    {
        auto l_XPos = p_XOffset;

             if (!m_ChildForceExpandWidth && IsCenterAlign()) l_XPos += (p_MaxWidth - p_RowWidth) * 0.5f;
        else if (!m_ChildForceExpandWidth && IsRightAlign())  l_XPos += (p_MaxWidth - p_RowWidth);

        auto l_ExtraWidth   = 0.0f;
        auto l_ExtraSpacing = 0.0f;

             if (m_ChildForceExpandWidth) l_ExtraWidth = (p_MaxWidth - p_RowWidth) / m_ItemList.size();
        else if (m_ExpandHorizontalSpacing)
        {
            l_ExtraSpacing = (p_MaxWidth - p_RowWidth) / (m_ItemList.size() - 1);
            if (m_ItemList.size() > 1)
            {
                     if (IsCenterAlign()) l_XPos -= l_ExtraSpacing * 0.5f * (m_ItemList.size() - 1);
                else if (IsRightAlign())  l_XPos -= l_ExtraSpacing * (m_ItemList.size() - 1);
            }
        }

        for (auto l_J = 0; l_J < m_ItemList.size(); ++l_J)
        {
            auto l_Index           = IsLowerAlign() ? m_ItemList.size() - 1 - l_J : l_J;
            auto& l_RowChild       = m_ItemList[l_Index];
            auto l_RowChildWidth   = LayoutUtility::GetPreferredSize(l_RowChild.Ptr(), 0) + l_ExtraWidth;
            auto l_RowChildHeight  = LayoutUtility::GetPreferredSize(l_RowChild.Ptr(), 1);

            if (m_ChildForceExpandHeight) l_RowChildHeight = p_RowHeight;

            l_RowChildWidth = std::min<float>(l_RowChildWidth, p_MaxWidth);

            auto l_YPos = p_YOffset;

                 if (IsMiddleAlign()) l_YPos += (p_RowHeight - l_RowChildHeight) * 0.5f;
            else if (IsLowerAlign())  l_YPos += (p_RowHeight - l_RowChildHeight);

            if (m_ExpandHorizontalSpacing && l_J > 0)
                l_XPos += l_ExtraSpacing;

            if (p_Axis == 0) SetChildAlongAxis(l_RowChild.Ptr(), 0, l_XPos, l_RowChildWidth);
            else             SetChildAlongAxis(l_RowChild.Ptr(), 1, l_YPos, l_RowChildHeight);

            /// Don't do horizontal spacing for the last one
            if (l_J < m_ItemList.size() - 1) l_XPos += l_RowChildWidth + m_SpacingX;
        }
    }
    /// @brief Layout column
    /// @param p_ColWidth  Column width
    /// @param p_ColHeight Column height
    /// @param p_MaxHeight Max height
    /// @param p_XOffset   X offset
    /// @param p_YOffset   Y offset
    /// @param p_Axis      Axis
    void DefaultCFLayout::LayoutCol(float p_ColWidth, float p_ColHeight, float p_MaxHeight, float p_XOffset, float p_YOffset, int p_Axis)
    {
        auto l_YPos = p_YOffset;

             if (!m_ChildForceExpandHeight && IsMiddleAlign()) l_YPos += (p_MaxHeight - p_ColHeight) * 0.5f;
        else if (!m_ChildForceExpandHeight && IsLowerAlign())  l_YPos += (p_MaxHeight - p_ColHeight);

        auto l_ExtraHeight  = 0.0f;
        auto l_ExtraSpacing = 0.0f;

             if (m_ChildForceExpandHeight) l_ExtraHeight = (p_MaxHeight - p_ColHeight) / m_ItemList.size();
        else if (m_ExpandHorizontalSpacing)
        {
            l_ExtraSpacing = (p_MaxHeight - p_ColHeight) / (m_ItemList.size() - 1);
            if (m_ItemList.size() > 1)
            {
                     if (IsMiddleAlign()) l_YPos -= l_ExtraSpacing * 0.5f * (m_ItemList.size() - 1);
                else if (IsLowerAlign())  l_YPos -= l_ExtraSpacing * (m_ItemList.size() - 1);
            }
        }

        for (auto l_J = 0; l_J < m_ItemList.size(); ++l_J)
        {
            auto l_Index             = IsRightAlign() ? m_ItemList.size() - 1 - l_J : l_J;
            auto& l_RowChild         = m_ItemList[l_Index];
            auto l_RowChildWidth     = LayoutUtility::GetPreferredSize(l_RowChild.Ptr(), 0);
            auto l_RowChildHeight    = LayoutUtility::GetPreferredSize(l_RowChild.Ptr(), 1) + l_ExtraHeight;

            if (m_ChildForceExpandWidth)
                l_RowChildWidth = p_ColWidth;

            l_RowChildHeight = std::min<float>(l_RowChildHeight, p_MaxHeight);

            auto l_XPos = p_XOffset;

                 if (IsCenterAlign()) l_XPos += (p_ColWidth - l_RowChildWidth) * 0.5f;
            else if (IsRightAlign())  l_XPos += (p_ColWidth - l_RowChildWidth);

            if (m_ExpandHorizontalSpacing && l_J > 0)
                l_YPos += l_ExtraSpacing;

            if (p_Axis == 0) SetChildAlongAxis(l_RowChild.Ptr(), 0, l_XPos, l_RowChildWidth);
            else             SetChildAlongAxis(l_RowChild.Ptr(), 1, l_YPos, l_RowChildHeight);

            /// Don't do vertical spacing for the last one
            if (l_J < m_ItemList.size() - 1) l_YPos += l_RowChildHeight + m_SpacingY;
        }
    }

}   ///< namespace CP_SDK::UI::DefaultComponents