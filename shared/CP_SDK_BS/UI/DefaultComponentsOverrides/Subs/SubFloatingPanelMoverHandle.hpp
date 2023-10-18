#pragma once

#include "../../../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../../../CP_SDK/Utils/MonoPtr.hpp"

namespace CP_SDK_BS::UI::DefaultComponentsOverrides {

    class BS_CFloatingPanel;

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides

namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Floating panel mover handle
    class SubFloatingPanelMoverHandle : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI.DefaultComponentsOverrides.Subs", SubFloatingPanelMoverHandle, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(SubFloatingPanelMoverHandle);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(SubFloatingPanelMoverHandle);

        public:
            CP_SDK::Utils::MonoPtr<BS_CFloatingPanel> FloatingPanel;

        public:
            /// @brief On component creation
            DECLARE_INSTANCE_METHOD(void, Awake);

    };

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::DefaultComponentsOverrides::Subs::SubFloatingPanelMoverHandle);