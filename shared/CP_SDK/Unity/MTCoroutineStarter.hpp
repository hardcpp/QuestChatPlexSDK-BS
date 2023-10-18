#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <custom-types/shared/coroutine.hpp>
#include <System/Collections/IEnumerator.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/Coroutine.hpp>

#include <mutex>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace System::Collections;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief MultiThreading coroutine starter
    class MTCoroutineStarter : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.Unity", MTCoroutineStarter, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(MTCoroutineStarter);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(MTCoroutineStarter);

        private:
            /// @brief Queue class
            struct Queue
            {
                _v::MonoPtr<Il2CppObject>*  Data;
                int                         WritePos;
            };

        private:
            static MTCoroutineStarter*  m_Instance;     /// Singleton

            static Queue**      m_Queues;           ///< Queues instance
            static bool         m_Queued;           ///< Have queued actions
            static int          m_FrontQueue;       ///< Current front queue
            static std::mutex   m_Mutex;            ///< Lock mutex

        public:
            /// @brief Unity GameObject initialize
            static void Initialize();
            /// @brief Stop
            static void Destroy();

        public:
            /// @brief Enqueue a new coroutine from thread
            /// @param p_Coroutine Coroutine to enqueue
            static void EnqueueFromThread(_u::IEnumerator* p_Coroutine);
            /// @brief Enqueue a new coroutine from thread
            /// @param p_Coroutine Coroutine to enqueue
            static void EnqueueFromThread(custom_types::Helpers::Coroutine&& p_Coroutine);
            /// @brief Start coroutine
            /// @param p_Coroutine Coroutine to enqueue
            static _u::Coroutine* Start(_u::IEnumerator* p_Coroutine);
            /// @brief Start coroutine
            /// @param p_Coroutine Coroutine to enqueue
            static _u::Coroutine* Start(custom_types::Helpers::Coroutine&& p_Coroutine);
            /// @brief Stop coroutine
            /// @param p_Coroutine Coroutine to stop
            static void Stop(_u::Coroutine* p_Coroutine);

        private:
            /// @brief Unity GameObject update
            DECLARE_INSTANCE_METHOD(void, Update);

    };

}   ///< namespace CP_SDK::Unity

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::Unity::MTCoroutineStarter);