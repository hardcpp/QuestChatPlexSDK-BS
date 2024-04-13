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

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Check for mod precense by ID
    /// @param p_ModID ID of the mod
    bool Scoring::HasMod(std::string_view p_ModID)
    {
        auto l_AllMods = modloader::get_all();
        return std::ranges::find_if(l_AllMods, [p_ModID](const modloader::ModResult& p_ModResult) {
            if (!std::holds_alternative<modloader::ModData>(p_ModResult))
                return false;

            auto& l_Right = std::get<modloader::ModData>(p_ModResult).info.id;
            if (p_ModID.length() != l_Right.length())
                return false;

            for (auto l_I = 0; l_I < p_ModID.length(); ++l_I)
            {
                if (std::towlower(p_ModID[l_I]) != std::towlower(l_Right[l_I]))
                    return false;
            }

            return true;
        }) != l_AllMods.end();
    }

}   ///< namespace CP_SDK_BS::Game
