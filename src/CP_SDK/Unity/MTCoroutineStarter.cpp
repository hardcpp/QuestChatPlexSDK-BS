#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>

constexpr std::size_t MAX_QUEUE_SIZE = 1000;

using namespace System::Collections;
using namespace UnityEngine;

namespace CP_SDK::Unity {

    CP_SDK_IL2CPP_INHERIT_INIT(MTCoroutineStarter);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MTCoroutineStarter* MTCoroutineStarter::m_Instance;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::deque<Utils::MonoPtr<Il2CppObject>> MTCoroutineStarter::m_Queue;
    bool                                     MTCoroutineStarter::m_Accepting = false;
    std::mutex                               MTCoroutineStarter::m_Mutex;

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
        std::lock_guard l_Lock(m_Mutex);
        if (m_Instance)
            return;

        auto l_GameObject = GameObject::New_ctor(u"[CP_SDK.Unity.MTCoroutineStarter]");
        Object::DontDestroyOnLoad(l_GameObject);

        m_Instance = l_GameObject->AddComponent<MTCoroutineStarter*>();
        m_Queue.clear();
        m_Accepting = true;
    }
    /// @brief Stop
    void MTCoroutineStarter::Destroy()
    {
        MTCoroutineStarter* l_Instance = nullptr;
        {
            std::lock_guard l_Lock(m_Mutex);
            if (!m_Instance)
                return;

            m_Accepting = false;
            m_Queue.clear();
            l_Instance = m_Instance;
            m_Instance = nullptr;
        }

        UnityEngine::GameObject::Destroy(l_Instance->get_gameObject());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a new coroutine from thread
    /// @param p_Coroutine Coroutine to enqueue
    void MTCoroutineStarter::EnqueueFromThread(IEnumerator* p_Coroutine)
    {
        if (!p_Coroutine)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.EnqueueFromThread] Null coroutine!");
            return;
        }

        std::lock_guard l_Lock(m_Mutex);

        if (!m_Accepting || !m_Instance)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.EnqueueFromThread] Starter is not running!");
            return;
        }

        if (m_Queue.size() >= MAX_QUEUE_SIZE)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.EnqueueFromThread] Too many coroutines pushed!");
            return;
        }

        m_Queue.emplace_back(reinterpret_cast<Il2CppObject*>(p_Coroutine));
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
        return m_Instance && p_Coroutine ? m_Instance->StartCoroutine(p_Coroutine) : nullptr;
    }
    /// @brief Start coroutine
    /// @param p_Coroutine Coroutine to enqueue
    Coroutine* MTCoroutineStarter::Start(custom_types::Helpers::Coroutine&& p_Coroutine)
    {
        return m_Instance
            ? m_Instance->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(std::move(p_Coroutine)))
            : nullptr;
    }
    /// @brief Stop coroutine
    /// @param p_Coroutine Coroutine to stop
    void MTCoroutineStarter::Stop(Coroutine* p_Coroutine)
    {
        if (m_Instance && p_Coroutine)
            m_Instance->StopCoroutine(p_Coroutine);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Unity GameObject update
    void MTCoroutineStarter::Update()
    {
        std::deque<_v::MonoPtr<Il2CppObject>> l_Coroutines;
        {
            std::lock_guard l_Lock(m_Mutex);
            l_Coroutines.swap(m_Queue);
        }

        for (auto& l_Coroutine : l_Coroutines)
        {
            try
            {
                this->StartCoroutine(reinterpret_cast<IEnumerator*>(l_Coroutine.Ptr()));
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.Update] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
            catch (...)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTCoroutineStarter.Update] Unknown error");
            }
        }
    }

}   ///< namespace CP_SDK::Unity
