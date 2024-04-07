#include "CP_SDK/Animation/WEBP/WEBPDecoder.hpp"
#include "CP_SDK/Unity/MTMainThreadInvoker.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include "webp/webp/decode.h"
#include "webp/webp/demux.h"

#include <UnityEngine/Texture2D.hpp>
#include <UnityEngine/TextureFormat.hpp>
#include <UnityEngine/TextureWrapMode.hpp>

namespace CP_SDK::Animation::WEBP {

    /// @brief Async decode WEBP image
    /// @param p_Raw            Raw data
    /// @param p_Callback       Completion callback
    /// @param p_StaticCallback On static frame completion callback
    void WEBPDecoder::Process(_v::CMonoPtrRef<::Array<uint8_t>> p_Raw, _v::Action<const AnimationInfo::Ptr&> p_Callback, _v::Action<_v::CMonoPtrRef<UnityEngine::Sprite>> p_StaticCallback)
    {
        Unity::MTThreadInvoker::EnqueueOnThread([=]() -> void {
            WebPBitstreamFeatures l_Features = {};

            if (WebPGetFeatures(p_Raw->_values, p_Raw->get_Length(), &l_Features) != VP8_STATUS_OK)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Failed to get WebPFeatures");
                p_Callback(nullptr);
                return;
            }

            if (l_Features.has_animation == 0)
            {
                throw std::runtime_error("EAnimationType::WEBP Static is not implemented!");
                /*auto l_Width    = l_Features.width;
                auto l_Height   = l_Features.height;

                var l_Bitmap        = null as Bitmap;
                var l_BitmapData    = null as BitmapData;

                try
                {
                    auto l_TextureFormat = UnityEngine::TextureFormat::RGB32;
                    if (l_Features.has_alpha == 1)
                        l_TextureFormat = UnityEngine::TextureFormat::RGBA32;


                        l_Bitmap = new Bitmap(l_Features.width, l_Features.height, PixelFormat.Format32bppArgb);
                    else
                        l_Bitmap = new Bitmap(l_Features.width, l_Features.height, PixelFormat.Format24bppRgb);
                    l_BitmapData = l_Bitmap.LockBits(new Rectangle(0, 0, l_Features.width, l_Features.height), ImageLockMode.WriteOnly, l_Bitmap.PixelFormat);

                    int l_ResultSize = 0;
                    if (l_Bitmap.PixelFormat == PixelFormat.Format24bppRgb)
                        l_ResultSize = WebPDecodeBGRInto(p_Raw, l_BitmapData.Scan0, l_BitmapData.Stride * l_Features.height, l_BitmapData.Stride);
                    else
                        l_ResultSize = WebPDecodeBGRAInto(p_Raw, l_BitmapData.Scan0, l_BitmapData.Stride * l_Features.height, l_BitmapData.Stride);

                    if (l_ResultSize == 0)
                        throw std::runtime_error("Can't decode WebP");

                    l_Bitmap.UnlockBits(l_BitmapData);


                    ArrayW<Color32> l_Colors(l_Width * l_Height);
                    for (auto l_Y = 0; l_Y < l_Bitmap.Height; l_Y++)
                    {
                        for (auto l_X = 0; l_X < l_Bitmap.Width; l_X++)
                        {
                            var l_SourceColor = l_Bitmap.GetPixel(l_X, l_Y);
                            l_Colors[(l_Bitmap.Height - l_Y - 1) * l_Bitmap.Width + l_X] = new UnityEngine.Color32(l_SourceColor.R, l_SourceColor.G, l_SourceColor.B, l_SourceColor.A);
                        }
                    }

                    Unity::MTMainThreadInvoker::Enqueue(_v::MakeDelegate([=]() -> void
                    {
                        UnityEngine::Texture2D* l_Texture = nullptr;

                        try
                        {
                            l_Texture = UnityEngine::Texture2D::New_ctor(l_Width, l_Height, UnityEngine::TextureFormat::RGBA32, false);
                            l_Texture->set_wrapMode(UnityEngine::TextureWrapMode::Clamp);

                            l_Texture->SetPixels32(l_Colors);
                            l_Texture->Apply(true);
                        }
                        catch (const std::exception& l_Exception)
                        {
                            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Error2:");
                            ChatPlexSDK::Logger()->Error(l_Exception);
                        }

                        try
                        {
                            p_StaticCallback(Unity::SpriteU::CreateFromTexture(l_Texture));
                        }
                        catch (const std::exception& l_Exception)
                        {
                            ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Error3:");
                            ChatPlexSDK::Logger()->Error(l_Exception);
                        }
                    }));
                }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Error4:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                    p_StaticCallback(nullptr);
                }*/
            }
            else
            {
                ///var l_Options = new Natives.WEBPDemux.WebPAnimDecoderOptions();
                ///l_Options.use_threads = 0;
                ///l_Options.color_mode = Natives.WEBP.WEBP_CSP_MODE.MODE_RGBA;

                auto l_WebPData = WebPData();
                l_WebPData.bytes = p_Raw->_values;
                l_WebPData.size  = p_Raw->get_Length();

                auto l_Decoder = WebPAnimDecoderNew(&l_WebPData, nullptr);
                if (l_Decoder)
                {
                    auto l_Infos = WebPAnimInfo();
                    if (WebPAnimDecoderGetInfo(l_Decoder, &l_Infos))
                    {
                        auto l_Buffer        = (uint8_t*)nullptr;
                        auto l_TimeStamp     = 0;
                        auto l_PrevTimeStamp = 0;
                        auto l_AnimationInfo = AnimationInfo::Make((int)l_Infos.canvas_width, (int)l_Infos.canvas_height, l_Infos.frame_count);
                        auto l_FrameI        = 0;

                        while (WebPAnimDecoderHasMoreFrames(l_Decoder))
                        {
                            if (!WebPAnimDecoderGetNext(l_Decoder, &l_Buffer, &l_TimeStamp))
                            {
                                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Failed to decode next frame");
                                break;
                            }

                            auto l_TargetArray = l_AnimationInfo->Frames[l_FrameI];
                            for (int l_Line = 0; l_Line < l_Infos.canvas_height; ++l_Line)
                            {
                                for (int l_X = 0; l_X < l_Infos.canvas_width; ++l_X)
                                {
                                    auto l_DestOffset = ((l_Infos.canvas_height - (l_Line + 1)) * l_Infos.canvas_width) + l_X;
                                    auto l_SourOffset = (l_Line * l_Infos.canvas_width * 4) + (l_X * 4);
                                    l_TargetArray[l_DestOffset].r = l_Buffer[l_SourOffset + 0];
                                    l_TargetArray[l_DestOffset].g = l_Buffer[l_SourOffset + 1];
                                    l_TargetArray[l_DestOffset].b = l_Buffer[l_SourOffset + 2];
                                    l_TargetArray[l_DestOffset].a = l_Buffer[l_SourOffset + 3];
                                }
                            }

                            l_AnimationInfo->Delays[l_FrameI]   = (uint16_t)(l_TimeStamp - l_PrevTimeStamp);
                            l_PrevTimeStamp                     = l_TimeStamp;

                            l_FrameI++;
                        }

                        WebPAnimDecoderDelete(l_Decoder);

                        p_Callback(l_AnimationInfo);
                    }
                    else
                    {
                        ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Failed to decode animated informations");
                        p_Callback(nullptr);
                    }
                }
                else
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation.WEBP][WEBP.ProcessingThread] Failed to decode");
                    p_Callback(nullptr);
                }
            }
        });
    }

}   ///< namespace CP_SDK::Animation::WEBP