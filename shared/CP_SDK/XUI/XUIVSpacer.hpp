#pragma once

#include "Generics/XUIHOrVSpacer.hpp"

namespace CP_SDK::XUI {

    /// @brief CVLayout XUI Element
    class XUIVSpacer
        : public _XUIHOrVSpacer<XUIVSpacer, _v::CVLayout>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIVSpacer);
        CP_SDK_PRIV_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::VSpacer;
            using Ptr = std::shared_ptr<XUIVSpacer>;

            XUIVSpacer(CP_SDK_PRIV_TAG_ARG(), float p_Spacing)
                : XUIVSpacer(CP_SDK_PRIV_TAG_FWD(), u"XUIVSpacer", p_Spacing)
            { }
            XUIVSpacer(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, float p_Spacing)
                : _XUIHOrVSpacer(p_Name, TypeID, p_Spacing)
            { }

        public:
            /// @brief Constructor
            /// @param p_Spacing Spacing
            static Ptr Make(float p_Spacing)
            {
                return std::make_shared<XUIVSpacer>(CP_SDK_PRIV_TAG_VAL(), p_Spacing);
            }
            /// @brief Constructor
            /// @param p_Name    Element name
            /// @param p_Spacing Spacing
            static Ptr Make(std::u16string_view p_Name, float p_Spacing)
            {
                return std::make_shared<XUIVSpacer>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Spacing);
            }

    };

}   ///< namespace CP_SDK::XUI