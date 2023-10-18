#pragma once

#include "../../../CP_SDK/Utils/Json.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    struct MapStats
    {
        int     plays       = 0;
        int     downloads   = 0;
        int     upvotes     = 0;
        int     downvotes   = 0;
        float   score       = 0;

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_INT32(plays);
            CP_SDK_JSON_SERIALIZE_INT32(downloads);
            CP_SDK_JSON_SERIALIZE_INT32(upvotes);
            CP_SDK_JSON_SERIALIZE_INT32(downvotes);
            CP_SDK_JSON_SERIALIZE_FLOAT(score);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_INT32(plays);
            CP_SDK_JSON_UNSERIALIZE_INT32(downloads);
            CP_SDK_JSON_UNSERIALIZE_INT32(upvotes);
            CP_SDK_JSON_UNSERIALIZE_INT32(downvotes);
            CP_SDK_JSON_UNSERIALIZE_FLOAT(score);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
