#include "CP_SDK/UI/IScreen.hpp"
#include "CP_SDK/UI/IViewController.hpp"

using namespace UnityEngine;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(IScreen);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(IScreen)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(IScreen)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void IScreen::SetViewController(_v::CMonoPtrRef<IViewController> p_ViewController)
    {
        m_SetViewController(p_ViewController);
    }

}   ///< namespace CP_SDK::UI