#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

namespace CP_SDK_BS::Game {

    /// @brief Scoring utils
    class Scoring
    {
        CP_SDK_NO_DEF_CTORS(Scoring);

        using t_BeatLeaderIsInReplay = std::optional<bool(*)()>;
        private:
            static bool                     m_Init;

            static bool                     m_IsScoreSaberPresent;
            static bool                     m_IsInScoreSaberReplay;

            static bool                     m_IsBeatLeaderPresent;
            static t_BeatLeaderIsInReplay   m_BeatLeaderIsInReplayFN;

        public:
            static bool HasMod(std::string id);

        public:
            static bool IsScoreSaberPresent();
            static bool IsBeatLeaderPresent();

            static bool IsInReplay();

        public:
            /// @brief Is in ScoreSaber replay
            static bool ScoreSaber_IsInReplay();

        public:
            /// @brief Is in BeatLeader replay
            static bool BeatLeader_IsInReplay();
            /// @brief Warmup BeatLeader score submission
            static void BeatLeader_ManualWarmUpSubmission();

        public:
            /// @brief Init scoring utils
            static void Init();

        public:
            /// @brief Set score saber is in replay
            /// @param p_Is Is in replay?
            static void __SetScoreSaberIsInReplay(bool p_Is);

    };

}   ///< namespace CP_SDK_BS::Game
