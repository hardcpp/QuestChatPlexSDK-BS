#pragma once

#include "../FactoryInterfaces/IGLayoutFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CGLayout factory
    class CP_SDK_EXPORT DefaultGLayoutFactory : public FactoryInterfaces::IGLayoutFactory
    {
        public:
            /// @brief Create an CGLayout into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CGLayout* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories