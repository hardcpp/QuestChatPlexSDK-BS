#pragma once

#include "LevelType.hpp"
#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <memory>

#include <GlobalNamespace/GameplayCoreSceneSetupData.hpp>
#include <GlobalNamespace/LevelCompletionResults.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace GlobalNamespace;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Level completion data
    class CP_SDK_EXPORT LevelCompletionData
    {
        CP_SDK_NO_DEF_CTORS(LevelCompletionData);
        CP_SDK_PRIV_TAG();

        public:
            LevelType                                   Type;
            _v::MonoPtr<_u::GameplayCoreSceneSetupData> Data;
            _v::MonoPtr<_u::LevelCompletionResults>     Results;
            int                                         MaxMultipliedScore;
            bool                                        IsNoodle();
            bool                                        IsChroma();
            bool                                        IsReplay;

        public:
            using Ptr = std::shared_ptr<LevelCompletionData>;

            /// @brief Constructor
            LevelCompletionData(CP_SDK_PRIV_TAG_ARG());
            /// @brief Constructor
            static Ptr Make();

    };

}   ///< namespace CP_SDK_BS::Game
