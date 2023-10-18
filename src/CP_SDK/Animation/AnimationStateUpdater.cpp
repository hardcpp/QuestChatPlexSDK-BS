#include "CP_SDK/Animation/AnimationStateUpdater.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::Animation {

    CP_SDK_IL2CPP_INHERIT_INIT(AnimationStateUpdater);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Contructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(AnimationStateUpdater)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(AnimationStateUpdater)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set the new AnimationControllerInstance
    /// @param p_ControllerDataInstance New controller
    void AnimationStateUpdater::SetControllerDataInstance(AnimationControllerInstance::Ptr& p_ControllerDataInstance)
    {
        if (m_ControllerDataInstance)
            OnDisable();

        m_ControllerDataInstance = p_ControllerDataInstance;

        if (get_isActiveAndEnabled())
            OnEnable();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On destroy
    void AnimationStateUpdater::OnDestroy()
    {
        if (m_ControllerDataInstance && TargetImage)
            m_ControllerDataInstance->Unregister(TargetImage.Ptr());

        m_ControllerDataInstance    = nullptr;
        TargetImage                 = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On enabled
    void AnimationStateUpdater::OnEnable()
    {
        if (m_ControllerDataInstance && TargetImage)
            m_ControllerDataInstance->Register(TargetImage.Ptr());

    }
    /// @brief On disable
    void AnimationStateUpdater::OnDisable()
    {
        if (m_ControllerDataInstance && TargetImage)
            m_ControllerDataInstance->Unregister(TargetImage.Ptr());
    }

}   ///< namespace CP_SDK::Animation