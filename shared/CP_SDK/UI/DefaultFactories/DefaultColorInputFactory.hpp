#pragma once

#include "../FactoryInterfaces/IColorInputFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CColorInput factory
    class CP_SDK_EXPORT DefaultColorInputFactory : public FactoryInterfaces::IColorInputFactory
    {
        public:
            /// @brief Create an CColorInput into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CColorInput* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories