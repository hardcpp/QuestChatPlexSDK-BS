#pragma once

#include "../../../CP_SDK/UI/DefaultComponents/DefaultCFloatingPanel.hpp"
#include "Subs/SubFloatingPanelMoverHandle.hpp"

#include <UnityEngine/Vector2.hpp>
#include <VRUIControls/VRPointer.hpp>

namespace CP_SDK_BS::UI::DefaultComponentsOverrides {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace VRUIControls;
    }

    /// @brief BeatSaber CFloatingPanel component
    class BS_CFloatingPanel : public CP_SDK::UI::DefaultComponents::DefaultCFloatingPanel
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI.DefaultComponentsOverrides", BS_CFloatingPanel, CP_SDK::UI::DefaultComponents::DefaultCFloatingPanel);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(BS_CFloatingPanel);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(BS_CFloatingPanel);

        private:
            CP_SDK::Utils::MonoPtr<Subs::SubFloatingPanelMoverHandle> m_MoverHandle;

        public:
            /// @brief On component creation
            void Init_BS_CFloatingPanel();

        public:
            /// @brief Set allow movements
            /// @param p_Allow Is allowed?
            void SetAllowMovement_Impl2(bool p_Allow);
            /// @brief Set radius on supported games
            /// @param p_Radius Canvas radius
            void SetRadius_Impl2(float p_Radius);
            /// @brief Set size
            /// @param p_Size New size
            void SetSize_Impl2(_u::Vector2 p_Size);

        private:
            /// @brief Create mover
            /// @param p_VRPointer VRPointer instance
            void CreateMoverOnPointerCreated(_u::VRPointer* p_VRPointer);
            /// @brief Create mover
            /// @param p_VRPointer VRPointer instance
            void CreateMover(_u::VRPointer* p_VRPointer = nullptr);
            /// @brief Update mover collision
            void UpdateMover();

    };

}   ///< namespace CP_SDK_BS::UI::DefaultComponentsOverrides

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::DefaultComponentsOverrides::BS_CFloatingPanel);