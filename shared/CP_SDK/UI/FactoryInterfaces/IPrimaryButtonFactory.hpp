#pragma once

#include "../Components/CPrimaryButton.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CPrimaryButton factory
    class IPrimaryButtonFactory
    {
        public:
            /// @brief Create an CPrimaryButton into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CPrimaryButton* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces