#pragma once

#include "../../../CP_SDK/Utils/Json.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    struct MapParitySummary
    {
        int errors = 0;
        int warns = 0;
        int resets = 0;

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_INT32(errors);
            CP_SDK_JSON_SERIALIZE_INT32(warns);
            CP_SDK_JSON_SERIALIZE_INT32(resets);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_INT32(errors);
            CP_SDK_JSON_UNSERIALIZE_INT32(warns);
            CP_SDK_JSON_UNSERIALIZE_INT32(resets);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
