#pragma once

#include "../Utils/Delegate.hpp"
#include "../Utils/Il2cpp.hpp"

#include <custom-types/shared/coroutine.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <mutex>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace System;
        using namespace System::Threading;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Main thread task system
    class CP_SDK_EXPORT_VISIBILITY MTMainThreadInvoker : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.Unity", MTMainThreadInvoker, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(MTMainThreadInvoker);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(MTMainThreadInvoker);

        private:
            /// @brief Queue class
            struct Queue
            {
                _v::Action<> ** Data;
                int WritePos;
            };

        private:
            static MTMainThreadInvoker* m_Instance;     /// Singleton

            static Queue**      m_Queues;           ///< Queues instance
            static bool         m_Queued;           ///< Have queued actions
            static int          m_FrontQueue;       ///< Current front queue
            static std::mutex   m_Mutex;            ///< Lock mutex

        public:
            /// @brief Unity GameObject initialize
            static void Initialize();
            /// @brief Stop
            static void Destroy();

        public:
            /// @brief Enqueue a new action
            /// @param p_Delegate Action to enqueue
            static void Enqueue(_v::CActionRef<> p_Delegate);

        private:
            /// @brief Unity GameObject update
            DECLARE_INSTANCE_METHOD(void, Update);

    };

}   ///< namespace CP_SDK::Unity

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::Unity::MTMainThreadInvoker);