#include "CP_SDK_BS/Game/Levels.hpp"
#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/Utils/Task.hpp"
#include "assets.hpp"

#include <custom-types/shared/delegate.hpp>
#include <songloader/shared/API.hpp>

#include <GlobalNamespace/BeatmapLevelSO.hpp>
#include <GlobalNamespace/BeatmapLevelDataExtensions.hpp>
#include <GlobalNamespace/CustomLevelLoader.hpp>
#include <GlobalNamespace/CustomPreviewBeatmapLevel.hpp>
#include <GlobalNamespace/FilteredBeatmapLevel.hpp>
#include <GlobalNamespace/IBeatmapLevelData.hpp>
#include <GlobalNamespace/GameplayModifiers.hpp>
#include <GlobalNamespace/PlayerData.hpp>
#include <GlobalNamespace/PlayerDataModel.hpp>
#include <GlobalNamespace/PlayerLevelStatsData.hpp>
#include <GlobalNamespace/PlayerSpecificSettings.hpp>
#include <GlobalNamespace/PreviewBeatmapLevelSO.hpp>
#include <GlobalNamespace/PreviewDifficultyBeatmapSet.hpp>
#include <GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp>
#include <System/Collections/Generic/IReadOnlyList_1.hpp>
#include <System/Threading/Tasks/Task_1.hpp>
#include <System/Threading/Tasks/TaskStatus.hpp>
#include <System/Action_2.hpp>
#include <System/Math.hpp>
#include <System/IO/File.hpp>
#include <System/IO/Path.hpp>
#include <UnityEngine/Resources.hpp>

using namespace GlobalNamespace;
using namespace System::Threading;
using namespace UnityEngine;

namespace CP_SDK_BS::Game {

    _v::MonoPtr<Sprite>                            Levels::m_DefaultPackCover;

    std::vector<_v::Action<>>                      Levels::m_ReloadSongsCallbacks;
    std::mutex                                     Levels::m_ReloadSongsCallbacksMutex;

    _v::MonoPtr<AdditionalContentModel>            Levels::m_AdditionalContentModel;
    _v::MonoPtr<BeatmapCharacteristicCollectionSO> Levels::m_BeatmapCharacteristicCollectionSO;
    _v::MonoPtr<BeatmapLevelsModel>                Levels::m_BeatmapLevelsModel;
    _v::MonoPtr<CancellationTokenSource>           Levels::m_GetLevelCancellationTokenSource;
    _v::MonoPtr<CancellationTokenSource>           Levels::m_GetLevelEntitlementStatusTokenSource;
    _v::MonoPtr<MenuTransitionsHelper>             Levels::m_MenuTransitionsHelper;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get default pack cover
    Sprite* Levels::GetDefaultPackCover()
    {
        if (!m_DefaultPackCover)
            m_DefaultPackCover = CP_SDK::Unity::SpriteU::CreateFromRaw(IncludedAssets::DefaultPackCover_png.Raw());

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
            RuntimeSongLoader::API::RefreshSongs(p_Full, [](const auto&) -> void
            {
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
        });
    }
    /// @brief Check for mapping capability
    /// @param p_Capability Capability name
    /// @return True or false
    bool Levels::HasMappingCapability(std::u16string_view p_Capability)
    {
        CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.HasMappingCapability] NOT YET IMPLEMENTED RETURNING FALSE");
        return false;
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

        if (!m_BeatmapCharacteristicCollectionSO)
        {
            auto l_CustomLevelLoader = Resources::FindObjectsOfTypeAll<CustomLevelLoader*>().FirstOrDefault();
            if (l_CustomLevelLoader)
                m_BeatmapCharacteristicCollectionSO = l_CustomLevelLoader->beatmapCharacteristicCollection;
        }

        if (!m_BeatmapCharacteristicCollectionSO)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetBeatmapCharacteristicSOBySerializedName] Invalid BeatmapCharacteristicCollectionSO");
            return false;
        }

        auto l_Result = m_BeatmapCharacteristicCollectionSO->GetBeatmapCharacteristicBySerializedName(SanitizeBeatmapCharacteristicSOSerializedName(p_SerializedName));
        if (l_Result && p_BeatmapCharacteristicSO)
            *p_BeatmapCharacteristicSO = l_Result;

        return l_Result != nullptr;
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
            return l_CharacteristicSO->sortingOrder;

        return 1000;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

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

            if (p_Hash->length() == 40/* TODO check for only hex*/)
                std::transform(p_Hash->begin(), p_Hash->end(), p_Hash->begin(), std::towupper);
        }

        return true;
    }
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

    /// @brief For each of PreviewDifficultyBeatmapSets for a PreviewBeatmapLevel
    /// @param p_PreviewBeatmapLevel Input preview beatmap level
    /// @param p_Functor             Functor for each element, return true mean we continue iterating
    void Levels::PreviewDifficultyBeatmapSets_ForEach(IPreviewBeatmapLevel* p_PreviewBeatmapLevel, _v::CFuncRef<bool, PreviewDifficultyBeatmapSet*> p_Functor)
    {
        if (!p_PreviewBeatmapLevel)
            return;

        auto l_List  = p_PreviewBeatmapLevel->get_previewDifficultyBeatmapSets();
        try
        {
            CP_SDK::ChatPlexSDK::Logger()->Info(u"[CP_SDK_BS.Game][Level.PreviewDifficultyBeatmapSets_ForEach] Trying method 1");
            auto l_Count = l_List->i_IReadOnlyCollection_1_T()->get_Count();
            for (auto l_I = 0; l_I < l_Count; ++l_I)
            {
                if (!p_Functor(l_List->get_Item(l_I)))
                    break;
            }
        }
        catch(const std::exception&)
        {
            CP_SDK::ChatPlexSDK::Logger()->Info(u"[CP_SDK_BS.Game][Level.PreviewDifficultyBeatmapSets_ForEach] Trying method 2");
            try
            {
                auto l_Enumerator   = l_List->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T()->GetEnumerator()->i_IEnumerator();
                while (l_Enumerator->MoveNext())
                {
                    if (!p_Functor((PreviewDifficultyBeatmapSet*)l_Enumerator->get_Current()))
                        break;
                }
            }
            catch(const std::exception&)
            {
                CP_SDK::ChatPlexSDK::Logger()->Info(u"[CP_SDK_BS.Game][Level.PreviewDifficultyBeatmapSets_ForEach] Resolution failed");
            }
        }
    }
    /// @brief Try get preview difficulty beatmap set by CharacteristicSO
    /// @param p_PreviewBeatmapLevel         Input preview beatmap level
    /// @param p_BeatmapCharacteristicSO     Input characteristic SO
    /// @param p_PreviewDifficultyBeatmapSet OUT result preview beatmap set
    /// @return True or false
    bool Levels::TryGetPreviewDifficultyBeatmapSet(IPreviewBeatmapLevel* p_PreviewBeatmapLevel, BeatmapCharacteristicSO* p_BeatmapCharacteristicSO, PreviewDifficultyBeatmapSet** p_PreviewDifficultyBeatmapSet)
    {
        if (p_PreviewDifficultyBeatmapSet) *p_PreviewDifficultyBeatmapSet = nullptr;
        if (!p_PreviewBeatmapLevel || !p_BeatmapCharacteristicSO)
            return false;

        auto l_Result = (PreviewDifficultyBeatmapSet*)nullptr;

        PreviewDifficultyBeatmapSets_ForEach(p_PreviewBeatmapLevel, [&](PreviewDifficultyBeatmapSet* l_Current) -> bool
        {
            if (l_Current->beatmapCharacteristic->serializedName != p_BeatmapCharacteristicSO->serializedName)
                return true;    ///< Continue

            l_Result = l_Current;
            return false;       ///< Break
        });

        if (l_Result != nullptr)
        {
            if (p_PreviewDifficultyBeatmapSet) *p_PreviewDifficultyBeatmapSet = l_Result;
            return true;
        }

        return false;
    }
    /// @brief Check if a difficulty is present in a PreviewDifficultyBeatmapSet
    /// @param p_PreviewDifficultyBeatmapSet Input PreviewDifficultyBeatmapSet
    /// @param p_Difficulty                  Requested difficulty
    /// @return True or false
    bool Levels::PreviewDifficultyBeatmapSet_HasDifficulty(PreviewDifficultyBeatmapSet* p_PreviewDifficultyBeatmapSet, BeatmapDifficulty p_Difficulty)
    {
        if (   p_PreviewDifficultyBeatmapSet == nullptr
            || p_PreviewDifficultyBeatmapSet->beatmapDifficulties.Length() == 0)
            return false;

        for (const auto& l_Current : p_PreviewDifficultyBeatmapSet->beatmapDifficulties)
        {
            if (l_Current != p_Difficulty)
                continue;

            return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Own a DLC level by level ID
    /// @param p_LevelID  Level ID
    /// @param p_Callback Callback for success/failure
    void Levels::OwnDLCLevelByLevelID(std::u16string_view p_LevelID, _v::Action<bool> p_Callback)
    {
        if (LevelID_IsCustom(p_LevelID))
        {
            p_Callback(true);
            return;
        }

        if (!m_AdditionalContentModel)
            m_AdditionalContentModel = Resources::FindObjectsOfTypeAll<AdditionalContentModel*>().FirstOrDefault();

        if (m_AdditionalContentModel)
        {
            if (m_GetLevelEntitlementStatusTokenSource)
                m_GetLevelEntitlementStatusTokenSource->Cancel();

            m_GetLevelEntitlementStatusTokenSource = CancellationTokenSource::New_ctor();

            try
            {
                auto l_Task = m_AdditionalContentModel->GetLevelEntitlementStatusAsync(p_LevelID, m_GetLevelEntitlementStatusTokenSource->get_Token());
                l_Task->ConfigureAwait(false);

                _v::AwaitTaskAsync<AdditionalContentModel::EntitlementStatus>(
                    l_Task,
                    [=](_v::MonoPtrRef<Tasks::Task_1<AdditionalContentModel::EntitlementStatus>> p_Task, bool p_Success) {
                        p_Callback(p_Success && p_Task->get_Result() == AdditionalContentModel::EntitlementStatus::Owned);
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
    /// @brief Try to get PreviewBeatmapLevel by level ID
    /// @param p_LevelID             ID of the level
    /// @param p_PreviewBeatmapLevel OUT Found PreviewBeatmapLevel or nullptr
    /// @return true or false
    bool Levels::TryGetPreviewBeatmapLevelForLevelID(std::u16string_view p_LevelID, IPreviewBeatmapLevel** p_PreviewBeatmapLevel)
    {
        if (p_PreviewBeatmapLevel) *p_PreviewBeatmapLevel = nullptr;

        auto l_LevelID = SanitizeLevelID(p_LevelID);
        if (LevelID_IsCustom(p_LevelID))
        {
            auto l_Custom = RuntimeSongLoader::API::GetLevelById(_v::U16StrToStr(l_LevelID));
            if (l_Custom.has_value())
            {
                if (p_PreviewBeatmapLevel) *p_PreviewBeatmapLevel = l_Custom.value()->i_IPreviewBeatmapLevel();
                return true;
            }
        }

        if (!m_BeatmapLevelsModel)
            m_BeatmapLevelsModel = Resources::FindObjectsOfTypeAll<BeatmapLevelsModel*>().FirstOrDefault();

        if (m_BeatmapLevelsModel)
        {
            auto l_Result = m_BeatmapLevelsModel->GetLevelPreviewForLevelId(l_LevelID);
            if (l_Result)
            {
                if (p_PreviewBeatmapLevel) *p_PreviewBeatmapLevel = l_Result;
                return true;
            }
        }
        else
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetPreviewBeatmapLevelForLevelID] Invalid BeatmapLevelsModel");

        return false;
    }
    /// @brief Try get custom requirements for a IPreviewBeatmapLevel->BeatmapCharacteristicSO->BeatmapDifficulty
    /// @param p_PreviewBeatmapLevel     Input preview beatmap level
    /// @param p_BeatmapCharacteristicSO Desired BeatmapCharacteristicSO
    /// @param p_BeatmapDifficulty       Desired BeatmapDifficulty
    /// @param p_CustomRequirements      OUT custom requirements
    /// @return true or false
    bool Levels::TryGetCustomRequirementsFor(IPreviewBeatmapLevel*           p_PreviewBeatmapLevel,
                                             BeatmapCharacteristicSO*        p_BeatmapCharacteristicSO,
                                             BeatmapDifficulty               p_BeatmapDifficulty,
                                             std::vector<std::u16string>*    p_CustomRequirements)
    {
        if (p_CustomRequirements) p_CustomRequirements->clear();
        if (!p_PreviewBeatmapLevel || !p_BeatmapCharacteristicSO)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Invalid IPreviewBeatmapLevel or BeatmapCharacteristicSO");
            return false;
        }

        if (!LevelID_IsCustom(p_PreviewBeatmapLevel->get_levelID()))
            return false;

        auto l_CustomLevel = (CustomPreviewBeatmapLevel*)nullptr;
        if (auto l_Filter = il2cpp_utils::try_cast<FilteredBeatmapLevel>(p_PreviewBeatmapLevel))
            l_CustomLevel = il2cpp_utils::try_cast<CustomPreviewBeatmapLevel>(l_Filter.value()->beatmapLevel).value_or(nullptr);
        else
            l_CustomLevel = il2cpp_utils::try_cast<CustomPreviewBeatmapLevel>(p_PreviewBeatmapLevel).value_or(nullptr);

        if (!l_CustomLevel)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Failed to convert to custom level for id: " + p_PreviewBeatmapLevel->get_levelID());
            return false;
        }

        auto l_StandardLevelInfoSaveData = il2cpp_utils::try_cast<CustomJSONData::CustomLevelInfoSaveData>(l_CustomLevel->get_standardLevelInfoSaveData()).value_or(nullptr);
        if (!l_StandardLevelInfoSaveData)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] Failed to retrieve custom data level for id: " + p_PreviewBeatmapLevel->get_levelID());
            return false;
        }

        auto& l_Document            = l_StandardLevelInfoSaveData->doc;
        auto  l_DifficultyToFind    = BeatmapDifficultyToBeatmapDifficultyEnumName(p_BeatmapDifficulty);
        auto  l_HasCustomData       = false;
        CustomJSONData::ValueUTF16 l_CustomData;

        auto l_DifficultyBeatmapSetsIT = l_Document->FindMember(u"_difficultyBeatmapSets");
        if (l_DifficultyBeatmapSetsIT != l_Document->MemberEnd())
        {
            auto l_SetArray = l_DifficultyBeatmapSetsIT->value.GetArray();
            for (auto& l_BeatmapCharacteristicIt : l_SetArray)
            {
                if (p_BeatmapCharacteristicSO->serializedName == l_BeatmapCharacteristicIt.FindMember(u"_beatmapCharacteristicName")->value.GetString())
                {
                    auto l_DifficultyBeatmaps = l_BeatmapCharacteristicIt.FindMember(u"_difficultyBeatmaps")->value.GetArray();
                    for (auto& l_DifficultyBeatmapIt : l_DifficultyBeatmaps)
                    {
                        if (l_DifficultyToFind != l_DifficultyBeatmapIt.FindMember(u"_difficulty")->value.GetString())
                            continue;

                        if (!l_DifficultyBeatmapIt.HasMember(u"_customData") || !l_DifficultyBeatmapIt[u"_customData"].IsObject())
                            return false;

                        l_HasCustomData = true;
                        l_CustomData.CopyFrom(l_DifficultyBeatmapIt[u"_customData"], l_Document->GetAllocator());
                        break;
                    }
                }

                if (l_HasCustomData)
                    break;
            }
        }

        if (!l_HasCustomData || !l_CustomData.HasMember(u"_requirements") || !l_CustomData[u"_requirements"].IsArray())
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryGetCustomRequirementsFor] no custom data");
            return false;
        }

        if (!p_CustomRequirements)
            return true;

        auto l_Requirements = l_CustomData[u"_requirements"].GetArray();
        p_CustomRequirements->reserve(l_Requirements.Size());
        for (auto& l_Requirement : l_Requirements)
        {
            auto l_CustomRequirement    = l_Requirement.GetString();
            auto l_It                   = std::find(p_CustomRequirements->begin(), p_CustomRequirements->end(), l_CustomRequirement);

            if (l_It == p_CustomRequirements->end())
                p_CustomRequirements->push_back(l_CustomRequirement);
        }

        std::sort(p_CustomRequirements->begin(), p_CustomRequirements->end());

        return true;
    }
    /// @brief Load a BeatmapLevel by level ID
    /// @param p_LevelID      ID of the level
    /// @param p_LoadCallback Load callback
    void Levels::LoadBeatmapLevelByLevelID( std::u16string_view                                                 p_LevelID,
                                            _v::Action<_v::MonoPtr<IBeatmapLevel, true>>  p_LoadCallback)
    {
        /// Custom levels
        if (LevelID_IsCustom(p_LevelID))
        {
            auto l_Level = RuntimeSongLoader::API::GetLevelById(_v::U16StrToStr(SanitizeLevelID(p_LevelID)));
            if (!l_Level)
            {
                CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]() { p_LoadCallback(nullptr); });
                return;
            }

            auto l_LevelType = l_Level.value()->GetType();
            if (csTypeOf(CustomPreviewBeatmapLevel*)->IsAssignableFrom(l_LevelType))
            {
                GetBeatmapLevelFromLevelID(l_Level.value()->levelID, [=](_v::MonoPtr<IBeatmapLevel, true> p_Result) {
                    if (p_Result)
                        p_LoadCallback(p_Result);
                    else
                        p_LoadCallback(nullptr);
                });
            }
        }
        /// Base levels
        else
        {
            OwnDLCLevelByLevelID(p_LevelID, [=](bool p_OwnDLCLevelByLevelIDResult) {
                if (!p_OwnDLCLevelByLevelIDResult)
                {
                    p_LoadCallback(nullptr);
                    return;
                }

                GetBeatmapLevelFromLevelID(p_LevelID, [=](_v::MonoPtr<IBeatmapLevel, true> p_Result) {
                    if (p_Result)
                        p_LoadCallback(p_Result);
                    else
                        p_LoadCallback(nullptr);
                });
            });
        }
    }
    /// @brief Try to load PreviewBeatmapLevel cover image async
    /// @param p_PreviewBeatmapLevel Input PreviewBeatmapLevel
    /// @param p_Callback            Callback
    void Levels::TryLoadPreviewBeatmapLevelCoverAsync(IPreviewBeatmapLevel* p_PreviewBeatmapLevel, _v::Action<bool, Sprite*> p_Callback)
    {
        if (!p_PreviewBeatmapLevel)
        {
            CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]() { p_Callback(false, Levels::GetDefaultPackCover()); });
            return;
        }

        auto l_CustomPreviewBeatmapLevel = il2cpp_utils::try_cast<CustomPreviewBeatmapLevel>(p_PreviewBeatmapLevel).value_or(nullptr);
        if (l_CustomPreviewBeatmapLevel)
        {
            if (!l_CustomPreviewBeatmapLevel->coverImage)
            {
                auto l_CoverImageFilename = l_CustomPreviewBeatmapLevel->standardLevelInfoSaveData->coverImageFilename;
                if (!System::String::IsNullOrEmpty(l_CoverImageFilename))
                {
                    _v::MonoPtr<CustomPreviewBeatmapLevel, true> l_CustomPreviewBeatmapLevelSafe(l_CustomPreviewBeatmapLevel);
                    auto l_Path          = StringW(System::IO::Path::Combine(l_CustomPreviewBeatmapLevel->customLevelPath, l_CoverImageFilename)).operator std::__ndk1::u16string();
                    auto l_TaskRunned    = false;

                    CP_SDK::Unity::MTThreadInvoker::EnqueueOnThread([=]()
                    {
                        try
                        {
                            auto l_Bytes = System::IO::File::ReadAllBytes(l_Path);
                            CP_SDK::Unity::SpriteU::CreateFromRawThreaded(l_Bytes.operator Array<uint8_t> *(), [=](Sprite* p_Sprite)
                            {
                                if (l_CustomPreviewBeatmapLevelSafe) l_CustomPreviewBeatmapLevelSafe.Ptr(false)->coverImage = p_Sprite ? p_Sprite : Levels::GetDefaultPackCover();
                                p_Callback(p_Sprite, p_Sprite ? p_Sprite : Levels::GetDefaultPackCover());
                            });
                        }
                        catch (const std::exception&)
                        {
                            if (l_CustomPreviewBeatmapLevelSafe) l_CustomPreviewBeatmapLevelSafe.Ptr(false)->coverImage = Levels::GetDefaultPackCover();
                            CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]() { p_Callback(false, Levels::GetDefaultPackCover()); });
                        }
                    });
                }
                else
                {
                    l_CustomPreviewBeatmapLevel->coverImage = Levels::GetDefaultPackCover();
                    CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]() { p_Callback(false, Levels::GetDefaultPackCover()); });
                }
            }
            else
            {
                _v::MonoPtr<Sprite> l_Cover = l_CustomPreviewBeatmapLevel->coverImage;
                CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]() { p_Callback(l_Cover, l_Cover ? l_Cover.Ptr(false) : Levels::GetDefaultPackCover()); });
            }
        }
        else
        {
            _v::MonoPtr<IPreviewBeatmapLevel, true> l_PreviewBeatmapLevel(p_PreviewBeatmapLevel);
            auto l_Task = p_PreviewBeatmapLevel->GetCoverImageAsync(CancellationToken::get_None());

            _v::AwaitTaskAsync<Sprite*>(
                l_Task,
                [=](_v::MonoPtrRef<Tasks::Task_1<Sprite*>> p_Task, bool p_Success) {
                    try
                    {
                        if (p_Success && p_Task->get_Result())
                            p_Callback(true, p_Task->get_Result());
                        else
                            p_Callback(false, Levels::GetDefaultPackCover());
                    }
                    catch (const std::exception& l_Exception)
                    {
                        CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Levels.TryLoadPreviewBeatmapLevelCoverAsync] Error:");
                        CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    }
                }
            );
        }
    }
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
    void Levels::StartBeatmapLevel( IBeatmapLevel*                  p_Level,
                                    BeatmapCharacteristicSO*        p_Characteristic,
                                    BeatmapDifficulty               p_Difficulty,
                                    OverrideEnvironmentSettings*    p_OverrideEnvironmentSettings,
                                    ColorScheme*                    p_ColorScheme,
                                    GameplayModifiers*              p_GameplayModifiers,
                                    PlayerSpecificSettings*         p_PlayerSettings,
                                    _v::Action<StandardLevelScenesTransitionSetupDataSO*, LevelCompletionResults*, IDifficultyBeatmap*> p_SongFinishedCallback,
                                    std::u16string_view             p_MenuButtonText)
    {
        if (p_Level == nullptr || p_Level->get_beatmapLevelData() == nullptr)
            return;

        if (!m_MenuTransitionsHelper)
            m_MenuTransitionsHelper = Resources::FindObjectsOfTypeAll<MenuTransitionsHelper*>().FirstOrDefault();

        if (m_MenuTransitionsHelper)
        {
            try
            {
                Scoring::BeatLeader_ManualWarmUpSubmission();

                using t_Delegate = System::Action_2<StandardLevelScenesTransitionSetupDataSO*, LevelCompletionResults*>*;

                auto l_DifficultyBeatmap = BeatmapLevelDataExtensions::GetDifficultyBeatmap(p_Level->get_beatmapLevelData(), p_Characteristic, p_Difficulty);
                auto l_Delegate          = custom_types::MakeDelegate<t_Delegate>(std::function([=](StandardLevelScenesTransitionSetupDataSO* __a, LevelCompletionResults* __b) {
                    p_SongFinishedCallback(__a, __b, l_DifficultyBeatmap);
                }));

                m_MenuTransitionsHelper->StartStandardLevel(
                    "Solo",
                    l_DifficultyBeatmap,
                    reinterpret_cast<IPreviewBeatmapLevel*>(p_Level),
                    p_OverrideEnvironmentSettings,
                    p_ColorScheme,
                    p_GameplayModifiers ? p_GameplayModifiers : GameplayModifiers::New_ctor(),
                    p_PlayerSettings    ? p_PlayerSettings    : PlayerSpecificSettings::New_ctor(),
                    nullptr,
                    p_MenuButtonText,
                    false,
                    false,
                    nullptr,
                    l_Delegate,
                    nullptr
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

    /// @brief Get a BeatmapLevel from a level ID
    /// @param p_LevelID  Level ID
    /// @param p_Callback Callback for success/failure
    void Levels::GetBeatmapLevelFromLevelID(std::u16string_view                                                 p_LevelID,
                                            _v::Action<_v::MonoPtr<IBeatmapLevel, true>>  p_Callback)
    {
        if (!m_BeatmapLevelsModel)
            m_BeatmapLevelsModel = Resources::FindObjectsOfTypeAll<BeatmapLevelsModel*>().FirstOrDefault();

        if (m_BeatmapLevelsModel)
        {
            if (m_GetLevelCancellationTokenSource)
                m_GetLevelCancellationTokenSource->Cancel();

            m_GetLevelCancellationTokenSource = CancellationTokenSource::New_ctor();

            try
            {
                auto l_Task = m_BeatmapLevelsModel->GetBeatmapLevelAsync(p_LevelID, m_GetLevelCancellationTokenSource->get_Token());

                _v::AwaitTaskAsync<BeatmapLevelsModel::GetBeatmapLevelResult>(
                    l_Task,
                    [=](_v::MonoPtrRef<Tasks::Task_1<BeatmapLevelsModel::GetBeatmapLevelResult>> p_Task, bool p_Success) {
                        try
                        {
                            if (p_Success && !p_Task->get_Result().isError)
                                p_Callback(p_Task->get_Result().beatmapLevel);
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

        auto l_LevelID              = SanitizeLevelID(p_LevelID);
        auto l_PreviewBeatmapLevel  = (IPreviewBeatmapLevel*)nullptr;
        if (!TryGetPreviewBeatmapLevelForLevelID(l_LevelID, &l_PreviewBeatmapLevel))
        {
            if (p_HaveAllScores) *p_HaveAllScores = false;
            return l_Results;
        }

        auto l_PlayerDataModel = Resources::FindObjectsOfTypeAll<PlayerDataModel*>().FirstOrDefault();
        PreviewDifficultyBeatmapSets_ForEach(l_PreviewBeatmapLevel, [&](PreviewDifficultyBeatmapSet* l_Current) -> bool
        {
            if (!l_Results.contains(l_Current->beatmapCharacteristic))
                l_Results[l_Current->beatmapCharacteristic] = t_CharacteristicScores();

            for (auto l_Difficulty : l_Current->beatmapDifficulties)
            {
                auto l_ScoreSO = l_PlayerDataModel->playerData->GetPlayerLevelStatsData(l_LevelID, l_Difficulty, l_Current->beatmapCharacteristic);

                if (l_ScoreSO->validScore)
                {
                    if (p_HaveAnyScore) *p_HaveAnyScore = true;
                    l_Results[l_Current->beatmapCharacteristic].push_back(std::make_tuple(l_Difficulty, l_ScoreSO->highScore));
                }
                else
                {
                    if (p_HaveAllScores) *p_HaveAllScores = false;
                    l_Results[l_Current->beatmapCharacteristic].push_back(std::make_tuple(l_Difficulty, -1));
                }
            }

            return true;    ///< Continue
        });

        return l_Results;
    }

}   ///< namespace CP_SDK_BS::Game
