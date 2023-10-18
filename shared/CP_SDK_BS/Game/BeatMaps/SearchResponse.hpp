#pragma once

#include "MapDetail.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    using namespace System;

    struct SearchResponse
    {
        SearchResponse() = default;
        DISABLE_COPY_MOVECONSTRUCTORS(SearchResponse);

        using Ptr = std::shared_ptr<SearchResponse>;

        std::vector<MapDetail::Ptr> docs;

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_OBJECT_ARRAY(docs);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_OBJECT_ARRAY(docs);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
