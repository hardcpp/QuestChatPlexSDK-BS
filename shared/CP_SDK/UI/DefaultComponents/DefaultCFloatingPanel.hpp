#pragma once

#include "../Components/CFloatingPanel.hpp"
#include "../../Utils/Event.hpp"

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

    /// @brief Default CFloatingPanel component
    class DefaultCFloatingPanel : public Components::CFloatingPanel
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.DefaultComponents", DefaultCFloatingPanel, Components::CFloatingPanel);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(DefaultCFloatingPanel, Components::CFloatingPanel);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(DefaultCFloatingPanel, Components::CFloatingPanel);

        private:
            _v::MonoPtr<_u::RectTransform>                          m_RTransform;
            _v::Event<_v::CMonoPtrRef<Components::CFloatingPanel>>  m_OnReleaseEvent;
            _v::Event<_v::CMonoPtrRef<Components::CFloatingPanel>>  m_OnGrabEvent;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCFloatingPanel();

        public:
            /// @brief When grabbed
            void FireOnGrab();
            /// @brief When released
            void FireOnRelease();

        public:
            /// @brief On grab event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnGrab_Impl(_v::CActionRef<_v::CMonoPtrRef<Components::CFloatingPanel>> p_Functor, bool p_Add = true);
            /// @brief On release event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            void OnRelease_Impl(_v::CActionRef<_v::CMonoPtrRef<Components::CFloatingPanel>> p_Functor, bool p_Add = true);

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCFloatingPanel);