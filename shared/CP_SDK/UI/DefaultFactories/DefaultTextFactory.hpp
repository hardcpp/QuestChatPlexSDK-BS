#pragma once

#include "../FactoryInterfaces/ITextFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CText factory
    class CP_SDK_EXPORT DefaultTextFactory : public FactoryInterfaces::ITextFactory
    {
        public:
            /// @brief Create an CText into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CText* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories