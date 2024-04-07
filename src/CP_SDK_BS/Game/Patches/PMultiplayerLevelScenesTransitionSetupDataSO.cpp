#include "CP_SDK_BS/Game/Patches/PMultiplayerLevelScenesTransitionSetupDataSO.hpp"
#include "CP_SDK_BS/Game/Logic.hpp"
#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK/Utils/Il2cpp.hpp"

#include <GlobalNamespace/MultiplayerLevelCompletionResults.hpp>
#include <GlobalNamespace/MultiplayerLevelScenesTransitionSetupDataSO.hpp>
#include <GlobalNamespace/MultiplayerPlayerResultsData.hpp>
#include <GlobalNamespace/MultiplayerResultsData.hpp>

using namespace GlobalNamespace;

namespace CP_SDK_BS::Game::Patches {

    static LevelData::Ptr s_PMultiplayerLevelScenesTransitionSetupDataSO_LevelData;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Restore the level data (Fix for the new restart mechanic)
    /// @param p_LevelData Level data to restore
    void PMultiplayerLevelScenesTransitionSetupDataSO::RestoreLevelData(const LevelData::Ptr& p_LevelData)
    {
        s_PMultiplayerLevelScenesTransitionSetupDataSO_LevelData = p_LevelData;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /*CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        MultiplayerLevelScenesTransitionSetupDataSO_Init, &MultiplayerLevelScenesTransitionSetupDataSO::Init,
        void, MultiplayerLevelScenesTransitionSetupDataSO* __Instance,

        StringW             __a, BeatmapLevel* __b, BeatmapDifficulty  __c, BeatmapCharacteristicSO* __d,
        BeatmapKey __e, ColorScheme*          __f, GameplayModifiers* __g, PlayerSpecificSettings*  __h,
        PracticeSettings*   __i, bool                  __j)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter MultiplayerLevelScenesTransitionSetupDataSO_Init");
        Scoring::__SetScoreSaberIsInReplay(false);
        MultiplayerLevelScenesTransitionSetupDataSO_Init(__Instance, __a, __b, __c, __d, __e, __f, __g, __h, __i, __j);

        try
        {
            s_PMultiplayerLevelScenesTransitionSetupDataSO_LevelData = LevelData::Make();
            auto& l_LevelData = s_PMultiplayerLevelScenesTransitionSetupDataSO_LevelData;
            l_LevelData->Type = LevelType::Multiplayer;
            l_LevelData->Data = __Instance->gameplayCoreSceneSetupData;

            Logic::FireLevelStarted(l_LevelData);
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][MultiplayerLevelScenesTransitionSetupDataSO_Init] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit MultiplayerLevelScenesTransitionSetupDataSO_Init");
    }
    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        MultiplayerLevelScenesTransitionSetupDataSO_Finish, &MultiplayerLevelScenesTransitionSetupDataSO::Finish,
        void, MultiplayerLevelScenesTransitionSetupDataSO* __Instance,

        MultiplayerResultsData* __a)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter MultiplayerLevelScenesTransitionSetupDataSO_Finish");
        MultiplayerLevelScenesTransitionSetupDataSO_Finish(__Instance, __a);

        try
        {
            auto& l_LevelData = s_PMultiplayerLevelScenesTransitionSetupDataSO_LevelData;
            if (!l_LevelData)
                return;

            auto l_LevelCompletionData = LevelCompletionData::Make();
            l_LevelCompletionData->Type     = LevelType::Multiplayer;
            l_LevelCompletionData->Data     = l_LevelData->Data;

            if (__a && __a->get_localPlayerResultData())
            {
                auto l_LocalResultData = __a->get_localPlayerResultData();

                if (l_LocalResultData->multiplayerLevelCompletionResults)
                    l_LevelCompletionData->Results = l_LocalResultData->multiplayerLevelCompletionResults->levelCompletionResults;
            }

            Scoring::__SetScoreSaberIsInReplay(false);

            Logic::FireLevelEnded(l_LevelCompletionData);
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][MultiplayerLevelScenesTransitionSetupDataSO_Finish] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        s_PMultiplayerLevelScenesTransitionSetupDataSO_LevelData = nullptr;

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit MultiplayerLevelScenesTransitionSetupDataSO_Finish");
    }*/

}   ///< namespace CP_SDK_BS::Game::Patches