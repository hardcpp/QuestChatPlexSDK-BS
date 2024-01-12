#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <conditional-dependencies/shared/main.hpp>
#include <scotland2/shared/modloader.h>
#include <GlobalNamespace/GameScenesManager.hpp>

namespace CP_SDK_BS::Game {

    bool                            Scoring::m_Init                    = false;

    bool                            Scoring::m_IsScoreSaberPresent     = false;
    bool                            Scoring::m_IsInScoreSaberReplay    = false;

    bool                            Scoring::m_IsBeatLeaderPresent     = false;
    Scoring::t_BeatLeaderIsInReplay Scoring::m_BeatLeaderIsInReplayFN;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    bool Scoring::IsScoreSaberPresent()
    {
        Init();
        return m_IsScoreSaberPresent;
    }
    bool Scoring::IsBeatLeaderPresent()
    {
        Init();
        return m_IsBeatLeaderPresent;
    }

    bool Scoring::IsInReplay()
    {
        return ScoreSaber_IsInReplay() || BeatLeader_IsInReplay();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Is in ScoreSaber replay
    bool Scoring::ScoreSaber_IsInReplay()
    {
        Init();

        if (!m_IsScoreSaberPresent)
            return false;

        return m_IsInScoreSaberReplay;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Is in BeatLeader replay
    bool Scoring::BeatLeader_IsInReplay()
    {
        Init();

        if (!m_IsBeatLeaderPresent || !m_BeatLeaderIsInReplayFN.has_value())
            return false;

        return m_BeatLeaderIsInReplayFN.value()();
    }
    /// @brief Warmup BeatLeader score submission
    void Scoring::BeatLeader_ManualWarmUpSubmission()
    {
        Init();

        /// Doesn't seem to be required on Quest
    }

    bool Scoring::HasMod(std::string id) {
        auto mods = modloader::get_all();
        return std::ranges::find_if(mods, [id](const modloader::ModResult& val) { return val.info.id == id; }) != mods.end();
    }

    /////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init scoring utils
    void Scoring::Init()
    {

        if (m_Init)
            return;

        m_IsScoreSaberPresent = HasMod("scoresaber");

        m_IsBeatLeaderPresent = HasMod("BeatLeader");
        if (m_IsBeatLeaderPresent)
            m_BeatLeaderIsInReplayFN = CondDeps::FindUnsafe<bool>("replay", "IsInReplay");

        m_Init = true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set score saber is in replay
    /// @param p_Is Is in replay?
    void Scoring::__SetScoreSaberIsInReplay(bool p_Is)
    {
        m_IsInScoreSaberReplay = p_Is;
    }

}   ///< namespace CP_SDK_BS::Game
