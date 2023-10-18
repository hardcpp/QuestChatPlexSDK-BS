#pragma once

#include "../Logging/BMBFLogger.hpp"

#include <vector>
#include <stdint.h>
#include <array>
#include <string>
#include <algorithm>
#include <memory>

#include <beatsaber-hook/shared/utils/hooking.hpp>
#include <custom-types/shared/macros.hpp>
#include <UnityEngine/Object.hpp>

#define DISABLE_CONSTRUCTORS(x)     x() = delete;           \
                                    x(const x &) = delete;  \
                                    x(x &&) = delete;

#define DISABLE_COPY_MOVECONSTRUCTORS(x)    x(const x &) = delete;  \
                                            x(x &&) = delete;

#define PRIVATE_TAG()       struct __this_is_private { explicit __this_is_private(int) {} };
#define PRIVATE_TAG_ARG()   const __this_is_private & __pvTag
#define PRIVATE_TAG_VAL()   __this_is_private{0}
#define PRIVATE_TAG_FWD()   __pvTag

namespace CP_SDK::Utils {

    class Hooks
    {
        DISABLE_CONSTRUCTORS(Hooks);

        private:
            /// Registered hooks
            static std::vector<void (*)(Logger& logger)> m_InstalledFuncs;

        public:
            /// Register a hook
            /// @param p_Function Function to register
            static void Register(void (*p_Function)(Logger&));
            /// Install all hooks
            static void InstallHooks();
    };

}   ///< namespace CP_SDK::Utils

template <typename ... t_Args> constexpr std::size_t __CP_SDK_IL2CPP_MACRO_ARG_COUNT(t_Args&&...) { return sizeof...(t_Args); }

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "Internals/Il2cpp_enum.hpp"
#include "Internals/Il2cpp_string.hpp"

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define CP_SDK_IL2CPP_INHERIT(__Namespace, __Name, __BaseClass)                             \
    protected: __Name() = default;                                                          \
    public:    __Name(const __Name&) = delete;                                              \
    public:    __Name(__Name&&) = delete;                                                   \
    ___CP_SDK_IL2CPP_INHERIT(__Namespace, __Name, __BaseClass, {}, nullptr)

#define CP_SDK_IL2CPP_INHERIT_INTERFACES(__Namespace, __Name, __BaseClass, ...)             \
    protected: __Name() = default;                                                          \
    public:    __Name(const __Name&) = delete;                                              \
    public:    __Name(__Name&&) = delete;                                                   \
    ___CP_SDK_IL2CPP_INHERIT(__Namespace, __Name, __BaseClass, std::vector<Il2CppClass*>({ __VA_ARGS__ }), nullptr)

#define CP_SDK_IL2CPP_INHERIT_CUSTOM(__Namespace, __Name, __BaseClass)                      \
    protected: __Name() = default;                                                          \
    public:    __Name(const __Name&) = delete;                                              \
    public:    __Name(__Name&&) = delete;                                                   \
    ___CP_SDK_IL2CPP_INHERIT(__Namespace, __Name, __BaseClass, {}, __BaseClass::___TypeRegistration::get())

