#pragma once

#include "../Utils/Il2cpp.hpp"

#include <array>
#include <string>
#include <stdint.h>

#include <System/DateTime.hpp>

namespace CP_SDK::Misc {

    namespace _u
    {
        using namespace System;
    }

    const std::array<const std::string, 12> Time_MonthNames = {
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
    };
    const std::array<const std::u16string, 12> Time_MonthNamesU16 = {
        u"January", u"February", u"March", u"April", u"May", u"June", u"July", u"August", u"September", u"October", u"November", u"December"
    };
    const std::array<const std::string, 12> Time_MonthNamesShort = {
        "Jan.", "Feb.", "Mar.", "Apr.", "May", "Jun.", "Jul.", "Aug.", "Sept.", "Oct.", "Nov.", "Dec."
    };
    const std::array<const std::u16string, 12> Time_MonthNamesShortU16 = {
        u"Jan.", u"Feb.", u"Mar.", u"Apr.", u"May", u"Jun.", u"Jul.", u"Aug.", u"Sept.", u"Oct.", u"Nov.", u"Dec."
    };

    /// @brief Time helper
    class Time
    {
        DISABLE_CONSTRUCTORS(Time);

        private:
            static bool         s_Init;
            static _u::DateTime s_UnixEpoch;

        public:
            /// @brief Get UnixTimestamp
            /// @return Unix timestamp
            static int64_t UnixTimeNow();
            /// @brief Get UnixTimestamp
            /// @return Unix timestamp
            static int64_t UnixTimeNowMS();
            /// @brief Convert DateTime to UnixTimestamp
            /// @param p_DateTime The DateTime to convert
            static int64_t ToUnixTime(_u::DateTime p_DateTime);
            /// @brief Convert DateTime to UnixTimestamp
            /// @param p_DateTime The DateTime to convert
            static int64_t ToUnixTimeMS(_u::DateTime p_DateTime);
            /// @brief Convert UnixTimestamp to DateTime
            /// @param p_TimeStamp
            static _u::DateTime FromUnixTime(int64_t p_TimeStamp);
            /// @brief Convert UnixTimestamp to DateTime
            /// @param p_TimeStamp
            static _u::DateTime FromUnixTimeMS(int64_t p_TimeStamp);
            /// @brief Try parse international data
            /// @param p_Input
            /// @param p_Result
            static bool TryParseInternational(std::u16string_view p_Input, ByRef<_u::DateTime> p_Result);

    };

}   ///< namespace CP_SDK::Misc