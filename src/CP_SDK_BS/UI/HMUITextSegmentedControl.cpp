#include "CP_SDK_BS/UI/HMUITextSegmentedControl.hpp"
#include "CP_SDK/Unity/Operators.hpp"

#include <HMUI/HoverHint.hpp>
#include <GlobalNamespace/BeatmapDifficultySegmentedControlController.hpp>
#include <GlobalNamespace/LocalizedHoverHint.hpp>
#include <System/Collections/ObjectModel/ReadOnlyCollection_1.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Resources.hpp>
#include <UnityEngine/Vector2.hpp>

using namespace GlobalNamespace;
using namespace UnityEngine;

namespace CP_SDK_BS::UI {

    CP_SDK::Utils::MonoPtr<HMUI::TextSegmentedControl> HMUITextSegmentedControl::m_Template;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create icon segmented control
    /// @param p_Parent             Parent game object transform
    /// @param p_HideCellBackground Should hide cell background
    /// @param p_Texts              Texts
    /// @return GameObject
    HMUI::TextSegmentedControl* HMUITextSegmentedControl::Create(RectTransform* p_Parent, bool p_HideCellBackground, const std::vector<std::u16string_view>& p_Texts)
    {
        if (!m_Template)
        {
            m_Template = Resources::FindObjectsOfTypeAll<HMUI::TextSegmentedControl*>()->First([](auto x) -> bool {
                return x->get_name() == u"BeatmapDifficultySegmentedControl" && x->_container != nullptr;
            });
        }

        auto l_Control = GameObject::Instantiate(m_Template.Ptr(), p_Parent, false);
        l_Control->set_name(u"BSPTextSegmentedControl");
        l_Control->_container            = m_Template->_container;
        l_Control->_hideCellBackground   = p_HideCellBackground;

        auto l_RectTransform = l_Control->get_transform().try_cast<RectTransform>().value_or(nullptr);
        l_RectTransform->set_anchorMin       (Vector2::get_one() * 0.5f);
        l_RectTransform->set_anchorMax       (Vector2::get_one() * 0.5f);
        l_RectTransform->set_anchoredPosition(Vector2::get_zero()      );
        l_RectTransform->set_pivot           (Vector2::get_one() * 0.5f);

        auto l_ChildCount = l_Control->get_transform()->get_childCount();
        for (auto l_I = 0; l_I < l_ChildCount; ++l_I)
            GameObject::Destroy(l_Control->get_transform()->GetChild(l_I)->get_gameObject());

        GameObject::Destroy(l_Control->GetComponent<BeatmapDifficultySegmentedControlController*>());

        auto l_List = System::Collections::Generic::List_1<StringW>::New_ctor();
        if (p_Texts.empty())
            l_List->Add(u"Tab");
        else
        {
            for (auto& l_Current : p_Texts)
                l_List->Add(l_Current);
        }

        l_Control->SetTexts(l_List->AsReadOnly()->i___System__Collections__Generic__IReadOnlyList_1_T_());

        return l_Control;
    }

}   ///< namespace CP_SDK_BS::UI