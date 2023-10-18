#include "CP_SDK/CPConfig.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

namespace CP_SDK {

    CP_SDK_CONFIG_JSONCONFIG_INSTANCE_IMPL(CPConfig);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Reset config to default
    void CPConfig::Reset_Impl()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Write the document
    /// @param p_Document Target
    CP_SDK_JSON_SERIALIZE_IMPL(CPConfig)
    {
        CP_SDK_JSON_SERIALIZE_BOOL(FirstRun);
        CP_SDK_JSON_SERIALIZE_BOOL(FirstChatServiceRun);
    }
    /// @brief Read the document
    /// @param p_Document Source
    CP_SDK_JSON_UNSERIALIZE_IMPL(CPConfig)
    {
        CP_SDK_JSON_UNSERIALIZE_BOOL(FirstRun);
        CP_SDK_JSON_UNSERIALIZE_BOOL(FirstChatServiceRun);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get relative config path
    std::filesystem::path CPConfig::GetRelativePath()
    {
        return std::filesystem::path("ChatPlexSDK");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On config init
    /// @param p_OnCreation On creation
    void CPConfig::OnInit(bool p_OnCreation)
    {

    }

}   ///< namespace CP_SDK