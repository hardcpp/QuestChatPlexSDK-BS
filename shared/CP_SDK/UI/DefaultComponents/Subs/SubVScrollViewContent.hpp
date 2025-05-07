#pragma once

#include "../../Components/CVScrollView.hpp"

#include <custom-types/shared/coroutine.hpp>

namespace CP_SDK::UI::DefaultComponents::Subs {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::EventSystems;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Vertical scroll view content updater
    class CP_SDK_EXPORT SubVScrollViewContent : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK::UI::DefaultComponents::Subs", SubVScrollViewContent, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(SubVScrollViewContent);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(SubVScrollViewContent);

        private:
            bool m_IsDirty;

        public:
            _v::MonoPtr<Components::CVScrollView> VScrollView;

        public:
            /// @brief Component first frame
            DECLARE_INSTANCE_METHOD(void, Start);

        public:
            /// @brief On frame
            DECLARE_INSTANCE_METHOD(void, Update);

        public:
            /// @brief On rect transform dimensions changed
            DECLARE_INSTANCE_METHOD(void, OnRectTransformDimensionsChange);

        public:
            /// @brief Timed canvas disabler
            /// @param p_Time Time in seconds
            static custom_types::Helpers::Coroutine Coroutine_DisableCanvas(SubVScrollViewContent* p_Self);
            /// @brief Update scroll view content size & buttons
            void UpdateScrollView();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::Subs::SubVScrollViewContent);