#pragma once

#include <UnityEngine/GameObject.hpp>

#define CP_SDK_UNITY_PERSISTANT_SINGLETON_NO_DESTROY_DECL(t_Type)   \
    protected:                                                      \
        static t_Type* m_Instance;                                  \
        static void WasDestroyed();                                 \
    public:                                                         \
        static t_Type* Instance();                                  \
        static void TouchInstance();

#define CP_SDK_UNITY_PERSISTANT_SINGLETON_DECL(t_Type)              \
    CP_SDK_UNITY_PERSISTANT_SINGLETON_NO_DESTROY_DECL(t_Type)       \
    public:                                                         \
        static void Destroy();

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define CP_SDK_UNITY_PERSISTANT_SINGLETON_NO_DESTROY_IMPL(t_Type)           \
    t_Type* t_Type::m_Instance = nullptr;                                   \
    t_Type* t_Type::Instance()                                              \
    {                                                                       \
        if (!CP_SDK::Utils::IsUnityPtrValid(m_Instance))                    \
        {                                                                   \
            m_Instance = GameObject::New_ctor()->AddComponent<t_Type*>();   \
            m_Instance->get_gameObject()->set_name(u"[" #t_Type u"]");      \
            GameObject::DontDestroyOnLoad(m_Instance->get_gameObject());    \
        }                                                                   \
                                                                            \
        return m_Instance;                                                  \
    }                                                                       \
    void t_Type::TouchInstance()                                            \
    {                                                                       \
        Instance();                                                         \
    }

#define CP_SDK_UNITY_PERSISTANT_SINGLETON_IMPL(t_Type)                      \
    CP_SDK_UNITY_PERSISTANT_SINGLETON_NO_DESTROY_IMPL(t_Type)               \
    void t_Type::Destroy()                                                  \
    {                                                                       \
        if (!CP_SDK::Utils::IsUnityPtrValid(m_Instance))                    \
            return;                                                         \
                                                                            \
        GameObject::Destroy(m_Instance->get_gameObject());                  \
        m_Instance = nullptr;                                               \
    }

