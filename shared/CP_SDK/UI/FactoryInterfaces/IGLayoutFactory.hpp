#pragma once

#include "../Components/CGLayout.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CGLayout factory
    class IGLayoutFactory
    {
        public:
            /// @brief Create an CGLayout into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CGLayout* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces