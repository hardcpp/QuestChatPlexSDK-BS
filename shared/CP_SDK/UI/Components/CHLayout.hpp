#pragma once

#include "Generics/CHOrVLayout.hpp"

#include <UnityEngine/UI/HorizontalLayoutGroup.hpp>

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

    /// @brief Horizontal layout component
    class CP_SDK_EXPORT CHLayout : public CHOrVLayout
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CHLayout, CHOrVLayout);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(CHLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(CHLayout);

        public:
            UIFieldRefDel<_u::HorizontalLayoutGroup> HLayoutGroup;

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CHLayout);