#pragma once

#include "../Utils/Delegate.hpp"
#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <condition_variable>
#include <deque>
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
    class CP_SDK_EXPORT MTThreadInvoker
    {
        private:
            static bool                                 m_RunCondition;     ///< Run condition
            static il2cpp_utils::il2cpp_aware_thread*   m_UpdateThread;     ///< Update thread
            static std::deque<_v::Action<>>             m_Queue;            ///< Pending actions
            static std::mutex                           m_Mutex;            ///< Lock mutex
            static std::condition_variable              m_Condition;        ///< Worker wakeup

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
