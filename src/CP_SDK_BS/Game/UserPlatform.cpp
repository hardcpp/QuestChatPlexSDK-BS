#include "CP_SDK_BS/Game/UserPlatform.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <System/Threading/Tasks/Task.hpp>
#include <System/Threading/Tasks/Task_1.hpp>
#include <UnityEngine/Resources.hpp>
#include <GlobalNamespace/IPlatformUserModel.hpp>
#include <GlobalNamespace/PlatformLeaderboardsModel.hpp>
#include <GlobalNamespace/UserInfo.hpp>

using namespace GlobalNamespace;
using namespace UnityEngine;

namespace CP_SDK_BS::Game {

    std::u16string UserPlatform::m_UserID;
    std::u16string UserPlatform::m_UserName;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Get User ID
    const std::u16string & UserPlatform::GetUserID()
    {
        if (!m_UserID.empty())
            return m_UserID;

        FetchPlatformInfos();

        return m_UserID;
    }
    /// Get User Name
    const std::u16string & UserPlatform::GetUserName()
    {
        if (!m_UserName.empty())
            return m_UserName;

        FetchPlatformInfos();

        return m_UserName;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Find platform informations
    void UserPlatform::FetchPlatformInfos()
    {
        try
        {
            auto l_PlatformLeaderboardsModels = Resources::FindObjectsOfTypeAll<PlatformLeaderboardsModel*>();

            for (GlobalNamespace::PlatformLeaderboardsModel * l_Current : l_PlatformLeaderboardsModels)
            {
                if (l_Current->platformUserModel == nullptr)
                    continue;

                auto l_Task = l_Current->platformUserModel->GetUserInfo();
                l_Task->Wait();

                auto l_PlayerID = l_Task->get_Result()->platformUserId;
                if (!System::String::IsNullOrEmpty(l_PlayerID))
                {
                    m_UserID    = l_PlayerID;
                    m_UserName  = l_Task->get_Result()->userName;
                    return;
                }
            }
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][UserPlatform] Unable to find user platform informations");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK_BS::Game