#pragma once

#include "../Components/CGLayout.hpp"
#include "../../Utils/Event.hpp"

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CGLayout component
    class CP_SDK_EXPORT DefaultCGLayout : public Components::CGLayout
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCGLayout, Components::CGLayout);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCGLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCGLayout);

        private:
            _v::MonoPtr<_u::RectTransform>       m_RTransform;
            _v::MonoPtr<_u::ContentSizeFitter>   m_CSizeFitter;
            _v::MonoPtr<_u::LayoutElement>       m_LElement;
            _v::MonoPtr<_u::GridLayoutGroup>     m_GLayoutGroup;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCGLayout();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCGLayout);