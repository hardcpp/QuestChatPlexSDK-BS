#pragma once

#include "Generics/CVXList.hpp"

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

    /// @brief Virtual Vertical List
    class CP_SDK_EXPORT CVVList : public CVXList
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CVVList, CVXList);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(CVVList);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(CVVList);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CVVList);