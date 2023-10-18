#pragma once

#include "../Components/CIconButton.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CIconButton factory
    class IIconButtonFactory
    {
        public:
            /// @brief Create an CIconButton into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CIconButton* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces