#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectOffset.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>

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

    /// @brief CVScrollView component
    class CP_SDK_EXPORT CVScrollView : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CVScrollView, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CVScrollView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CVScrollView);

        public:
            /// @brief Scroll type enum
            enum class EScrollType
            {
                PageSize,
                FixedCellSize
            };

        protected:
            _v::Action<_v::CActionRef<float>, bool> m_OnScrollChanged;

            _v::Action<>             m_UpdateContentSize;
            _v::Action<float>        m_SetContentSize;

            _v::Action<float, bool>  m_ScrollTo;
            _v::Action<bool>         m_ScrollToEnd;
            _v::Action<>             m_RefreshScrollButtons;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            UIFieldRefDel<_u::RectTransform>    Container;

            EScrollType                 ScrollType;
            float                       FixedCellSize;
            float                       PageStepNormalizedSize;

            _v::Delegate<float()>    Position;
            _v::Delegate<float()>    ViewPortWidth;
            _v::Delegate<float()>    ScrollableSize;
            _v::Delegate<float()>    ScrollPageSize;
            _v::Delegate<float()>    ContentSize;

        public:
            /// @brief On scroll changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CVScrollView* OnScrollChanged(_v::CActionRef<float> p_Functor, bool p_Add = true);

        public:
            /// @brief
            CVScrollView* UpdateContentSize();
            /// @brief
            /// @param p_ContentSize
            CVScrollView* SetContentSize(float p_ContentSize);

        public:
            /// @brief Scroll to position
            /// @param p_TargetPosition New target position
            /// @param p_Animated       Is animated?
            CVScrollView* ScrollTo(float p_TargetPosition, bool p_Animated);
            /// @brief Scroll to end
            /// @param p_Animated Is animated?
            CVScrollView* ScrollToEnd(bool p_Animated);
            /// @brief Refresh scroll buttons
            CVScrollView* RefreshScrollButtons();

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CVScrollView);