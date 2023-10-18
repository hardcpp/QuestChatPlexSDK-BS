#pragma once

#include "HMUIUIUtils.hpp"
#include "../../CP_SDK/Utils/Delegate.hpp"
#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <custom-types/shared/coroutine.hpp>

#include <HMUI/FlowCoordinator.hpp>
#include <HMUI/ViewController.hpp>

#include <tuple>
#include <queue>

#define SDK_UI_FLOW_COORDINATOR_INSTANCE() public: static CP_SDK::Utils::MonoPtr<___TargetType>& Instance() { return _Instance<___TargetType>(); }
#define SDK_UI_FLOW_COORDINATOR_DESTROY()  public: static void                                   Destroy()  {        _Destroy <___TargetType>(); }

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        //using namespace CP_SDK;
        //using namespace CP_SDK::UI;
        using namespace CP_SDK::Utils;
    }

    /// @brief View flow coordinator base class
    class HMUIViewFlowCoordinator : public HMUI::FlowCoordinator
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI", HMUIViewFlowCoordinator, HMUI::FlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_CTOR(HMUIViewFlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(HMUIViewFlowCoordinator);

        private:
            static std::map<System::Type*, _v::MonoPtr<HMUIViewFlowCoordinator>> m_Instances;

        public:
            template<class t_Base> requires(std::is_assignable_v<HMUIViewFlowCoordinator*&, t_Base*>)
            static _v::MonoPtr<t_Base>& _Instance()
            {
                auto l_Type = reinterpret_cast<System::Type*>(csTypeOf(t_Base*));
                if (l_Type && m_Instances.contains(l_Type))
                    return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&m_Instances[l_Type]);

                m_Instances[l_Type] = HMUIUIUtils::CreateFlowCoordinator<t_Base*>();
                return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&m_Instances[l_Type]);
            }
            template<class t_Base> requires(std::is_assignable_v<HMUIViewFlowCoordinator*&, t_Base*>)
            static void _Destroy()
            {
                auto l_Type = reinterpret_cast<System::Type*>(csTypeOf(t_Base*));
                auto l_It   = m_Instances.find(l_Type);
                if (!l_Type || l_It == m_Instances.end())
                    return;

                if (m_Instances[l_Type])
                    _u::GameObject::Destroy(m_Instances[l_Type]->get_gameObject());

                m_Instances.erase(l_It);
            }

        public:
            struct t_Views
            {
                _v::MonoPtr<HMUI::ViewController> Main;
                _v::MonoPtr<HMUI::ViewController> Left;
                _v::MonoPtr<HMUI::ViewController> Right;
            };

        private:
            std::queue<t_Views>                             m_SwitchQueue;
            bool                                            m_IsDequeueEngaged;
            _v::MonoPtr<HMUI::FlowCoordinator>   m_BackupFlowCoordinator;

        protected:
            /// Initial title
            _v::Func<std::u16string_view> Title;

        public:
            /// @brief On activation
            /// @param p_FirstActivation      Is the first activation?
            /// @param p_AddedToHierarchy     Activation type
            /// @param p_ScreenSystemEnabling Is the screen system enabling?
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(HMUI, FlowCoordinator, void, DidActivate, bool p_FirstActivation, bool p_AddedToHierarchy, bool p_ScreenSystemEnabling);
            /// @brief When the back button is pressed
            /// @param p_TopViewController Controller instance
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(HMUI, FlowCoordinator, void, BackButtonWasPressed, HMUI::ViewController* p_TopViewController);

        public:
            /// @brief Present this HMUIViewFlowCoordinator
            /// @param p_IgnoreBackuping Ignore existing flow coordinator
            void Present(bool p_IgnoreBackuping = false);
            /// @brief Dismiss this HMUIViewFlowCoordinator
            void Dismiss();

        protected:
            /// @brief Present this HMUIViewFlowCoordinator
            /// @param p_IgnoreBackuping Ignore existing flow coordinator
            _v::Action<bool> Present_Override;
            /// @brief Dismiss this HMUIViewFlowCoordinator
            _v::Action<>     Dismiss_Override;

        protected:
            /// @brief Get initial views controller (Middle, Left, Right)
            _v::Func<t_Views> GetInitialViewsController;
            /// @brief On back button pressed
            /// @param p_TopViewController Current top view controller
            _v::Func<bool, HMUI::ViewController*> OnBackButtonPressed;

        public:
            /// @brief Hide the left screen
            void HideLeftScreen();
            /// @brief Hide the right screen
            void HideRightScreen();

        public:
            /// @brief Enqueue a main view controller change
            /// @param p_NewView New main view controller
            void ChangeView(HMUI::ViewController* p_NewView, HMUI::ViewController* p_Left = nullptr, HMUI::ViewController* p_Right = nullptr);

        private:
            /// @brief Dequeue a controller change
            void DequeueViewController();
            /// @brief Dequeue a controller change but wait for old one to finish transition
            static custom_types::Helpers::Coroutine DequeueViewControllerWhileOldInTransition(HMUIViewFlowCoordinator* p_Self);

        protected:
            /// @brief Create a view controller
            /// @tparam t_ViewController View controller type
            template<class t_ViewController> requires(std::is_assignable_v<HMUI::ViewController*&, t_ViewController>)
            t_ViewController CreateViewController()
            {
                return HMUIUIUtils::CreateViewController<t_ViewController>();
            }
            /// @brief Destroy view controller
            /// @tparam t_ViewController View type
            /// @param p_ViewController View to destroy
            template<class t_ViewController> requires(std::is_assignable_v<HMUI::ViewController*&, t_ViewController*>)
            static void DestroyViewController(_v::MonoPtr<t_ViewController>* p_ViewController)
            {
                if (*p_ViewController)
                {
                    _u::GameObject::DestroyImmediate((*p_ViewController)->get_gameObject());
                    *p_ViewController = nullptr;
                }
            }

    };

}   ///< namespace CP_SDK_BS::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::HMUIViewFlowCoordinator);