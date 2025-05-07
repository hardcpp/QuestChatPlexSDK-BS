#pragma once

#include "CP_SDK/Utils/Il2cpp.hpp"
#include "CP_SDK/Utils/MonoPtr.hpp"

#include <HMUI/IconSegmentedControl.hpp>
#include <UnityEngine/RectTransform.hpp>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Vertical icon segmented control
    class CP_SDK_EXPORT HMUIIconSegmentedControl
    {
        CP_SDK_NO_DEF_CTORS(HMUIIconSegmentedControl);


        private:
            static CP_SDK::Utils::MonoPtr<HMUI::IconSegmentedControl> m_Template;

        public:
            /// @brief When the game is reloaded
            static void OnGameSoftReload();

        public:
            /// @brief Create icon segmented control
            /// @param p_Parent             Parent game object transform
            /// @param p_HideCellBackground Should hide cell background
            /// @return GameObject
            static HMUI::IconSegmentedControl* Create(_u::RectTransform* p_Parent, bool p_HideCellBackground);

        public:
            /// @brief Set data and remove hover hints
            /// @param p_Instance Control instance
            /// @param p_Data     Data to set
            static void SetDataNoHoverHint(HMUI::IconSegmentedControl* p_Instance, ::Array<HMUI::IconSegmentedControl::DataItem*>* p_Data);

    };

}   ///< namespace CP_SDK_BS::UI