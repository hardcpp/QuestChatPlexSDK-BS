#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <UnityEngine/Texture2D.hpp>

namespace CP_SDK::Unity {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Texture2D helper
    class Texture2DU
    {
        CP_SDK_NO_DEF_CTORS(Texture2DU);

        public:
            /// @brief Load texture from byte array
            /// @param p_Bytes Raw Texture 2D data
            static _u::Texture2D* CreateFromRaw(::Array<uint8_t>* p_Bytes);
            /// @brief Load texture from byte array
            /// @param p_Bytes    Raw Texture 2D data
            /// @param p_Callback Callback
            static void CreateFromRawThreaded(_v::MonoPtr<::Array<uint8_t>> p_Bytes, _v::Action<_u::Texture2D*> p_Callback);

    };

}   ///< namespace CP_SDK::Unity