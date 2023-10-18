#include "CP_SDK/UI/Components/CColorInput.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CColorInput);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CColorInput)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CColorInput)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CColorInput* CColorInput::OnValueChanged(_v::CActionRef<Color> p_Functor, bool p_Add)
    {
        m_OnValueChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get value
    Color CColorInput::GetValue()
    {
        return m_GetValue();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set alpha support
    /// @param p_Support New state
    CColorInput* CColorInput::SetAlphaSupport(bool p_Support)
    {
        m_SetAlphaSupport(p_Support);
        return this;
    }
    /// @brief Set interactable state
    /// @param p_Interactable New state
    CColorInput* CColorInput::SetInteractable(bool p_Interactable)
    {
        m_SetInteractable(p_Interactable);
        return this;
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    CColorInput* CColorInput::SetValue(Color p_Value, bool p_Notify)
    {
        m_SetValue(p_Value, p_Notify);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components