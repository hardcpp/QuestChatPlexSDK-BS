#include "CP_SDK/Unity/Texture2DU.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include "CP_SDK/ChatPlexSDK.hpp"

#include "stb_image.h"

#include <UnityEngine/ImageConversion.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

using namespace UnityEngine;

namespace CP_SDK::Unity {

    /// @brief Load texture from byte array
    /// @param p_Bytes Raw Texture 2D data
    Texture2D* Texture2DU::CreateFromRaw(::Array<uint8_t>* p_Bytes)
    {
        if (p_Bytes->Length() > 0)
        {
            try
            {
                auto l_Texture = Texture2D::New_ctor(2, 2);
                if (ImageConversion::LoadImage(l_Texture, p_Bytes))
                    return l_Texture;
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRaw] Failed");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }

        return nullptr;
    }
    /// @brief Load texture from byte array
    /// @param p_Bytes    Raw Texture 2D data
    /// @param p_Callback Callback
    void Texture2DU::CreateFromRawEx(_v::MonoPtr<::Array<uint8_t>> p_Bytes, _v::Action<Texture2D*> p_Callback)
    {
        if (p_Bytes && p_Bytes->Length() > 0)
        {
            stbi_uc* l_STBIBuffer = nullptr;
            try
            {
                int l_InputChannels;
                int l_Width;
                int l_Height;

                if (!stbi_info_from_memory(p_Bytes->values, p_Bytes->Length(), &l_Width, &l_Height, &l_InputChannels))
                    throw std::runtime_error("Failed to load picture");

                stbi_set_flip_vertically_on_load(1);

                l_STBIBuffer = stbi_load_from_memory(p_Bytes->values, p_Bytes->Length(), &l_Width, &l_Height, &l_InputChannels, 4);
                if (!l_STBIBuffer)
                    throw std::runtime_error("Failed to load picture");

                _v::MonoPtr<::Array<Color>> l_Pixels = ::Array<Color>::NewLength(l_Width * l_Height);

                for (auto l_I = 0; l_I < (l_Width * l_Height); ++l_I)
                {
                    auto l_SrcPixel = &l_STBIBuffer[l_I * 4];
                    l_Pixels->values[l_I] = Extensions::ColorU::Convert(Color32(l_SrcPixel[0], l_SrcPixel[1], l_SrcPixel[2], l_SrcPixel[3]));
                }

                stbi_image_free(l_STBIBuffer);
                l_STBIBuffer = nullptr;

                MTMainThreadInvoker::Enqueue([=]() -> void
                {
                    Texture2D* l_Texture = nullptr;

                    try
                    {
                        l_Texture = Texture2D::New_ctor(l_Width, l_Height, TextureFormat::RGBA32, false);
                        l_Texture->set_wrapMode(TextureWrapMode::Clamp);
                        l_Texture->SetPixels(l_Pixels.Ptr());
                        l_Texture->Apply(true);
                    }
                    catch (const std::exception& l_Exception)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRawEx] Error2:");
                        ChatPlexSDK::Logger()->Error(l_Exception);
                    }

                    try
                    {
                        p_Callback(l_Texture);
                    }
                    catch (const std::exception& l_Exception)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRawEx] Error3:");
                        ChatPlexSDK::Logger()->Error(l_Exception);
                    }
                });
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRawEx] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }

        p_Callback(nullptr);
    }

}   ///< namespace CP_SDK::Unity