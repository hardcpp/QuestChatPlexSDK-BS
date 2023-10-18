#pragma once

#include "../../../CP_SDK/Utils/Json.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    struct UserDetail
    {
        int             id;
        std::u16string  name;
        std::u16string  hash;
        std::u16string  avatar;

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_INT32(id);
            CP_SDK_JSON_SERIALIZE_STRING(name);
            CP_SDK_JSON_SERIALIZE_STRING(hash);
            CP_SDK_JSON_SERIALIZE_STRING(avatar);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_INT32(id);
            CP_SDK_JSON_UNSERIALIZE_STRING(name);
            CP_SDK_JSON_UNSERIALIZE_STRING(hash);
            CP_SDK_JSON_UNSERIALIZE_STRING(avatar);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
