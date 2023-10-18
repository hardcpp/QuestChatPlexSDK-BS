#include "CP_SDK_BS/UI/DefaultComponentsOverrides/Subs/SubFloatingPanelMover.hpp"
#include "CP_SDK_BS/UI/DefaultComponentsOverrides/Subs/SubFloatingPanelMoverHandle.hpp"
#include "CP_SDK_BS/UI/DefaultComponentsOverrides/BS_CFloatingPanel.hpp"
#include "CP_SDK/UI/UISystem.hpp"

#include <limits>

#include <UnityEngine/BoxCollider.hpp>
#include <UnityEngine/Physics.hpp>
#include <UnityEngine/Rigidbody.hpp>
#include <UnityEngine/Time.hpp>

using namespace UnityEngine;
using namespace VRUIControls;

namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs {

    CP_SDK_IL2CPP_INHERIT_INIT(SubFloatingPanelMover);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SubFloatingPanelMover)
    {
        m_RaycastBuffer = Array<RaycastHit>::NewLength(10);
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SubFloatingPanelMover)
    {
        m_VRPointer             = nullptr;
        m_FloatingPanel         = nullptr;
        m_GrabbingController    = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void SubFloatingPanelMover::Awake()
    {
        m_VRPointer = GetComponent<VRPointer*>();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame
    void SubFloatingPanelMover::Update()
    {
#if BEATSABER_1_29_4_OR_NEWER
        auto l_VRController          = m_VRPointer ? m_VRPointer->get_lastSelectedVrController() : nullptr;
#else
        auto l_VRController          = m_VRPointer ? m_VRPointer->get_vrController() : nullptr;
#endif
        auto l_VRControllerTransform = l_VRController != nullptr ? l_VRController->get_transform() : nullptr;
        auto l_ButtonDown            = l_VRController != nullptr ? l_VRController->get_triggerValue() > 0.9f : false;

        if (l_VRController != nullptr && l_ButtonDown)
        {
            if (m_GrabbingController)
                return;

            auto l_HitCount = Physics::RaycastNonAlloc( l_VRControllerTransform->get_position(),
                                                        l_VRControllerTransform->get_forward(),
                                                        m_RaycastBuffer.Ptr(),
                                                        MaxLaserDistance,
                                                        static_cast<int>(1 << CP_SDK::UI::UISystem::UILayer));

            for (auto l_I = 0; l_I < l_HitCount; ++l_I)
            {
                auto& l_Hit             = m_RaycastBuffer->get(l_I);
                auto l_Collider         = l_Hit.get_collider();
                auto l_TargetTransform  = (Transform*)nullptr;

                if (l_Collider && l_Collider->get_attachedRigidbody())
                    l_TargetTransform = l_Collider->get_attachedRigidbody()->get_transform();
                else if (l_Collider)
                    l_TargetTransform = l_Collider->get_transform();

                if (!l_TargetTransform)
                    continue;

                auto l_SubFloatingPanelMoverHandle = l_TargetTransform->GetComponent<SubFloatingPanelMoverHandle*>();
                if (!l_SubFloatingPanelMoverHandle)
                    continue;

                m_FloatingPanel         = l_SubFloatingPanelMoverHandle->FloatingPanel;
                m_GrabbingController    = l_VRController;
                m_GrabPosition          = l_VRControllerTransform->InverseTransformPoint(m_FloatingPanel->RTransform()->get_position());
                m_GrabRotation          = Quaternion::Inverse(l_VRControllerTransform->get_rotation()) * m_FloatingPanel->RTransform()->get_rotation();

                m_FloatingPanel->FireOnGrab();
                break;
            }
        }

        if (m_GrabbingController && !l_ButtonDown)
        {
            m_FloatingPanel->FireOnRelease();

            m_FloatingPanel         = nullptr;
            m_GrabbingController    = nullptr;
        }
    }
    /// @brief On frame (late)
    void SubFloatingPanelMover::LateUpdate()
    {
        if (!m_GrabbingController)
            return;

#if BEATSABER_1_29_4_OR_NEWER
        auto l_Delta = m_GrabbingController->get_thumbstick().y * Time::get_unscaledDeltaTime();
#else
        auto l_Delta = m_GrabbingController->get_verticalAxisValue() * Time::get_unscaledDeltaTime();
#endif
        if (m_GrabPosition.get_magnitude() > MinScrollDistance) m_GrabPosition = m_GrabPosition - (Vector3::get_forward() * l_Delta);
        else                                                    m_GrabPosition = m_GrabPosition - (Vector3::get_forward() * std::clamp<float>(l_Delta, std::numeric_limits<float>::min(), 0.0f));

        auto l_RTransform            = m_FloatingPanel->RTransform();
        auto l_ControllerTransform   = m_GrabbingController->get_transform();

        auto l_RealPosition = m_GrabbingController->get_transform()->TransformPoint(m_GrabPosition);
        auto l_RealRotation = m_GrabbingController->get_transform()->get_rotation() * m_GrabRotation;

        l_RTransform->set_position(Vector3::Lerp(l_RTransform->get_position(),       l_RealPosition, 10.0f * Time::get_unscaledDeltaTime()));
        l_RTransform->set_rotation(Quaternion::Slerp(l_RTransform->get_rotation(),   l_RealRotation,  5.0f * Time::get_unscaledDeltaTime()));
    }

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides::Subs
