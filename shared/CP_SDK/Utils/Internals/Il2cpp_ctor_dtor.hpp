#define CP_SDK_IL2CPP_DECLARE_CTOR(__Type)                                                                                                                                                                  \
    public:                                                                                                                                                                                                 \
        bool __Prevent_CPP_CTOR;                                                                                                                                                                            \
        void __Type##__Constructor();                                                                                                                                                                       \
        template<::il2cpp_utils::CreationType creationType = ::il2cpp_utils::CreationType::Temporary, class... TArgs>                                                                                       \
        static ___TargetType* New_ctor(TArgs&&... args) {                                                                                                                                                   \
            static_assert(::custom_types::Decomposer<decltype(&___TargetType::__Type##__Constructor)>::convertible<TArgs...>(), "Arguments provided to New_ctor must be convertible to the constructor!");  \
            ___TargetType* obj;                                                                                                                                                                             \
            if constexpr (creationType == ::il2cpp_utils::CreationType::Temporary) {                                                                                                                        \
                obj = static_cast<___TargetType*>(::il2cpp_functions::object_new(___TypeRegistration::klass_ptr));                                                                                          \
            } else {                                                                                                                                                                                        \
                obj = static_cast<___TargetType*>(::il2cpp_utils::createManual(___TypeRegistration::klass_ptr));                                                                                            \
            }                                                                                                                                                                                               \
            obj->__Type##__Constructor(std::forward<TArgs>(args)...);                                                                                                                                       \
            return obj;                                                                                                                                                                                     \
        }                                                                                                                                                                                                   \
        ___CREATE_INSTANCE_METHOD(__Type##__Constructor, ".ctor", METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG | METHOD_ATTRIBUTE_SPECIAL_NAME | METHOD_ATTRIBUTE_RT_SPECIAL_NAME, nullptr);      \
        void __Type##__Constructor_Impl()

#define CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(__Type, __BaseType)                                                                                                                                               \
    public:                                                                                                                                                                                                 \
        void __Type##__Constructor();                                                                                                                                                            \
        template<::il2cpp_utils::CreationType creationType = ::il2cpp_utils::CreationType::Temporary, class... TArgs>                                                                                       \
        static ___TargetType* New_ctor(TArgs&&... args) {                                                                                                                                                   \
            static_assert(::custom_types::Decomposer<decltype(&___TargetType::__Type##__Constructor)>::convertible<TArgs...>(), "Arguments provided to New_ctor must be convertible to the constructor!");  \
            ___TargetType* obj;                                                                                                                                                                             \
            if constexpr (creationType == ::il2cpp_utils::CreationType::Temporary) {                                                                                                                        \
                obj = static_cast<___TargetType*>(::il2cpp_functions::object_new(___TypeRegistration::klass_ptr));                                                                                          \
            } else {                                                                                                                                                                                        \
                obj = static_cast<___TargetType*>(::il2cpp_utils::createManual(___TypeRegistration::klass_ptr));                                                                                            \
            }                                                                                                                                                                                               \
            obj->__Type##__Constructor(std::forward<TArgs>(args)...);                                                                                                                                       \
            return obj;                                                                                                                                                                                     \
        }                                                                                                                                                                                                   \
        ___CREATE_INSTANCE_METHOD(__Type##__Constructor, ".ctor", METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG | METHOD_ATTRIBUTE_SPECIAL_NAME | METHOD_ATTRIBUTE_RT_SPECIAL_NAME, nullptr);      \
        void __Type##__Constructor_Impl()

#define CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(__Type)                                                                         \
        void __Type::__Type##__Constructor()                                                                            \
        {                                                                                                               \
            /*ChatPlexSDK::Logger()->Error(u"" + StringW(#__Type) + u"::Constructor");*/                                \
            auto __Can_CPP_CTOR = !__Prevent_CPP_CTOR;                                                                  \
            __Prevent_CPP_CTOR = true;                                                                                  \
            if (__Can_CPP_CTOR) INVOKE_CTOR();                                                                          \
            if (___TargetType::___TypeRegistration::get()->customBase())                                                \
                ::custom_types::InvokeBaseCtor(___TargetType::___TypeRegistration::get()->customBase()->klass(), this); \
            else                                                                                                        \
                ::custom_types::InvokeBaseCtor(___TargetType::___TypeRegistration::get()->baseType(), this);            \
            __Prevent_CPP_DTOR = false;                                                                                 \
            __Type##__Constructor_Impl();                                                                               \
        }                                                                                                               \
        void __Type::__Type##__Constructor_Impl()

#define CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN_IMPL(__Type, __BaseType)                                                       \
        void __Type::__Type##__Constructor()                                                                            \
        {                                                                                                               \
            /*ChatPlexSDK::Logger()->Error(u"" + StringW(#__Type) + u"::Constructor");*/                                \
            auto __Can_CPP_CTOR = !__Prevent_CPP_CTOR;                                                                  \
            __Prevent_CPP_CTOR = true;                                                                                  \
            if (__Can_CPP_CTOR) INVOKE_CTOR();                                                                          \
            if (___TargetType::___TypeRegistration::get()->customBase())                                                \
                ::custom_types::InvokeBaseCtor(___TargetType::___TypeRegistration::get()->customBase()->klass(), this); \
            else                                                                                                        \
                ::custom_types::InvokeBaseCtor(___TargetType::___TypeRegistration::get()->baseType(), this);            \
            __Prevent_CPP_DTOR = false;                                                                                 \
            __Type##__Constructor_Impl();                                                                               \
        }                                                                                                               \
        void __Type::__Type##__Constructor_Impl()

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(__Type)                                                        \
    public:                                                                                                     \
        bool __Prevent_CPP_DTOR = false;                                                                        \
        void __Type##__Destructor();                                                                            \
        ___CREATE_INSTANCE_METHOD(__Type##__Destructor, "OnDestroy", METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, nullptr); \
        void __Type##__Destructor_Impl()

#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(__Type, __BaseType)                                      \
    public:                                                                                                     \
        void __Type##__Destructor();                                                                            \
        ___CREATE_INSTANCE_METHOD(__Type##__Destructor, "OnDestroy", METHOD_ATTRIBUTE_PUBLIC | METHOD_ATTRIBUTE_HIDE_BY_SIG, nullptr); \
        void __Type##__Destructor_Impl()

#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(__Type)                                                   \
        void __Type::__Type##__Destructor()                                                                     \
        {                                                                                                       \
            /*ChatPlexSDK::Logger()->Error(u"" + StringW(#__Type) + u"::OnDestroy");*/                          \
            __Type##__Destructor_Impl();                                                                        \
            auto __Can_CPP_DTOR = !__Prevent_CPP_DTOR;                                                          \
            if (__Can_CPP_DTOR) CP_SDK_IL2CPP_CALL_CPP_DESTRUCTOR(__Type);                                      \
        }                                                                                                       \
        void __Type::__Type##__Destructor_Impl()

#define CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN_IMPL(__Type, __BaseType)                                 \
        void __Type::__Type##__Destructor()                                                                     \
        {                                                                                                       \
            /*ChatPlexSDK::Logger()->Error(u"" + StringW(#__Type) + u"::OnDestroy");*/                          \
            __Type##__Destructor_Impl();                                                                        \
            auto __Can_CPP_DTOR = !__Prevent_CPP_DTOR;                                                          \
            __Prevent_CPP_DTOR = true;                                                                          \
            CP_SDK_IL2CPP_CALL_BASE_METHOD(__BaseType, "OnDestroy");                                            \
            if (__Can_CPP_DTOR) CP_SDK_IL2CPP_CALL_CPP_DESTRUCTOR(__Type);                                      \
        }                                                                                                       \
        void __Type::__Type##__Destructor_Impl()

#define CP_SDK_IL2CPP_CALL_CPP_DESTRUCTOR(__Type) this->~__Type();