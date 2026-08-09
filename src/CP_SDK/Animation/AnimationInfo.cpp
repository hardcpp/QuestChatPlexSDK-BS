#include "CP_SDK/Animation/AnimationInfo.hpp"

#include <limits>
#include <stdexcept>

using namespace UnityEngine;

namespace CP_SDK::Animation {

    /// @brief Constructor
    /// @param p_Width      Width
    /// @param p_Height     Height
    /// @param p_FrameCount Animation frame count
    AnimationInfo::AnimationInfo(CP_SDK_PRIV_TAG_ARG(), int p_Width, int p_Height, uint32_t p_FrameCount)
    {
        if (p_Width <= 0 || p_Height <= 0 || p_FrameCount == 0)
            throw std::invalid_argument("Animation dimensions and frame count must be positive");

        const auto l_FrameByteCount = static_cast<uint64_t>(p_Width) * static_cast<uint64_t>(p_Height) * 4ULL;
        if (l_FrameByteCount > static_cast<uint64_t>(std::numeric_limits<int32_t>::max()))
            throw std::overflow_error("Animation frame is too large");

        Width   = p_Width;
        Height  = p_Height;
        Frames.reserve(p_FrameCount);
        Delays.resize(p_FrameCount);

        for (uint32_t l_I = 0; l_I < p_FrameCount; ++l_I)
            Frames.push_back(Array<uint8_t>::NewLength(static_cast<int32_t>(l_FrameByteCount)));
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
