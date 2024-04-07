#include "CP_SDK/ChatPlexSDK.hpp"
#include "CP_SDK/Animation/AnimationControllerManager.hpp"
#include "CP_SDK/Chat/Service.hpp"
#include "CP_SDK/UI/LoadingProgressBar.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/FontManager.hpp"
#include "CP_SDK/Unity/MonoPtrHolder.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ModuleBase.hpp"

namespace CP_SDK {

    Logging::ILogger*               ChatPlexSDK::m_Logger               = nullptr;

    std::u16string                  ChatPlexSDK::m_ProductName          = u"";
    std::u16string                  ChatPlexSDK::m_ProductVersion       = u"";
    std::string                     ChatPlexSDK::m_BasePath             = "";
    std::u16string                  ChatPlexSDK::m_NetworkUserAgent     = u"";
    ERenderPipeline                 ChatPlexSDK::m_RenderPipeline       = ERenderPipeline::BuiltIn;
    EGenericScene                   ChatPlexSDK::m_ActiveGenericScene   = EGenericScene::None;

    std::vector<IModuleBase*>       ChatPlexSDK::m_Modules;

    Utils::Event<EGenericScene>     ChatPlexSDK::OnGenericSceneChange;
    Utils::Event<bool>              ChatPlexSDK::OnGenericMenuSceneLoaded;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Configure
    /// @param p_Logger         Logger instance
    /// @param p_ProductName    Product name
    /// @param p_BasePath       Base path for file storage
    /// @param p_RenderPipeline Rendering pipeline
    void ChatPlexSDK::Configure(Logging::ILogger* p_Logger, std::u16string_view p_ProductName, std::string_view p_BasePath, ERenderPipeline p_RenderPipeline)
    {
        m_Logger            = p_Logger;

        m_ProductName       = p_ProductName;
        m_ProductVersion    = StringW(VERSION);
        m_BasePath          = p_BasePath;
        m_NetworkUserAgent  = std::u16string(u"ChatPlexSDK_") + m_ProductName;
        m_RenderPipeline    = p_RenderPipeline;
    }
    /// When the assembly is loaded
    void ChatPlexSDK::OnAssemblyLoaded()
    {
        InstallWEBPCodecs();

        /// Init config
        Chat::Service::Init();
    }
    /// On assembly exit
    void ChatPlexSDK::OnAssemblyExit()
    {
        try
        {
            Chat::Service::Release(true);
        }
        catch (const std::exception& l_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexSDK.OnAssemblyExit] Error:");
            m_Logger->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// When unity is ready
    void ChatPlexSDK::OnUnityReady()
    {
        try
        {
            Unity::MonoPtrHolder::TouchInstance();

            Unity::MTCoroutineStarter::Initialize();
            Unity::MTMainThreadInvoker::Initialize();
            Unity::MTThreadInvoker::Initialize();

            /// Init fonts
            Unity::FontManager::Init();

            /// Init UI
            UI::UISystem::Init();
        }
        catch (const std::exception& p_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexSDK.OnUnityReady] Error:");
            m_Logger->Error(p_Exception);
        }
    }
    /// When unity is exiting
    void ChatPlexSDK::OnUnityExit()
    {
        try
        {
            OnGenericSceneChange.Clear();
            OnGenericMenuSceneLoaded.Clear();

            UI::UISystem::Destroy();
            UI::LoadingProgressBar::Destroy();

            //Unity.EnhancedImageParticleMaterialProvider.Destroy();
            //Unity.EnhancedImageParticleSystemProvider.Destroy();

            Unity::MTThreadInvoker::Destroy();
            Unity::MTMainThreadInvoker::Destroy();
            Unity::MTCoroutineStarter::Destroy();

            Animation::AnimationControllerManager::Destroy();
        }
        catch (const std::exception& p_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexSDK.OnUnityExit] Error:");
            m_Logger->Error(p_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Register a module
    /// @p_Module: Module instance
    void ChatPlexSDK::RegisterModule(IModuleBase * p_Module)
    {
        m_Modules.push_back(p_Module);
    }
    /// Init all the available modules
    void ChatPlexSDK::InitModules()
    {
        try
        {
            m_Logger->Debug(u"[CP_SDK][ChatPlexUnitySDK.InitModules] Init modules.");

            for (auto l_Module : m_Modules)
            {
                m_Logger->Debug(u"[CP_SDK][ChatPlexUnitySDK.InitModules] - " + l_Module->Name());

                try                                            { l_Module->CheckForActivation(EIModuleBaseActivationType::OnStart);                                                                     }
                catch (const std::exception& p_InitException)  { m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.InitModules] Error on module init " + l_Module->Name()); m_Logger->Error(p_InitException); }
            }

            std::sort(m_Modules.begin(), m_Modules.end(), [](const IModuleBase * p_Left, const IModuleBase * p_Right) {
                return p_Left->Name() < p_Right->Name();
            });
        }
        catch (const std::exception& p_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.InitModules] Error:");
            m_Logger->Error(p_Exception);
        }
    }
    /// Stop modules
    void ChatPlexSDK::StopModules()
    {
        for (auto l_Module : m_Modules)
        {
            try
            {
                l_Module->OnApplicationExit();
            }
            catch (const std::exception& p_InitException)
            {
                m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.StopModules] Error:");
                m_Logger->Error(p_InitException);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// On generic menu scene
    void ChatPlexSDK::Fire_OnGenericMenuSceneLoaded()
    {
        try
        {
            //UI::LoadingProgressBar::TouchInstance();
        }
        catch (const std::exception& l_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.Fire_OnGenericMenuSceneLoaded] Error:");
            m_Logger->Error(l_Exception);
        }

        m_ActiveGenericScene = EGenericScene::Menu;

        for (auto l_Module : m_Modules)
        {
            try                                            { l_Module->CheckForActivation(EIModuleBaseActivationType::OnMenuSceneLoaded);                                                                             }
            catch (const std::exception& p_InitException)  { m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.Fire_OnGenericMenuSceneLoaded] Error on module init " + l_Module->Name()); m_Logger->Error(p_InitException); }
        }

        try
        {
            OnGenericMenuSceneLoaded.Invoke(true);
        }
        catch (const std::exception& l_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.Fire_OnGenericMenuSceneLoaded] Error:");
            m_Logger->Error(l_Exception);
        }
    }
    /// On generic menu scene
    void ChatPlexSDK::Fire_OnGenericMenuScene()
    {
        m_ActiveGenericScene = EGenericScene::Menu;

        try
        {
            OnGenericSceneChange.Invoke(EGenericScene::Menu);

            Chat::Service::StartServices();
        }
        catch (const std::exception& l_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.Fire_OnGenericMenuScene] Error:");
            m_Logger->Error(l_Exception);
        }
    }
    /// On generic play scene
    void ChatPlexSDK::Fire_OnGenericPlayingScene()
    {
        m_ActiveGenericScene = EGenericScene::Playing;

        try
        {
            OnGenericSceneChange.Invoke(EGenericScene::Playing);
        }
        catch (const std::exception& l_Exception)
        {
            m_Logger->Error(u"[CP_SDK][ChatPlexUnitySDK.OnGenericPlayingScene] Error:");
            m_Logger->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Install WEBP codecs
    void ChatPlexSDK::InstallWEBPCodecs()
    {
        /// Codecs are built-in sources
    }

}   ///< namespace CP_SDK