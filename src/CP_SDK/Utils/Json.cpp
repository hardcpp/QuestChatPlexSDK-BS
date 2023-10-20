#include "CP_SDK/Utils/Json.hpp"

namespace CP_SDK::Utils {

    /// @brief Try parse from utf8 string
    /// @param p_Str      Input string
    /// @param p_Document Target document
    /// @param p_Error    OUT OPTIONAL error
    /// @return True or false
    bool Json::TryFromU8String(std::string_view p_Str, U16Document& p_Document, std::u16string* p_Error)
    {
        p_Document.Clear();
        if (!p_Str.empty())
            p_Document.Parse<rapidjson::kParseNoFlags, UTF8>(&p_Str[0], p_Str.size());

        if (p_Document.HasParseError())
        {
            if (p_Error)
                *p_Error = u"JSON Parsing failed with error " + std::to_string(static_cast<int>(p_Document.GetParseError()));

            return false;
        }

        return true;
    }
    /// @brief Try parse from utf8 string
    /// @param p_Str      Input string
    /// @param p_Document Target document
    /// @param p_Error    OUT OPTIONAL error
    /// @return True or false
    bool Json::TryFromU8Stream(std::ifstream& p_Stream, U16Document& p_Document, std::u16string* p_Error)
    {
        if (!p_Stream.is_open())
        {
            if (p_Error) *p_Error = u"Stream was closed";
            return false;
        }

        std::string l_U8Str((std::istreambuf_iterator<char>(p_Stream)), std::istreambuf_iterator<char>());
        return TryFromU8String(l_U8Str, p_Document, p_Error);
    }
    /// @brief Try parse from utf8 string
    /// @param p_Str      Input string
    /// @param p_Document Target document
    /// @param p_Error    OUT OPTIONAL error
    /// @return True or false
    bool Json::TryFromU16String(std::u16string_view p_Str, U16Document& p_Document, std::u16string* p_Error)
    {
        auto l_U8Str = U16StrToStr(p_Str);
        return TryFromU8String(l_U8Str, p_Document, p_Error);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Encode to utf8 string
    /// @param p_Value  Source Json
    /// @param p_Pretty Should pretty format?
    /// @return Encoded Json
    std::string Json::ToU8String(U16Value& p_Value, bool p_Pretty)
    {
        U8GenericStringBuffer l_Buffer;
        if (p_Pretty)
        {
            U16ToU8PrettyWritter<U8GenericStringBuffer> l_Writer(l_Buffer);
            if (!p_Value.Accept(l_Writer))
                throw std::runtime_error("[CP_SDK.Utils][Json.ToU8String] Failed");
        }
        else
        {
            U16ToU8Writter<U8GenericStringBuffer> l_Writer(l_Buffer);
            if (!p_Value.Accept(l_Writer))
                throw std::runtime_error("[CP_SDK.Utils][Json.ToU8String] Failed");
        }

        return l_Buffer.GetString();
    }
    /// @brief Encode to utf8 string
    /// @param p_Value  Source Json
    /// @param p_Pretty Should pretty format?
    /// @param p_Stream Target stream
    /// @return Encoded Json
    bool Json::TryToU8Stream(U16Value& p_Value, bool p_Pretty, std::ofstream& p_Stream)
    {
        rapidjson::OStreamWrapper l_Wrapper(p_Stream);
        if (p_Pretty)
        {
            U16ToU8PrettyWritter<rapidjson::OStreamWrapper> l_Writer(l_Wrapper);
            if (!p_Value.Accept(l_Writer))
                return false;
        }
        else
        {
            U16ToU8Writter<rapidjson::OStreamWrapper> l_Writer(l_Wrapper);
            if (!p_Value.Accept(l_Writer))
                return false;
        }

        return true;
    }
    /// @brief Encode to utf16 string
    /// @param p_Value  Source Json
    /// @param p_Pretty Should pretty format?
    /// @return Encoded Json
    std::u16string Json::ToU16String(U16Value& p_Value, bool p_Pretty)
    {
        U16GenericStringBuffer l_Buffer;
        if (p_Pretty)
        {
            U16PrettyWritter<U16GenericStringBuffer> l_Writer(l_Buffer);
            if (!p_Value.Accept(l_Writer))
                throw std::runtime_error("[CP_SDK.Utils][Json.ToU16String] Failed");
        }
        else
        {
            U16Writter<U16GenericStringBuffer> l_Writer(l_Buffer);
            if (!p_Value.Accept(l_Writer))
                throw std::runtime_error("[CP_SDK.Utils][Json.ToU16String] Failed");
        }

        return l_Buffer.GetString();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    #define ENSURE_NOT_SERIALIZED() if (p_Document.HasMember(p_Name)) throw std::runtime_error("Serialize<T>: Field " + U16StrToStr(p_Name)  + " is already serialized");
    #define NEW_OBJECT()            U16Value l_Object; l_Object.SetObject();
    #define ADD_OBJECT()            p_Document.AddMember(rapidjson::StringRef(p_Name), l_Object, p_Allocator);

    void Json::SerializeBool(U16Value& p_Document, TAllocator& p_Allocator, bool p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeColor32(U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Color32 p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        NEW_OBJECT();
        SerializeUInt32(l_Object, p_Allocator, p_Value.r, u"r");
        SerializeUInt32(l_Object, p_Allocator, p_Value.g, u"g");
        SerializeUInt32(l_Object, p_Allocator, p_Value.b, u"b");
        SerializeUInt32(l_Object, p_Allocator, p_Value.a, u"a");
        ADD_OBJECT();
    }
    void Json::SerializeColor(U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Color p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        NEW_OBJECT();
        SerializeFloat(l_Object, p_Allocator, p_Value.r, u"r");
        SerializeFloat(l_Object, p_Allocator, p_Value.g, u"g");
        SerializeFloat(l_Object, p_Allocator, p_Value.b, u"b");
        SerializeFloat(l_Object, p_Allocator, p_Value.a, u"a");
        ADD_OBJECT();
    }
    void Json::SerializeFloat(U16Value& p_Document, TAllocator& p_Allocator, float p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeInt32(U16Value& p_Document, TAllocator& p_Allocator, int32_t p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeInt64(U16Value& p_Document, TAllocator& p_Allocator, int64_t p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeQuaternion(U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Quaternion p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        NEW_OBJECT();
        SerializeUInt32(l_Object, p_Allocator, p_Value.x, u"x");
        SerializeUInt32(l_Object, p_Allocator, p_Value.y, u"y");
        SerializeUInt32(l_Object, p_Allocator, p_Value.z, u"z");
        SerializeUInt32(l_Object, p_Allocator, p_Value.w, u"w");
        ADD_OBJECT();
    }
    void Json::SerializeString(U16Value& p_Document, TAllocator& p_Allocator, const std::u16string& p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeUInt32(U16Value& p_Document, TAllocator& p_Allocator, uint32_t p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeUInt64(U16Value& p_Document, TAllocator& p_Allocator, uint64_t p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        p_Document.AddMember(rapidjson::StringRef(p_Name), p_Value, p_Allocator);
    }
    void Json::SerializeVector2(U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Vector2 p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        NEW_OBJECT();
        SerializeUInt32(l_Object, p_Allocator, p_Value.x, u"x");
        SerializeUInt32(l_Object, p_Allocator, p_Value.y, u"y");
        ADD_OBJECT();
    }
    void Json::SerializeVector3(U16Value& p_Document, TAllocator& p_Allocator, UnityEngine::Vector3 p_Value, const char16_t* p_Name)
    {
        ENSURE_NOT_SERIALIZED();
        NEW_OBJECT();
        SerializeUInt32(l_Object, p_Allocator, p_Value.x, u"x");
        SerializeUInt32(l_Object, p_Allocator, p_Value.y, u"y");
        SerializeUInt32(l_Object, p_Allocator, p_Value.z, u"z");
        ADD_OBJECT();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    #define GET_OBJECT() auto l_Object = p_Document[p_Name].GetObject();

    void Json::UnserializeBool(U16Value& p_Document, bool& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsBool);
        p_Value = p_Document[p_Name].GetBool();
    }
    void Json::UnserializeColor32(U16Value& p_Document, UnityEngine::Color32& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsObject);
        GET_OBJECT();
        uint32_t l_R, l_G, l_B, l_A;
        UnserializeUInt32(l_Object, l_R, u"r"); p_Value.r = static_cast<uint8_t>(l_R);
        UnserializeUInt32(l_Object, l_G, u"g"); p_Value.g = static_cast<uint8_t>(l_G);
        UnserializeUInt32(l_Object, l_B, u"b"); p_Value.b = static_cast<uint8_t>(l_B);
        UnserializeUInt32(l_Object, l_A, u"a"); p_Value.a = static_cast<uint8_t>(l_A);
    }
    void Json::UnserializeColor(U16Value& p_Document, UnityEngine::Color& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsObject);
        GET_OBJECT();
        UnserializeFloat(l_Object, p_Value.r, u"r");
        UnserializeFloat(l_Object, p_Value.g, u"g");
        UnserializeFloat(l_Object, p_Value.b, u"b");
        UnserializeFloat(l_Object, p_Value.a, u"a");
    }
    void Json::UnserializeFloat(U16Value& p_Document, float& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsFloat);
        p_Value = p_Document[p_Name].GetFloat();
    }
    void Json::UnserializeInt32(U16Value& p_Document, int32_t& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsInt);
        p_Value = p_Document[p_Name].GetInt();
    }
    void Json::UnserializeInt64(U16Value& p_Document, int64_t& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsInt64);
        p_Value = p_Document[p_Name].GetInt64();
    }
    void Json::UnserializeQuaternion(U16Value& p_Document, UnityEngine::Quaternion& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsObject);
        GET_OBJECT();
        UnserializeFloat(l_Object, p_Value.x, u"x");
        UnserializeFloat(l_Object, p_Value.y, u"y");
        UnserializeFloat(l_Object, p_Value.z, u"z");
        UnserializeFloat(l_Object, p_Value.w, u"w");
    }
    void Json::UnserializeString(U16Value& p_Document, std::u16string& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsString);
        p_Value = p_Document[p_Name].GetString();
    }
    void Json::UnserializeUInt32(U16Value& p_Document, uint32_t& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsUint);
        p_Value = p_Document[p_Name].GetUint();
    }
    void Json::UnserializeUInt64(U16Value& p_Document, uint64_t& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsUint64);
        p_Value = p_Document[p_Name].GetUint64();
    }
    void Json::UnserializeVector2(U16Value& p_Document, UnityEngine::Vector2& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsObject);
        GET_OBJECT();
        UnserializeFloat(l_Object, p_Value.x, u"x");
        UnserializeFloat(l_Object, p_Value.y, u"y");
    }
    void Json::UnserializeVector3(U16Value& p_Document, UnityEngine::Vector3& p_Value, const char16_t* p_Name)
    {
        if (!p_Document.HasMember(p_Name)) return;
        CP_SDK_JSON_ENSURE_TYPE(IsObject);
        GET_OBJECT();
        UnserializeFloat(l_Object, p_Value.x, u"x");
        UnserializeFloat(l_Object, p_Value.y, u"y");
        UnserializeFloat(l_Object, p_Value.z, u"z");
    }

}   ///< namespace CP_SDK::Utils