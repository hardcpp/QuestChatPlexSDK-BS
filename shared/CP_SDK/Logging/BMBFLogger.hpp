#pragma once
#if CP_SDK_BMBF
#include "ILogger.hpp"

#include <beatsaber-hook/shared/utils/logging.hpp>

namespace CP_SDK::Logging {

    /// @brief BMBF Logger
    class BMBFLogger : public ILogger
    {
        public:
            /// @brief Constructor
            /// @param p_BMBFLogger BMBF logger instance
            BMBFLogger(Logger* p_BMBFLogger);

            /// @brief Get BMBF Logger
            Logger* GetBMBFLogger() const { return m_BMBFLogger; }

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
            Logger* m_BMBFLogger; ///< BMBF logger instance

    };

}   ///< namespace CP_SDK::Logging
#endif