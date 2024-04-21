#include "CP_SDK/Animation/AnimationInfo.hpp"

using namespace UnityEngine;

namespace CP_SDK::Animation {

    /// @brief Constructor
    /// @param p_Width      Width
    /// @param p_Height     Height
    /// @param p_FrameCount Animation frame count
    AnimationInfo::AnimationInfo(CP_SDK_PRIV_TAG_ARG(), int p_Width, int p_Height, uint32_t p_FrameCount)
    {
        Width   = p_Width;
        Height  = p_Width;
        Frames.reserve(p_FrameCount);
        Delays.resize(p_FrameCount);

        for (int l_I = 0; l_I < p_FrameCount; ++l_I)
            Frames.push_back(Array<uint8_t>::NewLength(4 * p_Width * p_Height));
    }
    /// @brief Destructor
    AnimationInfo::~AnimationInfo()
    {
        Frames.clear();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param p_Width      Width
    /// @param p_Height     Height
    /// @param p_FrameCount Animation frame count
    AnimationInfo::Ptr AnimationInfo::Make(int p_Width, int p_Height, uint32_t p_FrameCount)
    {
        return std::make_shared<AnimationInfo>(CP_SDK_PRIV_TAG_VAL(), p_Width, p_Height, p_FrameCount);
    }

}   ///< namespace CP_SDK::Animation