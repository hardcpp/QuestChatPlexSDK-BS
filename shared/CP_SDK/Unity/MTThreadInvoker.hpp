#pragma once

#include "../Utils/Delegate.hpp"
#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <mutex>

#include <System/Threading/Thread.hpp>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace System;
        using namespace System::Threading;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Thread task system
    class MTThreadInvoker
    {
        private:
            /// @brief Queue class
            struct Queue
            {
                _v::Action<>** Data;
                int WritePos;
            };

        private:
            static bool                     m_RunCondition;     ///< Run condition
            static _v::MonoPtr<_u::Thread>  m_UpdateThread;     ///< Update thread
            static Queue**                  m_Queues;           ///< Queues instance
            static bool                     m_Queued;           ///< Have queued actions
            static int                      m_FrontQueue;       ///< Current front queue
            static std::mutex               m_Mutex;            ///< Lock mutex

        public:
            /// @brief Initialize
            static void Initialize();
            /// @brief Stop
            static void Destroy();

        public:
            /// @brief Enqueue a new action
            /// @param p_Delegate Action to enqueue
            static void EnqueueOnThread(_v::CActionRef<> p_Delegate);

        public:
            /// @brief Thread update
            static void __INTERNAL_Update();

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Dummy il2cpp object for thread start
    class __MTThreadInvokerDummy : public Il2CppObject
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.Unity", __MTThreadInvokerDummy, Il2CppObject);

        public:
            /// @brief Dummy method for thread start
            DECLARE_STATIC_METHOD(void, Dummy);

    };

}   ///< namespace CP_SDK::Unity

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::Unity::__MTThreadInvokerDummy);