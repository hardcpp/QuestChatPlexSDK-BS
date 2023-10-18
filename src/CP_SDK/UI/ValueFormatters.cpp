#include "CP_SDK/UI/ValueFormatters.hpp"
#include "CP_SDK/Misc/Time.hpp"

#include <sstream>

#include <System/Math.hpp>

using namespace System;

namespace CP_SDK::UI {

    std::u16string ValueFormatters::Percentage(float p_Value)
    {
        std::ostringstream l_Builder;
        l_Builder.precision(0);
        l_Builder << std::fixed << (p_Value * 100.0f) << "%";

        return Utils::StrToU16Str(l_Builder.str());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string ValueFormatters::Minutes(float p_Value)
    {
        auto l_AsInt = static_cast<int>(p_Value);
        std::ostringstream l_Builder;
        l_Builder << l_AsInt << (l_AsInt > 1 ? " Minutes" : " Minute");

        return Utils::StrToU16Str(l_Builder.str());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string ValueFormatters::TimeShortBaseSeconds(float p_Value)
    {
        const int SECONDS_PER_MINUTE = 60;
        const int SECONDS_PER_HOUR   = 60 * SECONDS_PER_MINUTE;
        const int SECONDS_PER_DAY    = 24 * SECONDS_PER_HOUR;

        int l_TotalSeconds = (int)p_Value;

        int l_Days = l_TotalSeconds / SECONDS_PER_DAY;
        l_TotalSeconds -= l_Days * SECONDS_PER_DAY;

        int l_Hours = l_TotalSeconds / SECONDS_PER_HOUR;
        l_TotalSeconds -= l_Hours * SECONDS_PER_HOUR;

        int l_Minutes = l_TotalSeconds / SECONDS_PER_MINUTE;
        l_TotalSeconds -= l_Minutes * SECONDS_PER_MINUTE;

        std::ostringstream l_Builder;
        if (l_Days > 0)
            l_Builder << l_Days << "d " << l_Hours << "h " << l_Minutes << "m " << l_TotalSeconds << "s";
        else if (l_Hours > 0)
            l_Builder << l_Hours << "h " << l_Minutes << "m " << l_TotalSeconds << "s";
        else if (l_Minutes > 0)
            l_Builder << l_Minutes << "m " << l_TotalSeconds << "s";
        else
            l_Builder << l_TotalSeconds << "s";

        return Utils::StrToU16Str(l_Builder.str());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string ValueFormatters::MillisecondsShort(float p_Value)
    {
        auto l_AsInt = static_cast<int>(p_Value);
        std::ostringstream l_Builder;
        l_Builder << l_AsInt << "ms";

        return Utils::StrToU16Str(l_Builder.str());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string ValueFormatters::DateMonthFrom2018(float p_Value)
    {
        int l_Year = 2018 + (((int)p_Value) / 12);
        std::ostringstream l_Builder;
        l_Builder << Misc::Time_MonthNames[((int)p_Value) % 12] << " " << l_Year;

        return Utils::StrToU16Str(l_Builder.str());
    }
    std::u16string ValueFormatters::DateMonthFrom2018Short(float p_Value)
    {
        int l_Year = 2018 + (((int)p_Value) / 12);
        std::ostringstream l_Builder;
        l_Builder << Misc::Time_MonthNamesShort[((int)p_Value) % 12] << " " << l_Year;

        return Utils::StrToU16Str(l_Builder.str());
    }

}   ///< namespace CP_SDK::UI