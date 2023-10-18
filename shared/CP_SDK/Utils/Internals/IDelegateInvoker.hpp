#pragma once

namespace CP_SDK::Utils::Internals {

    /// @brief Delegate invoker interface
    template<class t_Ret, class... t_Args>
    class IDelegateInvoker
    {
        public:
            /// @brief Destructor
            ~IDelegateInvoker()
            {

            }

        public:
            /// @brief Get delegate invoker kind
            int GetKind() const
            {
                return m_Kind;
            }

        public:
            /// @brief Is the delegate valid
            virtual bool IsValid() const = 0;
            /// @brief Is this invoker equal to an other invoker
            /// @param p_Other Element to compare to
            virtual bool EqualTo(const IDelegateInvoker<t_Ret, t_Args...>* p_Other) const = 0;
            /// @brief Invoke this invoker
            /// @param p_Args... Arguments
            virtual t_Ret Invoke(t_Args... p_Args) const = 0;

        protected:
            int m_Kind;     ///< Invoker kind

    };

}   ///< namespace CP_SDK::Utils::Internals
