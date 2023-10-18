#include "CP_SDK/UI/Components/CDropdown.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CDropdown);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CDropdown)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CDropdown)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CDropdown* CDropdown::OnValueChanged(_v::CActionRef<int, std::u16string_view> p_Functor, bool p_Add)
    {
        m_OnValueChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get value
    std::u16string_view CDropdown::GetValue()
    {
        return m_GetValue();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set interactable state
    /// @param p_Interactable New state
    CDropdown* CDropdown::SetInteractable(bool p_Interactable)
    {
        m_SetInteractable(p_Interactable);
        return this;
    }
    /// @brief Set available options
    /// @param p_Options New options list
    CDropdown* CDropdown::SetOptions(const std::vector<std::u16string>& p_Options)
    {
        m_SetOptions(p_Options);
        return this;
    }
    /// @brief Set value
    /// @param p_Value  New value
    /// @param p_Notify Should notify?
    CDropdown* CDropdown::SetValue(std::u16string_view p_Value, bool p_Notify)
    {
        m_SetValue(p_Value, p_Notify);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components