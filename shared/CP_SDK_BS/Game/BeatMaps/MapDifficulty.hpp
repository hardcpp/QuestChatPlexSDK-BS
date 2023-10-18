#pragma once

#include "../../../CP_SDK/Utils/Json.hpp"
#include "MapParitySummary.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    struct MapDifficulty
    {
        MapDifficulty() = default;
        DISABLE_COPY_MOVECONSTRUCTORS(MapDifficulty);

        using Ptr = std::shared_ptr<MapDifficulty>;

        float                           njs             = 0;
        float                           offset          = 0;
        int                             notes           = 0;
        int                             bombs           = 0;
        int                             obstacles       = 0;
        float                           nps             = 0;
        float                           length          = 0;
        std::u16string                  characteristic  = u"";
        std::u16string                  difficulty      = u"";
        int                             events          = 0;
        bool                            chroma          = false;
        bool                            me              = false;
        bool                            ne              = false;
        bool                            cinema          = false;
        float                           seconds         = 0;
        std::optional<MapParitySummary> paritySummary;
        //float                           stars           = 0;

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_FLOAT(njs);
            CP_SDK_JSON_SERIALIZE_FLOAT(offset);
            CP_SDK_JSON_SERIALIZE_INT32(notes);
            CP_SDK_JSON_SERIALIZE_INT32(bombs);
            CP_SDK_JSON_SERIALIZE_INT32(obstacles);
            CP_SDK_JSON_SERIALIZE_FLOAT(nps);
            CP_SDK_JSON_SERIALIZE_FLOAT(length);
            CP_SDK_JSON_SERIALIZE_STRING(characteristic);
            CP_SDK_JSON_SERIALIZE_STRING(difficulty);
            CP_SDK_JSON_SERIALIZE_INT32(events);
            CP_SDK_JSON_SERIALIZE_BOOL(chroma);
            CP_SDK_JSON_SERIALIZE_BOOL(me);
            CP_SDK_JSON_SERIALIZE_BOOL(ne);
            CP_SDK_JSON_SERIALIZE_BOOL(cinema);
            CP_SDK_JSON_SERIALIZE_FLOAT(seconds);
            CP_SDK_JSON_SERIALIZE_OBJECT(paritySummary);
            //CP_SDK_JSON_SERIALIZE_FLOAT(stars);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_FLOAT(njs);
            CP_SDK_JSON_UNSERIALIZE_FLOAT(offset);
            CP_SDK_JSON_UNSERIALIZE_INT32(notes);
            CP_SDK_JSON_UNSERIALIZE_INT32(bombs);
            CP_SDK_JSON_UNSERIALIZE_INT32(obstacles);
            CP_SDK_JSON_UNSERIALIZE_FLOAT(nps);
            CP_SDK_JSON_UNSERIALIZE_FLOAT(length);
            CP_SDK_JSON_UNSERIALIZE_STRING(characteristic);
            CP_SDK_JSON_UNSERIALIZE_STRING(difficulty);
            CP_SDK_JSON_UNSERIALIZE_INT32(events);
            CP_SDK_JSON_UNSERIALIZE_BOOL(chroma);
            CP_SDK_JSON_UNSERIALIZE_BOOL(me);
            CP_SDK_JSON_UNSERIALIZE_BOOL(ne);
            CP_SDK_JSON_UNSERIALIZE_BOOL(cinema);
            CP_SDK_JSON_UNSERIALIZE_FLOAT(seconds);
            CP_SDK_JSON_UNSERIALIZE_OBJECT(paritySummary);
            //CP_SDK_JSON_UNSERIALIZE_FLOAT(stars);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
