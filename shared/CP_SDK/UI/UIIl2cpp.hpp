#pragma once

#include "../Utils/Delegate.hpp"
#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <custom-types/shared/delegate.hpp>
#include <UnityEngine/Object.hpp>
#include <UnityEngine/Events/UnityAction.hpp>
#include <UnityEngine/Events/UnityAction_1.hpp>
#include <UnityEngine/Events/UnityAction_2.hpp>
#include <UnityEngine/Events/UnityAction_3.hpp>
#include <UnityEngine/Events/UnityAction_4.hpp>
#include <UnityEngine/UI/Button.hpp>

namespace CP_SDK::UI {

    /// @brief UIFieldRef type helper
    template<class t_Type>
    using UIFieldRef = Utils::MonoPtr<t_Type>&;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief UIFieldRefDel alias helper
    template<class t_Type>
    using UIFieldRefDel = Utils::Func<UIFieldRef<t_Type>>;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    template<class t_Type>
    struct UIFieldDefault
    {
        static Utils::MonoPtr<t_Type> Value;
    };
    template<class t_Type> Utils::MonoPtr<t_Type> UIFieldDefault<t_Type>::Value;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    template<class t_Type> struct UIFieldRefDelExtractor;
    template<class t_Type> struct UIFieldRefDelExtractor<Utils::Func<Utils::MonoPtr<t_Type>&>>
    {
        using t_PtrType = UIFieldRef<t_Type>;
        using t_DelType = UIFieldRefDel<t_Type>;
    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    #define CP_SDK_UI_IL2CPP_BIND_FIELD(__Field, __Variable) __Field = [this]() -> CP_SDK::UI::UIFieldRefDelExtractor<typeof(__Field)>::t_PtrType { return __Variable; };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief a templated method to put the MakeDelegate call into a function so that when we change where the MakeDelegate call is, we just change this method
    /// @tparam t_Type type of the delegate
    /// @tparam t_DelegateType whatever you want to pass into MakeDelegate
    /// @param p_Delegate whatever you want to pass into MakeDelegate
    /// @return t_Type delegate
    template <typename t_Type, typename t_DelegateType>
    inline t_Type CustomTypesMakeDelegate(t_DelegateType p_Delegate)
    {
        return custom_types::MakeDelegate<t_Type>(p_Delegate);
    }

    /// @brief templated method to turn an std::function into an action
    /// @tparam t_Args the types of the arguments of the function
    /// @param p_Delegate the function
    /// @return UnityEngine::Events::UnityAction_N<t_Args>
    template<typename... t_Args>
    auto MakeUnityAction(std::function<void(t_Args...)> p_Delegate)
    {
        constexpr int l_ArgCount = sizeof...(t_Args);

             if constexpr (l_ArgCount == 0) return CustomTypesMakeDelegate<UnityEngine::Events::UnityAction*>(p_Delegate);
        else if constexpr (l_ArgCount == 1) return CustomTypesMakeDelegate<UnityEngine::Events::UnityAction_1<t_Args...>*>(p_Delegate);
        else if constexpr (l_ArgCount == 2) return CustomTypesMakeDelegate<UnityEngine::Events::UnityAction_2<t_Args...>*>(p_Delegate);
        else if constexpr (l_ArgCount == 3) return CustomTypesMakeDelegate<UnityEngine::Events::UnityAction_3<t_Args...>*>(p_Delegate);
        else if constexpr (l_ArgCount == 4) return CustomTypesMakeDelegate<UnityEngine::Events::UnityAction_4<t_Args...>*>(p_Delegate);

        /// > 4 is not allowed, UnityEngine::Events::UnityAction_N does not exist with N > 4
        static_assert(l_ArgCount <= 4, "argument count was greater than supported");
    }
    /// @brief templated method to turn lambdas into actions
    /// @tparam t_Type the type of the lambda
    /// @tparam t_Args the types of the arguments of the lambda
    /// @param p_Delegate the lambda
    /// @return UnityEngine::Events::UnityAction_N<t_Args>
    template<typename t_Type, typename...t_Args>
    requires(std::is_constructible_v<std::function<void(t_Args...)>, t_Type> && !std::is_same_v<std::function<void(t_Args...)>, t_Type>)
    auto MakeUnityAction(t_Type p_Delegate)
    {
        return MakeUnityAction<t_Args...>(std::function<void(t_Args...)>(p_Delegate));
    }
    /// @brief templated method to turn an instance + methodInfo into an action
    /// @tparam t_Args the types of the arguments of the lambda
    /// @param p_Instance the object instance
    /// @param p_MethodInfo the methodInfo of the method to run on instance
    /// @return UnityEngine::Events::UnityAction_N<t_Args>
    template<typename...t_Args>
    auto MakeUnityAction(Il2CppObject* p_Instance, const MethodInfo* p_MethodInfo)
    {
        if (p_MethodInfo->parameters_count != sizeof...(t_Args))
            throw std::runtime_error("Argcount mismatch between p_MethodInfo and t_Args");

        return MakeUnityAction<t_Args...>(std::function<void(t_Args...)>([p_Instance, p_MethodInfo](t_Args... p_Args){
            il2cpp_utils::RunMethod(p_Instance, p_MethodInfo, p_Args...);
        }));
    }

}   ///< namespace CP_SDK::UI