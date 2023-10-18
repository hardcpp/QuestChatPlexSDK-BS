#pragma once

#include "../../../CP_SDK/Utils/Json.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    struct MapDetailMetadata
    {
        float bpm = 0;
        int duration = 0;
        std::u16string songName = u"";
        std::u16string songSubName = u"";
        std::u16string songAuthorName = u"";
        std::u16string levelAuthorName = u"";

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_FLOAT(bpm);
            CP_SDK_JSON_SERIALIZE_INT32(duration);
            CP_SDK_JSON_SERIALIZE_STRING(songName);
            CP_SDK_JSON_SERIALIZE_STRING(songSubName);
            CP_SDK_JSON_SERIALIZE_STRING(songAuthorName);
            CP_SDK_JSON_SERIALIZE_STRING(levelAuthorName);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_FLOAT(bpm);
            CP_SDK_JSON_UNSERIALIZE_INT32(duration);
            CP_SDK_JSON_UNSERIALIZE_STRING(songName);
            CP_SDK_JSON_UNSERIALIZE_STRING(songSubName);
            CP_SDK_JSON_UNSERIALIZE_STRING(songAuthorName);
            CP_SDK_JSON_UNSERIALIZE_STRING(levelAuthorName);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
