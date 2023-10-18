#pragma once

#include "../FactoryInterfaces/IImageFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CImage factory
    class DefaultImageFactory : public FactoryInterfaces::IImageFactory
    {
        public:
            /// @brief Create an CImage into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CImage* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories