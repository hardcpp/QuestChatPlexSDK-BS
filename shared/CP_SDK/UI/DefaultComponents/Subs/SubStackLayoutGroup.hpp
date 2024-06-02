#pragma once

#include "../../../Utils/Il2cpp.hpp"

#include <UnityEngine/UI/LayoutGroup.hpp>
#include <UnityEngine/UI/ILayoutElement.hpp>
#include <UnityEngine/UI/ILayoutController.hpp>

namespace CP_SDK::UI::DefaultComponents::Subs {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Stack layout group
    class SubStackLayoutGroup : public _u::LayoutGroup
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK::UI::DefaultComponents::Subs", SubStackLayoutGroup, _u::LayoutGroup);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(SubStackLayoutGroup);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(SubStackLayoutGroup);
        CP_SDK_IL2CPP_CTOR_FIELD();
        CP_SDK_IL2CPP_DTOR_FIELD();

        private:
            bool m_ChildForceExpandWidth;
            bool m_ChildForceExpandHeight;

        public:
            /// @brief Get child force expand width
            bool GetChildForceExpandWidth() const;
            /// @brief Get child force expand height
            bool GetChildForceExpandHeight() const;

            /// @brief Set child force expand width
            /// @param p_Value New value
            void SetChildForceExpandWidth(bool p_Value);
            /// @brief Set child force expand height
            /// @param p_Value New value
            void SetChildForceExpandHeight(bool p_Value);

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,    void, CalculateLayoutInputHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutElement, void, CalculateLayoutInputHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,    void, CalculateLayoutInputVertical);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutElement, void, CalculateLayoutInputVertical);

        public:
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,          void, SetLayoutHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutController,    void, SetLayoutHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,          void, SetLayoutVertical);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutController,    void, SetLayoutVertical);

        private:
            /// @brief Update the layout group for given axis
            /// @param p_Axis 0 Horizontal 1 vertical
            void UpdateForAxis(int p_Axis);
            /// @brief Set childrens along axis for given axis
            /// @param p_Axis 0 Horizontal 1 vertical
            void SetChildrensAlongAxis(int p_Axis);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::Subs::SubStackLayoutGroup);