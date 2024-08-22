#include "CP_SDK_BS/Game/LevelData.hpp"
#include "CP_SDK_BS/Game/Levels.hpp"

#include <songcore/shared/SongCore.hpp>

#include <GlobalNamespace/IReadonlyBeatmapData.hpp>
#include <GlobalNamespace/BeatmapKey.hpp>

namespace CP_SDK_BS::Game {

    bool LevelData::HasRotations()
    {
        if (!Data || !Data->get_transformedBeatmapData())
            return false;

        return Data->get_transformedBeatmapData()->get_spawnRotationEventsCount() > 0;
    }
    bool LevelData::IsNoodle()
    {
        if (!Data || !Data->___beatmapLevel)
            return false;

        std::vector<std::u16string> l_Requirements;
        if (Levels::TryGetCustomRequirementsFor(Data->___beatmapLevel, Data->___beatmapKey.beatmapCharacteristic, Data->___beatmapKey.difficulty, &l_Requirements))
        {
            for (auto& l_Current : l_Requirements)
            {
                if (CP_SDK::Utils::U16EqualsToCaseInsensitive(Levels::SanatizeMappingCapability(l_Current), u"NoodleExtensions"))
                    continue;

                return true;
            }
        }

        return false;
    }
    bool LevelData::IsChroma()
    {
        if (!Data)
            return false;

        std::vector<std::u16string> l_Requirements;
        if (Levels::TryGetCustomRequirementsFor(Data->___beatmapLevel, Data->___beatmapKey.beatmapCharacteristic, Data->___beatmapKey.difficulty, &l_Requirements))
        {
            for (auto& l_Current : l_Requirements)
            {
                if (CP_SDK::Utils::U16EqualsToCaseInsensitive(Levels::SanatizeMappingCapability(l_Current), u"Chroma"))
                    continue;

                return true;
            }
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    LevelData::LevelData(CP_SDK_PRIV_TAG_ARG())
    {

    }
    /// @brief Constructor
    LevelData::Ptr LevelData::Make()
    {
        return std::make_shared<LevelData>(CP_SDK_PRIV_TAG_VAL());
    }

}   ///< namespace CP_SDK_BS::Game