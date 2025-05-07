#pragma once

#include "../Utils/Il2cpp.hpp"

namespace CP_SDK::Chat {

    class CP_SDK_EXPORT Service
    {
        CP_SDK_NO_DEF_CTORS(Service);

        public:
            /// Init
            static void Init();

        public:


        public:
            /// Acquire
            static void Acquire();
            /// Start the services
            static void StartServices();
            /// Release
            /// @p_OnExit: Should release all instances
            static void Release(bool p_OnExit = false);

        public:
            /// @brief Open web configurator
            static void OpenWebConfiguration();

    };

}   ///< namespace CP_SDK::Chat
