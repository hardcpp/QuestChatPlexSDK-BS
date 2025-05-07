#include "CP_SDK_BS/Game/Levels.hpp"
#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/Utils/Task.hpp"
#include "assets.hpp"

#include <custom-types/shared/delegate.hpp>
#include <songcore/shared/SongCore.hpp>

#include <GlobalNamespace/RecordingToolManager.hpp>
#include <GlobalNamespace/BeatmapLevel.hpp>
#include <GlobalNamespace/BeatmapLevelSO.hpp>
#include <GlobalNamespace/BeatmapLevelsEntitlementModel.hpp>
#include <GlobalNamespace/BeatmapLevelsModel.hpp>
#include <GlobalNamespace/CustomLevelLoader.hpp>
#include <GlobalNamespace/GameplayModifiers.hpp>
#include <GlobalNamespace/IPreviewMediaData.hpp>
#include <GlobalNamespace/MainFlowCoordinator.hpp>
#include <GlobalNamespace/PlayerData.hpp>
#include <GlobalNamespace/PlayerDataModel.hpp>
#include <GlobalNamespace/PlayerDataFileModel.hpp>
#include <GlobalNamespace/PlayerLevelStatsData.hpp>
#include <GlobalNamespace/PlayerSpecificSettings.hpp>
#include <GlobalNamespace/SimpleLevelStarter.hpp>
#include <GlobalNamespace/StandardLevelDetailViewController.hpp>
#include <GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp>
#include <System/Collections/Generic/IReadOnlyList_1.hpp>
#include <System/Threading/Tasks/Task_1.hpp>
#include <System/Threading/Tasks/TaskStatus.hpp>
#include <System/Action_2.hpp>
#include <System/Math.hpp>
#include <System/IO/File.hpp>
#include <System/IO/Path.hpp>
#include <System/Nullable_1.hpp>
#include <System/Type.hpp>
#include <System/Collections/IEnumerable.hpp>
#include <System/Collections/Generic/IEnumerable_1.hpp>
#include <System/Collections/Generic/IEnumerator_1.hpp>
#include <System/Collections/Generic/IReadOnlyCollection_1.hpp>
#include <UnityEngine/Resources.hpp>

using namespace GlobalNamespace;
using namespace System::Threading;
using namespace UnityEngine;

namespace CP_SDK_BS::Game {

    _v::MonoPtr<Sprite>                            Levels::m_DefaultPackCover;

    _v::MonoPtr<BeatmapCharacteristicCollection>   Levels::m_BeatmapCharacteristicCollection;
    _v::MonoPtr<BeatmapLevelsModel>                Levels::m_BeatmapLevelsModel;
    _v::MonoPtr<CancellationTokenSource>           Levels::m_GetLevelCancellationTokenSource;
    _v::MonoPtr<CancellationTokenSource>           Levels::m_GetLevelEntitlementStatusTokenSource;
    _v::MonoPtr<MenuTransitionsHelper>             Levels::m_MenuTransitionsHelper;
    _v::MonoPtr<SimpleLevelStarter>                Levels::m_SimpleLevelStarter;

