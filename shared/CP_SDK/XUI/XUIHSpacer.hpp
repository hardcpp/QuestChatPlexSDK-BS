#pragma once

#include "Generics/XUIHOrVSpacer.hpp"

namespace CP_SDK::XUI {

    /// @brief CHLayout XUI Element
    class XUIHSpacer
        : public _XUIHOrVSpacer<XUIHSpacer, _v::CHLayout>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIHSpacer);
        CP_SDK_PRIV_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::HSpacer;
            using Ptr = std::shared_ptr<XUIHSpacer>;

            XUIHSpacer(CP_SDK_PRIV_TAG_ARG(), float p_Spacing)
                : XUIHSpacer(CP_SDK_PRIV_TAG_FWD(), u"XUIHSpacer", p_Spacing)
            { }
            XUIHSpacer(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, float p_Spacing)
                : _XUIHOrVSpacer(p_Name, TypeID, p_Spacing)
            { }

        public:
            /// @brief Constructor
            /// @param p_Spacing Spacing
            static Ptr Make(float p_Spacing)
            {
                return std::make_shared<XUIHSpacer>(CP_SDK_PRIV_TAG_VAL(), p_Spacing);
            }
            /// @brief Constructor
            /// @param p_Name    Element name
            /// @param p_Spacing Spacing
            static Ptr Make(std::u16string_view p_Name, float p_Spacing)
            {
                return std::make_shared<XUIHSpacer>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Spacing);
            }

    };

}   ///< namespace CP_SDK::XUI