#pragma once

#include "../FactoryInterfaces/IVVListFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CVVList factory
    class DefaultVVListFactory : public FactoryInterfaces::IVVListFactory
    {
        public:
            /// @brief Create an CVVList into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CVVList* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories