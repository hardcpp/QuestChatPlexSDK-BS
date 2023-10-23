#pragma once

#include "Generics/CPOrSButton.hpp"

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

    /// @brief Primary button component
    class CPrimaryButton : public CPOrSButton
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CPrimaryButton, CPOrSButton);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(CPrimaryButton);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(CPrimaryButton);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CPrimaryButton);