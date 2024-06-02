#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"

#include <string>

namespace CP_SDK_BS::Game {

    /// UserPlatform helper
    class CP_SDK_EXPORT_VISIBILITY UserPlatform
    {
        CP_SDK_NO_DEF_CTORS(UserPlatform);

        private:
            /// User ID cache
            static std::u16string m_UserID;
            /// User name cache
            static std::u16string m_UserName;

        public:
            /// Get User ID
            static const std::u16string & GetUserID();
            /// Get User Name
            static const std::u16string & GetUserName();

        private:
            /// Find platform informations
            static void FetchPlatformInfos();

    };

}   ///< namespace CP_SDK_BS::Game