#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/UI/ContentSizeFitter.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>

#include <string>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief CTabControl component
    class CP_SDK_EXPORT CTabControl : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CTabControl, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CTabControl);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CTabControl);

        public:
            using t_TabDefinition = std::tuple<std::u16string, _u::RectTransform*>;

        protected:
            _v::Action<_v::CActionRef<int>, bool> m_OnActiveChanged;

            _v::Func<int> m_GetActiveTab;

            _v::Action<int, bool>                            m_SetActiveTab;
            _v::Action<const std::vector<t_TabDefinition>&>  m_SetTabs;

        public:
            UIFieldRefDel<_u::RectTransform>        RTransform;
            UIFieldRefDel<_u::LayoutElement>        LElement;

        public:
            /// @brief On active text changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CTabControl* OnActiveChanged(_v::CActionRef<int> p_Functor, bool p_Add = true);

        public:
            /// @brief Get active tab
            int GetActiveTab();

        public:
            /// @brief Set active tab
            /// @param p_Index  New active index
            /// @param p_Notify Should notify?
            CTabControl* SetActiveTab(int p_Index, bool p_Notify = true);
            /// @brief Set tabs
            /// @param p_Tabs New tabs
            CTabControl* SetTabs(const std::vector<t_TabDefinition>& p_Tabs);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CTabControl);