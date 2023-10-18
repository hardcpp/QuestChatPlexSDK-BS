#pragma once

#include "Internals/DelegateInvokerI.hpp"
#include "Internals/DelegateInvokerL.hpp"
#include "Internals/DelegateInvokerS.hpp"
#include "Internals/LambdaSignatureHelper.hpp"

namespace CP_SDK::Utils {

    /// @brief Forward declaration
    template <class t_FnType> class DelegateImpl;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Delegate class
    template<class t_Ret, class... t_Args>
    class DelegateImpl<t_Ret(t_Args...)>
    {
        public:
            /// @brief Constructor
            DelegateImpl()
            {
                m_Invoker = nullptr;
            }
            /// @brief Static function constructor
            /// @param p_Handler Handler instance
            DelegateImpl(t_Ret(*p_Handler)(t_Args...))
            {
                m_Invoker = std::make_shared<Internals::DelegateInvokerS<t_Ret, t_Args...>>(p_Handler);
            }
            /// @brief Instance function constructor
            /// @param p_Owner   Owner instance
            /// @param p_Handler Handler
            template<class t_Class> DelegateImpl(t_Class * p_Owner, t_Ret(t_Class::* p_Handler)(t_Args...))
            {
                m_Invoker = std::make_shared<Internals::DelegateInvokerI<t_Class, t_Ret, t_Args...>>(p_Owner, p_Handler);
            }
            /// @brief Lambda function constructor
            /// @param p_Null   Placeholder template helper
            /// @param p_Lambda Lambda function
            template<class t_Lambda> DelegateImpl(t_Ret(*p_Null)(t_Args...), const t_Lambda & p_Lambda)
            {
                m_Invoker = std::make_shared<Internals::DelegateInvokerL<t_Lambda, t_Ret, t_Args...>>(p_Lambda);
            }
            /// @brief Copy constructor
            /// @param p_Other Instance to copy
            DelegateImpl(const DelegateImpl<t_Ret(t_Args...)> & p_Other)
            {
                m_Invoker = p_Other.m_Invoker;
            }
            /// @brief Move constructor
            /// @param p_Other Instance to move
            DelegateImpl(DelegateImpl<t_Ret(t_Args...)> && p_Other)
            {
                m_Invoker = p_Other.m_Invoker;
                p_Other.m_Invoker = nullptr;
            }
            /// @brief Destructor
            virtual ~DelegateImpl()
            {
                m_Invoker = nullptr;
            }

        public:
            /// @brief Assign operator
            /// @param p_Other Source
            DelegateImpl& operator=(const DelegateImpl& p_Other)
            {
                m_Invoker = p_Other.m_Invoker;
                return *this;
            }

        public:
            /// @brief Is the delegate valid
            bool IsValid() const
            {
                return m_Invoker ? m_Invoker->IsValid() : false;
            }

        public:
            /// @brief Is this delegate equal to an other delegate
            /// @param p_Other Element to compare to
            bool EqualTo(const DelegateImpl<t_Ret(t_Args...)>* p_Other) const
            {
                return m_Invoker && p_Other && m_Invoker->EqualTo(p_Other->m_Invoker.get());
            }
            /// @brief Invoke this delegate
            /// @param p_Args... Arguments
            t_Ret operator()(t_Args... p_Args) const
            {
                return Invoke(std::forward<t_Args>(p_Args)...);
            }
            /// @brief Invoke this delegate
            /// @param p_Args... Arguments
            t_Ret Invoke(t_Args... p_Args) const
            {
                if (!IsValid())
                {
                    if constexpr (std::is_reference<t_Ret>::value)
                    {
                        static typename std::remove_reference<t_Ret>::type s_Defaultt_Ret;
                        return s_Defaultt_Ret;
                    }
                    else
                        return t_Ret();
                }

                return m_Invoker->Invoke(std::forward<t_Args>(p_Args)...);
            }

        private:
            std::shared_ptr<Internals::IDelegateInvoker<t_Ret, t_Args...>> m_Invoker;

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Delegate helper class
    template <class t_FnType>
    class Delegate : public DelegateImpl<t_FnType>
    {
        using DelegateImpl<t_FnType>::DelegateImpl;

        public:
            /// @brief Make delegate
            /// @param p_Lambda Lambda function
            template<class t_Lambda, class t_Sig = typename Internals::LambdaSignatureHelper<decltype(&t_Lambda::operator())>::Signature>
            Delegate(const t_Lambda & p_Lambda)
                : DelegateImpl<t_Sig>((t_Sig*)nullptr, p_Lambda)
            {

            }
            /// @brief Destructor
            virtual ~Delegate()
            {

            }

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Action type
    template <class... t_Args>
    using Action = Delegate<void(t_Args...)>;

    template <class... t_Args>
    using CActionRef = const Delegate<void(t_Args...)>&;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Func type
    template <class t_Ret, class... t_Args>
    using Func = Delegate<t_Ret(t_Args...)>;

    /// @brief Func type
    template <class t_Ret, class... t_Args>
    using CFuncRef = const Delegate<t_Ret(t_Args...)>&;

}   ///< namespace CP_SDK::Utils
