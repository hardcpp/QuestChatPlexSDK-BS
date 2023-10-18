#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <System/Threading/CancellationToken.hpp>
#include <UnityEngine/Sprite.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace System::Threading;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Player avatar picture provider
    class PlayerAvatarPicture
    {
        DISABLE_CONSTRUCTORS(PlayerAvatarPicture);

        public:
            using t_CallBack        = _v::CActionRef<_u::Sprite*>;
            using t_CallBackFailed  = _v::CActionRef<>;
            using t_Bytes           = ::Array<uint8_t>*;

        public:
            /// @brief Get player avatar picture
            /// @param p_PlayerID          ID of the player
            /// @param p_CancellationToken Cancellation token
            /// @param p_Callback          Request callback
            static void GetPlayerAvatarPicture(std::u16string p_PlayerID, _u::CancellationToken p_CancellationToken, t_CallBack p_Callback);

        private:
            /// @brief Get avatar picture from ScoreSaber
            /// @param p_PlayerID          ID of the player
            /// @param p_CancellationToken Cancellation token
            /// @param p_Callback          Request callback
            /// @param p_OnFailCallback    On error callback
            static void GetScoreSaberAvatarPicture(std::u16string p_PlayerID, _u::CancellationToken p_CancellationToken, t_CallBack p_Callback, t_CallBackFailed p_OnFailCallback);
            /// @brief Get avatar picture from BeatLeader
            /// @param p_PlayerID          ID of the player
            /// @param p_CancellationToken Cancellation token
            /// @param p_Callback          Request callback
            /// @param p_OnFailCallback    On error callback
            static void GetBeatLeaderAvatarPicture(std::u16string p_PlayerID, _u::CancellationToken p_CancellationToken, t_CallBack p_Callback, t_CallBackFailed p_OnFailCallback);

        private:
            /// @brief Process received avatar body bytes
            /// @param p_PlayerID  ID of the player
            /// @param p_Callback  Request callback
            /// @param p_BodyBytes Avatar bytes
            static void ProcessAvatarBytes(std::u16string p_PlayerID, t_CallBack p_Callback, t_Bytes p_BodyBytes);

    };

}   ///< namespace CP_SDK_BS::Game
