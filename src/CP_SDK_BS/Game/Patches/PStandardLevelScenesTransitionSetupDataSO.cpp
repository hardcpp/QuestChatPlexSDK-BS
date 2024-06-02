#include "CP_SDK_BS/Game/Patches/PStandardLevelScenesTransitionSetupDataSO.hpp"
#include "CP_SDK_BS/Game/Logic.hpp"
#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK/Utils/Il2cpp.hpp"

#include <GlobalNamespace/MenuTransitionsHelper.hpp>
#include <GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp>

using namespace GlobalNamespace;

namespace CP_SDK_BS::Game::Patches {

    static LevelData::Ptr s_PStandardLevelScenesTransitionSetupDataSO_LevelData;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Restore the level data (Fix for the new restart mechanic)
    /// @param p_LevelData Level data to restore
    void PStandardLevelScenesTransitionSetupDataSO::RestoreLevelData(const LevelData::Ptr& p_LevelData)
    {
        s_PStandardLevelScenesTransitionSetupDataSO_LevelData = p_LevelData;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes, &StandardLevelScenesTransitionSetupDataSO::InitAndSetupScenes,
        void, StandardLevelScenesTransitionSetupDataSO* __Instance,

        PlayerSpecificSettings* __a, StringW __b, bool __c)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes");
        Scoring::__SetScoreSaberIsInReplay(__Instance->get_gameMode() == u"Replay");
        StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes(__Instance, __a, __b, __c);

        try
        {
            s_PStandardLevelScenesTransitionSetupDataSO_LevelData = LevelData::Make();
            auto& l_LevelData = s_PStandardLevelScenesTransitionSetupDataSO_LevelData;
            l_LevelData->Type = LevelType::Solo;
            l_LevelData->Data = __Instance->get_gameplayCoreSceneSetupData();

            Logic::FireLevelStarted(l_LevelData);
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit StandardLevelScenesTransitionSetupDataSO_InitAndSetupScenes");
    }
    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        StandardLevelScenesTransitionSetupDataSO_Finish, &StandardLevelScenesTransitionSetupDataSO::Finish,
        void, StandardLevelScenesTransitionSetupDataSO* __Instance,

        LevelCompletionResults* __a)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter StandardLevelScenesTransitionSetupDataSO_Finish");
        StandardLevelScenesTransitionSetupDataSO_Finish(__Instance, __a);

        try
        {
            auto& l_LevelData = s_PStandardLevelScenesTransitionSetupDataSO_LevelData;
            if (!l_LevelData)
                return;

            auto l_LevelCompletionData = LevelCompletionData::Make();
            l_LevelCompletionData->Type     = LevelType::Solo;
            l_LevelCompletionData->Data     = l_LevelData->Data;
            l_LevelCompletionData->Results  = __a;

            Scoring::__SetScoreSaberIsInReplay(false);

            Logic::FireLevelEnded(l_LevelCompletionData);
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][StandardLevelScenesTransitionSetupDataSO_Finish] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        s_PStandardLevelScenesTransitionSetupDataSO_LevelData = nullptr;

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit StandardLevelScenesTransitionSetupDataSO_Finish");
    }

}   ///< namespace CP_SDK_BS::Game::Patches
