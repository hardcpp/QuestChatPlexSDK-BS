#pragma once

#include "IDelegateInvoker.hpp"

#include <algorithm>

namespace CP_SDK::Utils::Internals {

    /// @brief Lambda function invoker
    template<class t_Lambda, class t_Ret, class... t_Args>
    class DelegateInvokerL : public IDelegateInvoker<t_Ret, t_Args...>
    {
        public:
            /// @brief Constructor
            /// @param p_Lambda Lambda instance
            DelegateInvokerL(const t_Lambda & p_Lambda)
                : m_Lambda(p_Lambda)
            {
                this->m_Kind = 3;
            }

        public:
            /// @brief Is the delegate valid
            bool IsValid() const override
            {
                return true;
            }
            /// @brief Is this invoker equal to an other invoker
            /// @param p_Other Element to compare to
            bool EqualTo(const IDelegateInvoker<t_Ret, t_Args...>* p_Other) const override
            {
                return false;
            }
            /// @brief Invoke this invoker
            /// @param p_Args... Arguments
            t_Ret Invoke(t_Args... p_Args) const override
            {
                return m_Lambda(std::forward<t_Args>(p_Args)...);
            }

        private:
            t_Lambda m_Lambda;

    };

}   ///< namespace CP_SDK::Utils::Internals