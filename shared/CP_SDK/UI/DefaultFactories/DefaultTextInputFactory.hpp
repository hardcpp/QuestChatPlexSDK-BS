#pragma once

#include "../FactoryInterfaces/ITextInputFactory.hpp"

namespace CP_SDK::UI::DefaultFactories {

    namespace _u
    {
        using namespace UnityEngine;
    }

    /// @brief Default CTextInput factory
    class CP_SDK_EXPORT DefaultTextInputFactory : public FactoryInterfaces::ITextInputFactory
    {
        public:
            /// @brief Create an CTextInput into the parent
            /// @param p_Name   Name
            /// @param p_Parent Parent transform
            virtual Components::CTextInput* Create(std::u16string_view p_Name, _u::Transform* p_Parent) override final;

    };

}   ///< namespace CP_SDK::UI::DefaultFactories