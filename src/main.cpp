#include "git_info.h"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "CP_SDK/ModuleBase.hpp"
#include "CP_SDK/Logging/PaperLogger.hpp"
#include "CP_SDK/UI/FlowCoordinators/MainFlowCoordinator.hpp"
#include "CP_SDK/UI/ScreenSystem.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/FontManager.hpp"
#include "CP_SDK/Utils/Il2cpp.hpp"
#include "CP_SDK_BS/Game/BeatMapsClient.hpp"
#include "CP_SDK_BS/Game/Logic.hpp"
#include "CP_SDK_BS/UI/DefaultFactoriesOverrides/BS_FloatingPanelFactory.hpp"

#include <bsml/shared/BSML.hpp>

#include "main.hpp"

#include <GlobalNamespace/BasicUIAudioManager.hpp>
#include <HMUI/ImageView.hpp>
#include <HMUI/CurvedTextMeshPro.hpp>
#include <HMUI/ScreenSystem.hpp>
#include <HMUI/Screen.hpp>
#include <TMPro/TMP_FontAsset.hpp>
#include <UnityEngine/LayerMask.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/Resources.hpp>
#include <VRUIControls/VRGraphicRaycaster.hpp>

static modloader::ModInfo s_ModInfo{"ChatPlexSDK-BS", VERSION, GIT_COMMIT};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

static CP_SDK::Utils::MonoPtr<TMPro::TMP_FontAsset>                 m_BaseGameFont;
static CP_SDK::Utils::MonoPtr<UnityEngine::Material>                m_BaseGameFontSharedMaterial;
static CP_SDK::Utils::MonoPtr<UnityEngine::Material>                m_UINoGlowMaterial;
static CP_SDK::Utils::MonoPtr<GlobalNamespace::BasicUIAudioManager> m_BasicUIAudioManager;
static CP_SDK::Utils::MonoPtr<VRUIControls::VRGraphicRaycaster>     m_VRGraphicRaycasterCache;

