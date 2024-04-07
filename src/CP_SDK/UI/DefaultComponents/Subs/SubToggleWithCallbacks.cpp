#include "CP_SDK/UI/DefaultComponents/Subs/SubToggleWithCallbacks.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents::Subs {

    CP_SDK_IL2CPP_INHERIT_INIT(SubToggleWithCallbacks);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SubToggleWithCallbacks)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SubToggleWithCallbacks)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Calculate the layout input for horizontal axis
    void SubToggleWithCallbacks::Selectable__DoStateTransition(Selectable::SelectionState p_State, bool p_Instant)
    {
        CP_SDK_IL2CPP_CALL_BASE_METHOD(Selectable, "DoStateTransition", p_State, p_Instant);
        StateDidChangeEvent(static_cast<ESelectionState>(p_State.value__));
    }

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs