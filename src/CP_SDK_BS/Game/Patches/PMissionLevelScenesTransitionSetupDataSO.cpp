#include "CP_SDK_BS/Game/Patches/PMissionLevelScenesTransitionSetupDataSO.hpp"
#include "CP_SDK_BS/Game/Logic.hpp"
#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK/Utils/Il2cpp.hpp"

#include <GlobalNamespace/MenuTransitionsHelper.hpp>
#include <GlobalNamespace/MissionLevelScenesTransitionSetupDataSO.hpp>
#include <GlobalNamespace/MissionCompletionResults.hpp>

using namespace GlobalNamespace;

namespace CP_SDK_BS::Game::Patches {

    static LevelData::Ptr s_PMissionLevelScenesTransitionSetupDataSO_LevelData;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Restore the level data (Fix for the new restart mechanic)
    /// @param p_LevelData Level data to restore
    void PMissionLevelScenesTransitionSetupDataSO::RestoreLevelData(const LevelData::Ptr& p_LevelData)
    {
        s_PMissionLevelScenesTransitionSetupDataSO_LevelData = p_LevelData;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        MenuTransitionsHelper_StartMissionLevel, &MenuTransitionsHelper::StartMissionLevel,
        void, MenuTransitionsHelper* __Instance,

        StringW                                                                                         __a,
        ByRef<BeatmapKey>                                                                               __b,
        BeatmapLevel*                                                                                   __c,
        ColorScheme*                                                                                    __d,
        GameplayModifiers*                                                                              __e,
        ::ArrayW<MissionObjective*, ::Array<MissionObjective*>*>                                        __f,
        PlayerSpecificSettings*                                                                         __g,
        EnvironmentsListModel*                                                                          __h,
        System::Action*                                                                                 __i,
        System::Action_2<UnityW<MissionLevelScenesTransitionSetupDataSO>, MissionCompletionResults*>*   __j,
        System::Action_2<UnityW<MissionLevelScenesTransitionSetupDataSO>, MissionCompletionResults*>*   __k)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter MenuTransitionsHelper_StartMissionLevel");
        Scoring::__SetScoreSaberIsInReplay(false);
        MenuTransitionsHelper_StartMissionLevel(__Instance, __a, __b, __c, __d, __e, __f, __g, __h, __i, __j, __k);

        try
        {
            s_PMissionLevelScenesTransitionSetupDataSO_LevelData = LevelData::Make();
            auto& l_LevelData = s_PMissionLevelScenesTransitionSetupDataSO_LevelData;
            l_LevelData->Type = LevelType::Solo;
            l_LevelData->Data = __Instance->____missionLevelScenesTransitionSetupData->get_gameplayCoreSceneSetupData();

            Logic::FireLevelStarted(l_LevelData);
        }
        catch(const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][MenuTransitionsHelper_StartMissionLevel] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit MenuTransitionsHelper_StartMissionLevel");
    }
    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        MissionLevelScenesTransitionSetupDataSO_Finish, &MissionLevelScenesTransitionSetupDataSO::Finish,
        void, MissionLevelScenesTransitionSetupDataSO* __Instance,

        MissionCompletionResults* __a)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter MissionLevelScenesTransitionSetupDataSO_Finish");
        MissionLevelScenesTransitionSetupDataSO_Finish(__Instance, __a);

        try
        {
            auto& l_LevelData = s_PMissionLevelScenesTransitionSetupDataSO_LevelData;
            if (!l_LevelData)
                return;

            auto l_LevelCompletionData = LevelCompletionData::Make();
            l_LevelCompletionData->Type     = LevelType::Solo;
            l_LevelCompletionData->Data     = l_LevelData->Data;
            l_LevelCompletionData->Results  = __a->___levelCompletionResults;

            Scoring::__SetScoreSaberIsInReplay(false);

            Logic::FireLevelEnded(l_LevelCompletionData);
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][MissionLevelScenesTransitionSetupDataSO_Finish] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        s_PMissionLevelScenesTransitionSetupDataSO_LevelData = nullptr;

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit MissionLevelScenesTransitionSetupDataSO_Finish");
    }

}   ///< namespace CP_SDK_BS::Game::Patches