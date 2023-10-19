#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/Unity/Texture2DU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/Rect.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

using namespace UnityEngine;

namespace CP_SDK::Unity {

    /// @brief Create sprite from texture
    /// @param p_Texture       Source texture
    /// @param p_PixelsPerUnit Pixels per unit multiplier
    /// @param p_Pivot         Pivot point
    /// @param p_Extrude       Extrude amount
    /// @param p_Type          Sprite mesh type
    /// @param p_Borders       Borders
    Sprite* SpriteU::CreateFromTexture( Texture2D*      p_Texture,
                                        float           p_PixelsPerUnit,
                                        Vector2         p_Pivot,
                                        uint            p_Extrude,
                                        SpriteMeshType  p_Type,
                                        Vector4         p_Borders)
    {
        if (p_Texture)
        {
            auto l_Sprite = Sprite::Create(p_Texture, Rect(0, 0, p_Texture->get_width(), p_Texture->get_height()), p_Pivot, p_PixelsPerUnit, p_Extrude, p_Type, p_Borders, false);
            l_Sprite->get_texture()->set_wrapMode(TextureWrapMode::Clamp);

            return l_Sprite;
        }

        return nullptr;
    }
    /// @brief Create sprite from texture
    /// @param p_Bytes         Source texture
    /// @param p_PixelsPerUnit Pixels per unit multiplier
    /// @param p_Pivot         Pivot point
    /// @param p_Extrude       Extrude amount
    /// @param p_Type          Sprite mesh type
    /// @param p_Borders       Borders
    Sprite* SpriteU::CreateFromRaw( ::Array<uint8_t>*   p_Bytes,
                                    float               p_PixelsPerUnit,
                                    Vector2             p_Pivot,
                                    uint                p_Extrude,
                                    SpriteMeshType      p_Type,
                                    Vector4             p_Borders)
    {
        return CreateFromTexture(Texture2DU::CreateFromRaw(p_Bytes), p_PixelsPerUnit, p_Pivot, p_Extrude, p_Type, p_Borders);
    }
    /// @brief Create sprite from raw threaded
    /// @param p_Bytes         Source texture
    /// @param p_Callback      On result callback
    /// @param p_PixelsPerUnit Pixels per unit multiplier
    /// @param p_Pivot         Pivot point
    /// @param p_Extrude       Extrude amount
    /// @param p_Type          Sprite mesh type
    /// @param p_Borders       Borders
    void SpriteU::CreateFromRawThreaded(_v::MonoPtr<::Array<uint8_t>>   p_Bytes,
                                        _v::Action<Sprite*>             p_Callback,
                                        float                           p_PixelsPerUnit,
                                        Vector2                         p_Pivot,
                                        uint                            p_Extrude,
                                        SpriteMeshType                  p_Type,
                                        Vector4                         p_Borders)
    {
        Texture2DU::CreateFromRawThreaded(p_Bytes, [=](Texture2D* p_Texture) -> void
        {
            Sprite* l_Sprite = nullptr;
            if (_v::IsUnityPtrValid(p_Texture))
                l_Sprite = CreateFromTexture(p_Texture, p_PixelsPerUnit, p_Pivot, p_Extrude, p_Type, p_Borders);

            p_Callback(l_Sprite);
        });
    }

}   ///< namespace CP_SDK::Unity