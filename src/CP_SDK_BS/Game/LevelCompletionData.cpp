#include "CP_SDK_BS/Game/LevelCompletionData.hpp"
#include "CP_SDK_BS/Game/Levels.hpp"

#include <songloader/shared/API.hpp>

#include <GlobalNamespace/IReadonlyBeatmapData.hpp>
#include <GlobalNamespace/IDifficultyBeatmap.hpp>
#include <GlobalNamespace/IDifficultyBeatmapSet.hpp>

namespace CP_SDK_BS::Game {

    bool LevelCompletionData::IsNoodle()
    {
        if (!Data)
            return false;

        std::vector<std::u16string> l_Requirements;
        if (Levels::TryGetCustomRequirementsFor(Data->previewBeatmapLevel, Data->difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic(), Data->difficultyBeatmap->get_difficulty(), &l_Requirements))
        {
            for (auto& l_Current : l_Requirements)
            {
                if (_v::U16EqualsToCaseInsensitive(l_Current, u"Noodle Extensions"))
                    continue;

                return true;
            }
        }

        return false;
    }
    bool LevelCompletionData::IsChroma()
    {
        if (!Data)
            return false;

        std::vector<std::u16string> l_Requirements;
        if (Levels::TryGetCustomRequirementsFor(Data->previewBeatmapLevel, Data->difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic(), Data->difficultyBeatmap->get_difficulty(), &l_Requirements))
        {
            for (auto& l_Current : l_Requirements)
            {
                if (_v::U16EqualsToCaseInsensitive(l_Current, u"Chroma"))
                    continue;

                return true;
            }
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    LevelCompletionData::LevelCompletionData(PRIVATE_TAG_ARG())
    {

    }
    /// @brief Constructor
    LevelCompletionData::Ptr LevelCompletionData::Make()
    {
        return std::make_shared<LevelCompletionData>(PRIVATE_TAG_VAL());
    }

}   ///< namespace CP_SDK_BS::Game