#include "CP_SDK_BS/UI/HMUIIconSegmentedControl.hpp"
#include "CP_SDK/Unity/Operators.hpp"

#include <HMUI/HoverHint.hpp>
#include <GlobalNamespace/BeatmapCharacteristicSegmentedControlController.hpp>
#include <GlobalNamespace/LocalizedHoverHint.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Resources.hpp>
#include <UnityEngine/Vector2.hpp>

using namespace UnityEngine;
using namespace GlobalNamespace;

namespace CP_SDK_BS::UI {

    CP_SDK::Utils::MonoPtr<HMUI::IconSegmentedControl> HMUIIconSegmentedControl::m_Template;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief When the game is reloaded
    void HMUIIconSegmentedControl::OnGameSoftReload()
    {
        m_Template = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create icon segmented control
    /// @param p_Parent             Parent game object transform
    /// @param p_HideCellBackground Should hide cell background
    /// @return GameObject
    HMUI::IconSegmentedControl* HMUIIconSegmentedControl::Create(RectTransform* p_Parent, bool p_HideCellBackground)
    {
        if (!m_Template)
        {
            m_Template = Resources::FindObjectsOfTypeAll<HMUI::IconSegmentedControl*>()->First([](auto x) -> bool {
                return x->get_name() == u"BeatmapCharacteristicSegmentedControl" && x->____container != nullptr;
            });
        }

        auto l_Control = GameObject::Instantiate(m_Template.Ptr(), p_Parent, false);
        l_Control->set_name(u"BSPIconSegmentedControl");
        l_Control->____container            = m_Template->____container;
        l_Control->____hideCellBackground   = p_HideCellBackground;

        auto l_RectTransform = l_Control->get_transform().try_cast<RectTransform>().value_or(nullptr);
        l_RectTransform->set_anchorMin       (Vector2::get_one() * 0.5f);
        l_RectTransform->set_anchorMax       (Vector2::get_one() * 0.5f);
        l_RectTransform->set_anchoredPosition(Vector2::get_zero()      );
        l_RectTransform->set_pivot           (Vector2::get_one() * 0.5f);

        auto l_ChildCount = l_Control->get_transform()->get_childCount();
        for (auto l_I = 0; l_I < l_ChildCount; ++l_I)
            GameObject::Destroy(l_Control->get_transform()->GetChild(l_I)->get_gameObject());

        GameObject::Destroy(l_Control->GetComponent<BeatmapCharacteristicSegmentedControlController*>());

        return l_Control;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set data and remove hover hints
    /// @param p_Instance Control instance
    /// @param p_Data     Data to set
    void HMUIIconSegmentedControl::SetDataNoHoverHint(HMUI::IconSegmentedControl* p_Instance, ::Array<HMUI::IconSegmentedControl::DataItem*>* p_Data)
    {
        try
        {
            p_Instance->SetData(p_Data);

            auto l_HoverHints        = p_Instance->GetComponentsInChildren<HMUI::HoverHint*>(true);
            auto l_LocalHoverHints   = p_Instance->GetComponentsInChildren<LocalizedHoverHint*>(true);

            for (auto l_Current : l_HoverHints)         GameObject::Destroy(l_Current);
            for (auto l_Current : l_LocalHoverHints)    GameObject::Destroy(l_Current);
        }
        catch (const std::exception&)
        {

        }
    }

}   ///< namespace CP_SDK_BS::UI