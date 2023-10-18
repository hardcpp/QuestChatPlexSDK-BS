#pragma once

#include "../FactoryInterfaces/IVScrollViewFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CVScrollView factory
    class DefaultVScrollViewFactory : public FactoryInterfaces::IVScrollViewFactory
    {
        public:
            /// @brief Create an CVScrollView into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CVScrollView* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories