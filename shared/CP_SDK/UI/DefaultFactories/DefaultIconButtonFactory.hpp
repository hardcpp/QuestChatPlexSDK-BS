#pragma once

#include "../FactoryInterfaces/IIconButtonFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CIconButton factory
    class CP_SDK_EXPORT DefaultIconButtonFactory : public FactoryInterfaces::IIconButtonFactory
    {
        public:
            /// @brief Create an CIconButton into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CIconButton* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories