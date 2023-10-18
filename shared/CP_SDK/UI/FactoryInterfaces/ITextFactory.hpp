#pragma once

#include "../Components/CText.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CText factory
    class ITextFactory
    {
        public:
            /// @brief Create an CText into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CText* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces