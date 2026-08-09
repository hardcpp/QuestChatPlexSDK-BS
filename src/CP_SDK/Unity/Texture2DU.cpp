#include "CP_SDK/Unity/Texture2DU.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"

#include "CP_SDK/ChatPlexSDK.hpp"

#include "stb_image.h"

#include <UnityEngine/ImageConversion.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

using namespace UnityEngine;

namespace CP_SDK::Unity {

    /// @brief Load texture from byte array
    /// @param p_Bytes Raw Texture 2D data
    Texture2D* Texture2DU::CreateFromRaw(::Array<uint8_t>* p_Bytes)
    {
        if (p_Bytes && p_Bytes->get_Length() > 0)
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
    void Texture2DU::CreateFromRawThreaded(_v::MonoPtr<::Array<uint8_t>> p_Bytes, _v::Action<Texture2D*> p_Callback)
    {
        MTThreadInvoker::EnqueueOnThread([p_Bytes = std::move(p_Bytes), p_Callback = std::move(p_Callback)]() mutable -> void {
            if (!p_Bytes || p_Bytes->get_Length() <= 0)
            {
                MTMainThreadInvoker::Enqueue([p_Callback]() -> void { p_Callback(nullptr); });
                return;
            }

            try
            {
                int l_InputChannels;
                int l_Width;
                int l_Height;

                if (!stbi_info_from_memory(p_Bytes->_values, p_Bytes->get_Length(), &l_Width, &l_Height, &l_InputChannels))
                    throw std::runtime_error("Failed to load picture");
                if (l_Width <= 0 || l_Height <= 0
                    || static_cast<std::size_t>(l_Width) > static_cast<std::size_t>(std::numeric_limits<int>::max()) / static_cast<std::size_t>(l_Height))
                    throw std::runtime_error("Invalid picture dimensions");

                stbi_set_flip_vertically_on_load_thread(1);

                auto l_STBIBuffer = std::unique_ptr<stbi_uc, decltype(&stbi_image_free)>(
                    stbi_load_from_memory(p_Bytes->_values, p_Bytes->get_Length(), &l_Width, &l_Height, &l_InputChannels, 4),
                    &stbi_image_free
                );
                if (!l_STBIBuffer)
                    throw std::runtime_error("Failed to load picture");
                if (l_Width <= 0 || l_Height <= 0
                    || static_cast<std::size_t>(l_Width) > static_cast<std::size_t>(std::numeric_limits<int>::max()) / static_cast<std::size_t>(l_Height))
                    throw std::runtime_error("Decoded picture dimensions are invalid");

                const auto l_PixelCount = l_Width * l_Height;
                _v::MonoPtr<::Array<Color>> l_Pixels = ::Array<Color>::NewLength(l_PixelCount);

                for (auto l_I = 0; l_I < l_PixelCount; ++l_I)
                {
                    auto l_SrcPixel = &l_STBIBuffer.get()[l_I * 4];
                    l_Pixels->_values[l_I] = Extensions::ColorU::Convert(Color32(0, l_SrcPixel[0], l_SrcPixel[1], l_SrcPixel[2], l_SrcPixel[3]));
                }

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
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRawThreaded] Error2:");
                        ChatPlexSDK::Logger()->Error(l_Exception);
                    }

                    try
                    {
                        p_Callback(l_Texture);
                    }
                    catch (const std::exception& l_Exception)
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRawThreaded] Error3:");
                        ChatPlexSDK::Logger()->Error(l_Exception);
                    }
                });
                return;
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Unity][Texture2D.CreateFromRawThreaded] Error:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }

            MTMainThreadInvoker::Enqueue([p_Callback]() -> void { p_Callback(nullptr); });
        });
    }

}   ///< namespace CP_SDK::Unity
