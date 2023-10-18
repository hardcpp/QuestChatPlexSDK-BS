#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>

const int MAX_QUEUE_SIZE = 1000;

using namespace System::Collections;
using namespace UnityEngine;

namespace CP_SDK::Unity {

    CP_SDK_IL2CPP_INHERIT_INIT(MTCoroutineStarter);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MTCoroutineStarter* MTCoroutineStarter::m_Instance;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MTCoroutineStarter::Queue**     MTCoroutineStarter::m_Queues        = nullptr;
    bool                            MTCoroutineStarter::m_Queued        = false;
    int                             MTCoroutineStarter::m_FrontQueue    = 0;
    std::mutex                      MTCoroutineStarter::m_Mutex;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(MTCoroutineStarter)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(MTCoroutineStarter)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Unity GameObject initialize
    void MTCoroutineStarter::Initialize()
    {
        if (m_Instance)
            return;

        m_Queues = new MTCoroutineStarter::Queue*[2];

        for (int l_I = 0; l_I < 2; ++l_I)
        {
            m_Queues[l_I]           = new MTCoroutineStarter::Queue();
            m_Queues[l_I]->Data     = new _v::MonoPtr<Il2CppObject>[MAX_QUEUE_SIZE];
            m_Queues[l_I]->WritePos = 0;
        }

        auto l_GameObject = GameObject::New_ctor(u"[CP_SDK.Unity.MTCoroutineStarter]");
        Object::DontDestroyOnLoad(l_GameObject);

        m_Instance = l_GameObject->AddComponent<MTCoroutineStarter*>();
    }
    /// @brief Stop
    void MTCoroutineStarter::Destroy()
    {
        if (!m_Instance)
            return;

        for (int l_I = 0; l_I < 2; ++l_I)
            delete[] m_Queues[l_I]->Data;

        delete[] m_Queues;
        m_Queues = nullptr;

        UnityEngine::GameObject::Destroy(m_Instance->get_gameObject());
        m_Instance = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a new coroutine from thread
    /// @param p_Coroutine Coroutine to enqueue
    void MTCoroutineStarter::EnqueueFromThread(IEnumerator* p_Coroutine)
    {
        std::lock_guard l_Lock(m_Mutex);

        auto l_Queue = m_Queues[m_FrontQueue];
        if (l_Queue->WritePos >= MAX_QUEUE_SIZE)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.EnqueueFromThread] Too many coroutines pushed!");
            return;
        }

        l_Queue->Data[l_Queue->WritePos++] = reinterpret_cast<Il2CppObject*>(p_Coroutine);
        m_Queued = true;
    }
    /// @brief Enqueue a new coroutine from thread
    /// @param p_Coroutine Coroutine to enqueue
    void MTCoroutineStarter::EnqueueFromThread(custom_types::Helpers::Coroutine&& p_Coroutine)
    {
        EnqueueFromThread(custom_types::Helpers::CoroutineHelper::New(std::move(p_Coroutine)));
    }
    /// @brief Start coroutine
    /// @param p_Coroutine Coroutine to enqueue
    Coroutine* MTCoroutineStarter::Start(IEnumerator* p_Coroutine)
    {
        return m_Instance->StartCoroutine(p_Coroutine);
    }
    /// @brief Start coroutine
    /// @param p_Coroutine Coroutine to enqueue
    Coroutine* MTCoroutineStarter::Start(custom_types::Helpers::Coroutine&& p_Coroutine)
    {
        return m_Instance->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(std::move(p_Coroutine)));
    }
    /// @brief Stop coroutine
    /// @param p_Coroutine Coroutine to stop
    void MTCoroutineStarter::Stop(Coroutine* p_Coroutine)
    {
        m_Instance->StopCoroutine(p_Coroutine);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Unity GameObject update
    void MTCoroutineStarter::Update()
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
                this->StartCoroutine(reinterpret_cast<IEnumerator*>(l_Queue->Data[l_I].Ptr()));
                l_Queue->Data[l_I] = nullptr;
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.Update] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            ++l_I;
        } while (l_I < l_Count);

        if (l_I < l_Count)
        {
            auto l_ToCopy        = l_Count - l_I;
            auto l_FrontQueue    = m_Queues[m_FrontQueue];

            m_Mutex.lock();
            memcpy(&l_FrontQueue->Data[l_FrontQueue->WritePos], &l_Queue->Data[l_I], l_ToCopy * sizeof(IEnumerator*));
            l_FrontQueue->WritePos += l_ToCopy;
            m_Queued = true;
            m_Mutex.unlock();
        }

        memset(l_Queue->Data, 0, l_Count * sizeof(IEnumerator*));
        l_Queue->WritePos = 0;
    }

}   ///< namespace CP_SDK::Unity