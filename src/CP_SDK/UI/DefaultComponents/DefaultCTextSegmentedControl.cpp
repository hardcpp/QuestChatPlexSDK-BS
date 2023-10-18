#include "CP_SDK/UI/DefaultComponents/DefaultCTextSegmentedControl.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/UI/Button_ButtonClickedEvent.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents {

    CP_SDK_IL2CPP_INHERIT_INIT(DefaultCTextSegmentedControl);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(DefaultCTextSegmentedControl, Components::CTextSegmentedControl)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,     m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(CSizeFitter,    m_CSizeFitter);
        CP_SDK_UI_IL2CPP_BIND_FIELD(LElement,       m_LElement);

        m_ActiveControl = 0;

        m_OnActiveChanged   = {this, &DefaultCTextSegmentedControl::OnActiveChanged_Impl};

        m_GetActiveText     = {this, &DefaultCTextSegmentedControl::GetActiveText_Impl};
        m_GetTextCount      = {this, &DefaultCTextSegmentedControl::GetTextCount_Impl};

        m_SetActiveText     = {this, &DefaultCTextSegmentedControl::SetActiveText_Impl};
        m_SetTexts          = {this, &DefaultCTextSegmentedControl::SetTexts_Impl};

        Init += {this, &DefaultCTextSegmentedControl::Init_DefaultCTextSegmentedControl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(DefaultCTextSegmentedControl, Components::CTextSegmentedControl)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void DefaultCTextSegmentedControl::Init_DefaultCTextSegmentedControl()
    {
        if (m_RTransform)
            return;

        get_gameObject()->set_layer(UISystem::UILayer);

        m_RTransform = reinterpret_cast<RectTransform*>(get_transform());

        m_CSizeFitter = get_gameObject()->AddComponent<ContentSizeFitter*>();
        m_CSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
        m_CSizeFitter->set_verticalFit  (ContentSizeFitter::FitMode::PreferredSize);

        m_LElement = get_gameObject()->AddComponent<LayoutElement*>();
        m_LElement->set_minHeight      (5.0f);
        m_LElement->set_preferredHeight(5.0f);

        auto l_Layout = get_gameObject()->AddComponent<HorizontalLayoutGroup*>();
        l_Layout->set_childControlHeight(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On active text changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    void DefaultCTextSegmentedControl::OnActiveChanged_Impl(_v::CActionRef<int> p_Functor, bool p_Add)
    {
        if (p_Add)  m_OnActiveChangedEvent += p_Functor;
        else        m_OnActiveChangedEvent -= p_Functor;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get active text
    int DefaultCTextSegmentedControl::GetActiveText_Impl()
    {
        return m_ActiveControl;
    }
    /// @brief Get text count
    int DefaultCTextSegmentedControl::GetTextCount_Impl()
    {
        return m_Controls.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set active text
    /// @param p_Index  New active index
    /// @param p_Notify Should notify?
    void DefaultCTextSegmentedControl::SetActiveText_Impl(int p_Index, bool p_Notify)
    {
        if (p_Index < 0 || p_Index == m_ActiveControl || p_Index >= m_Controls.size())
            return;

        OnControlClicked(m_Controls[p_Index].Ptr(), p_Notify);
    }
    /// @brief Set texts
    /// @param p_Texts New texts
    void DefaultCTextSegmentedControl::SetTexts_Impl(const std::vector<std::u16string>& p_Texts)
    {
        if (p_Texts.size() == 0)
        {
            SetTexts_Impl({ u"Default "});
            return;
        }

        get_gameObject()->SetActive(false);

        m_Controls.clear();
        m_ActiveControl = 0;

        auto l_ChildCount = get_transform()->GetChildCount();
        for (auto l_I = l_ChildCount - 1; l_I >= 0; --l_I)
            GameObject::Destroy(get_transform()->GetChild(l_I)->get_gameObject());

        for (auto l_I = 0; l_I < p_Texts.size(); ++l_I)
        {
            auto l_Control = GameObject::New_ctor(u"Tab" + std::to_string(l_I), ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)) }))->GetComponent<RectTransform*>();
            l_Control->get_gameObject()->set_layer(UISystem::UILayer);
            l_Control->SetParent(get_transform(), false);
            l_Control->set_sizeDelta(Vector2(0.0f, 5.0f));

            auto l_Background = reinterpret_cast<Image*>(GameObject::New_ctor("BG", ArrayW<System::Type*>({ UISystem::Override_UnityComponent_Image.ptr() }))->GetComponent(UISystem::Override_UnityComponent_Image.ptr()));
            l_Background->get_gameObject()->set_layer(UISystem::UILayer);
            l_Background->get_rectTransform()->SetParent(l_Control->get_transform(), false);
            l_Background->get_rectTransform()->set_anchorMin       (Vector2::get_zero());
            l_Background->get_rectTransform()->set_anchorMax       (Vector2::get_one());
            l_Background->get_rectTransform()->set_sizeDelta       (Vector2::get_zero());
            l_Background->get_rectTransform()->set_anchoredPosition(Vector2::get_zero());
            l_Background->set_material               (UISystem::Override_GetUIMaterial());
            l_Background->set_type                   (Image::Type::Sliced);
            l_Background->set_pixelsPerUnitMultiplier(1.0f);
            l_Background->set_color                  (ColorU::ToUnityColor("#727272"));

            if (p_Texts.size() == 1)
                l_Background->set_sprite(UISystem::GetUIRoundBGSprite().Ptr());
            else if (l_I == 0)
                l_Background->set_sprite(UISystem::GetUIRoundRectLeftBGSprite().Ptr());
            else if (l_I == (p_Texts.size() - 1))
                l_Background->set_sprite(UISystem::GetUIRoundRectRightBGSprite().Ptr());
            else
                l_Background->set_sprite(UISystem::GetUISliderBGSprite().Ptr());

            auto l_Label = UISystem::TextFactory->Create(u"Label", l_Control->get_transform());
            l_Label->RTransform()->set_anchorMin       (Vector2::get_zero());
            l_Label->RTransform()->set_anchorMax       (Vector2::get_one());
            l_Label->RTransform()->set_sizeDelta       (Vector2::get_zero());
            l_Label->RTransform()->set_anchoredPosition(Vector2::get_zero());
            l_Label->SetText(p_Texts[l_I]);
            l_Label->SetAlign(TMPro::TextAlignmentOptions::Midline);
            l_Label->SetStyle(TMPro::FontStyles::Bold);

            auto l_Button = l_Control->get_gameObject()->AddComponent<Button*>();
            l_Button->set_targetGraphic(l_Background);
            l_Button->get_onClick()->AddListener(MakeUnityAction([=]() -> void {
                OnControlClicked(l_Button, true);
            }));

            auto l_Colors = l_Button->get_colors();
            l_Colors.set_normalColor(ColorU::WithAlpha(Color::get_white(), l_I == m_ActiveControl ? 0.75f : 0.25f));
            l_Button->set_colors(l_Colors);

            m_Controls.push_back(l_Button);
        }

        get_gameObject()->SetActive(true);

        try { m_OnActiveChangedEvent(m_ActiveControl); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCTextSegmentedControl.SetTexts_Impl] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On control clicked
    /// @param p_Button Clicked control's button
    /// @param p_Notify Should notify callback?
    void DefaultCTextSegmentedControl::OnControlClicked(Button* p_Button, bool p_Notify)
    {
        auto l_Index = -1;
        for (auto l_I = 0; l_I < m_Controls.size(); ++l_I)
        {
            if (m_Controls[l_I].Ptr(false) != p_Button)
                continue;

            l_Index = l_I;
            break;
        }

        if (l_Index == -1 || l_Index == m_ActiveControl)
            return;

        for (auto l_I = 0; l_I < m_Controls.size(); ++l_I)
        {
            auto l_Colors = m_Controls[l_I]->get_colors();
            l_Colors.set_normalColor(ColorU::WithAlpha(Color::get_white(), l_I == l_Index ? 0.75f : 0.25f));
            m_Controls[l_I]->set_colors(l_Colors);
        }

        m_ActiveControl = l_Index;

        if (p_Notify)
        {
            try { m_OnActiveChangedEvent(l_Index); }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI.DefaultComponents][DefaultCTextSegmentedControl.OnControlClicked] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
    }

}   ///< namespace CP_SDK::UI::DefaultComponents