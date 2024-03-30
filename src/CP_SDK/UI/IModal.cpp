#include "CP_SDK/UI/IModal.hpp"

using namespace UnityEngine;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(IModal);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(IModal)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(IModal)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void IModal::Awake()
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform, m_RTransform);

        m_RTransform = get_transform().try_cast<RectTransform>().value_or(nullptr);
    }

}   ///< namespace CP_SDK::UI