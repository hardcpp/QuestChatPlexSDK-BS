#pragma once

#include "Generics/XUIHOrVSpacer.hpp"

namespace CP_SDK::XUI {

    /// @brief CHLayout XUI Element
    class XUIHSpacer
        : public _XUIHOrVSpacer<XUIHSpacer, _v::CHLayout>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIHSpacer);
        PRIVATE_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::HSpacer;
            using Ptr = std::shared_ptr<XUIHSpacer>;

            XUIHSpacer(PRIVATE_TAG_ARG(), float p_Spacing)
                : XUIHSpacer(PRIVATE_TAG_FWD(), u"XUIHSpacer", p_Spacing)
            { }
            XUIHSpacer(PRIVATE_TAG_ARG(), std::u16string_view p_Name, float p_Spacing)
                : _XUIHOrVSpacer(p_Name, TypeID, p_Spacing)
            { }

        public:
            /// @brief Constructor
            /// @param p_Spacing Spacing
            static Ptr Make(float p_Spacing)
            {
                return std::make_shared<XUIHSpacer>(PRIVATE_TAG_VAL(), p_Spacing);
            }
            /// @brief Constructor
            /// @param p_Name    Element name
            /// @param p_Spacing Spacing
            static Ptr Make(std::u16string_view p_Name, float p_Spacing)
            {
                return std::make_shared<XUIHSpacer>(PRIVATE_TAG_VAL(), p_Name, p_Spacing);
            }

    };

}   ///< namespace CP_SDK::XUI