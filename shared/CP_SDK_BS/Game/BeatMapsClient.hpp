#pragma once

#include "BeatMaps/MapDetail.hpp"
#include "../../CP_SDK/Network/WebClientUnity.hpp"
#include "../../CP_SDK/ChatPlexSDK.hpp"

#include <System/Threading/CancellationToken.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace System;
        using namespace System::Threading;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
        using namespace CP_SDK::Network;
    }

    /// @brief BeatMaps client
    class BeatMapsClient
    {
        CP_SDK_NO_DEF_CTORS(BeatMapsClient);

        private:
            static _v::WebClientUnity::Ptr m_WebClient;

        public:
            static _v::WebClientUnity::Ptr& WebClient();

        public:
            /// @brief Init the BeatMaps client
            static void Init();

        public:
            /// @brief Get online by key
            /// @param p_Key      Key
            /// @param p_Callback Callback (p_Success, p_BeatMap)
            static void GetOnlineByKey(std::u16string_view p_Key, _v::CActionRef<bool, BeatMaps::MapDetail::Ptr> p_Callback);
            /// @brief Get online by hash
            /// @param p_Hash     Hash
            /// @param p_Callback Callback (p_Success, p_BeatMap)
            static void GetOnlineByHash(std::u16string_view p_Hash, _v::CActionRef<bool, BeatMaps::MapDetail::Ptr> p_Callback);
            /// @brief Get online by search
            /// @param p_Query    Search query
            /// @param p_Callback Callback (p_BeatMaps)
            static void GetOnlineBySearch(std::u16string_view p_Query, _v::CActionRef<bool, std::vector<BeatMaps::MapDetail::Ptr>&> p_Callback);

        public:
            /// @brief Populate partial BeatMap by key
            /// @param p_BeatMap  BeatMap to populate
            /// @param p_Callback Callback (p_Success)
            static void PopulateOnlineByKey(BeatMaps::MapDetail::Ptr& p_BeatMap, _v::CActionRef<bool> p_Callback);
            /// @brief Populate partial BeatMap by hash
            /// @param p_BeatMap  BeatMap to populate
            /// @param p_Callback Callback (p_Success)
            static void PopulateOnlineByHash(BeatMaps::MapDetail::Ptr& p_BeatMap, _v::CActionRef<bool> p_Callback);

        public:
            /// @brief Download a song
            /// @param p_Song     Beat map
            /// @param p_Version  Version
            /// @param p_Token    Cancellation token
            /// @param p_Callback Callback
            /// @param p_Progress Progress reporter
            static void DownloadSong(   BeatMaps::MapDetail::Ptr                    p_Song,
                                        BeatMaps::MapVersion::Ptr                   p_Version,
                                        _u::CancellationToken                       p_Token,
                                        _v::CActionRef<bool, std::u16string_view>   p_Callback,
                                        _v::CActionRef<float>                       p_Progress = nullptr);

        private:
            /// @brief Extract ZIP archive
            /// @param p_Song            Beat map
            /// @param p_ZIPBytes        Raw ZIP bytes
            /// @param p_CustomSongsPath Extract path
            /// @param p_Overwrite       Should overwrite ?
            static std::pair<bool, std::u16string> ExtractZipAsync( BeatMaps::MapDetail::Ptr            p_Song,
                                                                    _v::CMonoPtrRef<::Array<uint8_t>>   p_ZIPBytes,
                                                                    std::u16string_view                 p_CustomSongsPath,
                                                                    bool                                p_Overwrite = false);

        private:
            /// @brief Get Object from serialized JSON
            /// @tparam t_Type Object type
            /// @param p_Serialized Input
            /// @param p_Object     Result object
            /// @return Object or null
            template<typename t_Type>
            static bool GetObjectFromJsonString(std::u16string_view p_Serialized, std::shared_ptr<t_Type>& p_Object)
            {
                p_Object = nullptr;
                try
                {
                    _v::Json::U16Document l_Document;
                    if (!_v::Json::TryFromU16String(p_Serialized, l_Document))
                        return false;

                    p_Object = std::make_shared<t_Type>();
                    p_Object->Unserialize(l_Document);
                }
                catch (const std::exception& l_Exception)
                {
                    p_Object = nullptr;
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][BeatMapsClient.GetObjectFromJsonString] Error :");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    return false;
                }

                return p_Object != nullptr;
            }

    };

}   ///< namespace CP_SDK_BS::Game
