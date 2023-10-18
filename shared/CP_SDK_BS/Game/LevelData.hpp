#pragma once

#include "LevelType.hpp"
#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <memory>

#include <GlobalNamespace/GameplayCoreSceneSetupData.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace GlobalNamespace;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Level data instance
    class LevelData
    {
        DISABLE_CONSTRUCTORS(LevelData);
        PRIVATE_TAG();

        public:
            LevelType                                   Type;
            _v::MonoPtr<_u::GameplayCoreSceneSetupData> Data;
            int                                         MaxMultipliedScore;
            bool                                        HasRotations();
            bool                                        IsNoodle();
            bool                                        IsChroma();
            bool                                        IsReplay;

        public:
            using Ptr = std::shared_ptr<LevelData>;

            /// @brief Constructor
            LevelData(PRIVATE_TAG_ARG());
            /// @brief Constructor
            static Ptr Make();

    };

}   ///< namespace CP_SDK_BS::Game
