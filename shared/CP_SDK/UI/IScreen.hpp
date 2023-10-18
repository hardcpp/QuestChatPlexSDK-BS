#pragma once

#include "UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Screen slot enum
    enum class EScreenSlot
    {
        Left,
        Middle,
        Right
    };

    class IViewController;

    /// @brief Abstract screen
    class IScreen : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", IScreen, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(IScreen);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(IScreen);

        protected:
            _v::Delegate<void(_v::CMonoPtrRef<IViewController>)> m_SetViewController;

        public:
            UIFieldRefDel<IViewController> CurrentViewController;

        public:
            /// @brief Replace active view controller
            /// @param p_ViewController New view controller
            void SetViewController(_v::CMonoPtrRef<IViewController> p_ViewController);

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::IScreen);