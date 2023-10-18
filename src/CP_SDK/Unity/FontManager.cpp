#include "CP_SDK/Unity/FontManager.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "assets.hpp"

#include <UnityEngine/Application.hpp>
#include <TMPro/TMP_TextUtilities.hpp>
#include <beatsaber-hook/shared/utils/il2cpp-utils.hpp>

using namespace TMPro;
using namespace UnityEngine;

namespace CP_SDK::Unity {

    bool                                                FontManager::m_IsInitialized        = false;
    _v::MonoPtr<AssetBundle>                            FontManager::m_AssetBundle          = nullptr;
    _v::MonoPtr<TMP_FontAsset>                          FontManager::m_BundleFont           = nullptr;
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
        static auto s_LoadFromMemory = reinterpret_cast<t_AssetBundle_LoadFromMemory>(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemory_Internal"));

        m_AssetBundle = s_LoadFromMemory(IncludedAssets::QuestFonts_bundle.Raw(), 0);
        if (!m_AssetBundle)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][FontManager.Init] Failed to load the font asset bundle");
            return;
        }

        Object::DontDestroyOnLoad(m_AssetBundle.Ptr());
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
            if (!m_BundleFont)
                m_BundleFont = m_AssetBundle->LoadAsset<TMP_FontAsset*>("[CP_SDK]segoeui SDF");

            m_MainFont = TMP_FontAsset::CreateFontAsset(m_BundleFont.Ptr()->get_sourceFontFile());
            m_MainFont->set_name(m_BundleFont->get_name() + " CloneMain");
            m_MainFont->hashCode = TMP_TextUtilities::GetSimpleHashCode(m_MainFont->get_name());
            m_MainFont->set_fallbackFontAssetTable(m_BundleFont->get_fallbackFontAssetTable());

            if (m_TMPFontAssetSetup.IsValid())
                m_MainFont = m_TMPFontAssetSetup(m_MainFont.Ptr());

            m_MainFont->normalStyle          =  0.5f;
            m_MainFont->normalSpacingOffset  = -1.0f;
            m_MainFont->boldStyle            =  2.0f;
            m_MainFont->boldSpacing          =  2.0f;
            m_MainFont->italicStyle          = 15;

            m_ChatFont = m_BundleFont.Ptr();
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
            if (!m_BundleFont)
                m_BundleFont = m_AssetBundle->LoadAsset<TMP_FontAsset*>("[CP_SDK]segoeui SDF");

            m_ChatFont = m_BundleFont.Ptr();

            if (m_TMPFontAssetSetup.IsValid())
                m_ChatFont = m_TMPFontAssetSetup(m_ChatFont.Ptr());
        }

        return m_ChatFont.Ptr();
    }

}   ///< namespace CP_SDK::Unity