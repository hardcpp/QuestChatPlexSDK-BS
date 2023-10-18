#include "CP_SDK/UI/FlowCoordinator.hpp"

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(FlowCoordinator);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::map<System::Type*, _v::MonoPtr<FlowCoordinator>> FlowCoordinator::m_Instances;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(FlowCoordinator, IFlowCoordinator)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(FlowCoordinator, IFlowCoordinator)
    {

    }

}   ///< namespace CP_SDK::UI