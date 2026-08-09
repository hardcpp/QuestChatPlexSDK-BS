#include "CP_SDK/Animation/AnimationControllerManager.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Time.hpp>

#include <algorithm>

using namespace UnityEngine;

namespace CP_SDK::Animation {

    namespace {
        constexpr std::size_t MAX_CACHED_ANIMATIONS = 32;
    }

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
        if (!p_Atlas || p_UVs.empty() || p_UVs.size() != p_Delays.size())
            return nullptr;

        auto l_ControllerInstance = m_RegisteredDict.contains(p_ID) ? m_RegisteredDict[p_ID] : nullptr;
        if (!l_ControllerInstance)
        {
            try
            {
                if (m_Registered.size() >= MAX_CACHED_ANIMATIONS)
                    EvictUnused();

                l_ControllerInstance = AnimationControllerInstance::Make(p_Atlas, p_UVs, p_Delays);
                m_RegisteredDict[p_ID] = l_ControllerInstance;

                m_Registered.push_back(l_ControllerInstance);
                m_QuickUpdateListCount++;
            }
            catch (const std::exception& l_Exception)
            {
                ChatPlexSDK::Logger()->Error(u"[CP_SDK.Animation][AnimationControllerManager.Register] Failed to register animation:");
                ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
        else
        {
            GameObject::Destroy(p_Atlas.Ptr());
        }

        return l_ControllerInstance;
    }

    /// @brief Evict unused cached animations when the cache is full
    void AnimationControllerManager::EvictUnused()
    {
        while (m_Registered.size() >= MAX_CACHED_ANIMATIONS)
        {
            const auto l_VectorIt = std::find_if(m_Registered.begin(), m_Registered.end(), [](const auto& p_Controller) {
                return p_Controller && !p_Controller->HasActiveImages();
            });
            if (l_VectorIt == m_Registered.end())
                break;

            const auto l_Controller = *l_VectorIt;
            const auto l_DictIt = std::find_if(m_RegisteredDict.begin(), m_RegisteredDict.end(), [&](const auto& p_Pair) {
                return p_Pair.second == l_Controller;
            });
            if (l_DictIt != m_RegisteredDict.end())
                m_RegisteredDict.erase(l_DictIt);

            m_Registered.erase(l_VectorIt);
        }

        m_QuickUpdateListCount = static_cast<int>(m_Registered.size());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame
    void AnimationControllerManager::Update()
    {
        if (m_Registered.size() > MAX_CACHED_ANIMATIONS)
            EvictUnused();

        auto l_Now = static_cast<int64_t>(Time::get_realtimeSinceStartup() * 1000.0f);

        for (int l_I = 0; l_I < m_QuickUpdateListCount; ++l_I)
            m_Registered[l_I]->CheckForNextFrame(l_Now);
    }

}   ///< namespace CP_SDK::Animation
