#if CP_SDK_BMBF
#include "CP_SDK/Logging/BMBFLogger.hpp"

#include <locale>
#include <codecvt>

namespace CP_SDK::Logging {

    /// @brief Constructor
    /// @param p_BMBFLogger BMBF logger instance
    BMBFLogger::BMBFLogger(Logger * p_BMBFLogger)
        : m_BMBFLogger(p_BMBFLogger)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Internal log method
    /// @param p_Type Kind
    /// @param p_Data Data
    void BMBFLogger::LogImplementation(ELogType p_Type, std::u16string_view p_Data)
    {
        std::string l_UTF8Data;
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> l_Convertor;
        l_UTF8Data = l_Convertor.to_bytes(p_Data.begin(), p_Data.end());

        auto l_String = l_UTF8Data.c_str();
        switch (p_Type)
        {
            case ELogType::Error:    m_BMBFLogger->error("%s", l_String);   break;
            case ELogType::Warning:  m_BMBFLogger->warning("%s", l_String); break;
            case ELogType::Info:     m_BMBFLogger->info("%s", l_String);    break;
            case ELogType::Debug:    m_BMBFLogger->debug("%s", l_String);   break;
        }
    }
    /// @brief Internal log method
    /// @param p_Type Kind
    /// @param p_Data Data
    void BMBFLogger::LogImplementation(ELogType p_Type, const std::exception& p_Data)
    {
        auto l_String = p_Data.what();
        switch (p_Type)
        {
            case ELogType::Error:    m_BMBFLogger->error("%s", l_String);   break;
            case ELogType::Warning:  m_BMBFLogger->warning("%s", l_String); break;
            case ELogType::Info:     m_BMBFLogger->info("%s", l_String);    break;
            case ELogType::Debug:    m_BMBFLogger->debug("%s", l_String);   break;
        }
    }

}   ///< namespace CP_SDK::Logging
#endif