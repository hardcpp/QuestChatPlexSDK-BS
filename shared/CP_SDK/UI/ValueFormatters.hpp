#pragma once

#include "../Utils/Il2cpp.hpp"

#include <string>

namespace CP_SDK::UI {

    class CP_SDK_EXPORT_VISIBILITY ValueFormatters
    {
        public:
            static std::u16string Percentage(float p_Value);

            static std::u16string Minutes(float p_Value);

            static std::u16string TimeShortBaseSeconds(float p_Value);

            static std::u16string MillisecondsShort(float p_Value);

            static std::u16string DateMonthFrom2018(float p_Value);
            static std::u16string DateMonthFrom2018Short(float p_Value);

    };

}   ///< namespace CP_SDK::UI