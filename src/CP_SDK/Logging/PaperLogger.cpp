#if CP_SDK_BMBF
#include "CP_SDK/Logging/PaperLogger.hpp"

#include <locale>
#include <codecvt>

namespace CP_SDK::Logging {

    /// @brief Constructor
    /// @param p_Name Paper Logger name
    PaperLogger::PaperLogger(const std::string& p_Name)
        : m_PaperLogger(new Paper::LoggerContext(p_Name))
    {
        Error(u"Test Error");
        Warning(u"Test Warning");
        Info(u"Test Info");
        Debug(u"Test Debug");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Internal log method
    /// @param p_Type Kind
    /// @param p_Data Data
    void PaperLogger::LogImplementation(ELogType p_Type, std::u16string_view p_Data)
    {
        std::string l_UTF8Data;
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> l_Convertor;
        l_UTF8Data = l_Convertor.to_bytes(p_Data.begin(), p_Data.end());

        auto l_String = l_UTF8Data.c_str();
        switch (p_Type)
        {
            case ELogType::Error:    m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Error>("{}", l_String);   break;
            case ELogType::Warning:  m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Warn>("{}", l_String);    break;
            case ELogType::Info:     m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Info>("{}", l_String);    break;
            case ELogType::Debug:    m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Debug>("{}", l_String);   break;
        }
    }
    /// @brief Internal log method
    /// @param p_Type Kind
    /// @param p_Data Data
    void PaperLogger::LogImplementation(ELogType p_Type, const std::exception& p_Data)
    {
        auto l_String = p_Data.what();
        switch (p_Type)
        {
            case ELogType::Error:    m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Error>("{}", l_String);   break;
            case ELogType::Warning:  m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Warn>("{}", l_String);    break;
            case ELogType::Info:     m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Info>("{}", l_String);    break;
            case ELogType::Debug:    m_PaperLogger->fmtLog<(Paper::LogLevel)Paper::ffi::paper2_LogLevel::Debug>("{}", l_String);   break;
        }
    }

}   ///< namespace CP_SDK::Logging
#endif