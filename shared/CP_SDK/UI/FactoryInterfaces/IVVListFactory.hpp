#pragma once

#include "../Components/CVVList.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CVVList factory
    class IVVListFactory
    {
        public:
            /// @brief Create an CVVList into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CVVList* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces