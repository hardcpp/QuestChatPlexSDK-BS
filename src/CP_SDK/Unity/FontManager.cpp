#include "CP_SDK/Unity/FontManager.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "assets.hpp"

#include <UnityEngine/Application.hpp>
#include <TMPro/TMP_TextUtilities.hpp>
#include <beatsaber-hook/shared/utils/il2cpp-utils.hpp>

using namespace TMPro;
using namespace UnityEngine;

/// Temp fix from BSML until libunity is properly unstripped
    #include <UnityEngine/UI/Button.hpp>
    #include <TMPro/TextMeshProUGUI.hpp>
    #include <UnityEngine/Resources.hpp>
    #include <GlobalNamespace/MainMenuViewController.hpp>
    #include <UnityEngine/Transform.hpp>

    bool TryGetSoloButton(UnityEngine::UI::Button*& p_Button)
    {
        static SafePtrUnity<UnityEngine::UI::Button> g_SoloButton;
        if(!g_SoloButton)
        {
            auto l_MainMenuViewController = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MainMenuViewController*>()->First();
            g_SoloButton = l_MainMenuViewController->____soloButton;
        }

        p_Button = g_SoloButton.ptr();
        return g_SoloButton;
    }
    bool TryGetUITextTemplate(TMPro::TextMeshProUGUI*& p_TextMeshProUGUI)
    {
        UnityEngine::UI::Button* l_SoloButton;
        if (!TryGetSoloButton(l_SoloButton))
        {
            p_TextMeshProUGUI = nullptr;
            return false;
        }
        auto l_Transform    = l_SoloButton->get_transform();
        auto l_Text         = l_Transform->Find("Text");

        p_TextMeshProUGUI = l_Text->GetComponent<TMPro::TextMeshProUGUI*>();
        return p_TextMeshProUGUI;
    }
/// End of temp fix

namespace CP_SDK::Unity {

    bool                                                FontManager::m_IsInitialized        = false;
    _v::MonoPtr<AssetBundle>                            FontManager::m_AssetBundle          = nullptr;
    _v::MonoPtr<TMP_FontAsset>                          FontManager::m_BundleMainFont       = nullptr;
    _v::MonoPtr<TMP_FontAsset>                          FontManager::m_BundleChatFont       = nullptr;
    _v::MonoPtr<TMP_FontAsset>                          FontManager::m_MainFont             = nullptr;
    _v::MonoPtr<TMP_FontAsset>                          FontManager::m_ChatFont             = nullptr;
    _v::Func<_u::TMP_FontAsset*, _u::TMP_FontAsset*>    FontManager::m_TMPFontAssetSetup    = nullptr;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Setup for specific app
    /// @param p_TMPFontAssetSetup Font setup function
    void FontManager::Setup(_v::CFuncRef<_u::TMP_FontAsset*, _u::TMP_FontAsset*> p_TMPFontAssetSetup)
    {
        m_TMPFontAssetSetup = p_TMPFontAssetSetup;
    }
    /// @brief Init the font manager
    void FontManager::Init()
    {
        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][FontManager.Init] Loading font asset bundle...");

        /// Temp disable until libunity is properly unstripped
        /*
        static auto s_LoadFromMemory = reinterpret_cast<t_AssetBundle_LoadFromMemory>(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemory_Internal"));

        m_AssetBundle = s_LoadFromMemory(Assets::QuestFonts_bundle, 0);
        if (!m_AssetBundle)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][FontManager.Init] Failed to load the font asset bundle");
            return;
        }

        Object::DontDestroyOnLoad(m_AssetBundle.Ptr());
        */
        ChatPlexSDK::Logger()->Info(u"[CP_SDK.Unity][FontManager.Init] Font asset bundle loaded");

        m_IsInitialized = true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get main font
    TMP_FontAsset* FontManager::GetMainFont()
    {
        if (!IsInitialized())
            throw std::runtime_error("[CP_SDK.Unity][FontManager.GetChatFont] Font manager is not initialized!");

        if (!m_MainFont)
        {
            /// Temp fix from BSML until libunity is properly unstripped
            TMPro::TextMeshProUGUI* p_TextMeshProUGUI;
            if (TryGetUITextTemplate(p_TextMeshProUGUI))
                m_MainFont = p_TextMeshProUGUI->get_font();
            /// End of temp fix

            /// Temp disable until libunity is properly unstripped
            /*
            if (!m_BundleMainFont)
                m_BundleMainFont = m_AssetBundle->LoadAsset<TMP_FontAsset*>("[CP_SDK]segoeui SDF Main");

            m_MainFont = m_BundleMainFont;
            */

            if (m_TMPFontAssetSetup.IsValid())
                m_MainFont = m_TMPFontAssetSetup(m_MainFont.Ptr());

            /// Temp disable until libunity is properly unstripped
            /*
            m_MainFont->___normalStyle          =  0.5f;
            m_MainFont->___normalSpacingOffset  = -1.0f;
            m_MainFont->___boldStyle            =  2.0f;
            m_MainFont->___boldSpacing          =  2.0f;
            m_MainFont->___italicStyle          = 15;
            */
        }

        return m_MainFont.Ptr();
    }
    /// @brief Get chat font
    TMP_FontAsset* FontManager::GetChatFont()
    {
        if (!IsInitialized())
            throw std::runtime_error("[CP_SDK.Unity][FontManager.GetChatFont] Font manager is not initialized!");

        if (!m_ChatFont)
        {
            if (!m_BundleChatFont)
                m_BundleChatFont = m_AssetBundle->LoadAsset<TMP_FontAsset*>("[CP_SDK]segoeui SDF");

            m_ChatFont = m_BundleChatFont.Ptr();

            if (m_TMPFontAssetSetup.IsValid())
                m_ChatFont = m_TMPFontAssetSetup(m_ChatFont.Ptr());
        }

        return m_ChatFont.Ptr();
    }

}   ///< namespace CP_SDK::Unity