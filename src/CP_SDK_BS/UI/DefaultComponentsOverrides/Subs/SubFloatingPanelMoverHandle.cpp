#include "CP_SDK_BS/UI/DefaultComponentsOverrides/Subs/SubFloatingPanelMoverHandle.hpp"
#include "CP_SDK_BS/UI/DefaultComponentsOverrides/BS_CFloatingPanel.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <UnityEngine/BoxCollider.hpp>

using namespace UnityEngine;

namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs {

    CP_SDK_IL2CPP_INHERIT_INIT(SubFloatingPanelMoverHandle);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SubFloatingPanelMoverHandle)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SubFloatingPanelMoverHandle)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void SubFloatingPanelMoverHandle::Awake()
    {
        get_gameObject()->AddComponent<BoxCollider*>()->set_size(Vector3(1.0f, 1.0f, 0.1f));
        get_gameObject()->set_layer(CP_SDK::UI::UISystem::UILayer);
    }

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs
