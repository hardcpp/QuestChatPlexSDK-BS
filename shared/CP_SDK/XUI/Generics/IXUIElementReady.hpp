#pragma once

#include "IXUIElement.hpp"
#include "../../Utils/Event.hpp"

#include <UnityEngine/MonoBehaviour.hpp>

namespace CP_SDK::XUI {

    /// @brief IXUIElement OnReady functor interface
    /// @tparam t_Base Return type for daisy chaining
    /// @tparam t_Component Element type
    template<class t_Base, class t_Component>
    class IXUIElementReady
    {
        CP_SDK_NO_COPYMOVE_CTORS(IXUIElementReady);

        protected:
            /// @brief Constructor
            IXUIElementReady() {}
            /// @brief Destructor
            virtual ~IXUIElementReady() = default;

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual t_Base* OnReady(_v::CActionRef<t_Component*> p_Functor) = 0;

    };

}   ///< namespace CP_SDK::XUI