#pragma once

#define __CP_SDK_U16STR(__mX) u##__mX
#define CP_SDK_U16STR(__mX) __CP_SDK_U16STR(#__mX)

#include "../Logging/PaperLogger.hpp"
#include "Internals/Il2cpp_enum.hpp"
#include "Internals/il2cpp_customtype.hpp"
#include "Internals/il2cpp_hook.hpp"
#include "Internals/Il2cpp_string.hpp"

#include <vector>
#include <stdint.h>
#include <array>
#include <string>
#include <algorithm>
#include <memory>

#include <beatsaber-hook/shared/utils/hooking.hpp>
#include <custom-types/shared/macros.hpp>
#include <UnityEngine/Object.hpp>

#define CP_SDK_EXPORT_VISIBILITY CUSTOM_TYPES_EXPORT_VISIBILITY

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Remove default, copy, move constructors
#define CP_SDK_NO_DEF_CTORS(__mName)    \
    __mName() = delete;                 \
    __mName(const __mName&) = delete;   \
    __mName(__mName&&) = delete;

/// @brief Remove copy, move constructors
#define CP_SDK_NO_COPYMOVE_CTORS(__mName)   \
    __mName(const __mName&) = delete;       \
    __mName(__mName&&) = delete;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

namespace CP_SDK::Utils {

    /// @brief Hook manager
    class CP_SDK_EXPORT_VISIBILITY Hooks
    {
        CP_SDK_NO_DEF_CTORS(Hooks);

        private:
            /// @brief Registered hooks
            static std::vector<void (*)(Paper::LoggerContext& logger)> m_InstalledFuncs;

        public:
            /// @brief Register a hook
            /// @param p_Function Function to register
            static void Register(void (*p_Function)(Paper::LoggerContext&));
            /// @brief Install all hooks
            static void InstallHooks();

    };

}   ///< namespace CP_SDK::Utils

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Private tag declaration to forbid a public constructor
#define CP_SDK_PRIV_TAG()       struct __this_is_private { explicit __this_is_private(int) {} };
/// @brief Private tag argument declaration
#define CP_SDK_PRIV_TAG_ARG()   const __this_is_private & __pvTag
/// @brief Private tag argument value
#define CP_SDK_PRIV_TAG_VAL()   __this_is_private{0}
/// @brief Private tag argument value forward
#define CP_SDK_PRIV_TAG_FWD()   __pvTag

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Extract classes of raw il2cpp types into a vector
#define CP_SDK_IL2CPP_INTERFACES(...) \
     CP_SDK::Utils::Internals::ExtractInterfacesClasses<__VA_ARGS__>()

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Make a custom type that inherit a il2cpp type like Il2cppObject or any other type with optionial interfaces
///        It will also make a default c++ constructor and delete copy & move constructors
#define CP_SDK_IL2CPP_INHERIT(__mNamespace, __mName, __mBaseClass, ...) \
      __CP_SDK_IL2CPP_INHERIT(__mNamespace, __mName, __mBaseClass __VA_OPT__(,) __VA_ARGS__)

/// @brief Template specializations for custom types
#define CP_SDK_IL2CPP_INHERIT_HELPERS(__mName) \
      __CP_SDK_IL2CPP_INHERIT_HELPERS(__mName)

/// @brief Static fields initialisation for the custom type
#define CP_SDK_IL2CPP_INHERIT_INIT(__mName) \
      __CP_SDK_IL2CPP_INHERIT_INIT(__mName)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Prevent CTOR field declaration
#define CP_SDK_IL2CPP_CTOR_FIELD() \
      __CP_SDK_IL2CPP_CTOR_FIELD()

/// @brief Declare a il2cpp constructor that will call the c++ constructors and all parent il2cpp constructors implementations
#define CP_SDK_IL2CPP_DECLARE_CTOR(__mType) \
      __CP_SDK_IL2CPP_DECLARE_CTOR(__mType)

/// @brief Declare a il2cpp constructor that will call the c++ constructors and all parent il2cpp constructors implementations
#define CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(__mType) \
      __CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(__mType)

/// @brief Implement a il2cpp constructor that will call the c++ constructors and all parent il2cpp constructors implementations
#define CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(__mType) \
      __CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(__mType)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Prevent DTOR field declaration
#define CP_SDK_IL2CPP_DTOR_FIELD() \
      __CP_SDK_IL2CPP_DTOR_FIELD()

/// @brief Declare a destructor based on MonoBehavior::OnDestroy that will call the c++ destructor and all parent MonoBehavior::OnDestroy implementations
#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(__mType) \
      __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(__mType)

/// @brief Declare a destructor based on MonoBehavior::OnDestroy that will call the c++ destructor and all parent MonoBehavior::OnDestroy implementations
#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(__mType) \
      __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(__mType)

/// @brief Implement a destructor based on MonoBehavior::OnDestroy that will call the c++ destructor and all parent MonoBehavior::OnDestroy implementations
#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(__mType) \
      __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(__mType)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Override a base class method with a custom BASECLASS__NAME method (PRESERVE CSHARP NAME)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD(__mBaseClass, __mRet, __mMethodName, ...) \
      __CP_SDK_IL2CPP_OVERRIDE_METHOD(__mBaseClass, __mRet, __mMethodName, il2cpp_utils::il2cpp_type_check::MetadataGetter<&__mBaseClass::__mMethodName>::methodInfo() __VA_OPT__(,) __VA_ARGS__)

/// @brief Override a base class method with a custom BASECLASS__NAME method inside a namespace (PRESERVE CSHARP NAME)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(__mNamespace, __mBaseClass, __mRet, __mMethodName, ...) \
      __CP_SDK_IL2CPP_OVERRIDE_METHOD(__mBaseClass, __mRet, __mMethodName, il2cpp_utils::il2cpp_type_check::MetadataGetter<&__mNamespace::__mBaseClass::__mMethodName>::methodInfo() __VA_OPT__(,) __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Call parent class method implementation
#define CP_SDK_IL2CPP_CALL_BASE_METHOD(__mBaseClass, __mMethodName, ...) \
      __CP_SDK_IL2CPP_CALL_BASE_METHOD(true, classof(__mBaseClass*), CP_SDK_U16STR(__mBaseClass), __mMethodName __VA_OPT__(,) __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Hook a il2cpp to the origin
#define CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(__mName, __mPtr, __mRetval, ...) \
      __CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(__mName, __mPtr, __mRetval __VA_OPT__(,) __VA_ARGS__)

/// @brief Hook a il2cpp method closest to the origin
#define CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_ORIG_MATCH(__mName, __mPtr, __mRetval, ...) \
      __CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_ORIG_MATCH(__mName, __mPtr, __mRetval __VA_OPT__(,) __VA_ARGS__)
