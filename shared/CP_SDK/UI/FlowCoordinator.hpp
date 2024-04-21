#pragma once

#include "IFlowCoordinator.hpp"

#include <map>

#include <UnityEngine/GameObject.hpp>

#include <System/Type.hpp>

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
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", FlowCoordinator, IFlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(FlowCoordinator);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(FlowCoordinator);

        private:
            static std::map<_u::Type*, _v::MonoPtr<FlowCoordinator>> m_Instances;

        public:
            template<class t_Base> requires(std::is_assignable_v<FlowCoordinator*&, t_Base*>)
            static _v::MonoPtr<t_Base>& _Instance()
            {
                auto  l_Type = reinterpret_cast<_u::Type*>(csTypeOf(t_Base*).convert());
                auto& l_Ptr  = _InstanceEx(l_Type);

                return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&l_Ptr);
            }
            static _v::MonoPtr<FlowCoordinator>& _InstanceEx(_u::Type* p_Type);

            template<class t_Base> requires(std::is_assignable_v<FlowCoordinator*&, t_Base*>)
            static void _Destroy()
            {
                auto l_Type = reinterpret_cast<_u::Type*>(csTypeOf(t_Base*).convert());
                _DestroyEx(l_Type);
            }
            static void _DestroyEx(_u::Type* p_Type);

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::FlowCoordinator);