#pragma once
#if CP_SDK_BMBF
#include "ILogger.hpp"

#include <beatsaber-hook/shared/utils/logging.hpp>

#include "paper2_scotland2/shared/logger.hpp"

namespace CP_SDK::Logging {

    /// @brief Paper Logger
    class CP_SDK_EXPORT PaperLogger : public ILogger
    {
        public:
            /// @brief Constructor
            /// @param p_Name Paper Logger name
            PaperLogger(const std::string& p_Name);

            /// @brief Get Paper Logger
            Paper::LoggerContext* GetPaperLogger() const { return m_PaperLogger; }

        protected:
            /// @brief Log implementation
            /// @param p_Type Kind
            /// @param p_Data Data
            void LogImplementation(ELogType p_Type, std::u16string_view p_Data) override;
            /// @brief Log implementation
            /// @param p_Type Kind
            /// @param p_Data Data
            void LogImplementation(ELogType p_Type, const std::exception& p_Data) override;

        private:
        Paper::LoggerContext* m_PaperLogger; ///< Paper logger instance

    };

}   ///< namespace CP_SDK::Logging
#endif