#pragma once

#define __CP_SDK_IL2CPP_HOOK_AUTO_INSTALL_ORIG(__mName)                                     \
    struct Auto_Hook_##__mName                                                              \
    {                                                                                       \
        Auto_Hook_##__mName()                                                               \
        {                                                                                   \
            ::CP_SDK::Utils::Hooks::Register(::Hooking::InstallOrigHook<Hook_##__mName>);   \
        }                                                                                   \
    };                                                                                      \
    static Auto_Hook_##__mName Auto_Hook_Instance_##__mName;

#define __CP_SDK_IL2CPP_HOOK_AUTO_INSTALL(__mName)                                          \
    struct Auto_Hook_##__mName                                                              \
    {                                                                                       \
        Auto_Hook_##__mName()                                                               \
        {                                                                                   \
            ::CP_SDK::Utils::Hooks::Register(::Hooking::InstallHook<Hook_##__mName>);       \
        }                                                                                   \
    };                                                                                      \
    static Auto_Hook_##__mName Auto_Hook_Instance_##__mName;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define __CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(__mName, __mPtr, __mRetval, ...)                                                                      \
    struct Hook_##__mName                                                                                                                               \
    {                                                                                                                                                   \
        using funcType = __mRetval (*)(__VA_ARGS__);                                                                                                    \
        static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(__mPtr)>::funcType>, "Hook method signature does not match!");   \
        constexpr static const char* name() { return #__mName; }                                                                                        \
        static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<__mPtr>::get(); }                                 \
        static funcType* trampoline() { return &__mName; }                                                                                              \
        static inline __mRetval (*__mName)(__VA_ARGS__) = nullptr;                                                                                      \
        static funcType hook() { return hook_##__mName; }                                                                                               \
        static __mRetval hook_##__mName(__VA_ARGS__);                                                                                                   \
    };                                                                                                                                                  \
    __CP_SDK_IL2CPP_HOOK_AUTO_INSTALL(__mName);                                                                                                         \
    __mRetval Hook_##__mName::hook_##__mName(__VA_ARGS__)

#define __CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_ORIG_MATCH(__mName, __mPtr, __mRetval, ...)                                                                 \
    struct Hook_##__mName                                                                                                                               \
    {                                                                                                                                                   \
        using funcType = __mRetval (*)(__VA_ARGS__);                                                                                                    \
        static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(__mPtr)>::funcType>, "Hook method signature does not match!");   \
        constexpr static const char* name() { return #__mName; }                                                                                        \
        static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<__mPtr>::get(); }                                 \
        static funcType* trampoline() { return &__mName; }                                                                                              \
        static inline __mRetval (*__mName)(__VA_ARGS__) = nullptr;                                                                                      \
        static funcType hook() { return hook_##__mName; }                                                                                               \
        static __mRetval hook_##__mName(__VA_ARGS__);                                                                                                   \
    };                                                                                                                                                  \
    __CP_SDK_IL2CPP_HOOK_AUTO_INSTALL_ORIG(__mName);                                                                                                    \
    __mRetval Hook_##__mName::hook_##__mName(__VA_ARGS__)