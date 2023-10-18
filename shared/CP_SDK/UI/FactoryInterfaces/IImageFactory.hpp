#pragma once

#include "../Components/CImage.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CImage factory
    class IImageFactory
    {
        public:
            /// @brief Create an CImage into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CImage* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces