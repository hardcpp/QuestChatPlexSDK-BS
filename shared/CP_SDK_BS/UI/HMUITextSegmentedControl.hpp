#pragma once

#include "CP_SDK/Utils/Il2cpp.hpp"
#include "CP_SDK/Utils/MonoPtr.hpp"

#include <HMUI/TextSegmentedControl.hpp>
#include <UnityEngine/RectTransform.hpp>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Text segmented control
    class CP_SDK_EXPORT_VISIBILITY HMUITextSegmentedControl
    {
        CP_SDK_NO_DEF_CTORS(HMUITextSegmentedControl);

        private:
            static CP_SDK::Utils::MonoPtr<HMUI::TextSegmentedControl> m_Template;

        public:
            /// @brief Create text segmented control
            /// @param p_Parent             Parent game object transform
            /// @param p_HideCellBackground Should hide cell background
            /// @param p_Texts              Texts
            /// @return GameObject
            static HMUI::TextSegmentedControl* Create(_u::RectTransform* p_Parent, bool p_HideCellBackground, const std::vector<std::u16string_view>& p_Texts = {});

    };

}   ///< namespace CP_SDK_BS::UI