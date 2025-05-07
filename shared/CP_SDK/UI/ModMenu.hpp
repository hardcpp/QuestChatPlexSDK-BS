#pragma once

#include "Components/CFloatingPanel.hpp"
#include "Views/ModMenuView.hpp"
#include "ModButton.hpp"
#include "UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Mod menu
    class CP_SDK_EXPORT ModMenu : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", ModMenu, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(ModMenu);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(ModMenu);

        static ModMenu*                     m_Instance;
        static std::vector<ModButton::Ptr>  m_ModButtons;

        private:
            _v::MonoPtr<_u::Transform>               m_ScreenContainer;
            _v::MonoPtr<Components::CFloatingPanel>  m_Screen;
            _v::MonoPtr<Views::ModMenuView>          m_View;

        public:
            static ModMenu*                             Instance();
            static const std::vector<ModButton::Ptr>&   ModButtons();

            static _v::Event<>                         OnCreated;
            static _v::Event<const ModButton::Ptr&>    OnModButtonRegistered;
            static _v::Event<const ModButton::Ptr&>    OnModButtonChanged;

        public:
            UIFieldRefDel<_u::Transform>                ScreenContainer;
            UIFieldRefDel<Components::CFloatingPanel>   Screen;

        public:
            /// @brief Create the screen system
            static void Create();
            /// @brief Destroy
            static void Destroy();
            /// @brief Register a mod button
            /// @param p_Button Button to register
            static void Register(const ModButton::Ptr& p_Button);
            /// @brief Fire on changed
            /// @param p_Button On button changed
            static void FireOnModButtonChanged(const ModButton::Ptr& p_Button);

        public:
            /// @brief Present the screen system
            void Present();
            /// @brief Dismiss the screen system
            void Dismiss();

        private:
            /// @brief Init the screen system
            void Init();

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::ModMenu);