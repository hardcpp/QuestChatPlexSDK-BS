#pragma once

#include "../Components/CFloatingPanel.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CFloatingPanel factory
    class IFloatingPanelFactory
    {
        public:
            /// @brief Create an CFloatingPanel into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CFloatingPanel* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces