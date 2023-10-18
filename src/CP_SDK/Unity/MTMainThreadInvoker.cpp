#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>

const int MAX_QUEUE_SIZE = 1000;

using namespace UnityEngine;

namespace CP_SDK::Unity {

    CP_SDK_IL2CPP_INHERIT_INIT(MTMainThreadInvoker);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MTMainThreadInvoker* MTMainThreadInvoker::m_Instance;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MTMainThreadInvoker::Queue**        MTMainThreadInvoker::m_Queues       = nullptr;
    bool                                MTMainThreadInvoker::m_Queued       = false;
    int                                 MTMainThreadInvoker::m_FrontQueue   = 0;
    std::mutex                          MTMainThreadInvoker::m_Mutex;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(MTMainThreadInvoker)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(MTMainThreadInvoker)
    {

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Unity GameObject initialize
    void MTMainThreadInvoker::Initialize()
    {
        if (m_Instance)
            return;

        m_Queues = new MTMainThreadInvoker::Queue*[2];

        for (int l_I = 0; l_I < 2; ++l_I)
        {
            m_Queues[l_I]           = new MTMainThreadInvoker::Queue();
            m_Queues[l_I]->Data     = new _v::Action<>*[MAX_QUEUE_SIZE];
            m_Queues[l_I]->WritePos = 0;
        }

        auto l_GameObject = GameObject::New_ctor(u"[CP_SDK.Unity.MTMainThreadInvoker]");
        GameObject::DontDestroyOnLoad(l_GameObject);

        m_Instance = l_GameObject->AddComponent<MTMainThreadInvoker*>();
    }
    /// @brief Stop
    void MTMainThreadInvoker::Destroy()
    {
        if (!m_Instance)
            return;

        for (int l_I = 0; l_I < 2; ++l_I)
            delete[] m_Queues[l_I]->Data;

        delete[] m_Queues;
        m_Queues = nullptr;

        GameObject::Destroy(m_Instance->get_gameObject());
        m_Instance = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a new action
    /// @param p_Delegate Action to enqueue
    void MTMainThreadInvoker::Enqueue(_v::CActionRef<> p_Delegate)
    {
        std::lock_guard l_Lock(m_Mutex);

        auto l_Queue = m_Queues[m_FrontQueue];
        if (l_Queue->WritePos >= MAX_QUEUE_SIZE)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTMainThreadInvoker.Enqueue] Too many actions pushed!");
            return;
        }

        l_Queue->Data[l_Queue->WritePos++] = new _v::Action<>(p_Delegate);
        m_Queued = true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Unity GameObject update
    void MTMainThreadInvoker::Update()
    {
        if (!m_Queued)
            return;

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
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTMainThreadInvoker.Update] Error:");
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
    }

}   ///< namespace CP_SDK::Unity