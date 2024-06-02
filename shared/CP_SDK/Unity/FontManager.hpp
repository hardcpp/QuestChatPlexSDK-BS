#pragma once

#include "../Utils/Delegate.hpp"
#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <TMPro/TMP_FontAsset.hpp>
#include <UnityEngine/AssetBundle.hpp>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace TMPro;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Open type to TextMeshPro font manager
    class CP_SDK_EXPORT_VISIBILITY FontManager
    {
        CP_SDK_NO_DEF_CTORS(FontManager);

        using t_AssetBundle_LoadFromMemory = function_ptr_t<_u::AssetBundle*, ::Array<uint8_t>*, unsigned int>;

        private:
            static bool                                             m_IsInitialized;
            static _v::MonoPtr<_u::AssetBundle>                     m_AssetBundle;
            static _v::MonoPtr<_u::TMP_FontAsset>                   m_BundleMainFont;
            static _v::MonoPtr<_u::TMP_FontAsset>                   m_BundleChatFont;
            static _v::MonoPtr<_u::TMP_FontAsset>                   m_MainFont;
            static _v::MonoPtr<_u::TMP_FontAsset>                   m_ChatFont;
            static _v::Func<_u::TMP_FontAsset*, _u::TMP_FontAsset*> m_TMPFontAssetSetup;

        public:
            static bool                 IsInitialized() { return m_IsInitialized;   }
            static std::u16string_view  MainFontName()  { return u"Segoe UI";       }
            static std::u16string_view  ChatFontName()  { return u"Segoe UI";       }

        public:
            /// @brief Setup for specific app
            /// @param p_TMPFontAssetSetup Font setup function
            static void Setup(_v::CFuncRef<_u::TMP_FontAsset*, _u::TMP_FontAsset*> p_TMPFontAssetSetup);
            /// @brief Init the font manager
            static void Init();

        public:
            /// @brief Get main font
            static _u::TMP_FontAsset* GetMainFont();
            /// @brief Get chat font
            static _u::TMP_FontAsset* GetChatFont();

    };

}   ///< namespace CP_SDK::Unity