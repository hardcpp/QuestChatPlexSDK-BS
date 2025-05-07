#pragma once

#include "../../../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../../../CP_SDK/Utils/MonoPtr.hpp"

#include <VRUIControls/VRPointer.hpp>
#include <GlobalNamespace/VRController.hpp>
#include <UnityEngine/RaycastHit.hpp>

namespace CP_SDK_BS::UI::DefaultComponentsOverrides {

    class BS_CFloatingPanel;

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides

namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs {

    namespace _u
    {
        using namespace GlobalNamespace;
        using namespace UnityEngine;
        using namespace VRUIControls;
    }

    /// @brief Floating panel mover
    class CP_SDK_EXPORT SubFloatingPanelMover : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI.DefaultComponentsOverrides.Subs", SubFloatingPanelMover, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(SubFloatingPanelMover);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(SubFloatingPanelMover);

        const float MinScrollDistance = 0.25f;
        const float MaxLaserDistance  = 50.0f;

        public:
            CP_SDK::Utils::MonoPtr<BS_CFloatingPanel>           m_FloatingPanel;
            CP_SDK::Utils::MonoPtr<_u::VRPointer>               m_VRPointer;
            CP_SDK::Utils::MonoPtr<_u::VRController>            m_GrabbingController;
            _u::Vector3                                         m_GrabPosition;
            _u::Quaternion                                      m_GrabRotation;
            CP_SDK::Utils::MonoPtr<Array<_u::RaycastHit>>       m_RaycastBuffer;

        public:
            /// @brief On component creation
            DECLARE_INSTANCE_METHOD(void, Awake);

        public:
            /// @brief On frame
            DECLARE_INSTANCE_METHOD(void, Update);
            /// @brief On frame (late)
            DECLARE_INSTANCE_METHOD(void, LateUpdate);

    };

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::DefaultComponentsOverrides::Subs::SubFloatingPanelMover);