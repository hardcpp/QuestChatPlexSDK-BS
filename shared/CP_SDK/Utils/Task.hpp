#pragma once

#include "Delegate.hpp"
#include "MonoPtr.hpp"
#include "../Unity/MTCoroutineStarter.hpp"

#include <custom-types/shared/coroutine.hpp>

#include <System/Threading/Tasks/Task_1.hpp>
#include <System/Threading/Tasks/TaskStatus.hpp>

namespace CP_SDK::Utils {

    namespace _u
    {
        using namespace System::Threading;
        using namespace System::Threading::Tasks;
    }

    /// @brief Await a task using a coroutine on the main thread
    /// @tparam t_Result  Task result type
    /// @param p_Task     Task instance
    /// @param p_Callback Callback
    template<typename t_Result>
    static void AwaitTaskAsync(_u::Task_1<t_Result>*                              p_Task,
                               CActionRef<MonoPtrRef<_u::Task_1<t_Result>>, bool> p_Callback)
    {
        Unity::MTCoroutineStarter::EnqueueFromThread(
            custom_types::Helpers::CoroutineHelper::New(Coroutine_AwaitTaskAsync(MonoPtr<_u::Task_1<t_Result>>(p_Task), p_Callback))
        );
    }

    /// @brief Await a task using coroutine
    /// @tparam t_Result  Task result type
    /// @param p_Task     Task instance
    /// @param p_Callback Callback
    /// @return Coroutine
    template<typename t_Result>
    custom_types::Helpers::Coroutine Coroutine_AwaitTaskAsync(MonoPtr<_u::Task_1<t_Result>>                   p_Task,
                                                              Action<MonoPtrRef<_u::Task_1<t_Result>>, bool>  p_Callback)
    {
        co_yield nullptr;

        while (true)
        {
            if (!p_Task || p_Task->get_IsCanceled())
            {
                p_Callback(p_Task, false);
                co_return;
            }

            auto l_Status = p_Task->get_Status();
            if (l_Status == _u::TaskStatus::Faulted || l_Status == _u::TaskStatus::Canceled)
            {
                p_Callback(p_Task, false);
                co_return;
            }

            if (l_Status == _u::TaskStatus::RanToCompletion)
                break;

            co_yield nullptr;
        }

        p_Callback(p_Task, true);
    }

}   ///< namespace CP_SDK::Utils
