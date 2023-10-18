#pragma once

namespace CP_SDK::Utils::Internals {

    /// @brief Base structure
    template<typename> struct LambdaSignatureHelper;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Lambdas
    template<typename t_Ret, typename t_Wrapper, typename... t_Args>
    struct LambdaSignatureHelper<t_Ret(t_Wrapper::*)(t_Args...) const>
    {
        using Signature = t_Ret(t_Args...);
    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Mutable lambdas
    template<typename t_Ret, typename t_Wrapper, typename... t_Args>
    struct LambdaSignatureHelper<t_Ret(t_Wrapper::*)(t_Args...)>
    {
        using Signature = t_Ret(t_Args...);
    };

}   ///< namespace CP_SDK::Utils::Internals
