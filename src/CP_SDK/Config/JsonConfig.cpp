#include "CP_SDK/Config/JsonConfig.hpp"
#include "CP_SDK/Misc/Time.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <fstream>

namespace CP_SDK::Config {

    /// @brief Constructor
    JsonConfig::JsonConfig()
    {

    }
    /// @brief Destructor
    JsonConfig::~JsonConfig()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Dummy method for warmup
    void JsonConfig::Warmup()
    {
        ;
    }
    /// @brief Reset config to default
    void JsonConfig::Reset()
    {
        try
        {
            Reset_Impl();
        }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Config][JsonConfig.Reset] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief Save config file
    void JsonConfig::Save()
    {
        WriteFile(GetFullPath());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get full config path
    std::filesystem::path JsonConfig::GetFullPath()
    {
        auto l_Path = ChatPlexSDK::BasePath() / GetRelativePath();
        l_Path += ".json";

        return l_Path;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init config
    void JsonConfig::Init()
    {
        auto l_FullPath = GetFullPath();
        try
        {
            auto l_Path = l_FullPath.parent_path();
            if (!std::filesystem::exists(l_Path) && !std::filesystem::create_directories(l_Path))
                throw std::runtime_error("Failed to create directory for path " + l_Path.string());
        }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Config][JsonConfig.Init] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }

        bool l_FileExist = false;
        std::ifstream l_Stream;
        try
        {
            if (std::filesystem::exists(l_FullPath))
            {
                l_FileExist = true;

                l_Stream.open(l_FullPath);

                Utils::Json::U16Document l_Document;
                if (!Utils::Json::TryFromU8Stream(l_Stream, l_Document))
                {
                    l_Stream.close();
                    throw std::runtime_error("Unable to parse document");
                }

                Unserialize(l_Document);
                l_Stream.close();

                OnInit(false);
            }
            else
            {
                OnInit(true);
            }

            Save();
        }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Config][JsonConfig.Init] Failed to read config " + StringW(GetFullPath().string()));
            ChatPlexSDK::Logger()->Error(l_Exception);

            try
            {
                if (l_FileExist)
                {
                    auto l_NewPath = l_FullPath.parent_path() / l_FullPath.stem();
                    l_NewPath += ".broken_" + std::to_string(Misc::Time::UnixTimeNow()) + ".json";

                    std::filesystem::rename(l_FullPath, l_NewPath);

                    OnInit(true);
                    Save();
                }
                else
                {
                    OnInit(true);
                    Save();
                }
            }
            catch (const std::exception&) { }
        }

        if (l_Stream.is_open())
            l_Stream.close();
    }
    /// @brief Write file
    /// @param p_FullPath File path
    void JsonConfig::WriteFile(std::filesystem::path p_FullPath)
    {
        Unity::MTThreadInvoker::EnqueueOnThread([this, p_FullPath]() -> void
        {
            try
            {
                auto l_Path = p_FullPath.parent_path();
                if (!std::filesystem::exists(l_Path) && !std::filesystem::create_directories(l_Path))
                    throw std::runtime_error("Failed to create directory for path " + l_Path.string());
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Config][JsonConfig.WriteFile] Error for file " + StringW(p_FullPath.string()));
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            std::ofstream l_Stream;
            try
            {
                l_Stream.open(p_FullPath, std::ios::trunc);

                Utils::Json::U16Document l_Document;
                l_Document.SetObject();
                Serialize(l_Document, l_Document.GetAllocator());

                if (!Utils::Json::TryToU8Stream(l_Document, true, l_Stream))
                {
                    l_Stream.close();
                    throw std::runtime_error("Failed to write json");
                }

                l_Stream.close();
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Config][JsonConfig.WriteFile] Error for file " + StringW(p_FullPath.string()));
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            if (l_Stream.is_open())
                l_Stream.close();
        });
    }

}   ///< namespace CP_SDK::Config

