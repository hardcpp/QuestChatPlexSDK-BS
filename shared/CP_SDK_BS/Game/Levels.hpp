#pragma once

#include "../../CP_SDK/Utils/Delegate.hpp"
#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <map>

#include <custom-types/shared/coroutine.hpp>

#include <GlobalNamespace/AdditionalContentModel.hpp>
#include <GlobalNamespace/BeatmapCharacteristicCollection.hpp>
#include <GlobalNamespace/BeatmapCharacteristicSO.hpp>
#include <GlobalNamespace/BeatmapDifficulty.hpp>
#include <GlobalNamespace/BeatmapLevelsModel.hpp>
#include <GlobalNamespace/ColorScheme.hpp>
#include <GlobalNamespace/BeatmapLevel.hpp>
#include <GlobalNamespace/LevelCompletionResults.hpp>
#include <GlobalNamespace/MenuTransitionsHelper.hpp>
#include <GlobalNamespace/EnvironmentsListModel.hpp>
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
    class Levels
    {
        CP_SDK_NO_DEF_CTORS(Levels);

        private:
            static _v::MonoPtr<_u::Sprite>                              m_DefaultPackCover;

            static _v::MonoPtr<_u::AdditionalContentModel>              m_AdditionalContentModel;
            static _v::MonoPtr<_u::BeatmapCharacteristicCollection>   m_BeatmapCharacteristicCollection;
            static _v::MonoPtr<_u::BeatmapLevelsModel>                  m_BeatmapLevelsModel;
            static _v::MonoPtr<_u::CancellationTokenSource>             m_GetLevelCancellationTokenSource;
            static _v::MonoPtr<_u::CancellationTokenSource>             m_GetLevelEntitlementStatusTokenSource;
            static _v::MonoPtr<_u::MenuTransitionsHelper>               m_MenuTransitionsHelper;
            static _v::MonoPtr<_u::EnvironmentsListModel>                   m_EnvironmentsListModel;

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
            /// @brief Is level ID a custom level ID
            /// @param p_LevelID Input level ID
            /// @return true or false
            static bool LevelID_IsCustom(std::u16string_view p_LevelID);
            /// @brief Try get hash from level ID
            /// @param p_LevelID Input level ID
            /// @param p_Hash    OUT hash
            /// @return true or false
            static bool TryGetHashFromLevelID(std::u16string_view p_LevelID, std::u16string* p_Hash);
            /// @brief Sanitize a level ID for case matching
            /// @param p_LevelID Input level ID
            /// @return Sanitized level ID
            static std::u16string SanitizeLevelID(std::u16string_view p_LevelID);

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
            static void OwnDLCLevelByLevelID(std::u16string_view p_LevelID, _v::Action<bool> p_Callback);
            /// @brief Try to get PreviewBeatmapLevel by level ID
            /// @param p_LevelID             ID of the level
            /// @param p_PreviewBeatmapLevel OUT Found PreviewBeatmapLevel or nullptr
            /// @return true or false
            static bool TryGetPreviewBeatmapLevelForLevelID(std::u16string_view p_LevelID, _u::BeatmapLevel** p_PreviewBeatmapLevel);
            /// @brief Try get custom requirements for a BeatmapLevel->BeatmapCharacteristicSO->BeatmapDifficulty
            /// @param p_PreviewBeatmapLevel     Input preview beatmap level
            /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
            /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
            /// @param p_CustomRequirements      OUT custom requirements
            /// @return true or false
            static bool TryGetCustomRequirementsFor(_u::BeatmapLevel*       p_PreviewBeatmapLevel,
                                                    _u::BeatmapCharacteristicSO*    p_BeatmapCharacteristicSO,
                                                    _u::BeatmapDifficulty           p_BeatmapDifficulty,
                                                    std::vector<std::u16string>*    p_CustomRequirements);
            /// @brief Load a BeatmapLevel by level ID
            /// @param p_LevelID      ID of the level
            /// @param p_LoadCallback Load callback
            static void LoadBeatmapLevelByLevelID(  std::u16string_view p_LevelID,
                                                    _v::Action<_v::MonoPtr<_u::BeatmapLevel, true>> p_LoadCallback);
            /// @brief Try to load PreviewBeatmapLevel cover image async
            /// @param p_PreviewBeatmapLevel Input PreviewBeatmapLevel
            /// @param p_Callback            Callback
            static void TryLoadPreviewBeatmapLevelCoverAsync(_u::BeatmapLevel* p_PreviewBeatmapLevel, _v::Action<bool, _u::Sprite*> p_Callback);
            /// @brief Start a BeatmapLevel
            /// @param p_Level                       Loaded level
            /// @param p_Characteristic              Beatmap game mode
            /// @param p_Difficulty                  Beatmap difficulty
            /// @param p_OverrideEnvironmentSettings Environment settings
            /// @param p_ColorScheme                 Color scheme
            /// @param p_GameplayModifiers           Modifiers
            /// @param p_PlayerSettings              Player settings
            /// @param p_SongFinishedCallback        Callback when the song is finished
            /// @param p_MenuButtonText              Menu button text
            static void StartBeatmapLevel(  _u::BeatmapLevel*                  p_Level,
                                            _u::BeatmapCharacteristicSO*        p_Characteristic,
                                            _u::BeatmapDifficulty               p_Difficulty,
                                            _u::OverrideEnvironmentSettings*    p_OverrideEnvironmentSettings   = nullptr,
                                            _u::ColorScheme*                    p_ColorScheme                   = nullptr,
                                            _u::GameplayModifiers*              p_GameplayModifiers             = nullptr,
                                            _u::PlayerSpecificSettings*         p_PlayerSettings                = nullptr,
                                            _v::Action<_u::StandardLevelScenesTransitionSetupDataSO*, _u::LevelCompletionResults*, _u::BeatmapKey> p_SongFinishedCallback = nullptr,
                                            std::u16string_view                 p_MenuButtonText                = u"Menu");

        private:
            /// @brief Get a BeatmapLevel from a level ID
            /// @param p_LevelID  Level ID
            /// @param p_Callback Callback for success/failure
            static void GetBeatmapLevelFromLevelID( std::u16string_view                                                 p_LevelID,
                                                    _v::Action<_v::MonoPtr<_u::BeatmapLevel, true>>  p_Callback);

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
            static t_Scores GetScoresByLevelID(std::u16string_view p_SongHash, bool* p_HaveAnyScore, bool* p_HaveAllScores);

    };

}   ///< namespace CP_SDK_BS::Game
