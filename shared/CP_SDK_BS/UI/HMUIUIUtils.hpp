#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <GlobalNamespace/MainFlowCoordinator.hpp>
#include <HMUI/FlowCoordinator.hpp>
#include <HMUI/ViewController.hpp>
#include <System/Type.hpp>
#include <UnityEngine/Canvas.hpp>
#include <VRUIControls/PhysicsRaycasterWithCache.hpp>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace GlobalNamespace;
        using namespace UnityEngine;
        using namespace VRUIControls;
    }

    /// @brief View controller utils
    class CP_SDK_EXPORT_VISIBILITY HMUIUIUtils
    {
        CP_SDK_NO_DEF_CTORS(HMUIUIUtils);

        private:
            static CP_SDK::Utils::MonoPtr<_u::MainFlowCoordinator>          m_MainFlowCoordinator;
            static CP_SDK::Utils::MonoPtr<_u::Canvas>                       m_CanvasTemplate;
            static CP_SDK::Utils::MonoPtr<_u::PhysicsRaycasterWithCache>    m_PhysicsRaycaster;

        public:
            static _u::MainFlowCoordinator* MainFlowCoordinator();

        public:
            /// @brief Create a flow coordinator
            /// @tparam t_Base Flow coordinator type
            template<class t_Base> requires(std::is_assignable_v<HMUI::FlowCoordinator*&, t_Base>)
            static t_Base CreateFlowCoordinator()
            {
                return reinterpret_cast<t_Base>(CreateFlowCoordinator(csTypeOf(t_Base)));
            }
            /// @brief Create a flow coordinator
            /// @param p_Type Flow coordinator type
            static HMUI::FlowCoordinator* CreateFlowCoordinator(System::Type* p_Type);

            /// @brief Create a view controller
            /// @tparam t_Base View controller type
            template<class t_Base> requires(std::is_assignable_v<HMUI::ViewController*&, t_Base>)
            static t_Base CreateViewController()
            {
                return reinterpret_cast<t_Base>(CreateViewController(csTypeOf(t_Base)));
            }
            /// @brief Create a view controller
            /// @param p_Type View controller type
            static HMUI::ViewController* CreateViewController(System::Type* p_Type);

    };

}   ///< namespace CP_SDK_BS::UI