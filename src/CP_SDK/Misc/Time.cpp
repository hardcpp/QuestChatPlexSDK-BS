#include "CP_SDK/Misc/Time.hpp"

#include <System/Globalization/DateTimeStyles.hpp>
#include <System/Globalization/CultureInfo.hpp>
#include <System/DateTimeKind.hpp>
#include <System/TimeSpan.hpp>

namespace CP_SDK::Misc {

    bool            Time::s_Init      = false;
    _u::DateTime    Time::s_UnixEpoch;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get UnixTimestamp
    /// @return Unix timestamp
    int64_t Time::UnixTimeNow()
    {
        if (!s_Init) { s_UnixEpoch = _u::DateTime::getStaticF_UnixEpoch(); s_Init = true; }
        return static_cast<int64_t>((_u::DateTime::get_UtcNow().Subtract(s_UnixEpoch)).get_TotalSeconds());
    }
    /// @brief Get UnixTimestamp
    /// @return Unix timestamp
    int64_t Time::UnixTimeNowMS()
    {
        if (!s_Init) { s_UnixEpoch = _u::DateTime::getStaticF_UnixEpoch(); s_Init = true; }
        return static_cast<int64_t>((_u::DateTime::get_UtcNow().Subtract(s_UnixEpoch)).get_TotalMilliseconds());
    }
    /// @brief Convert DateTime to UnixTimestamp
    /// @param p_DateTime The DateTime to convert
    int64_t Time::ToUnixTime(_u::DateTime p_DateTime)
    {
        if (!s_Init) { s_UnixEpoch = _u::DateTime::getStaticF_UnixEpoch(); s_Init = true; }
        return static_cast<int64_t>(p_DateTime.ToUniversalTime().Subtract(s_UnixEpoch).get_TotalSeconds());
    }
    /// @brief Convert DateTime to UnixTimestamp
    /// @param p_DateTime The DateTime to convert
    int64_t Time::ToUnixTimeMS(_u::DateTime p_DateTime)
    {
        if (!s_Init) { s_UnixEpoch = _u::DateTime::getStaticF_UnixEpoch(); s_Init = true; }
        return static_cast<int64_t>(p_DateTime.ToUniversalTime().Subtract(s_UnixEpoch).get_TotalMilliseconds());
    }
    /// @brief Convert UnixTimestamp to DateTime
    /// @param p_TimeStamp
    _u::DateTime Time::FromUnixTime(int64_t p_TimeStamp)
    {
        if (!s_Init) { s_UnixEpoch = _u::DateTime::getStaticF_UnixEpoch(); s_Init = true; }
        return s_UnixEpoch.AddSeconds(p_TimeStamp).ToLocalTime();
    }
    /// @brief Convert UnixTimestamp to DateTime
    /// @param p_TimeStamp
    _u::DateTime Time::FromUnixTimeMS(int64_t p_TimeStamp)
    {
        if (!s_Init) { s_UnixEpoch = _u::DateTime::getStaticF_UnixEpoch(); s_Init = true; }
        return s_UnixEpoch.AddMilliseconds(p_TimeStamp).ToLocalTime();
    }
    /// @brief Try parse international data
    /// @param p_Input
    /// @param p_Result
    bool Time::TryParseInternational(std::u16string_view p_Input, ByRef<_u::DateTime> p_Result)
    {
        return _u::DateTime::TryParse(
            p_Input,
            reinterpret_cast<_u::IFormatProvider*>(System::Globalization::CultureInfo::get_InvariantCulture()),
            System::Globalization::DateTimeStyles::RoundtripKind,
            p_Result
        );
    }

}   ///< namespace CP_SDK::Misc