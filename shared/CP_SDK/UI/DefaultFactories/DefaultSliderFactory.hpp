#pragma once

#include "../FactoryInterfaces/ISliderFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CSlider factory
    class DefaultSliderFactory : public FactoryInterfaces::ISliderFactory
    {
        public:
            /// @brief Create an CSlider into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CSlider* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories