#include "CP_SDK_BS/Game/BeatMaps/MapDetail.hpp"
#include "CP_SDK_BS/Game/BeatMapsClient.hpp"
#include "CP_SDK/Misc/Time.hpp"

namespace CP_SDK_BS::Game::BeatMaps {

    /// @brief Partial BeatMap from ID
    /// @param p_Key ID of the BeatMap
    MapDetail::Ptr MapDetail::PartialFromKey(std::u16string_view p_Key)
    {
        auto l_New = std::make_shared<MapDetail>();
        l_New->id = p_Key;

        return l_New;
    }
    /// @brief Partial BeatMap from ID
    /// @param p_Hash Hash of the BeatMap
    MapDetail::Ptr MapDetail::PartialFromHash(std::u16string_view p_Hash)
    {
        auto l_New = std::make_shared<MapDetail>();
        l_New->PartialHash = p_Hash;

        return l_New;
    }
    /// @brief Populate partial BeatMap
    /// @param p_Callback Completion/failure callback
    void MapDetail::Populate(_v::CActionRef<bool> p_Callback)
    {
        if (PartialHash.empty() && Partial)
        {
            auto l_Self = shared_from_this();
            BeatMapsClient::PopulateOnlineByKey(l_Self, p_Callback);
        }
        else if (!PartialHash.empty() && Partial)
        {
            auto l_Self = shared_from_this();
            BeatMapsClient::PopulateOnlineByHash(l_Self, p_Callback);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Is the BeatMap valid
    bool MapDetail::IsValid()
    {
        if (Partial || id.empty())
            return false;

        auto l_Version = SelectMapVersion();
        if (!l_Version)
            return false;

        return l_Version->diffs.size() >= 1;
    }
    /// @brief Select default valid version
    MapVersion::Ptr MapDetail::SelectMapVersion()
    {
        if (versions.size() == 0)
            return nullptr;

        return versions.back();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get upload time
    _u::DateTime MapDetail::GetUploadTime()
    {
        _u::DateTime l_Date;
        if (!uploaded.empty() && _v::Time::TryParseInternational(uploaded, l_Date))
            return l_Date;
        else if (uploaded.empty() && versions.size() >= 1
            && _v::Time::TryParseInternational(SelectMapVersion()->createdAt, l_Date))
            return l_Date;

        return _v::Time::FromUnixTime(0);
    }

}   ///< namespace CP_SDK_BS::Game::BeatMaps
