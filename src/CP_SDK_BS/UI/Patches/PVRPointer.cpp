#include "CP_SDK_BS/UI/Patches/PVRPointer.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

using namespace VRUIControls;

namespace CP_SDK_BS::UI::Patches {

    CP_SDK::Utils::Event<VRPointer*> PVRPointer::OnActivated;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

#if BEATSABER_1_29_4_OR_NEWER
    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        VRPointer__OnEnable, &VRPointer::EnabledLastSelectedPointer,
        void, VRPointer* __Instance)
#else
    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        VRPointer__OnEnable, &VRPointer::OnEnable,
        void, VRPointer* __Instance)
#endif
    {
        VRPointer__OnEnable(__Instance);

        try { PVRPointer::OnActivated(__Instance); }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.UI.Patches][PVRPointer.VRPointer__OnEnable] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK_BS::UI::Patches
