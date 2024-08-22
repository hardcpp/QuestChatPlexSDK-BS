#pragma once

#include "../Unity/MonoPtrHolder.hpp"

#include <UnityEngine/Object.hpp>

namespace CP_SDK::Utils {

    template<class t_Ptr, bool t_Force = false>
    class MonoPtr
    {
        static_assert(t_Force || !std::is_assignable_v<Il2CppObject*, t_Ptr*>, "Don't use non Mono types with MonoPtr.");

        public:
            MonoPtr() : m_Wrapper(nullptr) { }
            MonoPtr(t_Ptr* p_Pointer)
                : m_Wrapper(Unity::MonoPtrHolder::GetOrRegister((Il2CppObject*)p_Pointer))
            {
                if (m_Wrapper) m_Wrapper->Grab();
            }
            MonoPtr(const MonoPtr& p_Other)
                : m_Wrapper(p_Other.m_Wrapper)
            {
                if (m_Wrapper) m_Wrapper->Grab();
            }
            MonoPtr(MonoPtr&& p_Other)
                : m_Wrapper(p_Other.m_Wrapper)
            {
                p_Other.m_Wrapper = nullptr;
            }
            ~MonoPtr()
            {
                if (m_Wrapper) m_Wrapper->Drop();
                m_Wrapper = nullptr;
            }

        public:
            t_Ptr* Ptr(bool p_Throw = true) const
            {
                auto l_IsDead = !m_Wrapper || !m_Wrapper->Ptr;
                if constexpr (std::is_assignable_v<UnityEngine::Object, t_Ptr>)
                {
                    auto l_UObject = reinterpret_cast<UnityEngine::Object*>(m_Wrapper->Ptr);
                    if (l_IsDead || !l_UObject->___m_CachedPtr.m_value)
                        l_IsDead = true;
                }

                if (p_Throw && l_IsDead)
                {
                    //ChatPlexSDK::Logger()->Error(u"Dead pointer " + csTypeOf(t_Ptr*).convert()->get_Name());
                    throw NullHandleException();
                }

                return !l_IsDead ? reinterpret_cast<t_Ptr*>(m_Wrapper->Ptr) : nullptr;
            }

            bool IsUnityObject()
            {
                if constexpr (std::is_assignable_v<UnityEngine::Object, t_Ptr>)
                    return true;

                return false;
            }

        public:
            operator bool() const { return Ptr(false); }

                  t_Ptr* operator ->()       { return                    Ptr(true);  }
            const t_Ptr* operator ->() const { return const_cast<t_Ptr*>(Ptr(true)); }

        public:
            MonoPtr& operator=(t_Ptr* p_Pointer)
            {
                if (m_Wrapper && m_Wrapper->Ptr == (Il2CppObject*)p_Pointer) return *this;
                if (m_Wrapper) m_Wrapper->Drop();
                if (!p_Pointer)
                {
                    m_Wrapper = nullptr;
                    return *this;
                }

                m_Wrapper = Unity::MonoPtrHolder::GetOrRegister((Il2CppObject*)p_Pointer);
                if (m_Wrapper) m_Wrapper->Grab();

                return *this;
            }
            MonoPtr& operator=(const MonoPtr<t_Ptr>& p_Other)
            {
                if (m_Wrapper) m_Wrapper->Drop();
                m_Wrapper = p_Other.m_Wrapper;
                if (m_Wrapper) m_Wrapper->Grab();

                return *this;
            }
            MonoPtr& operator=(MonoPtr&& p_Other)
            {
                if (m_Wrapper) m_Wrapper->Drop();
                m_Wrapper = p_Other.m_Wrapper;
                p_Other.m_Wrapper = nullptr;

                return *this;
            }

            template<class t_OtherPtr> requires(t_Force || std::is_assignable_v<t_Ptr*&, t_OtherPtr*> || std::is_same_v<t_Ptr*&, t_OtherPtr*>)
            bool operator==(const MonoPtr<t_OtherPtr>& p_Other) const { return m_Wrapper == p_Other.m_Wrapper;           }
            template<class t_OtherPtr>
            bool operator==(t_OtherPtr* p_Pointer)              const { return m_Wrapper && m_Wrapper->Ptr == p_Pointer; }
            template<class t_OtherPtr>
            bool operator==(::UnityW<t_OtherPtr> p_Pointer)     const { return Ptr(false) == p_Pointer.unsafePtr();      }

        public:
            Unity::MonoPtrHolder::Wrapper* m_Wrapper;

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    template<class t_Ptr>
    using CMonoPtrRef = const MonoPtr<t_Ptr>&;

    template<class t_Ptr>
    using MonoPtrRef = MonoPtr<t_Ptr>&;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    template<class t_Type>
    static bool IsUnityPtrValid(t_Type* p_Ptr)
    {
        if (!p_Ptr)
            return false;

        auto l_UObject = reinterpret_cast<UnityEngine::Object*>(p_Ptr);
        if (!l_UObject->___m_CachedPtr.m_value)
            return false;

        return true;
    }

}   ///< namespace CP_SDK::Utils

template <typename t_Left, typename t_Right> constexpr bool operator==(UnityW<t_Left> const& p_Left, CP_SDK::Utils::MonoPtr<t_Right> const& p_Right) {
    return p_Left.isAlive() == p_Right.operator bool() && p_Left.unsafePtr() == p_Right.Ptr(false);
}