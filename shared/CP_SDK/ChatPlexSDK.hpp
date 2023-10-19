#pragma once

#include "Logging/ILogger.hpp"
#include "Utils/Event.hpp"
#include "Utils/Il2cpp.hpp"

#include <vector>

namespace CP_SDK {

    class IModuleBase;

    enum class ERenderPipeline
    {
        BuiltIn,
        URP
    };

    enum class EGenericScene
    {
        None,
        Menu,
        Playing
    };

    /// ChatPlex SDK main class
    class ChatPlexSDK
    {
        DISABLE_CONSTRUCTORS(ChatPlexSDK);

        public:
            /// Logger instance
            static Logging::ILogger*    Logger()                { return m_Logger;              }

            static std::u16string_view  ProductName()           { return m_ProductName;         }
            static std::u16string_view  ProductVersion()        { return m_ProductVersion;      }
            static std::string_view     BasePath()              { return m_BasePath;            }
            static std::u16string_view  NetworkUserAgent()      { return m_NetworkUserAgent;    }
            static ERenderPipeline      RenderPipeline()        { return m_RenderPipeline;      }
            static EGenericScene        ActiveGenericScene()    { return m_ActiveGenericScene;  }

            static Utils::Event<EGenericScene>    OnGenericSceneChange;
            static Utils::Event<bool>             OnGenericMenuSceneLoaded;

        public:
            /// @brief Configure
            /// @param p_Logger         Logger instance
            /// @param p_ProductName    Product name
            /// @param p_BasePath       Base path for file storage
            /// @param p_RenderPipeline Rendering pipeline
            static void Configure(Logging::ILogger* p_Logger, std::u16string_view p_ProductName, std::string_view p_BasePath, ERenderPipeline p_RenderPipeline);
            /// When the assembly is loaded
            static void OnAssemblyLoaded();
            /// On assembly exit
            static void OnAssemblyExit();

        public:
            /// When unity is ready
            static void OnUnityReady();
            /// When unity is exiting
            static void OnUnityExit();

        public:
            /// Register a module
            /// @p_Module: Module instance
            static void RegisterModule(IModuleBase* p_Module);
            /// Init all the available modules
            static void InitModules();
            /// Stop modules
            static void StopModules();
            /// Get modules
            static const std::vector<IModuleBase*> & GetModules() { return m_Modules; }

        public:
            /// On generic menu scene
            static void Fire_OnGenericMenuSceneLoaded();
            /// On generic menu scene
            static void Fire_OnGenericMenuScene();
            /// On generic play scene
            static void Fire_OnGenericPlayingScene();

        private:
            /// Install WEBP codecs
            static void InstallWEBPCodecs();

        private:
            static Logging::ILogger*    m_Logger;

            static std::u16string   m_ProductName;
            static std::u16string   m_ProductVersion;
            static std::string      m_BasePath;
            static std::u16string   m_NetworkUserAgent;
            static ERenderPipeline  m_RenderPipeline;
            static EGenericScene    m_ActiveGenericScene;

            static std::vector<IModuleBase*> m_Modules;

    };

}   ///< namespace CP_SDK