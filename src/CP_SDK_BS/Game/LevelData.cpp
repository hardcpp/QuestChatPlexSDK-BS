#include "CP_SDK_BS/Game/LevelData.hpp"
#include "CP_SDK_BS/Game/Levels.hpp"

#include <songloader/shared/API.hpp>

#include <GlobalNamespace/IReadonlyBeatmapData.hpp>
#include <GlobalNamespace/IDifficultyBeatmap.hpp>
#include <GlobalNamespace/IDifficultyBeatmapSet.hpp>

namespace CP_SDK_BS::Game {

    bool LevelData::HasRotations()
    {
        if (!Data || !Data->transformedBeatmapData)
            return false;

        return Data->transformedBeatmapData->get_spawnRotationEventsCount() > 0;
    }
    bool LevelData::IsNoodle()
    {
        if (!Data || !Data->previewBeatmapLevel)
            return false;

        std::vector<std::u16string> l_Requirements;
        if (Levels::TryGetCustomRequirementsFor(Data->previewBeatmapLevel, Data->difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic(), Data->difficultyBeatmap->get_difficulty(), &l_Requirements))
        {
            for (auto& l_Current : l_Requirements)
            {
                if (CP_SDK::Utils::U16EqualsToCaseInsensitive(l_Current, u"Noodle Extensions"))
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
        if (Levels::TryGetCustomRequirementsFor(Data->previewBeatmapLevel, Data->difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic(), Data->difficultyBeatmap->get_difficulty(), &l_Requirements))
        {
            for (auto& l_Current : l_Requirements)
            {
                if (CP_SDK::Utils::U16EqualsToCaseInsensitive(l_Current, u"Chroma"))
                    continue;

                return true;
            }
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    LevelData::LevelData(PRIVATE_TAG_ARG())
    {

    }
    /// @brief Constructor
    LevelData::Ptr LevelData::Make()
    {
        return std::make_shared<LevelData>(PRIVATE_TAG_VAL());
    }

}   ///< namespace CP_SDK_BS::Game