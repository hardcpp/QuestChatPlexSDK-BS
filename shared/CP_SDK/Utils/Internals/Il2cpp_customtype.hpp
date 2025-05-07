#pragma once

#include <type_traits>

#include <custom-types/shared/macros.hpp>

namespace CP_SDK::Utils::Internals {

    template <typename ... t_Args>
    constexpr std::size_t ArgCount(t_Args&&...)
    {
        return sizeof...(t_Args);
    }

    template<class... t_Args>
    std::vector<Il2CppClass*> ExtractInterfacesClasses()
    {
        return { classof(t_Args*)... };
    }

    template <typename t_Type>
    concept Concept_IsCustomType = requires(t_Type)
    {
        { t_Type::___TypeRegistration::get() } -> std::same_as<::custom_types::TypeRegistration*>;
    };

    template<typename t_Type>
    constexpr ::custom_types::TypeRegistration* GetCustomBaseIf()
    {
        if constexpr (Concept_IsCustomType<t_Type>)
            return t_Type::___TypeRegistration::get();

        return nullptr;
    }

}   ///< namespace CP_SDK::Utils::Internals

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define __CP_SDK_IL2CPP_INHERIT(__mNamespace, __mName, __mBaseClass, ...)                      \
    protected: __mName() = default;                                                            \
    public:    __mName(const __mName&) = delete;                                               \
    public:    __mName(__mName&&) = delete;                                                    \
    __CP_SDK_IL2CPP_CUSTOM_TYPE_DECLARE(__mNamespace, __mName, __mBaseClass, std::vector<Il2CppClass*>({ __VA_ARGS__ }))

#define __CP_SDK_IL2CPP_INHERIT_HELPERS(__mName)                                            \
    MARK_REF_PTR_T(__mName);                                                                \
    inline constexpr const int __mName::__IL2CPP_REFERENCE_TYPE_SIZE = sizeof(__mName);     \
    template<> struct ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<::__mName*> {  \
        static inline Il2CppClass* get() {                                                  \
            return ::__mName::___TypeRegistration::klass_ptr;                               \
        }                                                                                   \
    };                                                                                      \
    template<> struct ::il2cpp_utils::il2cpp_type_check::need_box<::__mName> {              \
        constexpr static bool value = false;                                                \
    };

#define __CP_SDK_IL2CPP_INHERIT_INIT(__mName)                                               \
    ::custom_types::TypeRegistration* __mName::___TypeRegistration::instance;               \
    static __mName::___TypeRegistration __registration_instance_##__mName;                  \
    char* __mName::___TypeRegistration::st_fields;                                          \
    Il2CppClass* __mName::___TypeRegistration::klass_ptr;                                   \
    bool __mName::___TypeRegistration::init = false;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define __CP_SDK_IL2CPP_CTOR_FIELD() \
    protected: bool __Prevent_CPP_CTOR = false

#define __CP_SDK_IL2CPP_CALL_CPP_CTOR(__mType, ...) \
    INVOKE_CTOR(__VA_ARGS__);

#define __CP_SDK_IL2CPP_DECLARE_CTOR(__mType)       \
    __CP_SDK_IL2CPP_CTOR_FIELD();                   \
    __CP_SDK_IL2CPP_DECLARE_CTOR_BASE(__mType)

#define __CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(__mType) \
    __CP_SDK_IL2CPP_DECLARE_CTOR_BASE(__mType)

