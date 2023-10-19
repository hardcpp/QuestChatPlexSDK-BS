#include "CP_SDK_BS/UI/GameFont.hpp"

#include <UnityEngine/Resources.hpp>

using namespace UnityEngine;
using namespace TMPro;

namespace CP_SDK_BS::UI {

    CP_SDK::Utils::MonoPtr<TMP_FontAsset>   GameFont::m_BaseGameFont;
    CP_SDK::Utils::MonoPtr<Material>        GameFont::m_BaseGameFontSharedMaterial;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get main game font
    TMP_FontAsset* GameFont::GetGameFont()
    {
        if (m_BaseGameFont || CP_SDK::ChatPlexSDK::ActiveGenericScene() != CP_SDK::EGenericScene::Menu)
            return m_BaseGameFont.Ptr(false);

        m_BaseGameFont = Resources::FindObjectsOfTypeAll<TMP_FontAsset*>().FirstOrDefault([](TMP_FontAsset* t) { return t->get_name() == "Teko-Medium SDF"; });
        return m_BaseGameFont.Ptr(false);
    }
    /// @brief Get main game font curved material
    Material* GameFont::GetGameFontSharedMaterial()
    {
        if (m_BaseGameFontSharedMaterial || CP_SDK::ChatPlexSDK::ActiveGenericScene() != CP_SDK::EGenericScene::Menu)
            return m_BaseGameFontSharedMaterial.Ptr(false);

        m_BaseGameFontSharedMaterial = Material::Instantiate(Resources::FindObjectsOfTypeAll<Material*>().Last([](Material* t) { return t->get_name() == "Teko-Medium SDF Curved Softer"; }));
        return m_BaseGameFontSharedMaterial.Ptr(false);
    }

}   ///< namespace CP_SDK_BS::UI