static CP_SDK::Utils::MonoPtr<HMUI::ScreenSystem>                   m_HMUIScreenSystem;
static std::vector<CP_SDK::Utils::MonoPtr<UnityEngine::GameObject>> m_HMUIDeactivatedScreens;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void ScreenSystem_OnPresent()
{
    auto DeactivateScreenSafe = [](HMUI::Screen* p_HMUIScreen) -> void
    {
        if (!CP_SDK::Utils::IsUnityPtrValid(p_HMUIScreen) || !p_HMUIScreen->get_gameObject()->get_activeSelf())
            return;

        m_HMUIDeactivatedScreens.push_back(p_HMUIScreen->get_gameObject().ptr());
        p_HMUIScreen->get_gameObject()->SetActive(false);
    };

    if (!m_HMUIScreenSystem)
    {
        m_HMUIDeactivatedScreens.clear();
        m_HMUIScreenSystem = UnityEngine::Resources::FindObjectsOfTypeAll<HMUI::ScreenSystem*>()->FirstOrDefault();
    }

    if (!m_HMUIScreenSystem)
        return;

    m_HMUIDeactivatedScreens.clear();
    DeactivateScreenSafe(m_HMUIScreenSystem->____leftScreen);
    DeactivateScreenSafe(m_HMUIScreenSystem->____mainScreen);
    DeactivateScreenSafe(m_HMUIScreenSystem->____rightScreen);
    DeactivateScreenSafe(m_HMUIScreenSystem->____bottomScreen);
    DeactivateScreenSafe(m_HMUIScreenSystem->____topScreen);

    CP_SDK::UI::ScreenSystem::Instance()->get_transform()->set_localScale(m_HMUIScreenSystem->get_transform()->get_localScale());
}
void ScreenSystem_OnDismiss()
{
    for (auto l_I = 0; l_I < m_HMUIDeactivatedScreens.size(); ++l_I)
        m_HMUIDeactivatedScreens[l_I]->SetActive(true);

    m_HMUIDeactivatedScreens.clear();
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void PatchUI()
{
    CP_SDK::UI::UISystem::FloatingPanelFactory = std::make_shared<CP_SDK_BS::UI::DefaultFactoriesOverrides::BS_FloatingPanelFactory>();

    CP_SDK::UI::UISystem::UILayer = UnityEngine::LayerMask::NameToLayer("UI");

    CP_SDK::UI::UISystem::Override_UnityComponent_Image            = reinterpret_cast<System::Type*>(csTypeOf(HMUI::ImageView*).convert());
    CP_SDK::UI::UISystem::Override_UnityComponent_TextMeshProUGUI  = reinterpret_cast<System::Type*>(csTypeOf(HMUI::CurvedTextMeshPro*).convert());

    CP_SDK::UI::UISystem::Override_GetUIMaterial = []()
    {
        if (m_UINoGlowMaterial || CP_SDK::ChatPlexSDK::ActiveGenericScene() != CP_SDK::EGenericScene::Menu) return m_UINoGlowMaterial.Ptr();
        m_UINoGlowMaterial = UnityEngine::Material::Instantiate(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>()->FirstOrDefault([](auto x) { return x->get_name() == u"UINoGlow"; }));
        return m_UINoGlowMaterial.Ptr();
    };
    CP_SDK::UI::UISystem::Override_OnClick = [](UnityEngine::MonoBehaviour* p_MonoBehavior)
    {
        if (!m_BasicUIAudioManager || CP_SDK::ChatPlexSDK::ActiveGenericScene() != CP_SDK::EGenericScene::Menu)
            m_BasicUIAudioManager = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::BasicUIAudioManager*>()->FirstOrDefault();
        if (m_BasicUIAudioManager) m_BasicUIAudioManager->HandleButtonClickEvent();
    };

    CP_SDK::UI::UISystem::OnScreenCreated = [](CP_SDK::UI::IScreen* x) -> void
    {
        if (x->GetComponent<VRUIControls::VRGraphicRaycaster*>())
            return;

        if (!m_VRGraphicRaycasterCache)
            m_VRGraphicRaycasterCache = UnityEngine::Resources::FindObjectsOfTypeAll<VRUIControls::VRGraphicRaycaster*>()->FirstOrDefault([](auto y) { return y->_physicsRaycaster != nullptr; });

        if (m_VRGraphicRaycasterCache)
            x->get_gameObject()->AddComponent<VRUIControls::VRGraphicRaycaster*>()->____physicsRaycaster = m_VRGraphicRaycasterCache->____physicsRaycaster;
    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK::UI::ScreenSystem::OnCreated += []() -> void
    {
        auto l_Instance = CP_SDK::UI::ScreenSystem::Instance();
        l_Instance->LeftScreen()->SetTransformDirect(UnityEngine::Vector3(-2.47f, 0.00f, -1.30f), UnityEngine::Vector3(0.0f, -55.0f, 0.0f));
        l_Instance->LeftScreen()->SetRadius(140.0f);

        l_Instance->TopScreen()->SetRadius(140.0f);
        l_Instance->MainScreen()->SetRadius(140.0f);

        l_Instance->RightScreen()->SetTransformDirect(UnityEngine::Vector3(2.47f, 0.00f, -1.30f), UnityEngine::Vector3(0.0f, 55.0f, 0.0f));
        l_Instance->RightScreen()->SetRadius(140.0f);
    };

    CP_SDK::UI::ScreenSystem::OnPresent += &ScreenSystem_OnPresent;
    CP_SDK::UI::ScreenSystem::OnDismiss += &ScreenSystem_OnDismiss;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void OnEnable()
{
    CP_SDK_BS::Game::Logic::_OnUnityActiveSceneChanged -= &OnEnable;

    CP_SDK::ChatPlexSDK::Logger()->Error(u"Patching UI.");
    PatchUI();

    CP_SDK::ChatPlexSDK::Logger()->Error(u"Adding menu button.");

    auto& l_Modules         = CP_SDK::ChatPlexSDK::GetModules();
    auto  l_HasBSPModules   = false;

    for (auto& l_Module : l_Modules)
    {
        if (l_Module->Type() != CP_SDK::EIModuleBaseType::Integrated)
            continue;

        l_HasBSPModules = true;
        break;
    }

    l_HasBSPModules = true;
    if (l_HasBSPModules)
    {
        CP_SDK::UI::FlowCoordinators::MainFlowCoordinator::OverrideTitle(u"QBeatSaberPlus");

        BSML::Register::RegisterMenuButton("QBeatSaber+", s_ModInfo.id,
            [](){
                CP_SDK::UI::FlowCoordinators::MainFlowCoordinator::Instance()->Present(true);
            }
        );
    }

    CP_SDK_BS::Game::BeatMapsClient::Init();
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Called at the early stages of game loading
extern "C" __attribute__((visibility("default"))) void setup(CModInfo* p_ModInfo)
{
    p_ModInfo->id = s_ModInfo.id.c_str();
    p_ModInfo->version = s_ModInfo.version.c_str();
    p_ModInfo->version_long = s_ModInfo.versionLong;

    auto l_Logger = new CP_SDK::Logging::PaperLogger(p_ModInfo->id);

    CP_SDK::ChatPlexSDK::Configure(
        l_Logger,
        u"BeatSaber",
        "/sdcard/ModData/com.beatgames.beatsaber/Mods/",
        CP_SDK::ERenderPipeline::BuiltIn
    );
    CP_SDK::ChatPlexSDK::OnAssemblyLoaded();

    CP_SDK::Unity::FontManager::Setup([](TMPro::TMP_FontAsset* p_Input) -> TMPro::TMP_FontAsset* {
        auto l_MainFont = UnityEngine::Resources::FindObjectsOfTypeAll<TMPro::TMP_FontAsset*>()->FirstOrDefault([](auto x) { return x->get_name() == u"Teko-Medium SDF"; });
        if (l_MainFont && p_Input)
        {
            p_Input->___material->set_shader(l_MainFont->___material->get_shader());
            p_Input->___material->SetColor("_FaceColor", p_Input->___material->GetColor(u"_FaceColor"));
            p_Input->___material->EnableKeyword("CURVED");
            p_Input->___material->EnableKeyword("UNITY_UI_CLIP_RECT");
        }

        return p_Input;
    });

    CP_SDK::ChatPlexSDK::Logger()->Info(u"ChatPlexSDK-BS Completed setup!");
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

static bool s_IsLoaded = false;

// Called later on in the game loading - a good time to install function hooks
extern "C" __attribute__((visibility("default"))) void late_load()
{
    if (s_IsLoaded)
        return;

    s_IsLoaded = true;

    il2cpp_functions::Init();

    CP_SDK::ChatPlexSDK::Logger()->Error(u"Applying Harmony patches.");
    CP_SDK::Utils::Hooks::InstallHooks();

    CP_SDK::ChatPlexSDK::Logger()->Error(u"Registering custom types.");
    custom_types::Register::AutoRegister();

    CP_SDK_BS::Game::Logic::_OnUnityActiveSceneChanged += &OnEnable;
}