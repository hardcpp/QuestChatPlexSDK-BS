#pragma once

#include "../Components/CTabControl.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CTabControl factory
    class ITabControlFactory
    {
        public:
            /// @brief Create an CTabControl into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CTabControl* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces