#pragma once

#include "../Components/CSecondaryButton.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CSecondaryButton factory
    class ISecondaryButtonFactory
    {
        public:
            /// @brief Create an CSecondaryButton into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CSecondaryButton* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces