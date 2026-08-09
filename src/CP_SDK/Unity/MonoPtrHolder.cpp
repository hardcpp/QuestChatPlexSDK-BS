#include "CP_SDK/Utils/MonoPtr.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <memory>

using namespace System::Collections::Generic;
using namespace UnityEngine;

namespace CP_SDK::Unity {

    namespace {
        class MonitorGuard
        {
            public:
                explicit MonitorGuard(Il2CppObject* p_Monitor) : m_Monitor(p_Monitor)
                {
                    il2cpp_functions::monitor_enter(m_Monitor);
                }

                ~MonitorGuard()
                {
                    il2cpp_functions::monitor_exit(m_Monitor);
                }

                MonitorGuard(const MonitorGuard&) = delete;
                MonitorGuard& operator=(const MonitorGuard&) = delete;

            private:
                Il2CppObject* m_Monitor;
        };
    }

    std::map<Il2CppObject*, MonoPtrHolder::Wrapper*> MonoPtrHolder::m_PointersToWrapper;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK_IL2CPP_INHERIT_INIT(MonoPtrHolder);
    CP_SDK_UNITY_PERSISTANT_SINGLETON_NO_DESTROY_IMPL(CP_SDK::Unity::MonoPtrHolder);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(MonoPtrHolder)
    {
        m_Pointers = List_1<Il2CppObject*>::New_ctor();
    };
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(MonoPtrHolder)
    {
        ChatPlexSDK::Logger()->Error(u"MonoPtrHolder quitting app");
        // Nothing to do, here we are exiting the app
        //if (m_Pointers)
        //{
        //    il2cpp_functions::monitor_enter(m_Instance->m_Pointers);
        //    m_Pointers->Clear();
        //
        //    for (auto& l_Current : m_PointersToWrapper)
        //        delete l_Current.second;
        //
        //    m_PointersToWrapper.clear();
        //    il2cpp_functions::monitor_exit(m_Instance->m_Pointers);
        //}

        WasDestroyed();
    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Grab a reference to this wrapper
    void MonoPtrHolder::Wrapper::Grab()
    {
        m_Count++;
    }
    /// @brief Drop a reference to this wrapper
    void MonoPtrHolder::Wrapper::Drop() noexcept
    {
        try
        {
            MonoPtrHolder::Release(Ptr);
        }
        catch (...)
        {
            /// Destructors and cleanup paths must never propagate exceptions.
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get or register a new wrapper
    /// @param p_Pointer Il2Cpp object
    MonoPtrHolder::Wrapper* MonoPtrHolder::GetOrRegister(Il2CppObject* p_Pointer)
    {
        if (!p_Pointer)
            return nullptr;

        if (!m_Instance)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MonoPtrHolder.GetOrRegister] MonoPtrHolder was not initialized!");
            throw std::runtime_error("MonoPtrHolder was not initialized!");
        }

        MonitorGuard l_Guard(reinterpret_cast<Il2CppObject*>(m_Instance->m_Pointers));

        const auto& l_It = m_Instance->m_PointersToWrapper.find(p_Pointer);
        if (l_It != m_Instance->m_PointersToWrapper.end())
        {
            l_It->second->Grab();
            return l_It->second;
        }

        auto l_NewWrapper = std::make_unique<Wrapper>();
        l_NewWrapper->Ptr = p_Pointer;

        m_Instance->m_Pointers->Add(p_Pointer);
        try
        {
            m_Instance->m_PointersToWrapper.emplace(p_Pointer, l_NewWrapper.get());
        }
        catch (...)
        {
            m_Instance->m_Pointers->Remove(p_Pointer);
            throw;
        }

        l_NewWrapper->Grab();
        return l_NewWrapper.release();
    }
    /// @brief Release a wrapper
    /// @param p_Pointer Il2Cpp object
    void MonoPtrHolder::Release(Il2CppObject* p_Pointer)
    {
        if (!m_Instance)
            return;         ///< Most likely happen on exit, do nothing

        MonitorGuard l_Guard(reinterpret_cast<Il2CppObject*>(m_Instance->m_Pointers));

        const auto& l_It = m_Instance->m_PointersToWrapper.find(p_Pointer);
        if (l_It == m_Instance->m_PointersToWrapper.end())
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MonoPtrHolder.Release] Try to release a non registered object!");
            return;
        }

        const auto l_Count = l_It->second->m_Count.load();
        if (l_Count <= 0)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][MonoPtrHolder.Release] Wrapper reference count underflow prevented!");
            return;
        }

        if (l_It->second->m_Count.fetch_sub(1) != 1)
            return;

        m_Instance->m_Pointers->Remove(p_Pointer);
        delete l_It->second;
        m_Instance->m_PointersToWrapper.erase(l_It);
    }

}   ///< namespace CP_SDK::Unity
