#include "CP_SDK/UI/FlowCoordinator.hpp"

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(FlowCoordinator);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::map<System::Type*, _v::MonoPtr<FlowCoordinator>> FlowCoordinator::m_Instances;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(FlowCoordinator)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(FlowCoordinator)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    _v::MonoPtr<FlowCoordinator>& FlowCoordinator::_InstanceEx(_u::Type* p_Type)
    {
        if (p_Type && m_Instances.contains(p_Type))
            return m_Instances[p_Type];

        auto l_Ptr = _u::GameObject::New_ctor("[CP_SDK.UI.FlowCoordinator<" + p_Type->get_FullName() + ">]", ArrayW<_u::Type*>({
            p_Type
        }))->GetComponent(p_Type).try_cast<FlowCoordinator>().value_or(nullptr);
        _u::GameObject::DontDestroyOnLoad(l_Ptr->get_gameObject());

        m_Instances[p_Type] = l_Ptr;

        return m_Instances[p_Type];
    }

    void FlowCoordinator::_DestroyEx(_u::Type* p_Type)
    {
        auto l_It = m_Instances.find(p_Type);
        if (!p_Type || l_It == m_Instances.end())
            return;

        if (m_Instances[p_Type])
            _u::GameObject::Destroy(m_Instances[p_Type]->get_gameObject());

        m_Instances.erase(l_It);
    }

}   ///< namespace CP_SDK::UI