#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <UnityEngine/Material.hpp>
#include <TMPro/TMP_FontAsset.hpp>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace TMPro;
    }

    /// @brief Helpers for game font
    class CP_SDK_EXPORT GameFont
    {
        CP_SDK_NO_DEF_CTORS(GameFont);

        private:
            static CP_SDK::Utils::MonoPtr<_u::TMP_FontAsset>    m_BaseGameFont;
            static CP_SDK::Utils::MonoPtr<_u::Material>         m_BaseGameFontSharedMaterial;

        public:
            /// @brief Get main game font
            static _u::TMP_FontAsset* GetGameFont();
            /// @brief Get main game font curved material
            static _u::Material* GetGameFontSharedMaterial();

    };

}   ///< namespace CP_SDK_BS::UI