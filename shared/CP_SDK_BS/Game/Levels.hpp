#pragma once

#include "../../CP_SDK/Utils/Delegate.hpp"
#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <map>

#include <custom-types/shared/coroutine.hpp>

#include <GlobalNamespace/BeatmapCharacteristicCollection.hpp>
#include <GlobalNamespace/BeatmapCharacteristicSO.hpp>
#include <GlobalNamespace/BeatmapDifficulty.hpp>
#include <GlobalNamespace/BeatmapLevelsModel.hpp>
#include <GlobalNamespace/ColorScheme.hpp>
#include <GlobalNamespace/BeatmapLevel.hpp>
#include <GlobalNamespace/LevelCompletionResults.hpp>
#include <GlobalNamespace/MenuTransitionsHelper.hpp>
#include <GlobalNamespace/EnvironmentsListModel.hpp>
#include <GlobalNamespace/SimpleLevelStarter.hpp>
#include <GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp>
#include <System/Threading/CancellationTokenSource.hpp>
#include <UnityEngine/Sprite.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace GlobalNamespace;
        using namespace System::Threading;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Level helper
    class CP_SDK_EXPORT_VISIBILITY Levels
    {
        CP_SDK_NO_DEF_CTORS(Levels);

        private:
            static _v::MonoPtr<_u::Sprite>                              m_DefaultPackCover;

            static _v::MonoPtr<_u::BeatmapCharacteristicCollection>     m_BeatmapCharacteristicCollection;
            static _v::MonoPtr<_u::BeatmapLevelsModel>                  m_BeatmapLevelsModel;
            static _v::MonoPtr<_u::CancellationTokenSource>             m_GetLevelCancellationTokenSource;
            static _v::MonoPtr<_u::CancellationTokenSource>             m_GetLevelEntitlementStatusTokenSource;
            static _v::MonoPtr<_u::MenuTransitionsHelper>               m_MenuTransitionsHelper;
            static _v::MonoPtr<_u::SimpleLevelStarter>                  m_SimpleLevelStarter;

            static bool                                                 m_ReloadSongsInitialized;
            static std::vector<_v::Action<>>                            m_ReloadSongsCallbacks;
            static std::mutex                                           m_ReloadSongsCallbacksMutex;

        public:
            /// @brief Get default pack cover
            static _u::Sprite* GetDefaultPackCover();

        public:
            /// @brief Reload songs
            /// @param p_Full     Full reload?
            /// @param p_Callback On finish callback
            static void ReloadSongs(bool p_Full, _v::CActionRef<> p_Callback = nullptr);
            /// @brief Check for mapping capability
            /// @param p_Capability Capability name
            /// @return True or false
            static bool HasMappingCapability(std::u16string_view p_Capability);

        public:
            /// @brief Sanitize a level ID for case matching
            /// @param p_LevelID Input level ID
            /// @return Sanitized level ID
            static std::u16string SanitizeLevelID(std::u16string_view p_LevelID);
            /// @brief Try get hash from level ID
            /// @param p_LevelID Input level ID
            /// @param p_Hash    OUT hash
            /// @return true or false
            static bool TryGetHashFromLevelID(std::u16string_view p_LevelID, std::u16string* p_Hash);
            /// @brief Try get level ID from hash
            /// @param p_Hash    Input hash
            /// @param p_LevelID OUT level ID
            /// @return true or false
            static bool TryGetLevelIDFromHash(std::u16string_view p_Hash, std::u16string* p_LevelID);
            /// @brief Is level ID a custom level ID
            /// @param p_LevelID Input level ID
            /// @return true or false
            static bool LevelID_IsCustom(std::u16string_view p_LevelID);

        public:
            /// @brief Try get BeatmapCharacteristicSO by serialized name
            /// @param p_SerializedName             Characteristic serialized name
            /// @param p_BeatmapCharacteristicSO    OUT BeatmapCharacteristicSO
            /// @return true or false
            static bool TryGetBeatmapCharacteristicSOBySerializedName(std::u16string_view p_SerializedName, _u::BeatmapCharacteristicSO** p_BeatmapCharacteristicSO);
            /// @brief Sanitize BeatmapCharacteristicSO serialized name
            /// @param p_SerializedName Input serialized name
            /// @return Sanatized BeatmapCharacteristicSO serialized name or input
            static std::u16string_view SanitizeBeatmapCharacteristicSOSerializedName(std::u16string_view p_SerializedName);
            /// @brief Get ordering value for a BeatmapCharacteristicSO
            /// @param p_SerializedName Characteristic serialized name
            /// @return Sorting order or 1000
            static int GetBeatmapCharacteristicSOOrdering(std::u16string_view p_SerializedName);

        public:
            /// @brief BeatmapDifficulty to BeatmapDifficulty enum name
            /// @param p_BeatmapDifficulty BeatmapDifficulty
            /// @return BeatmapDifficulty enum name or "? ExpertPlus ?"
            static std::u16string_view BeatmapDifficultyToBeatmapDifficultyEnumName(_u::BeatmapDifficulty p_BeatmapDifficulty);
            /// @brief BeatmapDifficulty serialized name to difficulty name
            /// @param p_BeatmapDifficultySerializedName BeatmapDifficulty serialized name
            /// @return Difficulty name
            static std::u16string_view BeatmapDifficultySerializedNameToDifficultyName(std::u16string_view p_BeatmapDifficultySerializedName);
            /// @brief BeatmapDifficulty serialized name to difficulty name short
            /// @param p_BeatmapDifficultySerializedName BeatmapDifficulty serialized name
            /// @return Difficulty name short
            static std::u16string_view BeatmapDifficultySerializedNameToDifficultyNameShort(std::u16string_view p_BeatmapDifficultySerializedName);
            /// @brief BeatmapDifficulty serialized name to BeatmapDifficulty
            /// @param p_BeatmapDifficultySerializedName BeatmapDifficulty serialized name
            /// @return BeatmapDifficulty
            static _u::BeatmapDifficulty BeatmapDifficultySerializedNameToBeatmapDifficulty(std::u16string_view p_BeatmapDifficultySerializedName);

        public:
            /// @brief Own a DLC level by level ID
            /// @param p_LevelID  Level ID
            /// @param p_Callback Callback for success/failure
            static void OwnDLCLevelByLevelID(std::u16string p_LevelID, _v::Action<bool> p_Callback);

        public:
            /// @brief Try to get BeatmapLevel by level ID
            /// @param p_LevelID      ID of the level
            /// @param p_BeatmapLevel OUT Found BeatmapLevel or nullptr
            /// @return true or false
            static bool TryGetBeatmapLevelForLevelID(std::u16string_view p_LevelID, _u::BeatmapLevel** p_BeatmapLevel);
            /// @brief Try to get BeatmapLevel by hash
            /// @param p_Hash         Hash of the level
            /// @param p_BeatmapLevel OUT Found BeatmapLevel or nullptr
            /// @return true or false
            static bool TryGetBeatmapLevelForHash(std::u16string_view p_Hash, _u::BeatmapLevel** p_BeatmapLevel);
            /// @brief For each of BeatmapKey for a BeatmapLevel
            /// @param p_BeatmapLevel Input beatmap level
            /// @param p_Functor      Functor for each element, return true mean we continue iterating
            static void BeatmapLevel_ForEachBeatmapKey(_u::BeatmapLevel* p_BeatmapLevel, _v::CFuncRef<bool, const _u::BeatmapKey&> p_Functor);

        public:
            /// @brief Check if a difficulty is present in a BeatmapLevel
            /// @param p_BeatmapLevel            Input beatmap level
            /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
            /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
            /// @return True or false
            static bool BeatmapLevel_HasDifficulty( _u::BeatmapLevel*               p_BeatmapLevel,
                                                    _u::BeatmapCharacteristicSO*    p_BeatmapCharacteristicSO,
                                                    _u::BeatmapDifficulty           p_BeatmapDifficulty);
            /// @brief Try get a beatmap key from a BeatmapLevel
            /// @param p_BeatmapLevel            Input beatmap level
            /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
            /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
            /// @param p_BeatmapKey              Out beatmap key
            /// @return True or false
            static bool BeatmapLevel_TryGetBeatmapKey(  _u::BeatmapLevel*               p_BeatmapLevel,
                                                        _u::BeatmapCharacteristicSO*    p_BeatmapCharacteristicSO,
                                                        _u::BeatmapDifficulty           p_BeatmapDifficulty,
                                                        _u::BeatmapKey*                 p_BeatmapKey);
            /// @brief Try get custom requirements for a BeatmapLevel->BeatmapCharacteristicSO->BeatmapDifficulty
            /// @param p_PreviewBeatmapLevel     Input preview beatmap level
            /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
            /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
            /// @param p_CustomRequirements      OUT custom requirements
            /// @return true or false
            static bool TryGetCustomRequirementsFor(_u::BeatmapLevel*               p_PreviewBeatmapLevel,
                                                    _u::BeatmapCharacteristicSO*    p_BeatmapCharacteristicSO,
                                                    _u::BeatmapDifficulty           p_BeatmapDifficulty,
                                                    std::vector<std::u16string>*    p_CustomRequirements);

        public:
            /// @brief Try to load BeatmapLevel cover image async
            /// @param p_BeatmapLevel Input BeatmapLevel
            /// @param p_Callback     Callback
            static void TryLoadBeatmapLevelCoverAsync(_u::BeatmapLevel* p_BeatmapLevel, _v::Action<bool, _u::Sprite*> p_Callback);

        public:
            /// @brief Load a BeatmapLevelData by level ID
            /// @param p_LevelID      ID of the level
            /// @param p_LoadCallback Load callback
            static void LoadBeatmapLevelDataByLevelID(  std::u16string                                                                              p_LevelID,
                                                        _v::Action<_v::MonoPtr<_u::BeatmapLevel, true>, _v::MonoPtr<_u::IBeatmapLevelData, true>>   p_LoadCallback);
            /// @brief Start a BeatmapLevel
            /// @param p_Level                       Loaded level
            /// @param p_Characteristic              Beatmap game mode
            /// @param p_Difficulty                  Beatmap difficulty
            /// @param p_BeatmapLevelData            Beatmap level data
            /// @param p_OverrideEnvironmentSettings Environment settings
            /// @param p_ColorScheme                 Color scheme
            /// @param p_GameplayModifiers           Modifiers
            /// @param p_PlayerSettings              Player settings
            /// @param p_SongFinishedCallback        Callback when the song is finished
            /// @param p_MenuButtonText              Menu button text
            static void StartBeatmapLevel(  _u::BeatmapLevel*                   p_Level,
                                            _u::BeatmapCharacteristicSO*        p_Characteristic,
                                            _u::BeatmapDifficulty               p_Difficulty,
                                            _u::IBeatmapLevelData*              p_BeatmapLevelData,
                                            _u::OverrideEnvironmentSettings*    p_OverrideEnvironmentSettings   = nullptr,
                                            _u::ColorScheme*                    p_ColorScheme                   = nullptr,
                                            _u::GameplayModifiers*              p_GameplayModifiers             = nullptr,
                                            _u::PlayerSpecificSettings*         p_PlayerSettings                = nullptr,
                                            _v::Action<_u::StandardLevelScenesTransitionSetupDataSO*, _u::LevelCompletionResults*> p_SongFinishedCallback = nullptr,
                                            std::u16string_view                 p_MenuButtonText                = u"Menu");

        private:
            /// @brief Load IBeatmapLevelData from a level ID
            /// @param p_LevelID  Level ID
            /// @param p_Callback Callback for success/failure
            static void LoadIBeatmapLevelDataAsync( std::u16string                                          p_LevelID,
                                                    _v::Action<_v::MonoPtr<_u::IBeatmapLevelData, true>>    p_Callback);

        public:
            /// @brief Get accuracy
            /// @param p_MaxScore Max score
            /// @param p_Score    Result score
            static float GetAccuracy(int p_MaxScore, int p_Score);

        public:
            using t_CharacteristicScores = std::vector<std::pair<_u::BeatmapDifficulty, int>>;
            using t_Scores               = std::map<_v::MonoPtr<_u::BeatmapCharacteristicSO>, t_CharacteristicScores>;

            /// @brief Get scores from local cache for a level id
            /// @param p_LevelID       Level ID
            /// @param p_HaveAnyScore  OUT Have any score set
            /// @param p_HaveAllScores OUT Have all scores set
            /// @return Scores
            static t_Scores GetScoresByLevelID(std::u16string_view p_LevelID, bool* p_HaveAnyScore, bool* p_HaveAllScores);

    };

}   ///< namespace CP_SDK_BS::Game