    bool                                           Levels::m_ReloadSongsInitialized = false;
    std::vector<_v::Action<>>                      Levels::m_ReloadSongsCallbacks;
    std::mutex                                     Levels::m_ReloadSongsCallbacksMutex;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get default pack cover
    Sprite* Levels::GetDefaultPackCover()
    {
        if (!m_DefaultPackCover)
            m_DefaultPackCover = CP_SDK::Unity::SpriteU::CreateFromRaw(Assets::DefaultPackCover_png);

        return m_DefaultPackCover.Ptr(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Reload songs
    /// @param p_Full     Full reload?
    /// @param p_Callback On finish callback
    void Levels::ReloadSongs(bool p_Full, _v::CActionRef<> p_Callback)
    {
        if (p_Callback.IsValid())
        {
            //lock (m_ReloadSongsCallbacks)
            {
                std::lock_guard<std::mutex> l_Lock(m_ReloadSongsCallbacksMutex);
                m_ReloadSongsCallbacks.push_back(p_Callback);
            }
        }

        CP_SDK::Unity::MTMainThreadInvoker::Enqueue([p_Full]() -> void
        {
            if (!m_ReloadSongsInitialized)
            {
                m_ReloadSongsInitialized = true;

                SongCore::API::Loading::GetSongsLoadedEvent().addCallback([](std::span<::SongCore::SongLoader::CustomBeatmapLevel* const> p_Loaded) -> void {
                    CP_SDK::Unity::MTMainThreadInvoker::Enqueue([]() -> void
                    {
                        std::vector<_v::Action<>> l_Callbacks;
                        //lock (m_ReloadSongsCallbacks)
                        {
                            std::lock_guard<std::mutex> l_Lock(m_ReloadSongsCallbacksMutex);
                            l_Callbacks.insert(l_Callbacks.begin(), m_ReloadSongsCallbacks.begin(), m_ReloadSongsCallbacks.end());
                            m_ReloadSongsCallbacks.clear();
                        }

                        for (auto& l_Current : l_Callbacks)
                            l_Current();
                    });
                });
            }

            SongCore::API::Loading::RefreshSongs(p_Full);
        });
    }
    /// @brief Check for mapping capability
    /// @param p_Capability Capability name
    /// @return True or false
    bool Levels::HasMappingCapability(std::u16string_view p_Capability)
    {
        auto l_Capabilities = SongCore::API::Capabilities::GetRegisteredCapabilities();
        auto l_Sanatized    = SanatizeMappingCapability(p_Capability);
        CP_SDK::ChatPlexSDK::Logger()->Error(u"HasMappingCapability: " + l_Sanatized);
        for (auto& l_Capability : l_Capabilities)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"Mapping cap: " + CP_SDK::Utils::StrToU16Str(l_Capability));
            if (!CP_SDK::Utils::U16EqualsToCaseInsensitive(l_Sanatized, CP_SDK::Utils::StrToU16Str(l_Capability)))
                continue;

            return true;
        }

        return false;
    }
    /// @brief Sanatrize a mapping capability
    /// @param p_Capability Capability name
    /// @return Sanatized mapping capability
    std::u16string Levels::SanatizeMappingCapability(std::u16string_view p_Capability)
    {
        std::u16string l_Result;

        l_Result.reserve(p_Capability.size());
        for (auto l_Char : p_Capability)
        {
            if (l_Char == u' ')
                continue;

            l_Result.push_back(std::towlower(l_Char));
        }

        return l_Result;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Sanitize a level ID for case matching
    /// @param p_LevelID Input level ID
    /// @return Sanitized level ID
    std::u16string Levels::SanitizeLevelID(std::u16string_view p_LevelID)
    {
        std::u16string l_LevelHash;
        if (TryGetHashFromLevelID(p_LevelID, &l_LevelHash))
        {
            /// Level hash is sanitized by TryGetHashFromLevelID
            return u"custom_level_" + l_LevelHash;
        }

        return std::u16string(p_LevelID);
    }
    /// @brief Try get hash from level ID
    /// @param p_LevelID Input level ID
    /// @param p_Hash    OUT hash
    /// @return true or false
    bool Levels::TryGetHashFromLevelID(std::u16string_view p_LevelID, std::u16string* p_Hash)
    {
        if (p_Hash) p_Hash->clear();
        if (!LevelID_IsCustom(p_LevelID))
            return false;

        if (p_Hash)
        {
            *p_Hash = p_LevelID.substr(13);

            if (p_Hash->length() == 40) // TODO check for only hex
            {
                *p_Hash = p_Hash->substr(0, 40);
                std::transform(p_Hash->begin(), p_Hash->end(), p_Hash->begin(), std::towupper);
            }
        }

        return true;
    }
    /// @brief Try get level ID from hash
    /// @param p_Hash    Input hash
    /// @param p_LevelID OUT level ID
    /// @return true or false
    bool Levels::TryGetLevelIDFromHash(std::u16string_view p_Hash, std::u16string* p_LevelID)
    {
        if (p_LevelID) p_LevelID->clear();
        if (p_Hash.empty() || p_Hash.length() != 40)
            return false;

        if (p_LevelID)
        {
            *p_LevelID = u"custom_level_" + p_Hash;
            std::transform(std::next(p_LevelID->begin(), 13), p_LevelID->end(), std::next(p_LevelID->begin(), 13), std::towupper);
        }

        return true;
    }
    /// @brief Is level ID a custom level ID
    /// @param p_LevelID Input level ID
    /// @return true or false
    bool Levels::LevelID_IsCustom(std::u16string_view p_LevelID)
    {
        if (p_LevelID.length() < 13)
            return false;

        auto l_View = std::u16string_view(p_LevelID.data(), 13);
        return _v::U16EqualsToCaseInsensitive(l_View, u"custom_level_");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Try get BeatmapCharacteristicSO by serialized name
    /// @param p_SerializedName             Characteristic serialized name
    /// @param p_BeatmapCharacteristicSO    OUT BeatmapCharacteristicSO
    /// @return true or false
    bool Levels::TryGetBeatmapCharacteristicSOBySerializedName(std::u16string_view p_SerializedName, BeatmapCharacteristicSO** p_BeatmapCharacteristicSO)
    {
        if (p_BeatmapCharacteristicSO) *p_BeatmapCharacteristicSO = nullptr;

        if (p_SerializedName.length() == 0)
            return false;

        if (!m_BeatmapCharacteristicCollection)
        {
            auto l_PlayerDataModel = Resources::FindObjectsOfTypeAll<PlayerDataModel*>()->FirstOrDefault();
            if (l_PlayerDataModel)
                m_BeatmapCharacteristicCollection = l_PlayerDataModel->get_playerDataFileModel()->____beatmapCharacteristicCollection;
        }

        if (!m_BeatmapCharacteristicCollection)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetBeatmapCharacteristicSOBySerializedName] Invalid BeatmapCharacteristicCollectionSO");
            return false;
        }

        StringW                         l_SerializedName            = SanitizeBeatmapCharacteristicSOSerializedName(p_SerializedName);
        UnityW<BeatmapCharacteristicSO> l_BeatmapCharacteristicSO;
        bool                            l_Result                    = false;

        if (m_BeatmapCharacteristicCollection->____beatmapCharacteristicsBySerializedName->TryGetValue(l_SerializedName, byref(l_BeatmapCharacteristicSO)))
        {
            l_Result = true;

            if (p_BeatmapCharacteristicSO)
                *p_BeatmapCharacteristicSO = l_BeatmapCharacteristicSO.unsafePtr();
        }

        return l_Result;
    }
    /// @brief Sanitize BeatmapCharacteristicSO serialized name
    /// @param p_SerializedName Input serialized name
    /// @return Sanatized BeatmapCharacteristicSO serialized name or input
    std::u16string_view Levels::SanitizeBeatmapCharacteristicSOSerializedName(std::u16string_view p_SerializedName)
    {
        if (        _v::U16EqualsToCaseInsensitive(p_SerializedName, u"Standard"))
            return u"Standard";

        else if (   _v::U16EqualsToCaseInsensitive(p_SerializedName, u"One Saber")
                 || _v::U16EqualsToCaseInsensitive(p_SerializedName, u"OneSaber"))
            return u"OneSaber";

        else if (   _v::U16EqualsToCaseInsensitive(p_SerializedName, u"No Arrows")
                 || _v::U16EqualsToCaseInsensitive(p_SerializedName, u"NoArrows"))
            return u"NoArrows";

        else if (   _v::U16EqualsToCaseInsensitive(p_SerializedName, u"360Degree"))
            return u"360Degree";

        else if (   _v::U16EqualsToCaseInsensitive(p_SerializedName, u"Lawless"))
            return u"Lawless";

        else if (   _v::U16EqualsToCaseInsensitive(p_SerializedName, u"90Degree"))
            return u"90Degree";

        else if (   _v::U16EqualsToCaseInsensitive(p_SerializedName, u"LightShow")
                 || _v::U16EqualsToCaseInsensitive(p_SerializedName, u"Lightshow"))
            return u"Lightshow";

        return p_SerializedName;
    }
    /// @brief Get ordering value for a BeatmapCharacteristicSO
    /// @param p_SerializedName Characteristic serialized name
    /// @return Sorting order or 1000
    int Levels::GetBeatmapCharacteristicSOOrdering(std::u16string_view p_SerializedName)
    {
        auto l_SerializedName    = SanitizeBeatmapCharacteristicSOSerializedName(p_SerializedName);
        auto l_CharacteristicSO  = (BeatmapCharacteristicSO*)nullptr;

        if (TryGetBeatmapCharacteristicSOBySerializedName(p_SerializedName, &l_CharacteristicSO))
            return l_CharacteristicSO->____sortingOrder;

        return 1000;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief BeatmapDifficulty to BeatmapDifficulty enum name
    /// @param p_BeatmapDifficulty BeatmapDifficulty
    /// @return BeatmapDifficulty enum name or "? ExpertPlus ?"
    std::u16string_view Levels::BeatmapDifficultyToBeatmapDifficultyEnumName(BeatmapDifficulty p_BeatmapDifficulty)
    {
        if (p_BeatmapDifficulty == BeatmapDifficulty::Easy)
            return u"Easy";
        else if (p_BeatmapDifficulty == BeatmapDifficulty::Normal)
            return u"Normal";
        else if (p_BeatmapDifficulty == BeatmapDifficulty::Hard)
            return u"Hard";
        else if (p_BeatmapDifficulty == BeatmapDifficulty::Expert)
            return u"Expert";
        else if (p_BeatmapDifficulty == BeatmapDifficulty::ExpertPlus)
            return u"ExpertPlus";

        CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.BeatmapDifficultyToDifficultyName] Unknown difficulty, fall back to \"? ExpertPlus ?\"");

