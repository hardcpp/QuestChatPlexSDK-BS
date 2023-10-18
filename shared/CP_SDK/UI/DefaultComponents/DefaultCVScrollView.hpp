#pragma once

#include "Subs/SubVScrollIndicator.hpp"
#include "../Components/CIconButton.hpp"
#include "../Components/CVScrollView.hpp"
#include "../../Utils/Event.hpp"

#include <UnityEngine/UI/Image.hpp>
#include <UnityEngine/UI/VerticalLayoutGroup.hpp>

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

    /// @brief Default CVScrollView component
    class DefaultCVScrollView : public Components::CVScrollView
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.DefaultComponents", DefaultCVScrollView, Components::CVScrollView);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(DefaultCVScrollView, Components::CVScrollView);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(DefaultCVScrollView, Components::CVScrollView);

        private:
            _v::MonoPtr<_u::RectTransform>               m_RTransform;
            _v::MonoPtr<_u::LayoutElement>               m_LElement;
            _v::MonoPtr<Subs::SubVScrollIndicator>       m_VScrollIndicator;
            _v::MonoPtr<Components::CIconButton>         m_UpButton;
            _v::MonoPtr<Components::CIconButton>         m_DownButton;
            _v::MonoPtr<_u::Image>                       m_Handle;
            _v::MonoPtr<_u::RectTransform>               m_ViewPort;
            _v::MonoPtr<_u::RectTransform>               m_VScrollViewContent;
            _v::MonoPtr<_u::VerticalLayoutGroup>         m_VLayoutGroup;
            _v::MonoPtr<_u::RectTransform>               m_Container;

            float                                           m_Smooth;
            float                                           m_DestinationPos;
            float                                           m_ScrollBarWidth;
            float                                           m_LastContentSize;
            bool                                            m_ContentSizeChanged;

            _v::Event<float>                             m_ScrollPositionChangedEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCVScrollView();

        public:
            /// @brief On frame
            DECLARE_INSTANCE_METHOD(void, Update);

        public:
            /// @brief On scroll changed event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnScrollChanged_Impl(_v::CActionRef<float> p_Functor, bool p_Add = true);

        public:
            /// @brief
            void UpdateContentSize_Impl();
            /// @brief
            /// @param p_ContentSize
            void SetContentSize_Impl(float p_ContentSize);

        public:
            /// @brief Scroll to position
            /// @param p_TargetPosition New target position
            /// @param p_Animated       Is animated?
            void ScrollTo_Impl(float p_TargetPosition, bool p_Animated);
            /// @brief Scroll to end
            /// @param p_Animated Is animated?
            void ScrollToEnd_Impl(bool p_Animated);
            /// @brief Refresh scroll buttons
            void RefreshScrollButtons_Impl();

        private:
            /// @brief Set destination scroll position
            /// @param p_Value New destination
            void SetDestinationScrollPos(float p_Value);
            /// @brief Update the vertical scroll indicator
            /// @param p_Position Current position
            void UpdateVScrollIndicator(float p_Position);

        private:
            /// @brief On up button pressed
            void OnUpButton();
            /// @brief On down button pressed
            void OnDownButton();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCVScrollView);