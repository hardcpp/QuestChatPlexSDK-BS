#pragma once

#include "../Unity/PersistantSingleton.hpp"
#include "../Utils/Il2cpp.hpp"
#include "../ChatPlexSDK.hpp"

#include <System/Collections/Generic/Dictionary_2.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/Object.hpp>
#include <UnityEngine/Sprite.hpp>

#include <atomic>
#include <map>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace System::Collections::Generic;
        using namespace UnityEngine;
    }

    /// @brief Il2Cpp pointers holder
    class MonoPtrHolder : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.Unity", MonoPtrHolder, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(MonoPtrHolder);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(MonoPtrHolder);
        CP_SDK_UNITY_PERSISTANT_SINGLETON_NO_DESTROY_DECL(CP_SDK::Unity::MonoPtrHolder);

        public:
            /// @brief Ref counter wrapper
            class Wrapper
            {
                std::atomic_int32_t m_Count;

                public:
                    Il2CppObject* Ptr;

                    /// @brief Grab a reference to this wrapper
                    void Grab();
                    /// @brief Drop a reference to this wrapper
                    void Drop();
            };

        private:
            DECLARE_INSTANCE_FIELD(_u::List_1<Il2CppObject*>*, m_Pointers);

            static std::map<Il2CppObject*, Wrapper*> m_PointersToWrapper;

        public:
            /// @brief Get or register a new wrapper
            /// @param p_Pointer Il2Cpp object
            static Wrapper* GetOrRegister(Il2CppObject* p_Pointer);
            /// @brief Release a wrapper
            /// @param p_Pointer Il2Cpp object
            static void Release(Il2CppObject* p_Pointer);

    };

}   ///< namespace CP_SDK::Unity

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::Unity::MonoPtrHolder);