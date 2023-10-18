#pragma once

#include "../FactoryInterfaces/ITextSegmentedControlFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CTextSegmentedControl factory
    class DefaultTextSegmentedControlFactory : public FactoryInterfaces::ITextSegmentedControlFactory
    {
        public:
            /// @brief Create an CTextSegmentedControl into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CTextSegmentedControl* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories