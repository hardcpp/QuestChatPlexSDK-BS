#pragma once

#include "../FactoryInterfaces/IPrimaryButtonFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CPrimaryButton factory
    class CP_SDK_EXPORT DefaultPrimaryButtonFactory : public FactoryInterfaces::IPrimaryButtonFactory
    {
        public:
            /// @brief Create an CPrimaryButton into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CPrimaryButton* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories