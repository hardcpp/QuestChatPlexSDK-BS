#pragma once

#include "../Components/CTextSegmentedControl.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CTextSegmentedControl factory
    class ITextSegmentedControlFactory
    {
        public:
            /// @brief Create an CTextSegmentedControl into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CTextSegmentedControl* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces