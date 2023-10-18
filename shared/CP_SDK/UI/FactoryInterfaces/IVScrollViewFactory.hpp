#pragma once

#include "../Components/CVScrollView.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CVScrollView factory
    class IVScrollViewFactory
    {
        public:
            /// @brief Create an CVScrollView into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CVScrollView* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces