#pragma once

#include "../../../Utils/Event.hpp"
#include "../../../Utils/Il2cpp.hpp"

#include <UnityEngine/UI/Selectable.hpp>
#include <UnityEngine/UI/Toggle.hpp>

namespace CP_SDK::UI::DefaultComponents::Subs {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Toggle with callbacks component
    class SubToggleWithCallbacks : public _u::Toggle
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK::UI::DefaultComponents::Subs", SubToggleWithCallbacks, _u::Toggle);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(SubToggleWithCallbacks);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(SubToggleWithCallbacks);
        CP_SDK_IL2CPP_CTOR_FIELD();
        CP_SDK_IL2CPP_DTOR_FIELD();

        public:
            /// @brief Selection state enum
            enum class ESelectionState
            {
                Normal,
                Highlighted,
                Pressed,
                Selected,
                Disabled
            };

        public:
            /// @brief State did change event
            _v::Event<ESelectionState> StateDidChangeEvent;

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, Selectable, void, DoStateTransition, Selectable::SelectionState, bool);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::Subs::SubToggleWithCallbacks);