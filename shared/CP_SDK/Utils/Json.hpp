#pragma once

#include "Il2cpp.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <stdint.h>
#include <vector>

#include <beatsaber-hook/shared/config/config-utils.hpp>
#include <UnityEngine/Color32.hpp>
#include <UnityEngine/Color.hpp>
#include <UnityEngine/Quaternion.hpp>
#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector3.hpp>

#define CP_SDK_JSON_SERIALIZE_DECL()       void Serialize(CP_SDK::Utils::Json::U16Value& p_Document, CP_SDK::Utils::Json::TAllocator& p_Allocator)
#define CP_SDK_JSON_SERIALIZE_IMPL(__Type) void __Type::Serialize(CP_SDK::Utils::Json::U16Value& p_Document, CP_SDK::Utils::Json::TAllocator& p_Allocator)

#define CP_SDK_JSON_UNSERIALIZE_DECL()       void Unserialize(CP_SDK::Utils::Json::U16Value& p_Document)
#define CP_SDK_JSON_UNSERIALIZE_IMPL(__Type) void __Type::Unserialize(CP_SDK::Utils::Json::U16Value& p_Document)

#define CP_SDK_JSON_OPERATORS_INLINE(__Serialize, __Unserialize)                                            \
    void Serialize(CP_SDK::Utils::Json::U16Value& p_Document, CP_SDK::Utils::Json::TAllocator& p_Allocator) \
    { __Serialize                                                                               }           \
    void Unserialize(CP_SDK::Utils::Json::U16Value& p_Document)                                             \
    { __Unserialize                                                                             }


#define __CP_SDK_JSON_VAR_NAME(__Var) u##__Var

#define CP_SDK_JSON_SERIALIZE_BOOL(__Var)           CP_SDK::Utils::Json::SerializeBool         (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_COLOR32(__Var)        CP_SDK::Utils::Json::SerializeColor32      (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_COLOR(__Var)          CP_SDK::Utils::Json::SerializeColor        (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_FLOAT(__Var)          CP_SDK::Utils::Json::SerializeFloat        (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_INT32(__Var)          CP_SDK::Utils::Json::SerializeInt32        (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_INT64(__Var)          CP_SDK::Utils::Json::SerializeInt64        (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_QUATERNION(__Var)     CP_SDK::Utils::Json::SerializeQuaternion   (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_STRING(__Var)         CP_SDK::Utils::Json::SerializeString       (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_UINT32(__Var)         CP_SDK::Utils::Json::SerializeUInt32       (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_UINT64(__Var)         CP_SDK::Utils::Json::SerializeUInt64       (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_VECTOR2(__Var)        CP_SDK::Utils::Json::SerializeVector2      (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_VECTOR3(__Var)        CP_SDK::Utils::Json::SerializeVector3      (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_ENUM(__Var, __EnumT)  CP_SDK::Utils::Json::SerializeEnum<__EnumT>(p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_OBJECT(__Var)         CP_SDK::Utils::Json::SerializeObject       (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_SERIALIZE_OBJECT_ARRAY(__Var)   CP_SDK::Utils::Json::SerializeObjectArray  (p_Document, p_Allocator, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))

#define CP_SDK_JSON_UNSERIALIZE_BOOL(__Var)                 CP_SDK::Utils::Json::UnserializeBool         (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_COLOR32(__Var)              CP_SDK::Utils::Json::UnserializeColor32      (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_COLOR(__Var)                CP_SDK::Utils::Json::UnserializeColor        (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_FLOAT(__Var)                CP_SDK::Utils::Json::UnserializeFloat        (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_INT32(__Var)                CP_SDK::Utils::Json::UnserializeInt32        (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_INT64(__Var)                CP_SDK::Utils::Json::UnserializeInt64        (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_QUATERNION(__Var)           CP_SDK::Utils::Json::UnserializeQuaternion   (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_STRING(__Var)               CP_SDK::Utils::Json::UnserializeString       (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_UINT32(__Var)               CP_SDK::Utils::Json::UnserializeUInt32       (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_UINT64(__Var)               CP_SDK::Utils::Json::UnserializeUInt64       (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_VECTOR2(__Var)              CP_SDK::Utils::Json::UnserializeVector2      (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_VECTOR3(__Var)              CP_SDK::Utils::Json::UnserializeVector3      (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_ENUM(__Var, __EnumT, __Def) CP_SDK::Utils::Json::UnserializeEnum<__EnumT>(p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var), __Def)
#define CP_SDK_JSON_UNSERIALIZE_OBJECT(__Var)               CP_SDK::Utils::Json::UnserializeObject       (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))
#define CP_SDK_JSON_UNSERIALIZE_OBJECT_ARRAY(__Var)         CP_SDK::Utils::Json::UnserializeObjectArray  (p_Document, __Var, __CP_SDK_JSON_VAR_NAME(#__Var))

