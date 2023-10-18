#pragma once

#include "../FactoryInterfaces/IFloatingPanelFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CFloatingPanel factory
    class DefaultFloatingPanelFactory : public FactoryInterfaces::IFloatingPanelFactory
    {
        public:
            /// @brief Create an CFloatingPanel into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CFloatingPanel* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories