#pragma once

#include "../FactoryInterfaces/IFLayoutFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CFLayout factory
    class CP_SDK_EXPORT DefaultFLayoutFactory : public FactoryInterfaces::IFLayoutFactory
    {
        public:
            /// @brief Create an CFLayout into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CFLayout* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories