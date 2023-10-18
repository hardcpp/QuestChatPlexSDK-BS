#pragma once

#include "IDelegateInvoker.hpp"

#include <algorithm>

namespace CP_SDK::Utils::Internals {

    /// @brief Static function invoker
    template<class t_Ret, class... t_Args>
    class DelegateInvokerS : public IDelegateInvoker<t_Ret, t_Args...>
    {
        public:
            /// Constructor
            /// @p_Handler: Handler instance
            DelegateInvokerS(t_Ret(*p_Handler)(t_Args...))
            {
                this->m_Kind = 1;

                m_Handler = p_Handler;
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
                    && m_Handler    == reinterpret_cast<const DelegateInvokerS<t_Ret, t_Args...>*>(p_Other)->m_Handler;
            }
            /// @brief Invoke this invoker
            /// @param p_Args... Arguments
            t_Ret Invoke(t_Args... p_Args) const override
            {
                return m_Handler(std::forward<t_Args>(p_Args)...);
            }

        private:
            t_Ret(*m_Handler)(t_Args...);

    };

}   ///< namespace CP_SDK::Utils::Internals