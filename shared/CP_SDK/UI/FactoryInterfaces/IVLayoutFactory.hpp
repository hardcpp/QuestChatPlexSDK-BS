#pragma once

#include "../Components/CVLayout.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CHLayout factory
    class IVLayoutFactory
    {
        public:
            /// @brief Create an CVLayout into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CVLayout* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces