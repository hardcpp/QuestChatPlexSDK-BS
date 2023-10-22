#include "CP_SDK/Animation/AnimationControllerManager.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Time.hpp>

using namespace UnityEngine;

namespace CP_SDK::Animation {

    CP_SDK_IL2CPP_INHERIT_INIT(AnimationControllerManager);
    CP_SDK_UNITY_PERSISTANT_SINGLETON_IMPL(CP_SDK::Animation::AnimationControllerManager);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Contructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(AnimationControllerManager)
    {
        m_QuickUpdateListCount = 0;
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(AnimationControllerManager)
    {
        WasDestroyed();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Register
    /// @param p_ID     Identifier
    /// @param p_Atlas  Texture atlas
    /// @param p_UVs    UVs rects
    /// @param p_Delays Delays
    AnimationControllerInstance::Ptr AnimationControllerManager::Register(std::u16string p_ID, _v::CMonoPtrRef<Texture2D> p_Atlas, const std::vector<Rect>& p_UVs, const std::vector<uint16_t>& p_Delays)
    {
        auto l_ControllerInstance = m_RegisteredDict.contains(p_ID) ? m_RegisteredDict[p_ID] : nullptr;
        if (!l_ControllerInstance)
        {
            try
            {
                l_ControllerInstance = AnimationControllerInstance::Make(p_Atlas, p_UVs, p_Delays);
                m_RegisteredDict[p_ID] = l_ControllerInstance;

                m_Registered.push_back(l_ControllerInstance);
                m_QuickUpdateListCount++;
            }
            catch (const std::exception&)
            {

            }
        }
        else
        {
            GameObject::Destroy(p_Atlas.Ptr());
        }

        return l_ControllerInstance;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame
    void AnimationControllerManager::Update()
    {
        auto l_Now = static_cast<int64_t>(Time::get_realtimeSinceStartup() * 1000.0f);

        for (int l_I = 0; l_I < m_QuickUpdateListCount; ++l_I)
            m_Registered[l_I]->CheckForNextFrame(l_Now);
    }

}   ///< namespace CP_SDK::Animation
