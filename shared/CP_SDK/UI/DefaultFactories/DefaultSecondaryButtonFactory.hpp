#pragma once

#include "../FactoryInterfaces/ISecondaryButtonFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CSecondaryButton factory
    class DefaultSecondaryButtonFactory : public FactoryInterfaces::ISecondaryButtonFactory
    {
        public:
            /// @brief Create an CSecondaryButton into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CSecondaryButton* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories