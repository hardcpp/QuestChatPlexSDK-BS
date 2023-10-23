#pragma once

#include "Generics/XUIHOrVLayout.hpp"

namespace CP_SDK::XUI {

    /// @brief CHLayout XUI Element
    class XUIHLayout
        : public _XUIHOrVLayout<XUIHLayout, _v::CHLayout>
    {
        CP_SDK_NO_COPYMOVE_CTORS(XUIHLayout);
        CP_SDK_PRIV_TAG();

        public:
            static constexpr ETypeID TypeID = ETypeID::HLayout;
            using Ptr = std::shared_ptr<XUIHLayout>;

            XUIHLayout(CP_SDK_PRIV_TAG_ARG(), const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : XUIHLayout(CP_SDK_PRIV_TAG_FWD(), u"XUIHLayout", p_Childs)
            { }
            XUIHLayout(CP_SDK_PRIV_TAG_ARG(), std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : _XUIHOrVLayout(p_Name, TypeID, p_Childs)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                static std::vector<std::shared_ptr<IXUIElement>> s_Empty(0);
                return std::make_shared<XUIHLayout>(CP_SDK_PRIV_TAG_VAL(), s_Empty);
            }
            /// @brief Constructor
            /// @param p_Childs Child XUI Elements
            static Ptr Make(const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIHLayout>(CP_SDK_PRIV_TAG_VAL(), p_Childs);
            }
            /// @brief Constructor
            /// @param p_Name Element name
            /// @param p_Childs Child XUI Elements
            static Ptr Make(std::u16string_view p_Name, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
            {
                return std::make_shared<XUIHLayout>(CP_SDK_PRIV_TAG_VAL(), p_Name, p_Childs);
            }

    };

}   ///< namespace CP_SDK::XUI