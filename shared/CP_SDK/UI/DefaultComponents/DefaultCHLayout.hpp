#pragma once

#include "../Components/CHLayout.hpp"
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

    /// @brief Default CHLayout component
    class DefaultCHLayout : public Components::CHLayout
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.DefaultComponents", DefaultCHLayout, Components::CHLayout);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(DefaultCHLayout, Components::CHLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(DefaultCHLayout, Components::CHLayout);

        private:
            _v::MonoPtr<_u::RectTransform>                   m_RTransform;
            _v::MonoPtr<_u::ContentSizeFitter>               m_CSizeFitter;
            _v::MonoPtr<_u::LayoutElement>                   m_LElement;
            _v::MonoPtr<_u::HorizontalOrVerticalLayoutGroup> m_HOrVLayoutGroup;
            _v::MonoPtr<_u::HorizontalLayoutGroup>           m_HLayoutGroup;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCHLayout();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCHLayout);