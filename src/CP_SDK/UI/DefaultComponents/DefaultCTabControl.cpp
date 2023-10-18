#include "CP_SDK/UI/DefaultComponents/DefaultCTabControl.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    enum class ERectTransformEdge
    {
        Left,
        Right,
        Top,
        Bottom
    };
    static void RectTransform_SetInsetAndSizeFromParentEdge(RectTransform* p_RTransform, ERectTransformEdge edge, float p_Inset, float p_Size)
    {
        #define SET_VECTOR2_AXIS(_Vec, _Axis, _Val) (_Axis == 0 ? _Vec.x = (_Val) : _Vec.y = (_Val))
        #define GET_VECTOR2_AXIS(_Vec, _Axis)       (_Axis == 0 ? _Vec.x          : _Vec.y)

        int l_Axis = ((edge == ERectTransformEdge::Top || edge == ERectTransformEdge::Bottom) ? 1 : 0);
        bool l_TopOrRight = edge == ERectTransformEdge::Top || edge == ERectTransformEdge::Right;
        float value = (l_TopOrRight ? 1 : 0);

        auto l_AnchorMin = p_RTransform->get_anchorMin();
        SET_VECTOR2_AXIS(l_AnchorMin, l_Axis, value);
        p_RTransform->set_anchorMin(l_AnchorMin);

        auto l_AnchorMax = p_RTransform->get_anchorMax();
        SET_VECTOR2_AXIS(l_AnchorMax, l_Axis, value);
        p_RTransform->set_anchorMax(l_AnchorMax);

        auto l_SizeDelta = p_RTransform->get_sizeDelta();
        SET_VECTOR2_AXIS(l_SizeDelta, l_Axis, p_Size);
        p_RTransform->set_sizeDelta(l_SizeDelta);

        auto l_AnchoredPosition = p_RTransform->get_anchoredPosition();
        auto l_Pivot            = p_RTransform->get_pivot();
        SET_VECTOR2_AXIS(l_AnchoredPosition, l_Axis, l_TopOrRight ? (0.0f - p_Inset - p_Size * (1.0f - GET_VECTOR2_AXIS(l_Pivot, l_Axis))) : (p_Inset + p_Size * GET_VECTOR2_AXIS(l_Pivot, l_Axis)));
        p_RTransform->set_anchoredPosition(l_AnchoredPosition);

        #undef SET_VECTOR2_AXIS
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCTabControl);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(DefaultCTabControl, Components::CTabControl)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,     m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,       m_LElement);

        m_OnActiveChanged   = {this, &DefaultCTabControl::OnActiveChanged_Impl};

        m_GetActiveTab      = {this, &DefaultCTabControl::GetActiveTab_Impl};

        m_SetActiveTab      = {this, &DefaultCTabControl::SetActiveTab_Impl};
        m_SetTabs           = {this, &DefaultCTabControl::SetTabs_Impl};

        Init += {this, &DefaultCTabControl::Init_DefaultCTabControl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(DefaultCTabControl, Components::CTabControl)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCTabControl::Init_DefaultCTabControl()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minHeight      (15.0f);
        m_LElement->set_flexibleWidth(1000.0f);
        m_LElement->set_flexibleHeight(1000.0f);

        m_TextSegmentedControl = UISystem::TextSegmentedControlFactory->Create(u"TextSegmentedControl", get_transform());
        m_TextSegmentedControl->OnActiveChanged({this, &DefaultCTabControl::TextSegmentedControl_OnActiveChanged});
        m_TextSegmentedControl->RTransform()->set_anchorMin         (Vector2(0.0f, 1.0f));
        m_TextSegmentedControl->RTransform()->set_anchorMax         (Vector2(1.0f, 1.0f));
        m_TextSegmentedControl->RTransform()->set_pivot             (Vector2(0.5f, 1.0f));
        m_TextSegmentedControl->RTransform()->set_sizeDelta         (Vector2::get_zero());
        m_TextSegmentedControl->RTransform()->set_anchoredPosition3D(Vector3::get_zero());

        m_Content = UISystem::VLayoutFactory->Create(u"Content", get_transform());
        m_Content->RTransform()->set_anchorMin               (Vector2::get_zero());
        m_Content->RTransform()->set_anchorMax               (Vector2::get_one());
        m_Content->RTransform()->set_pivot                   (Vector2( 0.5f,  0.5f));
        m_Content->RTransform()->set_sizeDelta               (Vector2( 0.0f, -7.0f));
        m_Content->RTransform()->set_localPosition           (Vector3( 0.0f, -3.5f, 0.0f));
        m_Content->CSizeFitter()->set_horizontalFit          (ContentSizeFitter::FitMode::Unconstrained);
        m_Content->CSizeFitter()->set_verticalFit            (ContentSizeFitter::FitMode::Unconstrained);
        m_Content->VLayoutGroup()->set_childForceExpandHeight(false);
        m_Content->SetBackground(true);
        m_Content->SetPadding(0);
        m_Content->SetSpacing(0);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On active text changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCTabControl::OnActiveChanged_Impl(_v::CActionRef<int> p_Functor, bool p_Add)
    {
        m_TextSegmentedControl->OnActiveChanged(p_Functor, p_Add);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get active tab
    int DefaultCTabControl::GetActiveTab_Impl()
    {
        return m_TextSegmentedControl->GetActiveText();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set active tab
    /// @param p_Index  New active index
    /// @param p_Notify Should notify?
    void DefaultCTabControl::SetActiveTab_Impl(int p_Index, bool p_Notify)
    {
        m_TextSegmentedControl->SetActiveText(p_Index, p_Notify);
    }
    /// @brief Set tabs
    /// @param p_Texts New tabs
    void DefaultCTabControl::SetTabs_Impl(const std::vector<t_TabDefinition>& p_Tabs)
    {
        if (!m_Tabs.empty())
            throw std::runtime_error("Tabs already set!");

        auto l_Texts = std::vector<std::u16string>(m_Tabs.size());
        for (auto l_I = 0; l_I < p_Tabs.size(); ++l_I)
        {
            auto l_Tab = std::get<1>(p_Tabs[l_I]);
            l_Tab->get_gameObject()->SetActive(false);
            l_Tab->SetParent(m_Content->RTransform().Ptr(), false);
            l_Tab->set_localPosition   (Vector3::get_zero());
            l_Tab->set_localScale      (Vector3::get_one());
            l_Tab->set_localEulerAngles(Vector3::get_zero());
            RectTransform_SetInsetAndSizeFromParentEdge(l_Tab, ERectTransformEdge::Bottom, 0, 0);
            RectTransform_SetInsetAndSizeFromParentEdge(l_Tab, ERectTransformEdge::Top,    0, 0);
            RectTransform_SetInsetAndSizeFromParentEdge(l_Tab, ERectTransformEdge::Left,   0, 0);
            RectTransform_SetInsetAndSizeFromParentEdge(l_Tab, ERectTransformEdge::Right,  0, 0);
            l_Tab->set_anchorMin       (Vector2::get_zero());
            l_Tab->set_anchorMax       (Vector2::get_one());

            l_Texts.push_back(std::get<0>(p_Tabs[l_I]));
        }

        m_Tabs = p_Tabs;

        m_TextSegmentedControl->SetTexts(l_Texts);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On active text changed
    /// @param p_Index Index of the text
    void DefaultCTabControl::TextSegmentedControl_OnActiveChanged(int p_Index)
    {
        for (auto l_I = 0; l_I < m_Tabs.size(); ++l_I)
            std::get<1>(m_Tabs[l_I])->get_gameObject()->SetActive(l_I == p_Index);
    }

}   ///< namespace CP_SDK::UI::DefaultComponents