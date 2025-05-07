#pragma once

#include "IListCell.hpp"
#include "../Components/CText.hpp"

namespace CP_SDK::UI::Data {

    /// @brief Text list cell
    class CP_SDK_EXPORT TextListCell : public IListCell
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Data", TextListCell, IListCell);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(TextListCell);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(TextListCell);

        private:
            Utils::MonoPtr<Components::CText> m_Text;

        public:
            UIFieldRefDel<Components::CText> Text;

        public:
            /// @brief Build cell
            void Build_Impl();
            /// @brief Get cell height
            float GetCellHeight_Impl();

        protected:
            /// @brief Add self component
            /// @param p_Target Target gameobject
            IListCell* AddSelfComponent_Impl(_u::GameObject* p_Target);

    };

}   ///< namespace CP_SDK::UI::Data

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Data::TextListCell);