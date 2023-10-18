#include "CP_SDK/UI/Components/CFLayout.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CFLayout);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CFLayout)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CFLayout)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set preferred width
    /// @param p_Width Width
    CFLayout* CFLayout::SetWidth(float p_Width)
    {
        RTransform()->set_sizeDelta(Vector2(p_Width, RTransform()->get_sizeDelta().y));
        LElement()->set_preferredWidth(p_Width);
        return this;
    }
    /// @brief Set preferred height
    /// @param p_Height Height
    CFLayout* CFLayout::SetHeight(float p_Height)
    {
        RTransform()->set_sizeDelta(Vector2(RTransform()->get_sizeDelta().x, p_Height));
        LElement()->set_preferredHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set min width
    /// @param p_Width Width
    CFLayout* CFLayout::SetMinWidth(float p_Width)
    {
        LElement()->set_minWidth(p_Width);
        return this;
    }
    /// @brief Set min height
    /// @param p_Height Height
    CFLayout* CFLayout::SetMinHeight(float p_Height)
    {
        LElement()->set_minHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set child alignment
    /// @param p_ChildAlign New alignment
    CFLayout* CFLayout::SetChildAlign(TextAnchor p_ChildAlign)
    {
        set_childAlignment(p_ChildAlign);
        return this;
    }
    /// @brief Set spacing between elements
    /// @param p_Spacing New spacing
    CFLayout* CFLayout::SetSpacing(Vector2 p_Spacing)
    {
        SetSpacingX(p_Spacing.x);
        SetSpacingY(p_Spacing.y);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void CFLayout::LayoutGroup__CalculateLayoutInputHorizontal()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::LayoutGroup__CalculateLayoutInputHorizontal");
    }
    void CFLayout::ILayoutElement__CalculateLayoutInputHorizontal()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::ILayoutElement__CalculateLayoutInputHorizontal");
    }
    void CFLayout::LayoutGroup__CalculateLayoutInputVertical()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::LayoutGroup__CalculateLayoutInputVertical");
    }
    void CFLayout::ILayoutElement__CalculateLayoutInputVertical()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::ILayoutElement__CalculateLayoutInputVertical");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void CFLayout::LayoutGroup__SetLayoutHorizontal()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::LayoutGroup__SetLayoutHorizontal");
    }
    void CFLayout::ILayoutController__SetLayoutHorizontal()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::LayoutGroup__SetLayoutHorizontal");
    }
    void CFLayout::LayoutGroup__SetLayoutVertical()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::LayoutGroup__SetLayoutVertical");
    }
    void CFLayout::ILayoutController__SetLayoutVertical()
    {
        ChatPlexSDK::Logger()->Error(u"SHOULD NOT BE CALLED CFLayout::ILayoutController__SetLayoutVertical");
    }

}   ///< namespace CP_SDK::UI::Components