namespace CP_SDK::Utils {

    /// @brief Json utils
    class Json
    {
        DISABLE_CONSTRUCTORS(Json);

        public:
            using UTF8                      = rapidjson::UTF8<char>;
            using UTF16                     = rapidjson::UTF16<char16_t>;
            using U16Value                  = rapidjson::GenericValue<UTF16>;
            using U16Document               = rapidjson::GenericDocument<UTF16>;
            using U8GenericStringBuffer     = rapidjson::GenericStringBuffer<UTF8>;
            using U16GenericStringBuffer    = rapidjson::GenericStringBuffer<UTF16>;

            template <typename t_Writter>   using U16Writter            = rapidjson::Writer<t_Writter, UTF16, UTF16>;
            template <typename t_Writter>   using U16ToU8Writter        = rapidjson::Writer<t_Writter, UTF16, UTF8>;
            template <typename t_Writter>   using U16PrettyWritter      = rapidjson::PrettyWriter<t_Writter, UTF16, UTF16>;
            template <typename t_Writter>   using U16ToU8PrettyWritter  = rapidjson::PrettyWriter<t_Writter, UTF16, UTF8>;

            using TAllocator                = U16Document::AllocatorType;

        public:
            /// @brief Try parse from utf8 string
            /// @param p_Str      Input string
            /// @param p_Document Target document
            /// @param p_Error    OUT OPTIONAL error
            /// @return True or false
            static bool TryFromU8String(std::string_view p_Str, U16Document& p_Document, std::u16string* p_Error = nullptr);
            /// @brief Try parse from utf8 string
            /// @param p_Str      Input string
            /// @param p_Document Target document
            /// @param p_Error    OUT OPTIONAL error
            /// @return True or false
            static bool TryFromU8Stream(std::ifstream& p_Stream, U16Document& p_Document, std::u16string* p_Error = nullptr);
            /// @brief Try parse from utf8 string
            /// @param p_Str      Input string
            /// @param p_Document Target document
            /// @param p_Error    OUT OPTIONAL error
            /// @return True or false
            static bool TryFromU16String(std::u16string_view p_Str, U16Document& p_Document, std::u16string* p_Error = nullptr);

        public:
            /// @brief Encode to utf8 string
            /// @param p_Value  Source Json
            /// @param p_Pretty Should pretty format?
            /// @return Encoded Json
            static std::string ToU8String(U16Value& p_Value, bool p_Pretty);
            /// @brief Encode to utf8 string
            /// @param p_Value  Source Json
            /// @param p_Pretty Should pretty format?
            /// @param p_Stream Target stream
            /// @return Encoded Json
            static bool TryToU8Stream(U16Value& p_Value, bool p_Pretty, std::ofstream& p_Stream);
            /// @brief Encode to utf16 string
            /// @param p_Value  Source Json
            /// @param p_Pretty Should pretty format?
            /// @return Encoded Json
            static std::u16string ToU16String(U16Value& p_Value, bool p_Pretty);

