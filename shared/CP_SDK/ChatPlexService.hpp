#pragma once

#include "Network/JsonRPCResult.hpp"
#include "Network/JsonRPCClient.hpp"
#include "Utils/Event.hpp"
#include "Utils/Il2cpp.hpp"
#include "Utils/Delegate.hpp"

#include <beatsaber-hook/shared/utils/il2cpp-utils.hpp>
#include <string>
#include <string_view>

namespace CP_SDK {

    namespace _u
    {
        using namespace il2cpp_utils;
        using namespace System;
        using namespace UnityEngine;
    }

    namespace _v
    {
        using namespace CP_SDK::Network;
        using namespace CP_SDK::Utils;
    }

    /// @brief ChatPlexService
    class CP_SDK_EXPORT ChatPlexService
    {
        CP_SDK_NO_DEF_CTORS(ChatPlexService);

        public:
            enum class EState
            {
                Disconnected,
                LinkRequest,
                LinkWait,
                Connecting,

                Connected,

                Error
            };

        private:
            static bool                             m_ThreadCondition;
            static _u::il2cpp_aware_thread*         m_Thread;
            static EState                           m_State;
            static _v::WebClientCore::Ptr           m_WebClientCore;
            static _v::JsonRPCClient::Ptr           m_JsonRPCClient;
            static std::u16string                   m_LinkRequestID;
            static std::u16string                   m_LinkCode;
            static std::u16string                   m_LastError;
            static std::u16string                   m_ActiveSubscription;
            static std::vector<std::u16string>      m_UnlockedFeatures;
            static std::queue<_v::Action<>>         m_OnTokenReadyQueue;
            static std::mutex                       m_OnTokenReadyQueueMutex;
            static std::u16string                   m_DeviceName;

        public:
            static const EState                             State();
            static const std::u16string_view                Token();
            static const std::u16string_view                LinkCode();
            static const std::u16string_view                LastError();
            static const std::u16string_view                ActiveSubscription();
            static const std::vector<const std::u16string>& UnlockedFeatures();

            static _v::Event<EState, EState> StateChanged;

        public:
            /// @brief Init the service
            static void Init();
            /// @brief Release the service
            static void Release();

        public:
            /// @brief Add a callback to be called when the token is ready (call immediatly if ready)
            /// @param action Callback to be caled
            static void OnTokenReady(_v::CActionRef<> action);

        public:
            /// @brief Start linking procedure
            static void StartLinking();
            /// @brief Stop linking procedure
            static void StopLinking();
            /// @brief Refresh the session
            static void Refresh();
            /// @brief Disconnect and erase the saved connected application token
            static void Disconnect();

        private:
            /// @brief Change state and notify listenners
            static void ChangeState(const EState newState);
            /// @brief Fire on token ready actions
            static void FireOnTokenReady();

        private:
            /// @brief Thread function
            static void ThreadRunner();

        private:
            /// @brief Is RPC call result a success result?
            /// @param rpcResult Result of the RPC command
            /// @return True if success
            static bool IsRPCSuccess(_v::JsonRPCResult::Ptr& rpcResult);

        private:
            /// @brief When we are Authed
            /// @param rpcResult Result of the RPC command
            static void OnAuthed(_v::JsonRPCResult::Ptr& rpcResult);
            /// @brief On error received
            /// @param rpcResult Result of the RPC command
            static void OnError(_v::JsonRPCResult::Ptr& rpcResult);

    };

}   ///< namespace CP_SDK