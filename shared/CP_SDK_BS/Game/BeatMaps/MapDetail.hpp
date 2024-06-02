#pragma once

#include "../../../CP_SDK/Misc/Time.hpp"
#include "../../../CP_SDK/Utils/Json.hpp"
#include "MapDetailMetadata.hpp"
#include "MapStats.hpp"
#include "MapVersion.hpp"
#include "UserDetail.hpp"

#include <System/DateTime.hpp>

namespace CP_SDK_BS::Game::BeatMaps {

    namespace _u
    {
        using namespace System;
    }
    namespace _v
    {
        using namespace CP_SDK::Misc;
        using namespace CP_SDK::Utils;
    }

    struct CP_SDK_EXPORT_VISIBILITY MapDetail : public std::enable_shared_from_this<MapDetail>
    {
        MapDetail() = default;
        CP_SDK_NO_COPYMOVE_CTORS(MapDetail);

        using Ptr = std::shared_ptr<MapDetail>;

        std::u16string                      id;
        std::u16string                      name;
        std::u16string                      description;
        std::optional<UserDetail>           uploader;
        std::optional<MapDetailMetadata>    metadata;
        std::optional<MapStats>             stats;
        std::u16string                      uploaded;
        bool                                automapper  = false;
        bool                                ranked      = false;
        bool                                qualified   = false;
        std::vector<MapVersion::Ptr>        versions;

        /* NOT SERIALIZED */
        bool            Partial = true;
        std::u16string  PartialHash;

        /// @brief Partial BeatMap from ID
        /// @param p_Key ID of the BeatMap
        static MapDetail::Ptr PartialFromKey(std::u16string_view p_Key);
        /// @brief Partial BeatMap from ID
        /// @param p_Hash Hash of the BeatMap
        static MapDetail::Ptr PartialFromHash(std::u16string_view p_Hash);
        /// @brief Populate partial BeatMap
        /// @param p_Callback Completion/failure callback
        void Populate(_v::CActionRef<bool> p_Callback);

        /// @brief Is the BeatMap valid
        bool IsValid();
        /// @brief Select default valid version
        MapVersion::Ptr SelectMapVersion();

        /// @brief Get upload time
        _u::DateTime GetUploadTime();

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_STRING(id);
            CP_SDK_JSON_SERIALIZE_STRING(name);
            CP_SDK_JSON_SERIALIZE_STRING(description);
            CP_SDK_JSON_SERIALIZE_OBJECT(uploader);
            CP_SDK_JSON_SERIALIZE_OBJECT(metadata);
            CP_SDK_JSON_SERIALIZE_OBJECT(stats);
            CP_SDK_JSON_SERIALIZE_STRING(uploaded);
            CP_SDK_JSON_SERIALIZE_BOOL(automapper);
            CP_SDK_JSON_SERIALIZE_BOOL(ranked);
            CP_SDK_JSON_SERIALIZE_BOOL(qualified);
            CP_SDK_JSON_SERIALIZE_OBJECT_ARRAY(versions);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_STRING(id);
            CP_SDK_JSON_UNSERIALIZE_STRING(name);
            CP_SDK_JSON_UNSERIALIZE_STRING(description);
            CP_SDK_JSON_UNSERIALIZE_OBJECT(uploader);
            CP_SDK_JSON_UNSERIALIZE_OBJECT(metadata);
            CP_SDK_JSON_UNSERIALIZE_OBJECT(stats);
            CP_SDK_JSON_UNSERIALIZE_STRING(uploaded);
            CP_SDK_JSON_UNSERIALIZE_BOOL(automapper);
            CP_SDK_JSON_UNSERIALIZE_BOOL(ranked);
            CP_SDK_JSON_UNSERIALIZE_BOOL(qualified);
            CP_SDK_JSON_UNSERIALIZE_OBJECT_ARRAY(versions);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
