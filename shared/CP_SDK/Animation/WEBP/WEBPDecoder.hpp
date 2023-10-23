#pragma once

#include "../AnimationInfo.hpp"
#include "../../Utils/Delegate.hpp"
#include "../../Utils/Il2cpp.hpp"
#include "../../Utils/MonoPtr.hpp"

#include <UnityEngine/Sprite.hpp>

#include <memory>
#include <vector>
#include <stdint.h>

namespace CP_SDK::Animation::WEBP {

    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief WEBP decoder
    class WEBPDecoder
    {
        CP_SDK_NO_DEF_CTORS(WEBPDecoder);

        public:
            /// @brief Async decode WEBP image
            /// @param p_Raw            Raw data
            /// @param p_Callback       Completion callback
            /// @param p_StaticCallback On static frame completion callback
            static void Process(_v::CMonoPtrRef<::Array<uint8_t>> p_Raw, _v::Action<const AnimationInfo::Ptr&> p_Callback, _v::Action<_v::CMonoPtrRef<UnityEngine::Sprite>> p_StaticCallback);


    };

}   ///< namespace CP_SDK::Animation::WEBP