#pragma once

#include "../Utils/MonoPtr.hpp"

#include <UnityEngine/Sprite.hpp>
#include <UnityEngine/SpriteMeshType.hpp>
#include <UnityEngine/Texture2D.hpp>
#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector4.hpp>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Sprite helper
    class SpriteU
    {
        CP_SDK_NO_DEF_CTORS(SpriteU);

        public:
            /// @brief Create sprite from texture
            /// @param p_Texture       Source texture
            /// @param p_PixelsPerUnit Pixels per unit multiplier
            /// @param p_Pivot         Pivot point
            /// @param p_Extrude       Extrude amount
            /// @param p_Type          Sprite mesh type
            /// @param p_Borders       Borders
            static _u::Sprite* CreateFromTexture(   _u::Texture2D*      p_Texture,
                                                    float               p_PixelsPerUnit = 100.0f,
                                                    _u::Vector2         p_Pivot         = _u::Vector2(0.0f, 0.0f),
                                                    uint32_t            p_Extrude       = 0,
                                                    _u::SpriteMeshType  p_Type          = _u::SpriteMeshType::FullRect,
                                                    _u::Vector4         p_Borders       = _u::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
            /// @brief Create sprite from raw
            /// @param p_Bytes         Source texture
            /// @param p_PixelsPerUnit Pixels per unit multiplier
            /// @param p_Pivot         Pivot point
            /// @param p_Extrude       Extrude amount
            /// @param p_Type          Sprite mesh type
            /// @param p_Borders       Borders
            static _u::Sprite* CreateFromRaw(   ::Array<uint8_t>*   p_Bytes,
                                                float               p_PixelsPerUnit = 100.0f,
                                                _u::Vector2         p_Pivot         = _u::Vector2(0.0f, 0.0f),
                                                uint32_t            p_Extrude       = 0,
                                                _u::SpriteMeshType  p_Type          = _u::SpriteMeshType::FullRect,
                                                _u::Vector4         p_Borders       = _u::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
            /// @brief Create sprite from raw threaded
            /// @param p_Bytes         Source texture
            /// @param p_Callback      On result callback
            /// @param p_PixelsPerUnit Pixels per unit multiplier
            /// @param p_Pivot         Pivot point
            /// @param p_Extrude       Extrude amount
            /// @param p_Type          Sprite mesh type
            /// @param p_Borders       Borders
            static void CreateFromRawThreaded(  _v::MonoPtr<::Array<uint8_t>>   p_Bytes,
                                                _v::Action<_u::Sprite*>         p_Callback,
                                                float                           p_PixelsPerUnit = 100.0f,
                                                _u::Vector2                     p_Pivot         = _u::Vector2(0.0f, 0.0f),
                                                uint32_t                        p_Extrude       = 0,
                                                _u::SpriteMeshType              p_Type          = _u::SpriteMeshType::FullRect,
                                                _u::Vector4                     p_Borders       = _u::Vector4(0.0f, 0.0f, 0.0f, 0.0f));

    };

}   ///< namespace CP_SDK::Unity