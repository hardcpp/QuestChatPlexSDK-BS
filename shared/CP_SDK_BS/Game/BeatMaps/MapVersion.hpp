#pragma once

#include "../../../CP_SDK/Utils/Json.hpp"
#include "../../../CP_SDK/Utils/Delegate.hpp"
#include "../../../CP_SDK/Utils/MonoPtr.hpp"
#include "MapDifficulty.hpp"

#include <System/Threading/CancellationToken.hpp>

namespace CP_SDK_BS::Game::BeatMaps {

    namespace _u
    {
        using namespace System::Threading;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    struct MapVersionStates
    {
        enum class E
        {
            UNK,
            Uploaded,
            Testplay,
            Published,
            Feedback
        };

        static constexpr std::array<std::u16string_view, 5> const S = {
            u"UNK",
            u"Uploaded",
            u"Testplay",
            u"Published",
            u"Feedback"
        };

        CP_SDK_IL2CPP_ENUM_UTILS();
    };

    struct MapVersion
    {
        MapVersion() = default;
        DISABLE_COPY_MOVECONSTRUCTORS(MapVersion);

        using Ptr = std::shared_ptr<MapVersion>;

        std::u16string                  hash;
        MapVersionStates::E             state       = MapVersionStates::E::UNK;
        std::u16string                  createdAt;
        int                             sageScore   = 0;
        std::vector<MapDifficulty::Ptr> diffs;
        std::u16string                  downloadURL;
        std::u16string                  coverURL;
        std::u16string                  previewURL;

        /// @brief Get distinct list of BeatmapCharacteristicSO serialized names in order
        std::vector<std::u16string>     GetBeatmapCharacteristicSOSerializedNamesInOrder();
        /// @brief Get all difficulties for a specific BeatmapCharacteristicSO
        /// @param p_BeatmapCharacteristicSerializedName Target BeatmapCharacteristicSO serialized name
        std::vector<MapDifficulty::Ptr> GetDifficultiesPerBeatmapCharacteristicSOSerializedName(std::u16string_view p_BeatmapCharacteristicSerializedName);

        /// @brief Get cover image bytes
        /// @param p_Callback Callback (p_Valid, p_Bytes)
        void CoverImageBytes(_v::Action<bool, _v::CMonoPtrRef<::Array<uint8_t>>> p_Callback);
        /// @brief Get Zip archive bytes
        /// @param p_Token     Cancellation token
        /// @param p_Callback  Callback on result
        /// @param p_DontRetry Should not retry in case of failure?
        /// @param p_Progress  Progress reporte
        void ZipBytes(_u::CancellationToken p_Token, _v::CActionRef<_v::CMonoPtrRef<::Array<uint8_t>>> p_Callback, bool p_DontRetry = true, _v::CActionRef<float> p_Progress = nullptr);

        CP_SDK_JSON_OPERATORS_INLINE({
            CP_SDK_JSON_SERIALIZE_STRING(hash);
            CP_SDK_JSON_SERIALIZE_ENUM(state, MapVersionStates);
            CP_SDK_JSON_SERIALIZE_STRING(createdAt);
            CP_SDK_JSON_SERIALIZE_INT32(sageScore);
            CP_SDK_JSON_SERIALIZE_OBJECT_ARRAY(diffs);
            CP_SDK_JSON_SERIALIZE_STRING(downloadURL);
            CP_SDK_JSON_SERIALIZE_STRING(coverURL);
            CP_SDK_JSON_SERIALIZE_STRING(previewURL);
        },
        {
            CP_SDK_JSON_UNSERIALIZE_STRING(hash);
            CP_SDK_JSON_UNSERIALIZE_ENUM(state, MapVersionStates, MapVersionStates::E::UNK);
            CP_SDK_JSON_UNSERIALIZE_STRING(createdAt);
            CP_SDK_JSON_UNSERIALIZE_INT32(sageScore);
            CP_SDK_JSON_UNSERIALIZE_OBJECT_ARRAY(diffs);
            CP_SDK_JSON_UNSERIALIZE_STRING(downloadURL);
            CP_SDK_JSON_UNSERIALIZE_STRING(coverURL);
            CP_SDK_JSON_UNSERIALIZE_STRING(previewURL);
        });
    };

}   ///< namespace CP_SDK_BS::Game::BeatMaps
