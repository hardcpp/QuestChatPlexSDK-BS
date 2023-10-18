#include "CP_SDK_BS/Game/BeatMaps/MapVersion.hpp"
#include "CP_SDK_BS/Game/BeatMapsClient.hpp"
#include "CP_SDK_BS/Game/Levels.hpp"

using namespace GlobalNamespace;

namespace CP_SDK_BS::Game::BeatMaps {

    /// @brief Get distinct list of BeatmapCharacteristicSO serialized names in order
    std::vector<std::u16string> MapVersion::GetBeatmapCharacteristicSOSerializedNamesInOrder()
    {
        std::vector<std::u16string>                         l_Result;
        std::vector<std::pair<int, std::u16string_view>>    l_Intermediate;

        for (auto& l_Diff : diffs)
        {
            if (l_Diff->characteristic.empty())
                continue;

            auto l_Existing = std::find_if(l_Intermediate.begin(), l_Intermediate.end(), [&](auto& p_Pair) { return p_Pair.second == l_Diff->characteristic; });
            if (l_Existing != l_Intermediate.end())
                continue;

            l_Intermediate.push_back(std::make_pair(Levels::GetBeatmapCharacteristicSOOrdering(l_Diff->characteristic), Levels::SanitizeBeatmapCharacteristicSOSerializedName(l_Diff->characteristic)));
        }

        std::sort(l_Intermediate.begin(), l_Intermediate.end(), [](auto& p_Left, auto& p_Right) {
            return p_Left.first < p_Right.first;
        });

        l_Result.reserve(l_Intermediate.size());
        for (auto& l_Pair: l_Intermediate)
            l_Result.push_back(std::u16string(l_Pair.second));

        return l_Result;
    }
    /// @brief Get all difficulties for a specific BeatmapCharacteristicSO
    /// @param p_BeatmapCharacteristicSerializedName Target BeatmapCharacteristicSO serialized name
    std::vector<MapDifficulty::Ptr> MapVersion::GetDifficultiesPerBeatmapCharacteristicSOSerializedName(std::u16string_view p_BeatmapCharacteristicSerializedName)
    {
        std::vector<MapDifficulty::Ptr> l_Result;

        auto l_BeatmapCharacteristicSOSerializedName    = Levels::SanitizeBeatmapCharacteristicSOSerializedName(p_BeatmapCharacteristicSerializedName);
        auto l_CharacteristicSO                         = (BeatmapCharacteristicSO*)nullptr;
        for (auto& l_Diff : diffs)
        {
            if (   !Levels::TryGetBeatmapCharacteristicSOBySerializedName(l_Diff->characteristic, &l_CharacteristicSO)
                || !_v::U16EqualsToCaseInsensitive(l_CharacteristicSO->serializedName, l_BeatmapCharacteristicSOSerializedName))
                continue;

            l_Result.push_back(l_Diff);
        }

        return l_Result;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get cover image bytes
    /// @param p_Callback Callback (p_Valid, p_Bytes)
    void MapVersion::CoverImageBytes(_v::Action<bool, _v::CMonoPtrRef<::Array<uint8_t>>> p_Callback)
    {
        BeatMapsClient::WebClient()->GetAsync(
            coverURL,
            _u::CancellationToken::get_None(),
            [=](CP_SDK::Network::WebResponse::Ptr p_Result) -> void
            {
                try
                {
                    if (!p_Result)
                    {
                        p_Callback(false, nullptr);
                        return;
                    }

                    p_Callback(true, p_Result->BodyBytes());
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.BeatMaps][Version.CoverImageBytes] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_Callback(false, nullptr);
                }
            }
        );
    }
    /// @brief Get Zip archive bytes
    /// @param p_Token     Cancellation token
    /// @param p_Callback  Callback on result
    /// @param p_DontRetry Should not retry in case of failure?
    /// @param p_Progress  Progress reporter
    void MapVersion::ZipBytes(_u::CancellationToken p_Token, _v::CActionRef<_v::CMonoPtrRef<::Array<uint8_t>>> p_Callback, bool p_DontRetry, _v::CActionRef<float> p_Progress)
    {
        BeatMapsClient::WebClient()->DownloadAsync(
            downloadURL,
            p_Token,
            [=](CP_SDK::Network::WebResponse::Ptr x) { p_Callback(x ? x->BodyBytes() : nullptr); },
            p_DontRetry,
            p_Progress
        );
    }

}   ///< namespace CP_SDK_BS::Game::BeatMaps
