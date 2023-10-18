#pragma once

#include <string>
#include <exception>

namespace CP_SDK::Logging {

    /// @brief Base logger
    class ILogger
    {
        public:
            enum class ELogType
            {
                Error,
                Warning,
                Info,
                Debug
            };

        public:
            void Error(std::u16string_view p_Data);
            void Error(const std::exception& p_Data);
            void Warning(std::u16string_view p_Data);
            void Warning(const std::exception& p_Data);
            void Info(std::u16string_view p_Data);
            void Info(const std::exception& p_Data);
            void Debug(std::u16string_view p_Data);
            void Debug(const std::exception& p_Data);

        private:
            /// @brief Internal log method
            /// @param p_Type Kind
            /// @param p_Data Data
            void InternalLog(ELogType p_Type, std::u16string_view p_Data);
            /// @brief Internal log method
            /// @param p_Type Kind
            /// @param p_Data Data
            void InternalLog(ELogType p_Type, const  std::exception& p_Data);

        protected:
            /// @brief Log implementation
            /// @param p_Type Kind
            /// @param p_Data Data
            virtual void LogImplementation(ELogType p_Type, std::u16string_view p_Data) = 0;
            /// @brief Log implementation
            /// @param p_Type Kind
            /// @param p_Data Data
            virtual void LogImplementation(ELogType p_Type, const std::exception& p_Data) = 0;

    };

}   ///< namespace CP_SDK::Logging