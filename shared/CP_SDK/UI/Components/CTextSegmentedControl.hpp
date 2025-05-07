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

    /// @brief CTextSegmentedControl component
    class CP_SDK_EXPORT CTextSegmentedControl : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CTextSegmentedControl, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CTextSegmentedControl);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CTextSegmentedControl);

        protected:
            _v::Action<_v::CActionRef<int>, bool> m_OnActiveChanged;

            _v::Func<int> m_GetActiveText;
            _v::Func<int> m_GetTextCount;

            _v::Action<int, bool>                            m_SetActiveText;
            _v::Action<const std::vector<std::u16string>&>   m_SetTexts;

        public:
            UIFieldRefDel<_u::RectTransform>        RTransform;
            UIFieldRefDel<_u::ContentSizeFitter>    CSizeFitter;
            UIFieldRefDel<_u::LayoutElement>        LElement;

        public:
            /// @brief On active text changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CTextSegmentedControl* OnActiveChanged(_v::CActionRef<int> p_Functor, bool p_Add = true);

        public:
            /// @brief Get active text
            int GetActiveText();
            /// @brief Get text count
            int GetTextCount();

        public:
            /// @brief Set active text
            /// @param p_Index  New active index
            /// @param p_Notify Should notify?
            CTextSegmentedControl* SetActiveText(int p_Index, bool p_Notify = true);
            /// @brief Set texts
            /// @param p_Texts New texts
            CTextSegmentedControl* SetTexts(const std::vector<std::u16string>& p_Texts);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CTextSegmentedControl);