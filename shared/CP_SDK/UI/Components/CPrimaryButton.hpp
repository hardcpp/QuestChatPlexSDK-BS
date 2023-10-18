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
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.Components", CPrimaryButton, CPOrSButton);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(CPrimaryButton, CPOrSButton);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(CPrimaryButton, CPOrSButton);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CPrimaryButton);