#pragma once

#include "../../../CP_SDK/Utils/Event.hpp"
#include "../../../CP_SDK/Utils/MonoPtr.hpp"

#include <VRUIControls/VRPointer.hpp>

namespace CP_SDK_BS::UI::Patches {

    namespace _u
    {
        using namespace VRUIControls;
    }

    class PVRPointer
    {
        public:
            static CP_SDK::Utils::Event<_u::VRPointer*> OnActivated;

    };

}   ///< namespace CP_SDK_BS::UI::Patches