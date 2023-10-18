#pragma once

#include "Generics/XUIPOrSButton.hpp"

namespace CP_SDK::XUI {

    /// @brief CPrimaryButton XUI Element
    class XUIPrimaryButton
        : public _XUIPOrSButton<XUIPrimaryButton, _v::CPrimaryButton>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIPrimaryButton);
        PRIVATE_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::PrimaryButton;
            using Ptr = std::shared_ptr<XUIPrimaryButton>;

            XUIPrimaryButton(PRIVATE_TAG_ARG(), std::u16string_view p_Label, _v::CActionRef<> p_OnClick)
                : XUIPrimaryButton(PRIVATE_TAG_FWD(), u"XUIPrimaryButton", p_Label, p_OnClick)
            { }
            XUIPrimaryButton(PRIVATE_TAG_ARG(), std::u16string_view p_Name, std::u16string_view p_Label, _v::CActionRef<> p_OnClick = nullptr)
                : _XUIPOrSButton(p_Name, TypeID, p_Label, p_OnClick)
            { }

        public:
            /// @brief Constructor
            /// @param p_Label   Button label
            /// @param p_OnClick On click
            static Ptr Make(std::u16string_view p_Label, _v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUIPrimaryButton>(PRIVATE_TAG_VAL(), p_Label, p_OnClick);
            }
            /// @brief Constructor
            /// @param p_Name       Element name
            /// @param p_Label      Button label
            /// @param p_OnClick    On click
            static Ptr Make(std::u16string_view p_Name, std::u16string_view p_Label, _v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUIPrimaryButton>(PRIVATE_TAG_VAL(), p_Name, p_Label, p_OnClick);
            }

    };

}   ///< namespace CP_SDK::XUI