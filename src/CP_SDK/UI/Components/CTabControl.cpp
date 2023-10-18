#include "CP_SDK/UI/Components/CTabControl.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CTabControl);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CTabControl)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CTabControl)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On active text changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CTabControl* CTabControl::OnActiveChanged(_v::CActionRef<int> p_Functor, bool p_Add)
    {
        m_OnActiveChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get active text
    int CTabControl::GetActiveTab()
    {
        return m_GetActiveTab();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set active text
    /// @param p_Index  New active index
    /// @param p_Notify Should notify?
    CTabControl* CTabControl::SetActiveTab(int p_Index, bool p_Notify)
    {
        m_SetActiveTab(p_Index, p_Notify);
        return this;
    }
    /// @brief Set texts
    /// @param p_Tabs New tabs
    CTabControl* CTabControl::SetTabs(const std::vector<t_TabDefinition>& p_Tabs)
    {
        m_SetTabs(p_Tabs);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components