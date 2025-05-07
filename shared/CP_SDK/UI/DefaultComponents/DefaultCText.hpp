#pragma once

#include "../Components/CText.hpp"
#include "../../Utils/Event.hpp"

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace TMPro;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CText component
    class CP_SDK_EXPORT DefaultCText : public Components::CText
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCText, Components::CText);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCText);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCText);

        private:
            _v::MonoPtr<_u::RectTransform>   m_RTransform;
            _v::MonoPtr<_u::LayoutElement>   m_LElement;
            _v::MonoPtr<_u::TextMeshProUGUI> m_TMProUGUI;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCText();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCText);