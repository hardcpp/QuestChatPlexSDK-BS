#include "CP_SDK_BS/Game/PlayerAvatarPicture.hpp"
#include "CP_SDK/Network/WebClientUnity.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/Unity/Texture2DU.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Utils/Json.hpp"

#include <UnityEngine/Rect.hpp>
#include <UnityEngine/Texture2D.hpp>

using namespace CP_SDK::Network;

namespace CP_SDK_BS::Game {

    /// @brief Get player avatar picture
    /// @param p_PlayerID          ID of the player
    /// @param p_CancellationToken Cancellation token
    /// @param p_Callback          Request callback
    void PlayerAvatarPicture::GetPlayerAvatarPicture(std::u16string p_PlayerID, _u::CancellationToken p_CancellationToken, t_CallBack p_Callback)
    {
        GetScoreSaberAvatarPicture(p_PlayerID, p_CancellationToken, p_Callback, [=]()
        {
            GetBeatLeaderAvatarPicture(p_PlayerID, p_CancellationToken, p_Callback, [=]() { p_Callback(nullptr); });
        });
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get avatar picture from ScoreSaber
    /// @param p_PlayerID          ID of the player
    /// @param p_CancellationToken Cancellation token
    /// @param p_Callback          Request callback
    /// @param p_OnFailCallback    On error callback
    void PlayerAvatarPicture::GetScoreSaberAvatarPicture(std::u16string p_PlayerID, _u::CancellationToken p_CancellationToken, t_CallBack p_Callback, t_CallBackFailed p_OnFailCallback)
    {
        WebClientUnity::GlobalClient()->GetAsync(u"https://cdn.scoresaber.com/avatars/" + p_PlayerID + ".jpg", p_CancellationToken,
            [=](WebResponse::Ptr p_AvatarResult)
            {
                try
                {
                    if (   p_AvatarResult == nullptr    || !p_AvatarResult->IsSuccessStatusCode()
                        || !p_AvatarResult->BodyBytes() || p_AvatarResult->BodyBytes()->get_Length() == 0)
                    {
                        p_OnFailCallback();
                        return;
                    }

                    ProcessAvatarBytes(p_PlayerID, p_Callback, p_AvatarResult->BodyBytes().Ptr());
                }
                catch (const std::exception l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][PlayerAvatarPicture.GetScoreSaberAvatarPicture] Error:");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_OnFailCallback();
                }
            }
        );
    }
    /// @brief Get avatar picture from BeatLeader
    /// @param p_PlayerID          ID of the player
    /// @param p_CancellationToken Cancellation token
    /// @param p_Callback          Request callback
    /// @param p_OnFailCallback    On error callback
    void PlayerAvatarPicture::GetBeatLeaderAvatarPicture(std::u16string p_PlayerID, _u::CancellationToken p_CancellationToken, t_CallBack p_Callback, t_CallBackFailed p_OnFailCallback)
    {
        WebClientUnity::GlobalClient()->GetAsync(u"https://api.beatleader.xyz/player/" + p_PlayerID, p_CancellationToken,
            [=](WebResponse::Ptr p_PlayerResult)
            {
                try
                {
                    if (    p_PlayerResult == nullptr   || !p_PlayerResult->IsSuccessStatusCode()
                        || !p_PlayerResult->BodyBytes() || p_PlayerResult->BodyBytes()->get_Length() == 0)
                    {
                        p_OnFailCallback();
                        return;
                    }

                    _v::Json::U16Document l_Document;
                    _v::Json::TryFromU16String(p_PlayerResult->BodyString(), l_Document);
                    if (!l_Document.HasMember(u"avatar") || !l_Document[u"avatar"].IsString())
                    {
                        p_OnFailCallback();
                        return;
                    }

                    WebClientUnity::GlobalClient()->GetAsync(l_Document[u"avatar"].GetString(), p_CancellationToken,
                        [=](WebResponse::Ptr p_AvatarResult)
                        {
                            try
                            {
                                if (    p_AvatarResult == nullptr   || !p_AvatarResult->IsSuccessStatusCode()
                                    || !p_AvatarResult->BodyBytes() || p_AvatarResult->BodyBytes()->get_Length() == 0)
                                {
                                    p_OnFailCallback();
                                    return;
                                }

                                ProcessAvatarBytes(p_PlayerID, p_Callback, p_AvatarResult->BodyBytes().Ptr());
                            }
                            catch (const std::exception& l_Exception)
                            {
                                CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][PlayerAvatarPicture.GetBeatLeaderAvatarPicture_2] Error:");
                                CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                                p_OnFailCallback();
                            }
                        }
                    );
                }
                catch (const std::exception& l_Exception)
                {
                    CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][PlayerAvatarPicture.GetBeatLeaderAvatarPicture] Error:");
                    CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
                    p_OnFailCallback();
                }
            }
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Process received avatar body bytes
    /// @param p_PlayerID  ID of the player
    /// @param p_Callback  Request callback
    /// @param p_BodyBytes Avatar bytes
    void PlayerAvatarPicture::ProcessAvatarBytes(std::u16string p_PlayerID, t_CallBack p_Callback, t_Bytes p_BodyBytes)
    {
        CP_SDK::Unity::MTMainThreadInvoker::Enqueue([=]()
        {
            auto l_Texture = CP_SDK::Unity::Texture2DU::CreateFromRaw(p_BodyBytes);
            if (l_Texture == nullptr)
            {
                p_Callback(nullptr);
                return;
            }

            auto l_Avatar = CP_SDK::Unity::SpriteU::CreateFromTexture(l_Texture, 100.0f, _u::Vector2(0.5f, 0.5f));
            p_Callback(l_Avatar);
        });
    }

}   ///< namespace CP_SDK_BS::Game
