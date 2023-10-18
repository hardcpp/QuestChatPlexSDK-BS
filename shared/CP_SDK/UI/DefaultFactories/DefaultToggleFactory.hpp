#pragma once

#include "../FactoryInterfaces/IToggleFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CToggle factory
    class DefaultToggleFactory : public FactoryInterfaces::IToggleFactory
    {
        public:
            /// @brief Create an CToggle into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CToggle* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories