#pragma once

#include "MTObjectPool.hpp"

namespace CP_SDK::Pool {

    /// @brief Provides a static implementation of Pool.ObjectPool_1.
    /// @tparam t_Type Pooled object type
    template<class t_Type>
    class MTGenericPool
    {
        CP_SDK_NO_DEF_CTORS(MTGenericPool);

        private:
            static typename MTObjectPool<t_Type>::Ptr s_Pool;

        public:
            /// @brief Simple get
            static t_Type Get()
            {
                return s_Pool->Get();
            }
            /// @brief Release an element
            /// @param p_Element Element to release
            static void Release(t_Type& p_Element)
            {
                s_Pool->Release(p_Element);
            }

    };

    template <typename t_Type>
    typename MTObjectPool<t_Type>::Ptr MTGenericPool<t_Type>::s_Pool = MTObjectPool<t_Type>::Make(
        []() {
            if constexpr (std::is_pointer_v<t_Type>)
            {
                using t_ExtractedType = typename std::remove_pointer<t_Type>::type;
                return new t_ExtractedType();
            }
            else
                return t_Type();
        },
        nullptr,
        nullptr,
        [](t_Type& x) {
            if constexpr (std::is_pointer_v<t_Type>)
                delete x;
        }
    );

}   ///< namespace CP_SDK::Pool