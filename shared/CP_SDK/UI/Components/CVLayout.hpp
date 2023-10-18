#pragma once

#include "Generics/CHOrVLayout.hpp"

#include <UnityEngine/UI/VerticalLayoutGroup.hpp>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Vertical layout component
    class CVLayout : public CHOrVLayout
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.Components", CVLayout, CHOrVLayout);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(CVLayout, CHOrVLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(CVLayout, CHOrVLayout);

        public:
            UIFieldRefDel<_u::VerticalLayoutGroup> VLayoutGroup;

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CVLayout);