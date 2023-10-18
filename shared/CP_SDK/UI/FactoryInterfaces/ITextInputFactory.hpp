#pragma once

#include "../Components/CTextInput.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::UI::FactoryInterfaces {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief CTextInput factory
    class ITextInputFactory
    {
        public:
            /// @brief Create an CTextInput into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CTextInput* Create(std::u16string_view p_Name, _u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::UI::FactoryInterfaces