#define CP_SDK_IL2CPP_INHERIT_CUSTOM_INTERFACES(__Namespace, __Name, __BaseClass, ...)      \
    protected: __Name() = default;                                                          \
    public:    __Name(const __Name&) = delete;                                              \
    public:    __Name(__Name&&) = delete;                                                   \
    ___CP_SDK_IL2CPP_INHERIT(__Namespace, __Name, __BaseClass, std::vector<Il2CppClass*>({ __VA_ARGS__ }), __BaseClass::___TypeRegistration::get())

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "Internals/Il2cpp_ctor_dtor.hpp"

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Override a base class method with a custom BASECLASS__NAME method (PRESERVE CSHARP NAME)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD(__BaseClass, __Ret, __Name, ...) \
    	CP_SDK_IL2CPP_OVERRIDE_METHOD__(__BaseClass, __Ret, __Name, il2cpp_utils::il2cpp_type_check::MetadataGetter<&__BaseClass::__Name>::get(), __VA_ARGS__)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD__(__BaseClass, __Ret, __Name, __MethodInfo, ...)  \
    public:                                                                             \
        __Ret __BaseClass##__##__Name(__VA_ARGS__);                                     \
        ___CREATE_INSTANCE_METHOD(__BaseClass##__##__Name, #__Name, (__MethodInfo->flags & ~METHOD_ATTRIBUTE_ABSTRACT) | METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, __MethodInfo)

/// @brief Override a base class method with a custom BASECLASS__NAME method (PRESERVE CSHARP NAME)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD2(__Namespace, __BaseClass, __Ret, __Name, ...) \
    	CP_SDK_IL2CPP_OVERRIDE_METHOD__(__BaseClass, __Ret, __Name, il2cpp_utils::il2cpp_type_check::MetadataGetter<&__Namespace::__BaseClass::__Name>::get(), __VA_ARGS__)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD__(__BaseClass, __Ret, __Name, __MethodInfo, ...)  \
    public:                                                                             \
        __Ret __BaseClass##__##__Name(__VA_ARGS__);                                     \
        ___CREATE_INSTANCE_METHOD(__BaseClass##__##__Name, #__Name, (__MethodInfo->flags & ~METHOD_ATTRIBUTE_ABSTRACT) | METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, __MethodInfo)


/// @brief Override a base class method with a custom NAMESPACE__BASECLASS__NAME method (PRESERVE CSHARP NAME)
#define CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(__Namespace, __BaseClass, __Ret, __Name, ...) \
    CP_SDK_IL2CPP_OVERRIDE_METHOD_EX__(__Namespace, __BaseClass, __Ret, __Name, il2cpp_utils::il2cpp_type_check::MetadataGetter<&__Namespace::__BaseClass::__Name>::get(), __VA_ARGS__);
#define CP_SDK_IL2CPP_OVERRIDE_METHOD_EX__(__Namespace, __BaseClass, __Ret, __Name, __MethodInfo, ...)  \
    public:                                                                                             \
        __Ret __Namespace##__##__BaseClass##__##__Name(__VA_ARGS__);                                    \
        ___CREATE_INSTANCE_METHOD(__Namespace##__##__BaseClass##__##__Name, #__Name, (__MethodInfo->flags & ~METHOD_ATTRIBUTE_ABSTRACT) | METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, __MethodInfo)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Call parent class method implementation
#define CP_SDK_IL2CPP_CALL_BASE_METHOD(__Class, __Method, ...)                                                                                                      \
    {                                                                                                                                                               \
        static auto l_BaseMethodInfo = il2cpp_functions::class_get_method_from_name(classof(__Class*), __Method, __CP_SDK_IL2CPP_MACRO_ARG_COUNT(__VA_ARGS__));     \
        if (l_BaseMethodInfo) il2cpp_utils::RunMethod(this, l_BaseMethodInfo __VA_OPT__(,) __VA_ARGS__);                                                            \
        else CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_IL2CPP_CALL_BASE_METHOD] Method " + StringW(#__Method) + " not found in class " + StringW(#__Class));   \
    }

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////













#define ___CP_SDK_IL2CPP_INHERIT(namespaze_, name_, baseT, interfaces_, baseCustom) \
        using ___TargetType = name_; \
        constexpr static auto ___Base__Size = sizeof(baseT); \
        friend ::custom_types::Register; \
        public: \
        struct ___TypeRegistration : ::custom_types::TypeRegistration { \
            ___TypeRegistration() { \
                ::custom_types::Register::AddType(this); \
                instance = this; \
            } \
            static inline std::vector<::custom_types::FieldRegistrator*> fields; \
            std::vector<::custom_types::FieldRegistrator*> const getFields() const override { \
                return fields; \
            } \
            static void addField(::custom_types::FieldRegistrator* inst) { \
                fields.push_back(inst); \
                ::custom_types::_logger().debug("Adding instance field: %s.%s new size: %lu", #name_, inst->name(), fields.size()); \
            } \
            static inline std::vector<::custom_types::StaticFieldRegistrator*> staticFields; \
            std::vector<::custom_types::StaticFieldRegistrator*> const getStaticFields() const override { \
                return staticFields; \
            } \
            static void addStaticFieldInstance(::custom_types::StaticFieldRegistrator* inst) { \
                staticFields.push_back(inst); \
                ::custom_types::_logger().debug("Adding static field: %s.%s new size: %lu", #name_, inst->name(), staticFields.size()); \
            } \
            static inline std::vector<::custom_types::MethodRegistrator*> methods; \
            std::vector<::custom_types::MethodRegistrator*> const getMethods() const override { \
                return methods; \
            } \
            static void addMethod(::custom_types::MethodRegistrator* inst) { \
                methods.push_back(inst); \
                ::custom_types::_logger().debug("Adding method: %s.%s new size: %lu", #name_, inst->name(), methods.size()); \
            } \
            static inline size_t staticFieldOffset; \
            static size_t addStaticField(size_t sz) { \
                auto tmp = staticFieldOffset; \
                staticFieldOffset += sz; \
                return tmp; \
            } \
            static char* st_fields; \
            char*& static_fields() override { \
                return st_fields; \
            } \
            size_t static_fields_size() const override { \
                return staticFieldOffset; \
            } \
            constexpr const char* name() const override { \
                return #name_; \
            } \
            constexpr const char* namespaze() const override { \
                return namespaze_; \
            } \
            constexpr const char* dllName() const override { \
                return namespaze_; \
            } \
            Il2CppClass* baseType() const override { \
                return ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<baseT*>::get(); \
            } \
            std::vector<Il2CppClass*> const interfaces() const override { \
                return interfaces_; \
            } \
            constexpr Il2CppTypeEnum typeEnum() const override { \
                return Il2CppTypeEnum::IL2CPP_TYPE_CLASS; \
            } \
            constexpr uint32_t typeFlags() const override { \
                return 0; \
            } \
            static Il2CppClass* klass_ptr; \
            Il2CppClass*& klass() const override { \
                return klass_ptr; \
            } \
            size_t size() const override { \
                return sizeof(___TargetType); \
            } \
            TypeRegistration* customBase() const override { \
                return baseCustom; \
            } \
            bool initialized() const override { \
                return init; \
            } \
            void setInitialized() const override { \
                init = true; \
            } \
            static bool init; \
            static TypeRegistration* instance; \
            static TypeRegistration* get() { \
                return instance; \
            } \
        };

#define CP_SDK_IL2CPP_INHERIT_HELPERS(__Name)                                               \
    template<> struct ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<::__Name*> {   \
        static inline Il2CppClass* get() {                                                  \
            return ::__Name::___TypeRegistration::klass_ptr;                                \
        }                                                                                   \
    };                                                                                      \
    template<> struct ::il2cpp_utils::il2cpp_type_check::need_box<::__Name> {               \
        constexpr static bool value = false;                                                \
    };

#define CP_SDK_IL2CPP_INHERIT_INIT(__Name)                                                  \
    ::custom_types::TypeRegistration* __Name::___TypeRegistration::instance;                \
    static __Name::___TypeRegistration __registration_instance_##__Name;                    \
    char* __Name::___TypeRegistration::st_fields;                                           \
    Il2CppClass* __Name::___TypeRegistration::klass_ptr;                                    \
    bool __Name::___TypeRegistration::init = false;

#define CP_SDK_IL2CPP_HOOK_AUTO_INSTALL_ORIG(name_)                            \
    struct Auto_Hook_##name_                                                   \
    {                                                                          \
        Auto_Hook_##name_()                                                    \
        {                                                                      \
            ::CP_SDK::Utils::Hooks::Register(::Hooking::InstallOrigHook<Hook_##name_>); \
        }                                                                      \
    };                                                                         \
    static Auto_Hook_##name_ Auto_Hook_Instance_##name_;

#define CP_SDK_IL2CPP_HOOK_AUTO_INSTALL(name_)                             \
    struct Auto_Hook_##name_                                               \
    {                                                                      \
        Auto_Hook_##name_()                                                \
        {                                                                  \
            ::CP_SDK::Utils::Hooks::Register(::Hooking::InstallHook<Hook_##name_>); \
        }                                                                  \
    };                                                                     \
    static Auto_Hook_##name_ Auto_Hook_Instance_##name_;


#define CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(name_, mPtr, retval, ...)                                                                           \
    struct Hook_##name_                                                                                                                             \
    {                                                                                                                                               \
        using funcType = retval (*)(__VA_ARGS__);                                                                                                   \
        static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
        constexpr static const char* name() { return #name_; }                                                                                      \
        static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::get(); }                               \
        static funcType* trampoline() { return &name_; }                                                                                            \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                                                       \
        static funcType hook() { return hook_##name_; }                                                                                             \
        static retval hook_##name_(__VA_ARGS__);                                                                                                    \
    };                                                                                                                                              \
    CP_SDK_IL2CPP_HOOK_AUTO_INSTALL(name_)                                                                                                          \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)

#define CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_ORIG_MATCH(name_, mPtr, retval, ...)                                                                      \
    struct Hook_##name_                                                                                                                             \
    {                                                                                                                                               \
        using funcType = retval (*)(__VA_ARGS__);                                                                                                   \
        static_assert(std::is_same_v<funcType, ::Hooking::InternalMethodCheck<decltype(mPtr)>::funcType>, "Hook method signature does not match!"); \
        constexpr static const char* name() { return #name_; }                                                                                      \
        static const MethodInfo* getInfo() { return ::il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::get(); }                               \
        static funcType* trampoline() { return &name_; }                                                                                            \
        static inline retval (*name_)(__VA_ARGS__) = nullptr;                                                                                       \
        static funcType hook() { return hook_##name_; }                                                                                             \
        static retval hook_##name_(__VA_ARGS__);                                                                                                    \
    };                                                                                                                                              \
    CP_SDK_IL2CPP_HOOK_AUTO_INSTALL_ORIG(name_)                                                                                                     \
    retval Hook_##name_::hook_##name_(__VA_ARGS__)


