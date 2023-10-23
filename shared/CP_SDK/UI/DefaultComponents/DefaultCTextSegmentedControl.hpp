#pragma once

#include "../Components/CTextSegmentedControl.hpp"
#include "../../Utils/Event.hpp"

#include <UnityEngine/UI/Button.hpp>

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

    /// @brief Default CTextSegmentedControl component
    class DefaultCTextSegmentedControl : public Components::CTextSegmentedControl
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCTextSegmentedControl, Components::CTextSegmentedControl);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCTextSegmentedControl);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCTextSegmentedControl);

        private:
            _v::MonoPtr<_u::RectTransform>          m_RTransform;
            _v::MonoPtr<_u::ContentSizeFitter>      m_CSizeFitter;
            _v::MonoPtr<_u::LayoutElement>          m_LElement;
            std::vector<_v::MonoPtr<_u::Button>>    m_Controls;

            int                                 m_ActiveControl;

            _v::Event<int>                      m_OnActiveChangedEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCTextSegmentedControl();

        public:
            /// @brief On active text changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnActiveChanged_Impl(_v::CActionRef<int> p_Functor, bool p_Add = true);

        public:
            /// @brief Get active text
            int GetActiveText_Impl();
            /// @brief Get text count
            int GetTextCount_Impl();

        public:
            /// @brief Set active text
            /// @param p_Index  New active index
            /// @param p_Notify Should notify?
            void SetActiveText_Impl(int p_Index, bool p_Notify = true);
            /// @brief Set texts
            /// @param p_Texts New texts
            void SetTexts_Impl(const std::vector<std::u16string>& p_Texts);

        private:
            /// @brief On control clicked
            /// @param p_Button Clicked control's button
            /// @param p_Notify Should notify callback?
            void OnControlClicked(_u::Button* p_Button, bool p_Notify);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCTextSegmentedControl);