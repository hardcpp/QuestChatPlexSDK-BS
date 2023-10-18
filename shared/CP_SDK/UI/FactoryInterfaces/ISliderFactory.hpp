#pragma once

#include "../Components/CSlider.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CSlider factory
    class ISliderFactory
    {
        public:
            /// @brief Create an CSlider into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CSlider* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces