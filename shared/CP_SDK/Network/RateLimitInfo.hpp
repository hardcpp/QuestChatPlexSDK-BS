#pragma once

#include "../Utils/Il2cpp.hpp"
#include "../Utils/MonoPtr.hpp"

#include <System/DateTime.hpp>

#include <string>
#include <stdint.h>

namespace CP_SDK::Network {

    namespace _u
    {
        using namespace System;
    }

    /// @brief Rate Limit Info
    class RateLimitInfo
    {
        CP_SDK_NO_COPYMOVE_CTORS(RateLimitInfo);
        CP_SDK_PRIV_TAG();

        private:
            /// @brief Total allowed requests for a given time window
            int m_Limit;
            /// @brief Number of requests remaining
            int m_Remaining;
            /// @brief Time at which rate limit window resets
            _u::DateTime m_Reset;

        public:
            /// @brief Total allowed requests for a given time window
            int Limit();
            /// @brief Number of requests remaining
            int Remaining();
            /// @brief Time at which rate limit window resets
            _u::DateTime Reset();

        public:


    };

}   ///< namespace CP_SDK::Network