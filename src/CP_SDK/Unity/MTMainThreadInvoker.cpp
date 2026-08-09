#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>

namespace {
    constexpr std::size_t MAX_QUEUE_SIZE = 1000;
}

using namespace UnityEngine;

namespace CP_SDK::Unity {

    CP_SDK_IL2CPP_INHERIT_INIT(MTMainThreadInvoker);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MTMainThreadInvoker* MTMainThreadInvoker::m_Instance;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::deque<Utils::Action<>> MTMainThreadInvoker::m_Queue;
    bool                        MTMainThreadInvoker::m_Accepting = false;
    std::mutex                  MTMainThreadInvoker::m_Mutex;

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
        std::lock_guard l_Lock(m_Mutex);
        if (m_Instance)
            return;

        auto l_GameObject = GameObject::New_ctor(u"[CP_SDK.Unity.MTMainThreadInvoker]");
        GameObject::DontDestroyOnLoad(l_GameObject);

        m_Instance = l_GameObject->AddComponent<MTMainThreadInvoker*>();
        m_Queue.clear();
        m_Accepting = true;
    }
    /// @brief Stop
    void MTMainThreadInvoker::Destroy()
    {
        MTMainThreadInvoker* l_Instance = nullptr;
        {
            std::lock_guard l_Lock(m_Mutex);
            if (!m_Instance)
                return;

            m_Accepting = false;
            m_Queue.clear();
            l_Instance = m_Instance;
            m_Instance = nullptr;
        }

        GameObject::Destroy(l_Instance->get_gameObject());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a new action
    /// @param p_Delegate Action to enqueue
    void MTMainThreadInvoker::Enqueue(_v::CActionRef<> p_Delegate)
    {
        std::lock_guard l_Lock(m_Mutex);

        if (!m_Accepting || !m_Instance)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTMainThreadInvoker.Enqueue] Invoker is not running!");
            return;
        }

        if (m_Queue.size() >= MAX_QUEUE_SIZE)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTMainThreadInvoker.Enqueue] Too many actions pushed!");
            return;
        }

        m_Queue.emplace_back(p_Delegate);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Unity GameObject update
    void MTMainThreadInvoker::Update()
    {
        {
            std::deque<_v::Action<>> l_Actions;
            {
                std::lock_guard l_Lock(m_Mutex);
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
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTMainThreadInvoker.Update] Error:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
                catch (...)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MTMainThreadInvoker.Update] Unknown error");
                }
            }
        }
    }

}   ///< namespace CP_SDK::Unity
