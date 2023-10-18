#pragma once

#include "IDelegateInvoker.hpp"

#include <algorithm>

namespace CP_SDK::Utils::Internals {

    /// @brief Instance function invoker
    template<class t_Class, class t_Ret, class... t_Args>
    class DelegateInvokerI : public IDelegateInvoker<t_Ret, t_Args...>
    {
        public:
            /// @brief Constructor
            /// @param p_Owner   Owner instance
            /// @param p_Handler Handler
            DelegateInvokerI(t_Class* p_Owner, t_Ret(t_Class::* p_Handler)(t_Args...))
            {
                this->m_Kind = 2;

                m_Owner     = p_Owner;
                m_Handler   = p_Handler;
            }

        public:
            /// @brief Is the delegate valid
            bool IsValid() const override
            {
                return m_Handler != nullptr;
            }
            /// @brief Is this invoker equal to an other invoker
            /// @param p_Other Element to compare to
            bool EqualTo(const IDelegateInvoker<t_Ret, t_Args...>* p_Other) const override
            {
                return p_Other
                    && this->m_Kind == p_Other->GetKind()
                    && m_Owner      == reinterpret_cast<const DelegateInvokerI<t_Class, t_Ret, t_Args...>*>(p_Other)->m_Owner
                    && m_Handler    == reinterpret_cast<const DelegateInvokerI<t_Class, t_Ret, t_Args...>*>(p_Other)->m_Handler;
            }
            /// @brief Invoke this invoker
            /// @param p_Args... Arguments
            t_Ret Invoke(t_Args... p_Args) const override
            {
                return (m_Owner->*m_Handler)(std::forward<t_Args>(p_Args)...);
            }

        private:
            t_Class* m_Owner;
            t_Ret(t_Class::* m_Handler)(t_Args...);

    };

}   ///< namespace CP_SDK::Utils::Internals