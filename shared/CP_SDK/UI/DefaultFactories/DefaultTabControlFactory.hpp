#pragma once

#include "../FactoryInterfaces/ITabControlFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CTabControl factory
    class DefaultTabControlFactory : public FactoryInterfaces::ITabControlFactory
    {
        public:
            /// @brief Create an CTabControl into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CTabControl* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories