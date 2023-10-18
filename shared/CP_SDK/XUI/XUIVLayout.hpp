#pragma once

#include "Generics/XUIHOrVLayout.hpp"

namespace CP_SDK::XUI {

    /// @brief CVLayout XUI Element
    class XUIVLayout
        : public _XUIHOrVLayout<XUIVLayout, _v::CVLayout>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIVLayout);
        PRIVATE_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::VLayout;
            using Ptr = std::shared_ptr<XUIVLayout>;

            XUIVLayout(PRIVATE_TAG_ARG(), const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : XUIVLayout(PRIVATE_TAG_FWD(), u"XUIVLayout", p_Childs)
            { }
            XUIVLayout(PRIVATE_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : _XUIHOrVLayout(p_Name, TypeID, p_Childs)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                static std::vector<std::shared_ptr<IXUIElement>> s_Empty(0);
                return std::make_shared<XUIVLayout>(PRIVATE_TAG_VAL(), s_Empty);
            }
            /// @brief Constructor
            /// @param p_Childs Child XUI Elements
            static Ptr Make(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIVLayout>(PRIVATE_TAG_VAL(), p_Childs);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_Childs Child XUI Elements
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIVLayout>(PRIVATE_TAG_VAL(), p_Name, p_Childs);
            }

    };

}   ///< namespace CP_SDK::XUI