        public:
            static void SerializeBool      (U16Value& p_Document, TAllocator& p_Allocator, bool                    p_Value, const char16_t* p_Name);
            static void SerializeColor32   (U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Color32    p_Value, const char16_t* p_Name);
            static void SerializeColor     (U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Color      p_Value, const char16_t* p_Name);
            static void SerializeFloat     (U16Value& p_Document, TAllocator& p_Allocator, float                   p_Value, const char16_t* p_Name);
            static void SerializeInt32     (U16Value& p_Document, TAllocator& p_Allocator, int32_t                 p_Value, const char16_t* p_Name);
            static void SerializeInt64     (U16Value& p_Document, TAllocator& p_Allocator, int64_t                 p_Value, const char16_t* p_Name);
            static void SerializeQuaternion(U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Quaternion p_Value, const char16_t* p_Name);
            static void SerializeString    (U16Value& p_Document, TAllocator& p_Allocator, const std::u16string&   p_Value, const char16_t* p_Name);
            static void SerializeUInt32    (U16Value& p_Document, TAllocator& p_Allocator, uint32_t                p_Value, const char16_t* p_Name);
            static void SerializeUInt64    (U16Value& p_Document, TAllocator& p_Allocator, uint64_t                p_Value, const char16_t* p_Name);
            static void SerializeVector2   (U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Vector2    p_Value, const char16_t* p_Name);
            static void SerializeVector3   (U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Vector3    p_Value, const char16_t* p_Name);

            template<typename t_Enum>
            static void SerializeEnum(U16Value& p_Document, TAllocator& p_Allocator, typename t_Enum::E p_Value, const char16_t* p_Name)
            {
                if (p_Document.HasMember(p_Name))
                    throw std::runtime_error("_Serialize<T>: Field " + U16StrToStr(p_Name) + " is already serialized");

                auto l_Str = t_Enum::ToStr(p_Value);
                p_Document.AddMember(rapidjson::StringRef(p_Name), rapidjson::StringRef(l_Str.data(), l_Str.size()), p_Allocator);
            }
            template<typename t_Object>
            static void SerializeObject(U16Value& p_Document, TAllocator& p_Allocator, std::shared_ptr<t_Object>& p_Object, const char16_t* p_Name)
            {
                if (p_Document.HasMember(p_Name))
                    throw std::runtime_error("_Serialize<T>: Field " + U16StrToStr(p_Name) + " is already serialized");

                U16Value l_Object;
                l_Object.SetObject();

                p_Object->Serialize(l_Object, p_Allocator);

                p_Document.AddMember(rapidjson::StringRef(p_Name), l_Object, p_Allocator);
            }
            template<typename t_Object> requires(!std::is_pointer_v<t_Object>)
            static void SerializeObject(U16Value& p_Document, TAllocator& p_Allocator, std::optional<t_Object>& p_Object, const char16_t* p_Name)
            {
                if (!p_Object.has_value())
                    return;
                if (p_Document.HasMember(p_Name))
                    throw std::runtime_error("_Serialize<T>: Field " + U16StrToStr(p_Name) + " is already serialized");

                U16Value l_Object;
                l_Object.SetObject();

                p_Object.value().Serialize(l_Object, p_Allocator);

                p_Document.AddMember(rapidjson::StringRef(p_Name), l_Object, p_Allocator);
            }
            template<typename t_Object>
            static void SerializeObjectArray(U16Value& p_Document, TAllocator& p_Allocator, std::vector<std::shared_ptr<t_Object>>& p_Array, const char16_t* p_Name)
            {
                if (p_Document.HasMember(p_Name))
                    throw std::runtime_error("_Serialize<T>: Field " + U16StrToStr(p_Name) + " is already serialized");

                U16Value l_Array;
                l_Array.SetArray();

                for (auto& l_Current : p_Array)
                {
                    U16Value l_Object;
                    l_Object.SetObject();
                    l_Current->Serialize(l_Object, p_Allocator);
                    l_Array.PushBack(l_Object, p_Allocator);
                }

                p_Document.AddMember(rapidjson::StringRef(p_Name), l_Array, p_Allocator);
            }

