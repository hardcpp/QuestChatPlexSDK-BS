#include "CP_SDK_BS/Game/BeatMapsClient.hpp"
#include "CP_SDK_BS/Game/BeatMaps/SearchResponse.hpp"
#include "zip/src/zip.h"

#include <filesystem>

#include <songloader/shared/API.hpp>
#include <System/TimeSpan.hpp>
#include <System/IO/Path.hpp>
#include <System/Threading/CancellationTokenSource.hpp>

using namespace System::IO;

namespace CP_SDK_BS::Game {

    CP_SDK::Network::WebClientUnity::Ptr BeatMapsClient::m_WebClient;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK::Network::WebClientUnity::Ptr& BeatMapsClient::WebClient()
    {
        return m_WebClient;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init the BeatMaps client
    void BeatMapsClient::Init()
    {
        m_WebClient = CP_SDK::Network::WebClientUnity::Make(u"", _u::TimeSpan::FromSeconds(10));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get online by key
    /// @param p_Key      Key
    /// @param p_Callback Callback (p_Success, p_BeatMap)
    void BeatMapsClient::GetOnlineByKey(std::u16string_view p_Key, _v::CActionRef<bool, BeatMaps::MapDetail::Ptr> p_Callback)
    {
        m_WebClient->GetAsync(u"https://api.beatsaver.com/maps/id/" + p_Key, _u::CancellationToken::get_None(),
            [=](CP_SDK::Network::WebResponse::Ptr p_Result) -> void
            {
                try
                {
                    BeatMaps::MapDetail::Ptr l_BeatMap;
                    if (!p_Result->IsSuccessStatusCode()
                        || !GetObjectFromJsonString<BeatMaps::MapDetail>(p_Result->BodyString(), l_BeatMap))
                    {
                        p_Callback(p_Result->StatusCode() == System::Net::HttpStatusCode::NotFound, nullptr);
                        return;
                    }

                    l_BeatMap->Partial = false;
                    p_Callback(true, l_BeatMap);
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient.GetOnlineByKey] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_Callback(false, nullptr);
                }
            }
        );
    }
    /// @brief Get online by hash
    /// @param p_Hash     Hash
    /// @param p_Callback Callback (p_Success, p_BeatMap)
    void BeatMapsClient::GetOnlineByHash(std::u16string_view p_Hash, _v::CActionRef<bool, BeatMaps::MapDetail::Ptr> p_Callback)
    {
        m_WebClient->GetAsync(u"https://api.beatsaver.com/maps/hash/" + p_Hash, _u::CancellationToken::get_None(),
            [=](CP_SDK::Network::WebResponse::Ptr p_Result) -> void
            {
                try
                {
                    BeatMaps::MapDetail::Ptr l_BeatMap;
                    if (!p_Result->IsSuccessStatusCode()
                        || !GetObjectFromJsonString<BeatMaps::MapDetail>(p_Result->BodyString(), l_BeatMap))
                    {
                        p_Callback(p_Result->StatusCode() == System::Net::HttpStatusCode::NotFound, nullptr);
                        return;
                    }

                    l_BeatMap->Partial = false;
                    p_Callback(true, l_BeatMap);
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient.GetOnlineByHash] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_Callback(false, nullptr);
                }
            }
        );
    }
    /// @brief Get online by search
    /// @param p_Query    Search query
    /// @param p_Callback Callback (p_BeatMaps)
    void BeatMapsClient::GetOnlineBySearch(std::u16string_view p_Query, _v::CActionRef<bool, std::vector<BeatMaps::MapDetail::Ptr>&> p_Callback)
    {
        static std::vector<BeatMaps::MapDetail::Ptr> _default;

        m_WebClient->GetAsync(u"https://api.beatsaver.com/search/text/0?sortOrder=Relevance&q=" + _v::U16UrlEncode(p_Query), _u::CancellationToken::get_None(),
            [=](CP_SDK::Network::WebResponse::Ptr p_Result) -> void
            {
                try
                {
                    BeatMaps::SearchResponse::Ptr l_SearchResponse;
                    if (!p_Result->IsSuccessStatusCode()
                        || !GetObjectFromJsonString<BeatMaps::SearchResponse>(p_Result->BodyString(), l_SearchResponse))
                    {
                        p_Callback(false, _default);
                        return;
                    }

                    for (auto& l_Doc : l_SearchResponse->docs)
                        l_Doc->Partial = false;

                    p_Callback(true, l_SearchResponse->docs);
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient.GetOnlineBySearch] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_Callback(false, _default);
                }
            }
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Populate partial BeatMap by key
    /// @param p_BeatMap  BeatMap to populate
    /// @param p_Callback Callback (p_Success)
    void BeatMapsClient::PopulateOnlineByKey(BeatMaps::MapDetail::Ptr& p_BeatMap, _v::CActionRef<bool> p_Callback)
    {
        m_WebClient->GetAsync(u"https://api.beatsaver.com/maps/id/" + p_BeatMap->id, _u::CancellationToken::get_None(),
            [=](CP_SDK::Network::WebResponse::Ptr p_Result) -> void
            {
                try
                {
                    if (!p_Result->IsSuccessStatusCode())
                    {
                        p_Callback(false);
                        return;
                    }

                    _v::Json::U16Document l_Document;
                    _v::Json::TryFromU16String(p_Result->BodyString(), l_Document);
                    p_BeatMap->Unserialize(l_Document);
                    p_BeatMap->Partial = false;

                    p_Callback(true);
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient.PopulateOnlineByKey] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_Callback(false);
                }
            }
        );
    }
    /// @brief Populate partial BeatMap by hash
    /// @param p_BeatMap  BeatMap to populate
    /// @param p_Callback Callback (p_Success)
    void BeatMapsClient::PopulateOnlineByHash(BeatMaps::MapDetail::Ptr& p_BeatMap, _v::CActionRef<bool> p_Callback)
    {
        m_WebClient->GetAsync(u"https://api.beatsaver.com/maps/hash/" + p_BeatMap->PartialHash, _u::CancellationToken::get_None(),
            [=](CP_SDK::Network::WebResponse::Ptr p_Result) -> void
            {
                try
                {
                    if (!p_Result->IsSuccessStatusCode())
                    {
                        p_Callback(false);
                        return;
                    }

                    _v::Json::U16Document l_Document;
                    _v::Json::TryFromU16String(p_Result->BodyString(), l_Document);
                    p_BeatMap->Unserialize(l_Document);
                    p_BeatMap->Partial = false;

                    p_Callback(true);
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient.PopulateOnlineByHash] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_Callback(false);
                }
            }
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Download a song
    /// @param p_Song     Beat map
    /// @param p_Version  Version
    /// @param p_Token    Cancellation token
    /// @param p_Callback Callback
    /// @param p_Progress Progress reporter
    void BeatMapsClient::DownloadSong(  BeatMaps::MapDetail::Ptr                    p_Song,
                                        BeatMaps::MapVersion::Ptr                   p_Version,
                                        _u::CancellationToken                       p_Token,
                                        _v::CActionRef<bool, std::u16string_view>   p_Callback,
                                        _v::CActionRef<float>                       p_Progress)
    {
        static std::u16string _default;

        if (!p_Song || !p_Version)
        {
            p_Callback(false, _default);
            return;
        }

        p_Version->ZipBytes(
            p_Token,
            [=](_v::CMonoPtrRef<::Array<uint8_t>> p_Result) -> void
            {
                if (!p_Result || (p_Token._source && p_Token._source->get_IsCancellationRequested()))
                {
                    p_Callback(false, _default);
                    return;
                }

                try
                {
                    std::u16string l_CustomSongsPath = _v::StrToU16Str(RuntimeSongLoader::API::GetCustomLevelsPath());

                    CP_SDK::ChatPlexSDK::Logger()->Info(u"[CP_SDK_BS.Game][BeatMapsClient] Downloaded zip!");

                    if (p_Token._source && p_Token._source->get_IsCancellationRequested())
                    {
                        p_Callback(false, _default);
                        return;
                    }

                    auto l_ExtractResult = ExtractZipAsync(p_Song, p_Result, l_CustomSongsPath);
                    p_Callback(l_ExtractResult.first, l_ExtractResult.second);
                }
                catch (const std::exception& p_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient] Failed to download Song!");
                }
            },
            true,
            p_Progress
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Extract ZIP archive
    /// @param p_Song            Beat map
    /// @param p_ZIPBytes        Raw ZIP bytes
    /// @param p_CustomSongsPath Extract path
    /// @param p_Overwrite       Should overwrite ?
    std::pair<bool, std::u16string> BeatMapsClient::ExtractZipAsync(BeatMaps::MapDetail::Ptr            p_Song,
                                                                    _v::CMonoPtrRef<::Array<uint8_t>>   p_ZIPBytes,
                                                                    std::u16string_view                 p_CustomSongsPath,
                                                                    bool                                p_Overwrite)
    {

        try
        {
            CP_SDK::ChatPlexSDK::Logger()->Info(u"[CP_SDK_BS.Game][BeatMapsClient] Extracting...");

            /// Prepare base path
            auto l_BasePath     = p_Song->id + u" (" + p_Song->metadata.value().songName + u" - " + p_Song->metadata.value().levelAuthorName + u")";
            auto l_InvalidChars = Path::GetInvalidPathChars();

            for (auto l_I = 0; l_I < l_BasePath.length(); ++l_I)
            {
                if (!l_InvalidChars.Contains(l_BasePath[l_I]))
                    continue;

                l_BasePath = l_BasePath.substr(0, l_I) + l_BasePath.substr(l_I + 1);
            }

            /// Build out path
            auto l_OutPath = _v::U16StrToStr(u"" + p_CustomSongsPath + (p_CustomSongsPath.back() != u'/' ? u"/" : u"") + l_BasePath);

            /// Check to avoid overwrite
            if (!p_Overwrite && std::filesystem::exists(l_OutPath))
            {
                int l_FolderCount = 1;

                while (std::filesystem::exists(l_OutPath + " (" + std::to_string(l_FolderCount) + ")"))
                    ++l_FolderCount;

                l_OutPath   +=  " (" + std::to_string(l_FolderCount) +  ")";
                l_BasePath  += u" (" + std::to_string(l_FolderCount) + u")";
            }

            CP_SDK::ChatPlexSDK::Logger()->Info(u"[CP_SDK_BS.Game][BeatMapsClient] " + l_OutPath);

            /// zip_stream_extract create directory if needed
            int  l_Args = 2;
            auto l_ExtractResult = zip_stream_extract((const char*)p_ZIPBytes->values, p_ZIPBytes->Length(), l_OutPath.c_str(), +[](const char*, void*) -> int {
                return 0;
            }, &l_Args);

            if (l_ExtractResult != 0)
            {
                CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient] Unable to extract ZIP!");
                return std::make_pair(false, u"");
            }

            return std::make_pair(true, l_BasePath);
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient] Unable to extract ZIP! Exception");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);
        }

        return std::make_pair(false, u"");
    }

}   ///< namespace CP_SDK_BS::Game
