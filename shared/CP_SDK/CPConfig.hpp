#pragma once

#include "Config/JsonConfig.hpp"

namespace CP_SDK {

    /// @brief ChatPlex SDK config
    class CP_SDK_EXPORT CPConfig : public Config::JsonConfig
    {
        CP_SDK_CONFIG_JSONCONFIG_INSTANCE_DECL(CPConfig);

        public:
            bool FirstRun               = true;
            bool FirstChatServiceRun    = true;

        protected:
            /// @brief Reset config to default
            void Reset_Impl() override;

        protected:
            /// @brief Write the document
            /// @param p_Document Target
            CP_SDK_JSON_SERIALIZE_DECL() override;
            /// @brief Read the document
            /// @param p_Document Source
            CP_SDK_JSON_UNSERIALIZE_DECL() override;

        protected:
            /// @brief Get relative config path
            virtual std::filesystem::path GetRelativePath() override;

        protected:
            /// @brief On config init
            /// @param p_OnCreation On creation
            virtual void OnInit(bool p_OnCreation) override;

    };

}   ///< namespace CP_SDK