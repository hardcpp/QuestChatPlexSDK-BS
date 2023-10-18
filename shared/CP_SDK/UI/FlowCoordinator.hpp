#pragma once

#include "IFlowCoordinator.hpp"

#include <map>

#include <UnityEngine/GameObject.hpp>

#define CP_SDK_UI_FLOW_COORDINATOR_INSTANCE() public: static CP_SDK::Utils::MonoPtr<___TargetType>& Instance() { return _Instance<___TargetType>(); }
#define CP_SDK_UI_FLOW_COORDINATOR_DESTROY()  public: static void                                   Destroy()  {        _Destroy <___TargetType>(); }

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace System;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Flow coordinator base class
    class FlowCoordinator : public IFlowCoordinator
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI", FlowCoordinator, IFlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(FlowCoordinator, IFlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(FlowCoordinator, IFlowCoordinator);

        private:
            static std::map<_u::Type*, _v::MonoPtr<FlowCoordinator>> m_Instances;

        public:
            template<class t_Base> requires(std::is_assignable_v<FlowCoordinator*&, t_Base*>)
            static _v::MonoPtr<t_Base>& _Instance()
            {
                auto l_Type = reinterpret_cast<_u::Type*>(csTypeOf(t_Base*));
                if (l_Type && m_Instances.contains(l_Type))
                    return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&m_Instances[l_Type]);

                auto l_Ptr = reinterpret_cast<t_Base*>(_u::GameObject::New_ctor("[CP_SDK.UI.FlowCoordinator<" + l_Type->get_FullName() + ">]", ArrayW<_u::Type*>({
                    l_Type
                }))->GetComponent(l_Type));
                _u::GameObject::DontDestroyOnLoad(l_Ptr->get_gameObject());

                m_Instances[l_Type] = l_Ptr;
                return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&m_Instances[l_Type]);
            }
            template<class t_Base> requires(std::is_assignable_v<FlowCoordinator*&, t_Base*>)
            static void _Destroy()
            {
                auto l_Type = reinterpret_cast<_u::Type*>(csTypeOf(t_Base*));
                auto l_It   = m_Instances.find(l_Type);
                if (!l_Type || l_It == m_Instances.end())
                    return;

                if (m_Instances[l_Type])
                    _u::GameObject::Destroy(m_Instances[l_Type]->get_gameObject());

                m_Instances.erase(l_It);
            }

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::FlowCoordinator);