#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <utility>

constexpr std::size_t MAX_QUEUE_SIZE = 1000;

namespace CP_SDK::Unity {

    bool                                MTThreadInvoker::m_RunCondition = false;
    il2cpp_utils::il2cpp_aware_thread*  MTThreadInvoker::m_UpdateThread = nullptr;
    std::deque<Utils::Action<>>         MTThreadInvoker::m_Queue;
    std::mutex                          MTThreadInvoker::m_Mutex;
    std::condition_variable             MTThreadInvoker::m_Condition;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Initialize
    void MTThreadInvoker::Initialize()
    {
        std::lock_guard l_Lock(m_Mutex);
        if (m_UpdateThread)
            return;

        m_Queue.clear();
        m_RunCondition = true;
        try
        {
            m_UpdateThread = new il2cpp_utils::il2cpp_aware_thread(&MTThreadInvoker::__INTERNAL_Update);
        }
        catch (...)
        {
            m_RunCondition = false;
            throw;
        }
    }
    /// @brief Stop
    void MTThreadInvoker::Destroy()
    {
        il2cpp_utils::il2cpp_aware_thread* l_Thread = nullptr;
        {
            std::lock_guard l_Lock(m_Mutex);
            if (!m_UpdateThread)
                return;

            m_RunCondition = false;
            l_Thread = m_UpdateThread;
        }
        m_Condition.notify_all();

        if (l_Thread->joinable())
            l_Thread->join();
        delete l_Thread;

        {
            std::lock_guard l_Lock(m_Mutex);
            if (m_UpdateThread == l_Thread)
                m_UpdateThread = nullptr;

            m_Queue.clear();
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a new action
    /// @param p_Delegate Action to enqueue
    void MTThreadInvoker::EnqueueOnThread(_v::CActionRef<> p_Delegate)
    {
        std::lock_guard l_Lock(m_Mutex);

        if (!m_RunCondition || !m_UpdateThread)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTThreadInvoker.Enqueue] Invoker is not running!");
            return;
        }

        if (m_Queue.size() >= MAX_QUEUE_SIZE)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTThreadInvoker.Enqueue] Too many actions pushed!");
            return;
        }

        m_Queue.emplace_back(p_Delegate);
        m_Condition.notify_one();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Thread update
    void MTThreadInvoker::__INTERNAL_Update()
    {
        while (true)
        {
            {
                std::deque<_v::Action<>> l_Actions;
                {
                    std::unique_lock l_Lock(m_Mutex);
                    m_Condition.wait(l_Lock, [] { return !m_RunCondition || !m_Queue.empty(); });
                    if (!m_RunCondition && m_Queue.empty())
                        break;

                    l_Actions.swap(m_Queue);
                }

                for (auto& l_Action : l_Actions)
                {
                    try
                    {
                        l_Action.Invoke();
                    }
                    catch (const std::exception& l_Exception)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTThreadInvoker.Update] Error:");
                        ChatPlexSDK::Logger()->Error(l_Exception);
                    }
                    catch (...)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTThreadInvoker.Update] Unknown error");
                    }
                }
            }
        }
    }

}   ///< namespace CP_SDK::Unity
