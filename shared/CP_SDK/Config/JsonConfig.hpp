#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/Json.hpp"

#include <string>
#include <filesystem>

#define CP_SDK_CONFIG_JSONCONFIG_INSTANCE_DECL(__Type)      \
    private:                                                \
        DISABLE_COPY_MOVECONSTRUCTORS(__Type)               \
        __Type() = default;                                 \
        static __Type* m_Instance;                          \
    public:                                                 \
        static __Type* Instance();

#define CP_SDK_CONFIG_JSONCONFIG_INSTANCE_IMPL(__Type)      \
    __Type* __Type::m_Instance = nullptr;                   \
    __Type* __Type::Instance() {                            \
        if (!m_Instance)                                    \
        {                                                   \
            m_Instance = new __Type();                      \
            m_Instance->Init();                             \
        }                                                   \
        return m_Instance;                                  \
    }

namespace CP_SDK::Config {

    /// @brief Json config file
    class JsonConfig
    {
        DISABLE_COPY_MOVECONSTRUCTORS(JsonConfig);

        protected:
            /// @brief Constructor
            JsonConfig();
            /// @brief Destructor
            virtual ~JsonConfig();

        public:
            /// @brief Dummy method for warmup
            void Warmup();
            /// @brief Reset config to default
            void Reset();
            /// @brief Save config file
            void Save();

        protected:
            /// @brief Reset config to default
            virtual void Reset_Impl() = 0;

        protected:
            /// @brief Read the document
            /// @param p_Document Source
            virtual CP_SDK_JSON_SERIALIZE_DECL() = 0;
            /// @brief Write the document
            /// @param p_Document Target
            virtual CP_SDK_JSON_UNSERIALIZE_DECL() = 0;

        protected:
            /// @brief Get relative config path
            virtual std::filesystem::path GetRelativePath() = 0;
            /// @brief Get full config path
            virtual std::filesystem::path GetFullPath();

        protected:
            /// @brief On config init
            /// @param p_OnCreation On creation
            virtual void OnInit(bool p_OnCreation) = 0;

        protected:
            /// @brief Init config
            void Init();
            /// @brief Write file
            /// @param p_FullPath File path
            void WriteFile(std::filesystem::path p_FullPath);

    };

}   ///< namespace CP_SDK::Config