#pragma once

#include "../../../CP_SDK/UI/FactoryInterfaces/IFloatingPanelFactory.hpp"

namespace CP_SDK_BS::UI::DefaultFactoriesOverrides {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief BeatSaber CFloatingPanel factory
    class BS_FloatingPanelFactory : public CP_SDK::UI::FactoryInterfaces::IFloatingPanelFactory
    {
        public:
            /// @brief Create an CFloatingPanel into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual CP_SDK::UI::Components::CFloatingPanel* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK_BS::UI::DefaultFactoriesOverrides