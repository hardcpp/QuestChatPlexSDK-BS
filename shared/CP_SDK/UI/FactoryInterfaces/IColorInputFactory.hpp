#pragma once

#include "../Components/CColorInput.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CColorInput factory
    class IColorInputFactory
    {
        public:
            /// @brief Create an CColorInput into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CColorInput* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces