#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <beatsaber-hook/shared/utils/typedefs-array.hpp>
#include <UnityEngine/Color32.hpp>

#include <memory>
#include <vector>
#include <stdint.h>

namespace CP_SDK::Animation {

    namespace _u
    {
        using namespace UnityEngine;
    }

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Animation frame info
    class CP_SDK_EXPORT_VISIBILITY AnimationInfo
    {
        CP_SDK_NO_COPYMOVE_CTORS(AnimationInfo);
        CP_SDK_PRIV_TAG();

        public:
            using Ptr = std::shared_ptr<AnimationInfo>;

        public:
            int32_t                                     Width;
            int32_t                                     Height;
            std::vector<_v::MonoPtr<Array<uint8_t>>>    Frames;
            std::vector<uint16_t>                       Delays;

        public:
            /// @brief Constructor
            /// @param p_Width      Width
            /// @param p_Height     Height
            /// @param p_FrameCount Animation frame count
            AnimationInfo(CP_SDK_PRIV_TAG_ARG(), int p_Width, int p_Height, uint32_t p_FrameCount);
            /// @brief Destructor
            ~AnimationInfo();

        public:
            /// @brief Constructor
            /// @param p_Width      Width
            /// @param p_Height     Height
            /// @param p_FrameCount Animation frame count
            static Ptr Make(int p_Width, int p_Height, uint32_t p_FrameCount);

    };

}   ///< namespace CP_SDK::Animation