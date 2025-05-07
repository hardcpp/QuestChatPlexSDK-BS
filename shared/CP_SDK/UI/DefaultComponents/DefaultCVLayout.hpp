#pragma once

#include "../Components/CVLayout.hpp"
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

    /// @brief Default CVLayout component
    class CP_SDK_EXPORT DefaultCVLayout : public Components::CVLayout
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCVLayout, Components::CVLayout);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCVLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCVLayout);

        private:
            _v::MonoPtr<_u::RectTransform>                   m_RTransform;
            _v::MonoPtr<_u::ContentSizeFitter>               m_CSizeFitter;
            _v::MonoPtr<_u::LayoutElement>                   m_LElement;
            _v::MonoPtr<_u::HorizontalOrVerticalLayoutGroup> m_HOrVLayoutGroup;
            _v::MonoPtr<_u::VerticalLayoutGroup>             m_VLayoutGroup;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCVLayout();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCVLayout);