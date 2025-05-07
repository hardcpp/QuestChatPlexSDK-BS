#pragma once

#include "../Components/CFLayout.hpp"
#include "../../Utils/Event.hpp"
#include <UnityEngine/UI/ILayoutElement.hpp>
#include <UnityEngine/UI/ILayoutController.hpp>

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CFLayout component
    class CP_SDK_EXPORT DefaultCFLayout : public Components::CFLayout
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCFLayout, Components::CFLayout);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCFLayout);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCFLayout);

        private:
            _v::MonoPtr<_u::RectTransform>              m_RTransform;
            _v::MonoPtr<_u::LayoutElement>              m_LElement;
            EAxis                                       m_StartAxis;
            bool                                        m_ChildForceExpandWidth;
            bool                                        m_ChildForceExpandHeight;
            bool                                        m_ExpandHorizontalSpacing;
            float                                       m_SpacingX;
            float                                       m_SpacingY;
            bool                                        m_InvertOrder;
            float                                       m_LayoutWidth;
            float                                       m_LayoutHeight;
            std::vector<_v::MonoPtr<_u::RectTransform>> m_ItemList;

            template<typename t_Type>
            void SetPropertyCustom(t_Type& p_Current, t_Type& p_New)
            {
                if (p_Current == p_New) return;
                p_Current = p_New;
                SetDirty();
            }

        private:
            bool IsCenterAlign();
            bool IsRightAlign();
            bool IsMiddleAlign();
            bool IsLowerAlign();

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCFLayout();

        public:
            /// @brief Calculate the layout input for horizontal axis
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,    void, CalculateLayoutInputHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutElement, void, CalculateLayoutInputHorizontal);
            /// @brief Calculate the layout input for vertical axis
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,    void, CalculateLayoutInputVertical);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutElement, void, CalculateLayoutInputVertical);

        public:
            /// @brief Set layout horizontal
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,          void, SetLayoutHorizontal);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutController,    void, SetLayoutHorizontal);
            /// @brief Set layout vertical
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, LayoutGroup,          void, SetLayoutVertical);
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, ILayoutController,    void, SetLayoutVertical);

        private:
            /// @brief Get greatest minimum child width
            float GetGreatestMinimumChildWidth();
            /// @brief Get greatest minimum child height
            float GetGreatestMinimumChildHeigth();

        public:
            /// @brief Main layout method
            /// @param p_Axis 0 for horizontal axis, 1 for vertical
            /// @param p_LayoutInput If true, sets the layout input for the axis. If false, sets child position for axis
            float SetLayout(int p_Axis, bool p_LayoutInput);

        private:
            /// @brief Calculate row vertical offset
            /// @param p_GroupHeight      Group height
            /// @param p_YOffset          Y offset
            /// @param p_CurrentRowHeight Current row height
            float CalculateRowVerticalOffset(float p_GroupHeight, float p_YOffset, float p_CurrentRowHeight);
            /// @brief Calculate column horizontal offset
            /// @param p_GroupWidth      Group width
            /// @param p_XOffset         X Offset
            /// @param p_CurrentColWidth Current column width
            float CalculateColHorizontalOffset(float p_GroupWidth, float p_XOffset, float p_CurrentColWidth);

        private:
            /// @brief Layout row
            /// @param p_RowWidth  Row width
            /// @param p_RowHeight Row height
            /// @param p_MaxWidth  Max width
            /// @param p_XOffset   X offset
            /// @param p_YOffset   Y offset
            /// @param p_Axis      Axis
            void LayoutRow(float p_RowWidth, float p_RowHeight, float p_MaxWidth, float p_XOffset, float p_YOffset, int p_Axis);
            /// @brief Layout column
            /// @param p_ColWidth  Column width
            /// @param p_ColHeight Column height
            /// @param p_MaxHeight Max height
            /// @param p_XOffset   X offset
            /// @param p_YOffset   Y offset
            /// @param p_Axis      Axis
            void LayoutCol(float p_ColWidth, float p_ColHeight, float p_MaxHeight, float p_XOffset, float p_YOffset, int p_Axis);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCFLayout);