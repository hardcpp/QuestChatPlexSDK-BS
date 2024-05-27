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
    class CP_SDK_EXPORT_VISIBILITY LevelData
    {
        CP_SDK_NO_DEF_CTORS(LevelData);
        CP_SDK_PRIV_TAG();

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
            LevelData(CP_SDK_PRIV_TAG_ARG());
            /// @brief Constructor
            static Ptr Make();

    };

}   ///< namespace CP_SDK_BS::Game
