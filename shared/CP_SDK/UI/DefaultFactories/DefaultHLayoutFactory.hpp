#pragma once

#include "../FactoryInterfaces/IHLayoutFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CHLayout factory
    class CP_SDK_EXPORT DefaultHLayoutFactory : public FactoryInterfaces::IHLayoutFactory
    {
        public:
            /// @brief Create an CHLayout into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CHLayout* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories