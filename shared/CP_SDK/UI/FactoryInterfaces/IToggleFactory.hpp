#pragma once

#include "../Components/CToggle.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CToggle factory
    class IToggleFactory
    {
        public:
            /// @brief Create an CToggle into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CToggle* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces