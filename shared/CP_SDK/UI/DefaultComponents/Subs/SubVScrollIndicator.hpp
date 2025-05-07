#pragma once

#include "../../../Utils/Il2cpp.hpp"
#include "../../../Utils/MonoPtr.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>

namespace CP_SDK::UI::DefaultComponents::Subs {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Vertical scroll indicator component
    class CP_SDK_EXPORT SubVScrollIndicator : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK::UI::DefaultComponents::Subs", SubVScrollIndicator, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(SubVScrollIndicator);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(SubVScrollIndicator);

        private:
            _v::MonoPtr<_u::RectTransform>  m_Handle;
            float                           m_Padding;
            float                           m_Progress;
            float                           m_NormalizedPageHeight;

        public:
            /// @brief Handle transform
            _u::RectTransform* Handle();
            _u::RectTransform* Handle(_u::RectTransform* p_Handle);

            /// @brief Progress
            float Progress();
            float Progress(float p_Progress);

            /// @brief Normalized page height
            float NormalizedPageHeight();
            float NormalizedPageHeight(float p_NormalizedPageHeight);

        public:
            /// @brief Refresh handle
            void RefreshHandle();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::Subs::SubVScrollIndicator);