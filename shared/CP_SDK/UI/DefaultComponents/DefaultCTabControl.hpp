#pragma once

#include "../Components/CTabControl.hpp"
#include "../Components/CTextSegmentedControl.hpp"
#include "../Components/CVLayout.hpp"

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

    /// @brief Default CTabControl component
    class DefaultCTabControl : public Components::CTabControl
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCTabControl, Components::CTabControl);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCTabControl);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCTabControl);

        private:
            _v::MonoPtr<_u::RectTransform>                   m_RTransform;
            _v::MonoPtr<_u::LayoutElement>                   m_LElement;
            _v::MonoPtr<Components::CTextSegmentedControl>   m_TextSegmentedControl;
            _v::MonoPtr<Components::CVLayout>                m_Content;

            std::vector<t_TabDefinition> m_Tabs;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCTabControl();

        public:
            /// @brief On active tab changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnActiveChanged_Impl(_v::CActionRef<int> p_Functor, bool p_Add = true);

        public:
            /// @brief Get active tab
            int GetActiveTab_Impl();

        public:
            /// @brief Set active tab
            /// @param p_Index  New active index
            /// @param p_Notify Should notify?
            void SetActiveTab_Impl(int p_Index, bool p_Notify = true);
            /// @brief Set tabs
            /// @param p_Tabs New tabs
            void SetTabs_Impl(const std::vector<t_TabDefinition>& p_Tabs);

        private:
            /// @brief On active text changed
            /// @param p_Index Index of the text
            void TextSegmentedControl_OnActiveChanged(int p_Index);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCTabControl);