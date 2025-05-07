#pragma once

#include "IViewController.hpp"

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Flow coordinator interface
    class CP_SDK_EXPORT IFlowCoordinator : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", IFlowCoordinator, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(IFlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(IFlowCoordinator);

        public:
            struct t_InitialViews
            {
                _v::MonoPtr<IViewController> Main;
                _v::MonoPtr<IViewController> Left;
                _v::MonoPtr<IViewController> Right;
            };

        private:
            bool                            m_FirstActivation;
            _v::MonoPtr<IFlowCoordinator>   m_BackupFlowCoordinator;
            _v::MonoPtr<IViewController>    m_LeftViewController;
            _v::MonoPtr<IViewController>    m_MainViewController;
            _v::MonoPtr<IViewController>    m_RightViewController;

        protected:
            _v::Func<std::u16string_view>                       m_Title;

            _v::Action<>                                        m_Init;

            _v::Func<t_InitialViews>                            m_GetInitialViewsController;
            _v::Func<bool, _v::CMonoPtrRef<IViewController>>    m_OnBackButtonPressed;

        public:
            std::u16string_view         Title();
            UIFieldRef<IViewController> LeftViewController();
            UIFieldRef<IViewController> MainViewController();
            UIFieldRef<IViewController> RightViewController();

        public:
            /// @brief Activate
            void __Activate();
            /// @brief Deactivate
            void __Deactivate();

        public:
            /// @brief Init
            void Init();

        public:
            /// @brief Present this FlowCoordinator
            /// @param p_IgnoreBackuping Ignore existing flow coordinator
            void Present(bool p_IgnoreBackuping = false);
            /// @brief Dismiss this FlowCoordinator
            void Dismiss();

        protected:
            /// @brief Get initial views controller
            /// @return (Main, Left, Right)
            t_InitialViews GetInitialViewsController();

        public:
            /// @brief On back button pressed
            /// @param p_MainViewController Current main view controller
            /// @return True if the event is catched, false if we should dismiss the flow coordinator
            bool OnBackButtonPressed(_v::CMonoPtrRef<IViewController> p_MainViewController);

        public:
            /// @brief Change view controllers
            /// @param p_MainViewController  New main view controller
            /// @param p_LeftViewController  New left view controller
            /// @param p_RightViewController New right view controller
            void ChangeViewControllers(_v::CMonoPtrRef<IViewController> p_MainViewController, _v::CMonoPtrRef<IViewController> p_LeftViewController = nullptr, _v::CMonoPtrRef<IViewController> p_RightViewController = nullptr);

        protected:
            /// @brief Set left view controller
            /// @param p_ViewController New view controller
            void SetLeftViewController(_v::CMonoPtrRef<IViewController> p_ViewController);
            /// @brief Set main view controller
            /// @param p_ViewController New view controller
            void SetMainViewController(_v::CMonoPtrRef<IViewController> p_ViewController);
            /// @brief Set left view controller
            /// @param p_ViewController New view controller
            void SetRightViewController(_v::CMonoPtrRef<IViewController> p_ViewController);

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::IFlowCoordinator);