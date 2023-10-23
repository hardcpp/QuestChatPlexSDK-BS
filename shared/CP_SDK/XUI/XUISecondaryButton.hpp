#pragma once

#include "Generics/XUIPOrSButton.hpp"

namespace CP_SDK::XUI {

    /// @brief CSecondaryButton XUI Element
    class XUISecondaryButton
        : public _XUIPOrSButton<XUISecondaryButton, _v::CSecondaryButton>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUISecondaryButton);
        CP_SDK_PRIV_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::SecondaryButton;
            using Ptr = std::shared_ptr<XUISecondaryButton>;

            XUISecondaryButton(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Label, _v::CActionRef<> p_OnClick)
                : XUISecondaryButton(CP_SDK_PRIV_TAG_FWD(), u"XUISecondaryButton", p_Label, p_OnClick)
            { }
            XUISecondaryButton(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, std::u16string_view p_Label, _v::CActionRef<> p_OnClick)
                : _XUIPOrSButton(p_Name, TypeID, p_Label, p_OnClick)
            { }

        public:
            /// @brief Constructor
            /// @param p_Label   Button label
            /// @param p_OnClick On click
            static Ptr Make(std::u16string_view p_Label, _v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUISecondaryButton>(CP_SDK_PRIV_TAG_VAL(), p_Label, p_OnClick);
            }
            /// @brief Constructor
            /// @param p_Name       Element name
            /// @param p_Label      Button label
            /// @param p_OnClick    On click
            static Ptr Make(std::u16string_view p_Name, std::u16string_view p_Label, _v::CActionRef<> p_OnClick = nullptr)
            {
                return std::make_shared<XUISecondaryButton>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Label, p_OnClick);
            }

    };

}   ///< namespace CP_SDK::XUI