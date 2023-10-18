#define CP_SDK_IL2CPP_ENUM_UTILS()                                                                                              \
        static constexpr int ValueCount() { return S.size(); };                                                                 \
                                                                                                                                \
        static int ToInt(std::u16string_view p_Str)                                                                             \
        {                                                                                                                       \
            auto l_It = std::find_if(S.begin(), S.end(), [&](const std::u16string_view& p_A) -> bool {                          \
                return p_A.size() == p_Str.size() && std::memcmp(p_A.data(), p_Str.data(), p_A.size() * sizeof(char16_t)) == 0; \
            });                                                                                                                 \
            auto l_Index = l_It != S.end() ? l_It - S.begin() : -1;                                                             \
            return std::clamp<int>(l_Index, 0, ValueCount() - 1);                                                               \
        }                                                                                                                       \
        static int ToInt(E p_Enum)                  { return std::clamp<int>((int)p_Enum, 0, ValueCount() - 1);     }           \
                                                                                                                                \
        static E ToEnum(std::u16string_view p_Str)  { return (E)ToInt(p_Str);                                       }           \
        static E ToEnum(int p_Int)                  { return (E)std::clamp<int>(p_Int, 0, ValueCount() - 1);        }           \
                                                                                                                                \
        static std::u16string_view ToStr(int p_Int) { return S[std::clamp<int>(p_Int, 0, ValueCount() - 1)];        }           \
        static std::u16string_view ToStr(E p_Enum)  { return S[std::clamp<int>((int)p_Enum, 0, ValueCount() - 1)];  }