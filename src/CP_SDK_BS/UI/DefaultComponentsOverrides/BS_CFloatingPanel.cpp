#include "CP_SDK_BS/UI/DefaultComponentsOverrides/BS_CFloatingPanel.hpp"
#include "CP_SDK_BS/UI/DefaultComponentsOverrides/Subs/SubFloatingPanelMover.hpp"
#include "CP_SDK_BS/UI/Patches/PVRPointer.hpp"

#include <HMUI/CurvedCanvasSettings.hpp>
#include <UnityEngine/Resources.hpp>

using namespace HMUI;
using namespace UnityEngine;
using namespace VRUIControls;

namespace CP_SDK_BS::UI::DefaultComponentsOverrides {

    CP_SDK_IL2CPP_INHERIT_INIT(BS_CFloatingPanel);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(BS_CFloatingPanel)
    {
        m_SetAllowMovement  = {this, &BS_CFloatingPanel::SetAllowMovement_Impl2};
        m_SetRadius         = {this, &BS_CFloatingPanel::SetRadius_Impl2};
        m_SetSize           = {this, &BS_CFloatingPanel::SetSize_Impl2};

        Init.Clear();
        Init += {this, &BS_CFloatingPanel::Init_BS_CFloatingPanel};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(BS_CFloatingPanel)
    {
        Patches::PVRPointer::OnActivated -= {this, &BS_CFloatingPanel::CreateMoverOnPointerCreated};
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void BS_CFloatingPanel::Init_BS_CFloatingPanel()
    {
        auto l_ShouldContinue = !RTransform();
        Init_DefaultCFloatingPanel();

        if (!l_ShouldContinue)
            return;

        auto l_CurvedCanvasSettings = get_gameObject()->AddComponent<CurvedCanvasSettings*>();
        l_CurvedCanvasSettings->SetRadius(140.0f);

        CreateMover();
        SetAllowMovement(false);

        Patches::PVRPointer::OnActivated -= {this, &BS_CFloatingPanel::CreateMoverOnPointerCreated};
        Patches::PVRPointer::OnActivated += {this, &BS_CFloatingPanel::CreateMoverOnPointerCreated};
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set allow movements
    /// @param p_Allow Is allowed?
    void BS_CFloatingPanel::SetAllowMovement_Impl2(bool p_Allow)
    {
        /// Handled by super method
        //base.SetAllowMovement(p_Allow);

        if (m_MoverHandle)
            m_MoverHandle->get_gameObject()->SetActive(p_Allow);

        if (p_Allow)
        {
            /// Refresh VR pointer due to bug
            auto l_VRPointers    = Resources::FindObjectsOfTypeAll<VRPointer*>();
            auto l_VRPointer     = CP_SDK::ChatPlexSDK::ActiveGenericScene() == CP_SDK::EGenericScene::Playing ? l_VRPointers.LastOrDefault() : l_VRPointers.FirstOrDefault();

            if (l_VRPointer)
            {
                if (!l_VRPointer->GetComponent<Subs::SubFloatingPanelMover*>())
                    l_VRPointer->get_gameObject()->AddComponent<Subs::SubFloatingPanelMover*>();
            }
            else
                CP_SDK::ChatPlexSDK::Logger()->Warning(u"[CP_SDK_BS.UI.DefaultComponentsOverrides][BS_CFloatingPanel.SetAllowMovement] Failed to get VRPointer!");
        }
    }
    /// @brief Set radius on supported games
    /// @param p_Radius Canvas radius
    void BS_CFloatingPanel::SetRadius_Impl2(float p_Radius)
    {
        /// Handled by super method
        //base.SetRadius(p_Radius);

        auto l_CurvedCanvasSettings = get_gameObject()->GetComponent<CurvedCanvasSettings*>();
        if (l_CurvedCanvasSettings)
            l_CurvedCanvasSettings->SetRadius(p_Radius);
    }
    /// @brief Set size
    /// @param p_Size New size
    void BS_CFloatingPanel::SetSize_Impl2(Vector2 p_Size)
    {
        /// Handled by super method
        //base.SetSize(p_Size);

        UpdateMover();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create mover
    /// @param p_VRPointer VRPointer instance
    void BS_CFloatingPanel::CreateMoverOnPointerCreated(VRPointer* p_VRPointer)
    {
        CreateMover(p_VRPointer);
    }
    /// @brief Create mover
    /// @param p_VRPointer VRPointer instance
    void BS_CFloatingPanel::CreateMover(VRPointer* p_VRPointer)
    {
        if (!p_VRPointer)
            p_VRPointer = Resources::FindObjectsOfTypeAll<VRPointer*>().FirstOrDefault();

        if (p_VRPointer == nullptr)
        {
            CP_SDK::ChatPlexSDK::Logger()->Warning(u"[CP_SDK_BS.UI.DefaultComponentsOverrides][BS_CFloatingPanel.CreateMover] Failed to get VRPointer!");
            return;
        }

        if (!p_VRPointer->GetComponent<Subs::SubFloatingPanelMover*>())
            p_VRPointer->get_gameObject()->AddComponent<Subs::SubFloatingPanelMover*>();

        if (!m_MoverHandle)
        {
            m_MoverHandle = GameObject::New_ctor("MoverHandle", ArrayW<System::Type*>({
                reinterpret_cast<System::Type*>(csTypeOf(Subs::SubFloatingPanelMoverHandle*))
            }))->GetComponent<Subs::SubFloatingPanelMoverHandle*>();
            m_MoverHandle->get_transform()->SetParent(get_transform());
            m_MoverHandle->get_transform()->set_localPosition(Vector3::get_zero());
            m_MoverHandle->get_transform()->set_localRotation(Quaternion::get_identity());
            m_MoverHandle->get_transform()->set_localScale   (Vector3::get_one());
            m_MoverHandle->FloatingPanel = this;

            UpdateMover();
        }
    }
    /// @brief Update mover collision
    void BS_CFloatingPanel::UpdateMover()
    {
        if (!m_MoverHandle)
            return;

        auto l_Size = RTransform()->get_sizeDelta();

        m_MoverHandle->get_transform()->set_localPosition(Vector3(0.0f, 0.0f, 0.1f));
        m_MoverHandle->get_transform()->set_localRotation(Quaternion::get_identity());
        m_MoverHandle->get_transform()->set_localScale   (Vector3(l_Size.x, l_Size.y, 0.1f));
    }

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides