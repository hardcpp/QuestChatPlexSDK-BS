#pragma once

#include "../FactoryInterfaces/IVLayoutFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CVLayout factory
    class DefaultVLayoutFactory : public FactoryInterfaces::IVLayoutFactory
    {
        public:
            /// @brief Create an CVLayout into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CVLayout* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories