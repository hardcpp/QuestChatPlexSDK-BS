#include "CP_SDK/UI/Components/CTextInput.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CTextInput);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CTextInput)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CTextInput)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CTextInput* CTextInput::OnValueChanged(_v::CActionRef<std::u16string_view> p_Functor, bool p_Add)
    {
        m_OnValueChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get value
    std::u16string_view CTextInput::GetValue()
    {
        return m_GetValue();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set button interactable state
    /// @param p_Interactable New state
    CTextInput* CTextInput::SetInteractable(bool p_Interactable)
    {
        m_SetInteractable(p_Interactable);
        return this;
    }
    /// @brief Set is password
    /// @param p_IsPassword Is password?
    CTextInput* CTextInput::SetIsPassword(bool p_IsPassword)
    {
        m_SetIsPassword(p_IsPassword);
        return this;
    }
    /// @brief Set place holder
    /// @param p_PlaceHolder New place holder
    CTextInput* CTextInput::SetPlaceHolder(std::u16string_view p_PlaceHolder)
    {
        m_SetPlaceHolder(p_PlaceHolder);
        return this;
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    CTextInput* CTextInput::SetValue(std::u16string_view p_Value, bool p_Notify)
    {
        m_SetValue(p_Value, p_Notify);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components