#pragma once

#include "../Components/CDropdown.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CDropdown factory
    class IDropdownFactory
    {
        public:
            /// @brief Create an CDropdown into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CDropdown* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces