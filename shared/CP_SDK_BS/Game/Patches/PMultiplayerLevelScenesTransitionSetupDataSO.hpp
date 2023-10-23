#pragma once

#include "../LevelData.hpp"
#include "../../../CP_SDK/Utils/Il2cpp.hpp"

namespace CP_SDK_BS::Game::Patches {

    /// @brief Level data finder
    class PMultiplayerLevelScenesTransitionSetupDataSO
    {
        CP_SDK_NO_DEF_CTORS(PMultiplayerLevelScenesTransitionSetupDataSO);

        public:
            /// @brief Restore the level data (Fix for the new restart mechanic)
            /// @param p_LevelData Level data to restore
            static void RestoreLevelData(const LevelData::Ptr& p_LevelData);

    };

}   ///< namespace CP_SDK_BS::Game::Patches