#define __CP_SDK_IL2CPP_DECLARE_CTOR_BASE(__mType)                                                                                                                                                          \
    public:                                                                                                                                                                                                 \
        void __mType##__Constructor();                                                                                                                                                                      \
        template<::il2cpp_utils::CreationType creationType = ::il2cpp_utils::CreationType::Temporary, class... TArgs>                                                                                       \
        static ___TargetType* New_ctor(TArgs&&... args) {                                                                                                                                                   \
            static_assert(::custom_types::Decomposer<decltype(&___TargetType::__mType##__Constructor)>::convertible<TArgs...>(), "Arguments provided to New_ctor must be convertible to the constructor!"); \
            ___TargetType* obj;                                                                                                                                                                             \
            if constexpr (creationType == ::il2cpp_utils::CreationType::Temporary) {                                                                                                                        \
                obj = static_cast<___TargetType*>(::il2cpp_functions::object_new(___TypeRegistration::klass_ptr));                                                                                          \
            } else {                                                                                                                                                                                        \
                obj = static_cast<___TargetType*>(::il2cpp_utils::createManual(___TypeRegistration::klass_ptr));                                                                                            \
            }                                                                                                                                                                                               \
            obj->__mType##__Constructor(std::forward<TArgs>(args)...);                                                                                                                                      \
            return obj;                                                                                                                                                                                     \
        }                                                                                                                                                                                                   \
        ___CREATE_INSTANCE_METHOD(__mType##__Constructor, ".ctor", METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG | METHOD_ATTRIBUTE_SPECIAL_NAME | METHOD_ATTRIBUTE_RT_SPECIAL_NAME, nullptr);     \
        void __mType##__Constructor_Impl()


#define __CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(__mType)                                                                      \
        void __mType::__mType##__Constructor()                                                                          \
        {                                                                                                               \
            /*ChatPlexSDK::Logger()->Error(u"" + StringW(#__mType) + u"::Constructor");*/                               \
            auto __Can_CPP_CTOR = !__Prevent_CPP_CTOR;                                                                  \
            __Prevent_CPP_CTOR = true;                                                                                  \
            if (__Can_CPP_CTOR) __CP_SDK_IL2CPP_CALL_CPP_CTOR(__mType);                                                 \
            if (___TargetType::___TypeRegistration::get()->customBase())                                                \
                ::custom_types::InvokeBaseCtor(___TargetType::___TypeRegistration::get()->customBase()->klass(), this); \
            else                                                                                                        \
                ::custom_types::InvokeBaseCtor(___TargetType::___TypeRegistration::get()->baseType(), this);            \
            __Prevent_CPP_DTOR = false;                                                                                 \
            __mType##__Constructor_Impl();                                                                              \
        }                                                                                                               \
        void __mType::__mType##__Constructor_Impl()

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define __CP_SDK_IL2CPP_DTOR_FIELD() \
    protected: bool __Prevent_CPP_DTOR = false

#define __CP_SDK_IL2CPP_CALL_CPP_DTOR(__mType) \
    this->~__mType()

#define __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(__mType)     \
    __CP_SDK_IL2CPP_DTOR_FIELD();                               \
    __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_BASE(__mType)

#define __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(__mType) \
    __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_BASE(__mType)

#define __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_BASE(__mType)                                                                        \
    public:                                                                                                                             \
        void __mType##__Destructor();                                                                                                   \
        ___CREATE_INSTANCE_METHOD(__mType##__Destructor, "OnDestroy", METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, nullptr); \
        void __mType##__Destructor_Impl()

#define __CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(__mType)                                                        \
        void __mType::__mType##__Destructor()                                                                           \
        {                                                                                                               \
            /*ChatPlexSDK::Logger()->Error(CP_SDK_U16STR(__Type) u"::OnDestroy");*/                                     \
            __mType##__Destructor_Impl();                                                                               \
            auto __Can_CPP_DTOR = !__Prevent_CPP_DTOR;                                                                  \
            __Prevent_CPP_DTOR = true;                                                                                  \
            auto l_BaseType = ___TypeRegistration::get()->baseType();                                                   \
            __CP_SDK_IL2CPP_CALL_BASE_METHOD(false, l_BaseType, CP_SDK_U16STR(__mType) u"__BaseClass", "OnDestroy");    \
            if (__Can_CPP_DTOR) __CP_SDK_IL2CPP_CALL_CPP_DTOR(__mType);                                                 \
        }                                                                                                               \
        void __mType::__mType##__Destructor_Impl()

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define __CP_SDK_IL2CPP_OVERRIDE_METHOD(__mBaseClass, __mRet, __mName, __mMethodInfo, ...)  \
    public:                                                                                 \
        __mRet __mBaseClass##__##__mName(__VA_ARGS__);                                      \
        ___CREATE_INSTANCE_METHOD(__mBaseClass##__##__mName, #__mName, (__mMethodInfo->flags & ~METHOD_ATTRIBUTE_ABSTRACT) | METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, __mMethodInfo)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/// @brief Call parent class method implementation
#define __CP_SDK_IL2CPP_CALL_BASE_METHOD(__mLog, __mBaseClass, __mBaseClassName, __mMethodName, ...)                                                                                               \
    {                                                                                                                                                                                               \
        static auto l_BaseMethodInfo = il2cpp_functions::class_get_method_from_name(__mBaseClass, __mMethodName, CP_SDK::Utils::Internals::ArgCount(__VA_ARGS__));                                  \
        if (l_BaseMethodInfo) il2cpp_utils::RunMethod(this, l_BaseMethodInfo __VA_OPT__(,) __VA_ARGS__);                                                                                            \
        else if (__mLog) CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_IL2CPP_CALL_BASE_METHOD] Method " CP_SDK_U16STR(__mMethodName) u" not found in class " __mBaseClassName);   \
    }

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define __CP_SDK_IL2CPP_CUSTOM_TYPE_DECLARE(__mNamespace, __mName, __mBaseClass, __mInterfaces)                                             \
        using ___TargetType = __mName;                                                                                                      \
        constexpr static auto ___Base__Size = sizeof(__mBaseClass);                                                                         \
        friend ::custom_types::Register;                                                                                                    \
        public:                                                                                                                             \
        constexpr static bool __IL2CPP_IS_VALUE_TYPE = false;                                                                               \
        static const int __IL2CPP_REFERENCE_TYPE_SIZE;                                                                                      \
        struct ___TypeRegistration : ::custom_types::TypeRegistration {                                                                     \
            ___TypeRegistration() {                                                                                                         \
                ::custom_types::Register::AddType(this);                                                                                    \
                instance = this;                                                                                                            \
            }                                                                                                                               \
            static inline std::vector<::custom_types::FieldRegistrator*> fields;                                                            \
            std::vector<::custom_types::FieldRegistrator*> const getFields() const override {                                               \
                return fields;                                                                                                              \
            }                                                                                                                               \
            static void addField(::custom_types::FieldRegistrator* inst) {                                                                  \
                fields.push_back(inst);                                                                                                     \
                ::custom_types::logger.debug("Adding instance field: %s.%s new size: %lu", #__mName, inst->name(), fields.size());          \
            }                                                                                                                               \
            static inline std::vector<::custom_types::StaticFieldRegistrator*> staticFields;                                                \
            std::vector<::custom_types::StaticFieldRegistrator*> const getStaticFields() const override {                                   \
                return staticFields;                                                                                                        \
            }                                                                                                                               \
            static void addStaticFieldInstance(::custom_types::StaticFieldRegistrator* inst) {                                              \
                staticFields.push_back(inst);                                                                                               \
                ::custom_types::logger.debug("Adding static field: %s.%s new size: %lu", #__mName, inst->name(), staticFields.size());      \
            }                                                                                                                               \
            static inline std::vector<::custom_types::MethodRegistrator*> methods;                                                          \
            std::vector<::custom_types::MethodRegistrator*> const getMethods() const override {                                             \
                return methods;                                                                                                             \
            }                                                                                                                               \
            static void addMethod(::custom_types::MethodRegistrator* inst) {                                                                \
                methods.push_back(inst);                                                                                                    \
                ::custom_types::logger.debug("Adding method: %s.%s new size: %lu", #__mName, inst->name(), methods.size());                 \
            }                                                                                                                               \
            static inline size_t staticFieldOffset;                                                                                         \
            static size_t addStaticField(size_t sz) {                                                                                       \
                auto tmp = staticFieldOffset;                                                                                               \
                staticFieldOffset += sz;                                                                                                    \
                return tmp;                                                                                                                 \
            }                                                                                                                               \
            static char* st_fields;                                                                                                         \
            char*& static_fields() override {                                                                                               \
                return st_fields;                                                                                                           \
            }                                                                                                                               \
            size_t static_fields_size() const override {                                                                                    \
                return staticFieldOffset;                                                                                                   \
            }                                                                                                                               \
            constexpr const char* name() const override {                                                                                   \
                return #__mName;                                                                                                            \
            }                                                                                                                               \
            constexpr const char* namespaze() const override {                                                                              \
                return __mNamespace;                                                                                                        \
            }                                                                                                                               \
            constexpr const char* dllName() const override {                                                                                \
                return __mNamespace;                                                                                                        \
            }                                                                                                                               \
            Il2CppClass* baseType() const override {                                                                                        \
                auto klass = ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<__mBaseClass*>::get();                                  \
                if (!klass->initialized) il2cpp_functions::Class_Init(klass);                                                               \
                return klass;                                                                                                               \
            }                                                                                                                               \
            std::vector<Il2CppClass*> const interfaces() const override {                                                                   \
                return __mInterfaces;                                                                                                       \
            }                                                                                                                               \
            constexpr Il2CppTypeEnum typeEnum() const override {                                                                            \
                return Il2CppTypeEnum::IL2CPP_TYPE_CLASS;                                                                                   \
            }                                                                                                                               \
            constexpr uint32_t typeFlags() const override {                                                                                 \
                return 0;                                                                                                                   \
            }                                                                                                                               \
            static Il2CppClass* klass_ptr;                                                                                                  \
            Il2CppClass*& klass() const override {                                                                                          \
                return klass_ptr;                                                                                                           \
            }                                                                                                                               \
            size_t size() const override {                                                                                                  \
                return sizeof(___TargetType);                                                                                               \
            }                                                                                                                               \
            TypeRegistration* customBase() const override {                                                                                 \
                return CP_SDK::Utils::Internals::GetCustomBaseIf<__mBaseClass>();                                                           \
            }                                                                                                                               \
            bool initialized() const override {                                                                                             \
                return init;                                                                                                                \
            }                                                                                                                               \
            void setInitialized() const override {                                                                                          \
                init = true;                                                                                                                \
            }                                                                                                                               \
            static bool init;                                                                                                               \
            static TypeRegistration* instance;                                                                                              \
            static TypeRegistration* get() {                                                                                                \
                return instance;                                                                                                            \
            }                                                                                                                               \
        };