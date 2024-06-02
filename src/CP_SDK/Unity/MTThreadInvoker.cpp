#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/Threading/ThreadStart.hpp>
#include <System/Threading/ParameterizedThreadStart.hpp>
#include <System/Action.hpp>

#include <custom-types/shared/delegate.hpp>

const int MAX_QUEUE_SIZE = 1000;

namespace CP_SDK::Unity {

    bool                                MTThreadInvoker::m_RunCondition = false;
    il2cpp_utils::il2cpp_aware_thread*  MTThreadInvoker::m_UpdateThread = nullptr;
    MTThreadInvoker::Queue**            MTThreadInvoker::m_Queues;
    bool                                MTThreadInvoker::m_Queued       = false;
    int                                 MTThreadInvoker::m_FrontQueue   = 0;
    std::mutex                          MTThreadInvoker::m_Mutex;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Initialize
    void MTThreadInvoker::Initialize()
    {
        if (m_UpdateThread)
            return;

        m_Queues = new MTThreadInvoker::Queue*[2];

        for (int l_I = 0; l_I < 2; ++l_I)
        {
            m_Queues[l_I] = new MTThreadInvoker::Queue();
            m_Queues[l_I]->Data     = new _v::Action<>*[MAX_QUEUE_SIZE];
            m_Queues[l_I]->WritePos = 0;
        }

        m_RunCondition = true;

        m_UpdateThread = new il2cpp_utils::il2cpp_aware_thread(&MTThreadInvoker::__INTERNAL_Update);
    }
    /// @brief Stop
    void MTThreadInvoker::Destroy()
    {
        if (!m_UpdateThread)
            return;

        m_RunCondition = false;
        if (m_UpdateThread->joinable())
            m_UpdateThread->join();
        delete m_UpdateThread;
        m_UpdateThread = nullptr;

        for (int l_I = 0; l_I < 2; ++l_I)
        {
            for (int l_Y = 0; l_Y < m_Queues[l_I]->WritePos; ++l_Y)
            {
                auto l_Delegate = m_Queues[l_I]->Data[l_Y];
                if (l_Delegate)
                    delete l_Delegate;
            }

            delete[] m_Queues[l_I]->Data;
        }

        delete[] m_Queues;
        m_Queues = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a new action
    /// @param p_Delegate Action to enqueue
    void MTThreadInvoker::EnqueueOnThread(_v::CActionRef<> p_Delegate)
    {
        std::lock_guard l_Lock(m_Mutex);

        auto l_Queue = m_Queues[m_FrontQueue];
        if (l_Queue->WritePos >= MAX_QUEUE_SIZE)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTThreadInvoker.Enqueue] Too many actions pushed!");
            return;
        }

        l_Queue->Data[l_Queue->WritePos++] = new _v::Action<>(p_Delegate);
        m_Queued = true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Thread update
    void MTThreadInvoker::__INTERNAL_Update()
    {
        while (m_RunCondition)
        {
            if (!m_Queued)
            {
                _u::Thread::Sleep(10);
                continue;
            }

            auto l_QueueToHandle     = m_FrontQueue;
            auto l_NextFrontQueue    = (m_FrontQueue + 1) & 1;

            m_Mutex.lock();
            m_FrontQueue    = l_NextFrontQueue;
            m_Queued        = false;
            m_Mutex.unlock();

            auto l_Queue = m_Queues[l_QueueToHandle];
            auto l_Count = l_Queue->WritePos;
            auto l_I     = 0;

            do
            {
                try
                {
                    l_Queue->Data[l_I]->Invoke();
                    delete l_Queue->Data[l_I];
                }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTThreadInvoker.Update] Error:");
                    ChatPlexSDK::Logger()->Error(l_Exception);

                    delete l_Queue->Data[l_I];
                }

                ++l_I;
            } while (l_I < l_Count);

            if (l_I < l_Count)
            {
                auto l_ToCopy        = l_Count - l_I;
                auto l_FrontQueue    = m_Queues[m_FrontQueue];

                m_Mutex.lock();
                memcpy(&l_FrontQueue->Data[l_FrontQueue->WritePos], &l_Queue->Data[l_I], l_ToCopy * sizeof(_v::Action<>*));
                l_FrontQueue->WritePos += l_ToCopy;
                m_Queued = true;
                m_Mutex.unlock();
            }

            memset(l_Queue->Data, 0, l_Count * sizeof(_v::Action<>*));
            l_Queue->WritePos = 0;

            _u::Thread::Sleep(10);
        }
    }

}   ///< namespace CP_SDK::Unity