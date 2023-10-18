#include "CP_SDK/UI/Components/CTextSegmentedControl.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CTextSegmentedControl);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CTextSegmentedControl)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CTextSegmentedControl)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On active text changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CTextSegmentedControl* CTextSegmentedControl::OnActiveChanged(_v::CActionRef<int> p_Functor, bool p_Add)
    {
        m_OnActiveChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get active text
    int CTextSegmentedControl::GetActiveText()
    {
        return m_GetActiveText();
    }
    /// @brief Get text count
    int CTextSegmentedControl::GetTextCount()
    {
        return m_GetTextCount();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set active text
    /// @param p_Index  New active index
    /// @param p_Notify Should notify?
    CTextSegmentedControl* CTextSegmentedControl::SetActiveText(int p_Index, bool p_Notify)
    {
        m_SetActiveText(p_Index, p_Notify);
        return this;
    }
    /// @brief Set texts
    /// @param p_Texts New texts
    CTextSegmentedControl* CTextSegmentedControl::SetTexts(const std::vector<std::u16string>& p_Texts)
    {
        m_SetTexts(p_Texts);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components