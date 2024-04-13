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
            static bool                                 m_RunCondition;     ///< Run condition
            static il2cpp_utils::il2cpp_aware_thread*   m_UpdateThread;     ///< Update thread
            static Queue**                              m_Queues;           ///< Queues instance
            static bool                                 m_Queued;           ///< Have queued actions
            static int                                  m_FrontQueue;       ///< Current front queue
            static std::mutex                           m_Mutex;            ///< Lock mutex

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

}   ///< namespace CP_SDK::Unity