        return u"? ExpertPlus ?";
    }
    /// @brief BeatmapDifficulty serialized name to difficulty name
    /// @param p_BeatmapDifficultySerializedName BeatmapDifficulty serialized name
    /// @return Difficulty name
    std::u16string_view Levels::BeatmapDifficultySerializedNameToDifficultyName(std::u16string_view p_BeatmapDifficultySerializedName)
    {
        if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"easy"))
            return u"Easy";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"normal"))
            return u"Normal";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"hard"))
            return u"Hard";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"expert"))
            return u"Expert";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"expertplus"))
            return u"Expert+";

        CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.DifficultySerializedNameToDifficultyName] Unknown serialized difficulty \"" + p_BeatmapDifficultySerializedName + "\", fall back to \"? Expert+ ?\"");

        return u"? Expert+ ?";
    }
    /// @brief BeatmapDifficulty serialized name to difficulty name short
    /// @param p_BeatmapDifficultySerializedName BeatmapDifficulty serialized name
    /// @return Difficulty name short
    std::u16string_view Levels::BeatmapDifficultySerializedNameToDifficultyNameShort(std::u16string_view p_BeatmapDifficultySerializedName)
    {
        if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"easy"))
            return u"E";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"normal"))
            return u"N";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"hard"))
            return u"H";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"expert"))
            return u"Ex";
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"expertplus"))
            return u"Ex+";

        CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.BeatmapDifficultySerializedNameToDifficultyNameShort] Unknown serialized difficulty \"" + p_BeatmapDifficultySerializedName + "\", fall back to ExpertPlus");

        return u"E+";
    }
    /// @brief BeatmapDifficulty serialized name to BeatmapDifficulty
    /// @param p_BeatmapDifficultySerializedName BeatmapDifficulty serialized name
    /// @return BeatmapDifficulty
    BeatmapDifficulty Levels::BeatmapDifficultySerializedNameToBeatmapDifficulty(std::u16string_view p_BeatmapDifficultySerializedName)
    {
        if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"easy"))
            return BeatmapDifficulty::Easy;
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"normal"))
            return BeatmapDifficulty::Normal;
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"hard"))
            return BeatmapDifficulty::Hard;
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"expert"))
            return BeatmapDifficulty::Expert;
        else if (_v::U16EqualsToCaseInsensitive(p_BeatmapDifficultySerializedName, u"expertplus"))
            return BeatmapDifficulty::ExpertPlus;

        CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.BeatmapDifficultySerializedNameToBeatmapDifficulty] Unknown serialized difficulty \"" + p_BeatmapDifficultySerializedName + "\", fall back to ExpertPlus");

        return BeatmapDifficulty::ExpertPlus;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Own a DLC level by level ID
    /// @param p_LevelID  Level ID
    /// @param p_Callback Callback for success/failure
    void Levels::OwnDLCLevelByLevelID(std::u16string p_LevelID, _v::Action<bool> p_Callback)
    {
        if (LevelID_IsCustom(p_LevelID))
        {
            p_Callback(true);
            return;
        }

        if (!m_BeatmapLevelsModel)
        {
            auto l_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<_u::MainFlowCoordinator*>()->FirstOrDefault([](_u::MainFlowCoordinator* x) -> bool {
                return x->____beatmapLevelsModel;
            });
            if (l_MainFlowCoordinator)
                m_BeatmapLevelsModel = l_MainFlowCoordinator->____beatmapLevelsModel;
        }

        if (m_BeatmapLevelsModel && m_BeatmapLevelsModel->____entitlements)
        {
            if (m_GetLevelEntitlementStatusTokenSource)
                m_GetLevelEntitlementStatusTokenSource->Cancel();

            m_GetLevelEntitlementStatusTokenSource = CancellationTokenSource::New_ctor();

            try
            {
                auto l_Task = m_BeatmapLevelsModel->____entitlements->GetLevelEntitlementStatusAsync(p_LevelID, m_GetLevelEntitlementStatusTokenSource->get_Token());
                l_Task->ConfigureAwait(false);

                _v::AwaitTaskAsync<EntitlementStatus>(
                    l_Task,
                    [=](_v::MonoPtrRef<Tasks::Task_1<EntitlementStatus>> p_Task, bool p_Success) {
                        p_Callback(p_Success && p_Task->get_Result() == EntitlementStatus::Owned);
                    }
                );

                return;
            }
            catch (const std::exception& l_Exception)
            {
                CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.OwnDLCLevelByLevelID] Error:");
                CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
        else
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.OwnDLCLevelByLevelID] Invalid AdditionalContentModel");

        p_Callback(false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Try to get BeatmapLevel by level ID
    /// @param p_LevelID      ID of the level
    /// @param p_BeatmapLevel OUT Found BeatmapLevel or nullptr
    /// @return true or false
    bool Levels::TryGetBeatmapLevelForLevelID(std::u16string_view p_LevelID, BeatmapLevel** p_BeatmapLevel)
    {
        if (p_BeatmapLevel) *p_BeatmapLevel = nullptr;

        auto l_LevelID = SanitizeLevelID(p_LevelID);
        if (LevelID_IsCustom(p_LevelID))
        {
            auto l_Custom = SongCore::API::Loading::GetLevelByLevelID(_v::U16StrToStr(l_LevelID));
            if (l_Custom)
            {
                if (p_BeatmapLevel) *p_BeatmapLevel = l_Custom;
                return true;
            }
        }

        if (!m_BeatmapLevelsModel)
        {
            auto l_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<_u::MainFlowCoordinator*>()->FirstOrDefault([](_u::MainFlowCoordinator* x) -> bool {
                return x->____beatmapLevelsModel;
            });
            if (l_MainFlowCoordinator)
                m_BeatmapLevelsModel = l_MainFlowCoordinator->____beatmapLevelsModel;
        }

        if (m_BeatmapLevelsModel)
        {
            auto l_Result = m_BeatmapLevelsModel->GetBeatmapLevel(l_LevelID);
            if (l_Result)
            {
                if (p_BeatmapLevel) *p_BeatmapLevel = l_Result;
                return true;
            }
        }
        else
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetBeatmapLevelForLevelID] Invalid BeatmapLevelsModel");

        return false;
    }
    /// @brief Try to get BeatmapLevel by hash
    /// @param p_Hash         Hash of the level
    /// @param p_BeatmapLevel OUT Found BeatmapLevel or nullptr
    /// @return true or false
    bool Levels::TryGetBeatmapLevelForHash(std::u16string_view p_Hash, _u::BeatmapLevel** p_BeatmapLevel)
    {
        std::u16string l_LevelID;
        if (!TryGetLevelIDFromHash(p_Hash, &l_LevelID))
            return false;

        return TryGetBeatmapLevelForLevelID(l_LevelID, p_BeatmapLevel);
    }
    /// @brief For each of BeatmapKey for a BeatmapLevel
    /// @param p_BeatmapLevel Input beatmap level
    /// @param p_Functor      Functor for each element, return true mean we continue iterating
    void Levels::BeatmapLevel_ForEachBeatmapKey(BeatmapLevel* p_BeatmapLevel, _v::CFuncRef<bool, const BeatmapKey&> p_Functor)
    {
        if (!p_BeatmapLevel)
            return;

        try
        {
            /// Force cache generation
            p_BeatmapLevel->GetBeatmapKeys();

            for (const auto& l_Current : p_BeatmapLevel->____beatmapKeysCache)
            {
                if (!p_Functor(l_Current))
                    break;
            }
        }
        catch(const std::exception&)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Level.BeatmapLevel_ForEachBeatmapKey] Resolution failed");
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Check if a difficulty is present in a BeatmapLevel
    /// @param p_BeatmapLevel            Input beatmap level
    /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
    /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
    /// @return True or false
    bool Levels::BeatmapLevel_HasDifficulty(_u::BeatmapLevel*               p_BeatmapLevel,
                                            _u::BeatmapCharacteristicSO*    p_BeatmapCharacteristicSO,
                                            _u::BeatmapDifficulty           p_BeatmapDifficulty)
    {
        if (!p_BeatmapLevel || !p_BeatmapCharacteristicSO)
            return false;

        if (p_BeatmapLevel->GetDifficultyBeatmapData(p_BeatmapCharacteristicSO, p_BeatmapDifficulty) == nullptr)
            return false;

        return true;
    }
    /// @brief Try get a beatmap key from a BeatmapLevel
    /// @param p_BeatmapLevel            Input beatmap level
    /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
    /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
    /// @param p_BeatmapKey              Out beatmap key
    /// @return True or false
    bool Levels::BeatmapLevel_TryGetBeatmapKey( _u::BeatmapLevel*               p_BeatmapLevel,
                                                _u::BeatmapCharacteristicSO*    p_BeatmapCharacteristicSO,
                                                _u::BeatmapDifficulty           p_BeatmapDifficulty,
                                                _u::BeatmapKey*                 p_BeatmapKey)
    {
        if (!p_BeatmapLevel || !p_BeatmapCharacteristicSO)
            return false;

        auto l_Found = false;
        BeatmapLevel_ForEachBeatmapKey(p_BeatmapLevel, [&](const BeatmapKey& l_Current) -> bool
        {
            auto l_BeatmapCharacteristicSO = l_Current.beatmapCharacteristic.unsafePtr();
            if (l_BeatmapCharacteristicSO->____serializedName != p_BeatmapCharacteristicSO->____serializedName)
                return true;    ///< Continue

            if (l_Current.difficulty != p_BeatmapDifficulty)
                return true;    ///< Continue

            if (p_BeatmapKey) *p_BeatmapKey = l_Current;
            l_Found = true;

            return false;    ///< Continue
        });

        return l_Found;
    }
    /// @brief Try get custom requirements for a BeatmapLevel->BeatmapCharacteristicSO->BeatmapDifficulty
    /// @param p_BeatmapLevel            Input beatmap level
    /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
    /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
    /// @param p_CustomRequirements      OUT custom requirements
    /// @return true or false
    bool Levels::TryGetCustomRequirementsFor(_u::BeatmapLevel*              p_BeatmapLevel,
                                             _u::BeatmapCharacteristicSO*   p_BeatmapCharacteristicSO,
                                             _u::BeatmapDifficulty          p_BeatmapDifficulty,
                                             std::vector<std::u16string>*   p_CustomRequirements)
    {
        if (p_CustomRequirements) p_CustomRequirements->clear();
        if (!p_BeatmapLevel || !p_BeatmapCharacteristicSO)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Invalid BeatmapLevel or BeatmapCharacteristicSO");
            return false;
        }

        if (!LevelID_IsCustom(p_BeatmapLevel->___levelID))
            return false;

        auto l_CustomLevel = (SongCore::SongLoader::CustomBeatmapLevel*)nullptr;
        if (auto l_Custom = il2cpp_utils::try_cast<SongCore::SongLoader::CustomBeatmapLevel>(p_BeatmapLevel))
            l_CustomLevel = l_Custom.value_or(nullptr);

        if (!l_CustomLevel)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Failed to convert to custom level for id: " + p_BeatmapLevel->___levelID);
            return false;
        }

        auto l_CustomSaveDataInfoWrapper = l_CustomLevel->get_CustomSaveDataInfo();
        if (!l_CustomSaveDataInfoWrapper)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Failed to retrieve custom data level for id: " + p_BeatmapLevel->___levelID);
            return false;
        }

        auto& l_CustomSaveDataInfo = l_CustomSaveDataInfoWrapper->get();

        auto l_CharacteristicAndDifficultyWrapper = l_CustomSaveDataInfo.TryGetCharacteristicAndDifficulty(p_BeatmapCharacteristicSO->____serializedName, p_BeatmapDifficulty);
        if (!l_CharacteristicAndDifficultyWrapper)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Failed to retrieve custom data level for id: " + p_BeatmapLevel->___levelID);
            return false;
        }

        const auto& l_CharacteristicAndDifficulty = l_CharacteristicAndDifficultyWrapper.value().get();

        p_CustomRequirements->reserve(l_CharacteristicAndDifficulty.requirements.size());
        for (auto& l_Requirement : l_CharacteristicAndDifficulty.requirements)
        {
            auto l_CustomRequirement    = CP_SDK::Utils::StrToU16Str(l_Requirement);
            auto l_It                   = std::find(p_CustomRequirements->begin(), p_CustomRequirements->end(), l_CustomRequirement);

            if (l_It == p_CustomRequirements->end())
                p_CustomRequirements->push_back(l_CustomRequirement);
        }

        std::sort(p_CustomRequirements->begin(), p_CustomRequirements->end());

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Try to load BeatmapLevel cover image async
    /// @param p_BeatmapLevel Input BeatmapLevel
    /// @param p_Callback     Callback
    void Levels::TryLoadBeatmapLevelCoverAsync(_u::BeatmapLevel* p_BeatmapLevel, _v::Action<bool, _u::Sprite*> p_Callback)
    {
        if (!p_BeatmapLevel || !p_BeatmapLevel->___previewMediaData)
        {
            CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]() { p_Callback(false, Levels::GetDefaultPackCover()); });
            return;
        }

        _v::MonoPtr<BeatmapLevel, true> l_PreviewBeatmapLevel(p_BeatmapLevel);
        auto l_Task = p_BeatmapLevel->___previewMediaData->GetCoverSpriteAsync();

        _v::AwaitTaskAsync<UnityW<Sprite>>(
            l_Task,
            [=](_v::MonoPtrRef<Tasks::Task_1<UnityW<Sprite>>> p_Task, bool p_Success) {
                try
                {
                    if (p_Success && p_Task->get_Result())
                        p_Callback(true, p_Task->get_Result().unsafePtr());
                    else
                        p_Callback(false, Levels::GetDefaultPackCover());
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryLoadBeatmapLevelCoverAsync] Error:");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);

                    p_Callback(false, Levels::GetDefaultPackCover());
                }
            }
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Load a BeatmapLevelData by level ID
    /// @param p_LevelID      ID of the level
    /// @param p_LoadCallback Load callback
    void Levels::LoadBeatmapLevelDataByLevelID( std::u16string                                                                              p_LevelID,
                                                _v::Action<_v::MonoPtr<_u::BeatmapLevel, true>, _v::MonoPtr<_u::IBeatmapLevelData, true>>   p_LoadCallback)
    {
        auto            l_LevelID               = SanitizeLevelID(p_LevelID);
        BeatmapLevel*   l_BeatmapLevelUnsafe    = nullptr;
        if (!TryGetBeatmapLevelForLevelID(l_LevelID, &l_BeatmapLevelUnsafe))
        {
            p_LoadCallback(nullptr, nullptr);
            return;
        }

        _v::MonoPtr<BeatmapLevel> l_BeatmapLevel = l_BeatmapLevelUnsafe;
        if (!LevelID_IsCustom(p_LevelID))
        {
            OwnDLCLevelByLevelID(p_LevelID, [=](bool p_OwnDLCLevelByLevelIDResult) {
                if (!p_OwnDLCLevelByLevelIDResult)
                {
                    p_LoadCallback(nullptr, nullptr);
                    return;
                }

                LoadIBeatmapLevelDataAsync(p_LevelID, [=](_v::MonoPtr<IBeatmapLevelData, true> p_Result) {
                    if (p_Result)
                        p_LoadCallback(l_BeatmapLevel.Ptr(false), p_Result);
                    else
                        p_LoadCallback(nullptr, nullptr);
                });
            });
        }
        else
        {
            LoadIBeatmapLevelDataAsync(p_LevelID, [=](_v::MonoPtr<IBeatmapLevelData, true> p_Result) {
                if (p_Result)
                    p_LoadCallback(l_BeatmapLevel.Ptr(false), p_Result);
                else
                    p_LoadCallback(nullptr, nullptr);
            });
        }
    }
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
    void Levels::StartBeatmapLevel( _u::BeatmapLevel*                   p_Level,
                                    _u::BeatmapCharacteristicSO*        p_Characteristic,
                                    _u::BeatmapDifficulty               p_Difficulty,
                                    _u::IBeatmapLevelData*              p_BeatmapLevelData,
                                    _u::OverrideEnvironmentSettings*    p_OverrideEnvironmentSettings,
                                    _u::ColorScheme*                    p_ColorScheme,
                                    _u::GameplayModifiers*              p_GameplayModifiers,
                                    _u::PlayerSpecificSettings*         p_PlayerSettings,
                                    _v::Action<_u::StandardLevelScenesTransitionSetupDataSO*, _u::LevelCompletionResults*> p_SongFinishedCallback,
                                    std::u16string_view                 p_MenuButtonText)
    {
        if (p_Level == nullptr)
            return;

        if (!m_MenuTransitionsHelper)
            m_MenuTransitionsHelper = Resources::FindObjectsOfTypeAll<MenuTransitionsHelper*>()->FirstOrDefault();

        if (!m_SimpleLevelStarter)
            m_SimpleLevelStarter = Resources::FindObjectsOfTypeAll<SimpleLevelStarter*>()->FirstOrDefault();

        if (m_MenuTransitionsHelper && m_SimpleLevelStarter)
        {
            try
            {
                Scoring::BeatLeader_ManualWarmUpSubmission();

                using t_Delegate = System::Action_2<UnityW<StandardLevelScenesTransitionSetupDataSO>, LevelCompletionResults*>*;

                auto l_BeatmapKey   = BeatmapKey(p_Characteristic, p_Difficulty, p_Level->___levelID);
                auto l_Delegate     = custom_types::MakeDelegate<t_Delegate>(std::function([=](UnityW<StandardLevelScenesTransitionSetupDataSO> __a, LevelCompletionResults* __b) {
                    p_SongFinishedCallback(__a, __b);
                }));

                m_MenuTransitionsHelper->StartStandardLevel(
                    /* string                                                                   gameMode:                    */ "Solo",
                    /* in BeatmapKey                                                            beatmapKey:                  */ byref(l_BeatmapKey),
                    /* BeatmapLevel                                                             beatmapLevel:                */ p_Level,
                    /* IBeatmapLevelData                                                        beatmapLevelData:            */ p_BeatmapLevelData,
                    /* OverrideEnvironmentSettings                                              overrideEnvironmentSettings: */ p_OverrideEnvironmentSettings,
                    /* ColorScheme                                                              playerOverrideColorScheme:   */ p_ColorScheme,
                    /* bool                                                                     playerOverrideLightshowColors*/ false,
                    /* ColorScheme                                                              beatmapOverrideColorScheme:  */ nullptr,
                    /* GameplayModifiers                                                        gameplayModifiers:           */ p_GameplayModifiers ? p_GameplayModifiers : GameplayModifiers::New_ctor(),
                    /* PlayerSpecificSettings                                                   playerSpecificSettings:      */ p_PlayerSettings    ? p_PlayerSettings    : PlayerSpecificSettings::New_ctor(),
                    /* PracticeSettings                                                         practiceSettings:            */ nullptr,
                    /* EnvironmentsListModel                                                    environmentsListModel:       */ m_SimpleLevelStarter->____environmentsListModel,
                    /* string                                                                   backButtonText:              */ p_MenuButtonText,
                    /* bool                                                                     useTestNoteCutSoundEffects:  */ false,
                    /* bool                                                                     startPaused:                 */ false,
                    /* Action                                                                   beforeSceneSwitchCallback:   */ nullptr,
                    /* Action<DiContainer>                                                      afterSceneSwitchCallback:    */ nullptr,
                    /* Action<StandardLevelScenesTransitionSetupDataSO, LevelCompletionResults> levelFinishedCallback:       */ l_Delegate,
                    /* Action<LevelScenesTransitionSetupDataSO, LevelCompletionResults>         levelRestartedCallback:      */ nullptr,
                    /* RecordingToolManager.SetupData?                                          recordingToolData:           */ System::Nullable_1<_u::RecordingToolManager_SetupData>()
                );
            }
            catch (const std::exception& l_Exception)
            {
                CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.StartBeatmapLevel] Error:");
                CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
        else
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.StartBeatmapLevel] Invalid MenuTransitionsHelper");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Load IBeatmapLevelData from a level ID
    /// @param p_LevelID  Level ID
    /// @param p_Callback Callback for success/failure
    void Levels::LoadIBeatmapLevelDataAsync(std::u16string                                          p_LevelID,
                                            _v::Action<_v::MonoPtr<_u::IBeatmapLevelData, true>>    p_Callback)
    {
        if (!m_BeatmapLevelsModel)
        {
            auto l_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<_u::MainFlowCoordinator*>()->FirstOrDefault([](_u::MainFlowCoordinator* x) -> bool {
                return x->____beatmapLevelsModel;
            });
            if (l_MainFlowCoordinator)
                m_BeatmapLevelsModel = l_MainFlowCoordinator->____beatmapLevelsModel;
        }

        if (m_BeatmapLevelsModel)
        {
            if (m_GetLevelCancellationTokenSource)
                m_GetLevelCancellationTokenSource->Cancel();

            m_GetLevelCancellationTokenSource = CancellationTokenSource::New_ctor();

            try
            {
                auto l_VersionTask = m_MenuTransitionsHelper->____beatmapLevelsEntitlementModel->GetLevelDataVersionAsync(p_LevelID, m_GetLevelCancellationTokenSource->get_Token());
                _v::AwaitTaskAsync<BeatmapLevelDataVersion>(
                    l_VersionTask,
                    [=](_v::MonoPtrRef<Tasks::Task_1<BeatmapLevelDataVersion>> p_VersionTask, bool p_Success) {
                        try
                        {
                            if (p_Success)
                            {
                                auto l_Task = m_BeatmapLevelsModel->LoadBeatmapLevelDataAsync(p_LevelID, p_VersionTask->get_Result(), m_GetLevelCancellationTokenSource->get_Token());

                                _v::AwaitTaskAsync<LoadBeatmapLevelDataResult>(
                                    l_Task,
                                    [=](_v::MonoPtrRef<Tasks::Task_1<LoadBeatmapLevelDataResult>> p_Task, bool p_Success) {
                                        try
                                        {
                                            if (p_Success && !p_Task->get_Result().isError)
                                                p_Callback(p_Task->get_Result().beatmapLevelData);
                                            else
                                                p_Callback(nullptr);
                                        }
                                        catch (const std::exception& l_Exception)
                                        {
                                            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.GetLevelFromLevelID] Error:");
                                            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                                        }
                                    }
                                );
                            }
                            else
                                p_Callback(nullptr);
                        }
                        catch (const std::exception& l_Exception)
                        {
                            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.GetLevelFromLevelID] Error:");
                            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                        }
                    }
                );

                return;
            }
            catch (const std::exception& l_Exception)
            {
                CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.GetLevelFromLevelID] Error:");
                CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
        else
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.GetLevelFromLevelID] Invalid BeatmapLevelsModel");

        p_Callback(nullptr);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get accuracy
    /// @param p_MaxScore Max score
    /// @param p_Score    Result score
    float Levels::GetAccuracy(int p_MaxScore, int p_Score)
    {
        auto l_Rounded   = System::Math::Round(1.0 / static_cast<double>(p_MaxScore) * static_cast<double>(p_Score));
        auto l_Truncated = static_cast<double>(static_cast<int>(l_Rounded * 1000.0)) / 1000.0;

        return static_cast<float>(l_Truncated);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get scores from local cache for a level id
    /// @param p_LevelID       Level ID
    /// @param p_HaveAnyScore  OUT Have any score set
    /// @param p_HaveAllScores OUT Have all scores set
    /// @return Scores
    Levels::t_Scores Levels::GetScoresByLevelID(std::u16string_view p_LevelID, bool* p_HaveAnyScore, bool* p_HaveAllScores)
    {
        Levels::t_Scores l_Results;

        if (p_HaveAnyScore)  *p_HaveAnyScore  = false;
        if (p_HaveAllScores) *p_HaveAllScores = true;

        auto l_LevelID      = SanitizeLevelID(p_LevelID);
        auto l_BeatmapLevel = (BeatmapLevel*)nullptr;
        if (!TryGetBeatmapLevelForLevelID(l_LevelID, &l_BeatmapLevel))
        {
            if (p_HaveAllScores) *p_HaveAllScores = false;
            return l_Results;
        }

        auto l_PlayerDataModel  = Resources::FindObjectsOfTypeAll<PlayerDataModel*>()->FirstOrDefault();
        auto l_PlayerData       = l_PlayerDataModel ? l_PlayerDataModel->____playerData : nullptr;
        auto l_LevelStatsData   = l_PlayerData ? l_PlayerData->get_levelsStatsData() : nullptr;

        BeatmapLevel_ForEachBeatmapKey(l_BeatmapLevel, [&](const BeatmapKey& l_Current) -> bool
        {
            auto l_BeatmapCharacteristicSO = const_cast<BeatmapCharacteristicSO*>(l_Current.beatmapCharacteristic.unsafePtr());
            if (!l_Results.contains(l_BeatmapCharacteristicSO))
                l_Results[l_BeatmapCharacteristicSO] = t_CharacteristicScores();

            PlayerLevelStatsData* l_PlayerLevelStatsData = nullptr;
            if (l_LevelStatsData->TryGetValue(l_Current, byref(l_PlayerLevelStatsData)) && l_PlayerLevelStatsData && l_PlayerLevelStatsData->____playCount)
            {
                if (p_HaveAnyScore) *p_HaveAnyScore = true;
                l_Results[l_BeatmapCharacteristicSO].push_back(std::make_tuple(l_Current.difficulty, l_PlayerLevelStatsData->____highScore));
            }
            else
            {
                if (p_HaveAllScores) *p_HaveAllScores = false;
                l_Results[l_BeatmapCharacteristicSO].push_back(std::make_tuple(l_Current.difficulty, -1));
            }

            return true;    ///< Continue
        });

        return l_Results;
    }

}   ///< namespace CP_SDK_BS::Game
