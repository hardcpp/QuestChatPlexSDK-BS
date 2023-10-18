#include "CP_SDK/UI/Components/CToggle.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CToggle);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CToggle)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CToggle)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CToggle* CToggle::OnValueChanged(_v::CActionRef<bool> p_Functor, bool p_Add)
    {
        m_OnValueChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get value
    bool CToggle::GetValue()
    {
        return m_GetValue();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set button interactable state
    /// @param p_Interactable New state
    CToggle* CToggle::SetInteractable(bool p_Interactable)
    {
        Toggle()->set_interactable(p_Interactable);
        return this;
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    CToggle* CToggle::SetValue(bool p_Value, bool p_Notify)
    {
        m_SetValue(p_Value, p_Notify);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components