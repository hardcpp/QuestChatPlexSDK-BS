#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Unity/PersistantSingleton.hpp"
#include "AnimationControllerInstance.hpp"

#include <UnityEngine/MonoBehaviour.hpp>

#include <map>

namespace CP_SDK::Animation {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Animation controller manager
    class CP_SDK_EXPORT AnimationControllerManager : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.Animation", AnimationControllerManager, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(AnimationControllerManager);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(AnimationControllerManager);
        CP_SDK_UNITY_PERSISTANT_SINGLETON_DECL(CP_SDK::Animation::AnimationControllerManager);

        private:
            std::map<std::u16string, AnimationControllerInstance::Ptr>  m_RegisteredDict;
            std::vector<AnimationControllerInstance::Ptr>               m_Registered;
            int                                                         m_QuickUpdateListCount;

        public:
            /// @brief Register
            /// @param p_ID     Identifier
            /// @param p_Atlas  Texture atlas
            /// @param p_UVs    UVs rects
            /// @param p_Delays Delays
            AnimationControllerInstance::Ptr Register(  std::u16string                  p_ID,
                                                        _v::CMonoPtrRef<_u::Texture2D>  p_Atlas,
                                                        const std::vector<_u::Rect>&    p_UVs,
                                                        const std::vector<uint16_t>&    p_Delays);

        public:
            /// @brief On frame
            DECLARE_INSTANCE_METHOD(void, Update);

    };

}   ///< namespace CP_SDK::Animation

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::Animation::AnimationControllerManager);