#include "CP_SDK/Logging/ILogger.hpp"

namespace CP_SDK::Logging {

    void ILogger::Error(std::u16string_view p_Data)         { InternalLog(ELogType::Error,      p_Data); }
    void ILogger::Error(const std::exception& p_Data)       { InternalLog(ELogType::Error,      p_Data); }
    void ILogger::Warning(std::u16string_view p_Data)       { InternalLog(ELogType::Warning,    p_Data); }
    void ILogger::Warning(const std::exception& p_Data)     { InternalLog(ELogType::Warning,    p_Data); }
    void ILogger::Info(std::u16string_view p_Data)          { InternalLog(ELogType::Info,       p_Data); }
    void ILogger::Info(const std::exception& p_Data)        { InternalLog(ELogType::Info,       p_Data); }
    void ILogger::Debug(std::u16string_view p_Data)         { InternalLog(ELogType::Debug,      p_Data); }
    void ILogger::Debug(const std::exception& p_Data)       { InternalLog(ELogType::Debug,      p_Data); }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Internal log method
    /// @param p_Type Kind
    /// @param p_Data Data
    void ILogger::InternalLog(ELogType p_Type, std::u16string_view p_Data)
    {
        LogImplementation(p_Type, p_Data);
    }
    /// @brief Internal log method
    /// @param p_Type Kind
    /// @param p_Data Data
    void ILogger::InternalLog(ELogType p_Type, const std::exception& p_Data)
    {
        LogImplementation(p_Type, p_Data);
    }

}   ///< namespace CP_SDK::Logging