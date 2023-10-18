#pragma once

#include "../Utils/Il2cpp.hpp"

namespace CP_SDK::Pool {

    /// @brief Object pool interface
    /// @tparam t_Type Pooled object type
    template<class t_Type>
    class IObjectPool
    {
        DISABLE_CONSTRUCTORS(IObjectPool);

        protected:
            PRIVATE_TAG();

            /// @brief Constructor
            IObjectPool(PRIVATE_TAG_ARG()) {}
            /// @brief Destructor
            virtual ~IObjectPool() {}

        public:
            /// @brief Released element
            virtual int CountInactive() = 0;

        public:
            /// @brief Simple get
            virtual t_Type Get() = 0;
            /// @brief Release an element
            /// @param p_Element Element to release
            virtual void Release(t_Type& p_Element) = 0;

        public:
            /// @brief Clear the object pool
            virtual void Clear() = 0;

    };

}   ///< namespace CP_SDK::Pool