        public:
            static void UnserializeBool      (U16Value& p_Document, bool&                    p_Value, const char16_t* p_Name);
            static void UnserializeColor32   (U16Value& p_Document, UnityEngine::Color32&    p_Value, const char16_t* p_Name);
            static void UnserializeColor     (U16Value& p_Document, UnityEngine::Color&      p_Value, const char16_t* p_Name);
            static void UnserializeFloat     (U16Value& p_Document, float&                   p_Value, const char16_t* p_Name);
            static void UnserializeInt32     (U16Value& p_Document, int32_t&                 p_Value, const char16_t* p_Name);
            static void UnserializeInt64     (U16Value& p_Document, int64_t&                 p_Value, const char16_t* p_Name);
            static void UnserializeQuaternion(U16Value& p_Document, UnityEngine::Quaternion& p_Value, const char16_t* p_Name);
            static void UnserializeString    (U16Value& p_Document, std::u16string&          p_Value, const char16_t* p_Name);
            static void UnserializeUInt32    (U16Value& p_Document, uint32_t&                p_Value, const char16_t* p_Name);
            static void UnserializeUInt64    (U16Value& p_Document, uint64_t&                p_Value, const char16_t* p_Name);
            static void UnserializeVector2   (U16Value& p_Document, UnityEngine::Vector2&    p_Value, const char16_t* p_Name);
            static void UnserializeVector3   (U16Value& p_Document, UnityEngine::Vector3&    p_Value, const char16_t* p_Name);

            template<typename t_Enum>
            static void UnserializeEnum(U16Value& p_Document, typename t_Enum::E& p_Value, const char16_t* p_Name, typename t_Enum::E p_Default)
            {
                if (!p_Document.HasMember(p_Name) || !p_Document[p_Name].IsString())
                {
                    p_Value = p_Default;
                    return;
                }

                auto l_U16String = p_Document[p_Name].GetString();
                p_Value = t_Enum::ToEnum(l_U16String);

                if (t_Enum::ToStr(p_Value) != l_U16String)
                    p_Value = p_Default;
            }
            template<typename t_Object>
            static void UnserializeObject(U16Value& p_Document, std::shared_ptr<t_Object>& p_Object, const char16_t* p_Name)
            {
                if (!p_Document.HasMember(p_Name) || !p_Document[p_Name].IsObject())
                    throw std::runtime_error("UnserializeObject<T>: Field " + U16StrToStr(p_Name) + " doesn't exist or is of the wrong type");

                auto l_Object = p_Document[p_Name].GetObject();

                p_Object->Unserialize(l_Object);
            }
            template<typename t_Object> requires(!std::is_pointer_v<t_Object>)
            static void UnserializeObject(U16Value& p_Document, std::optional<t_Object>& p_Object, const char16_t* p_Name)
            {
                if (!p_Document.HasMember(p_Name))
                    return;

                if (!p_Document[p_Name].IsObject())
                    throw std::runtime_error("UnserializeObject<T>: Field " + U16StrToStr(p_Name) + " doesn't exist or is of the wrong type");

                auto l_Object = p_Document[p_Name].GetObject();

                if (!p_Object.has_value())
                    p_Object = t_Object {};

                p_Object.value().Unserialize(l_Object);
            }
            template<typename t_Object>
            static void UnserializeObjectArray(U16Value& p_Document, std::vector<std::shared_ptr<t_Object>>& p_Array, const char16_t* p_Name)
            {
                if (!p_Document.HasMember(p_Name) || !p_Document[p_Name].IsArray())
                    throw std::runtime_error("UnserializeObjectArray<T>: Field " + U16StrToStr(p_Name) + " doesn't exist or is of the wrong type");

                auto l_Array = p_Document[p_Name].GetArray();
                p_Array.clear();
                p_Array.reserve(l_Array.Size());

                for (auto l_I = 0; l_I < l_Array.Size(); ++l_I)
                {
                    auto l_Object = std::make_shared<t_Object>();
                    l_Object->Unserialize(l_Array[l_I]);

                    p_Array.push_back(l_Object);
                }
            }
    };

}   ///< namespace CP_SDK::Utils
