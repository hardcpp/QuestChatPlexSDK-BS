#pragma once

#include "IXUIElement.hpp"

namespace CP_SDK::XUI {

    /// @brief IXUIElement Bind interface
    /// @tparam t_Base Return type for daisy chaining
    template<class t_Base>
    class IXUIBindable
    {
        CP_SDK_NO_COPYMOVE_CTORS(IXUIBindable);

        protected:
            /// @brief Constructor
            IXUIBindable() {}
            /// @brief Destructor
            virtual ~IXUIBindable() = default;

        public:
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual t_Base* Bind(std::shared_ptr<t_Base>* p_Target) = 0;

    };

}   ///< namespace CP_SDK::XUI