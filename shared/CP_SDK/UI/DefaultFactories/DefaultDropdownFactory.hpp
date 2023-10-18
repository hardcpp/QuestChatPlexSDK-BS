#pragma once

#include "../FactoryInterfaces/IDropdownFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CDropdown factory
    class DefaultDropdownFactory : public FactoryInterfaces::IDropdownFactory
    {
        public:
            /// @brief Create an CDropdown into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CDropdown* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories