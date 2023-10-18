#include "CP_SDK/UI/ModButton.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

namespace CP_SDK::UI {

    std::u16string_view ModButton::Text()
    {
        return m_Text;
    }
    std::u16string_view ModButton::Tooltip()
    {
        return m_Tooltip;
    }
    bool ModButton::Interactable()
    {
        return m_Interactable;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param p_Text         Button text
    /// @param p_OnClick      Action when clicked
    /// @param p_Tooltip      Tooltip text
    /// @param p_Interactable Is interactable?
    ModButton::ModButton(PRIVATE_TAG_ARG(), std::u16string_view p_Text, _v::CActionRef<> p_OnClick, std::u16string_view p_Tooltip, bool p_Interactable)
    {
        m_Text          = p_Text;
        m_Tooltip       = p_Tooltip;
        m_Interactable  = p_Interactable;

        m_OnClick = p_OnClick;
    }
    /// @brief Destructor
    ModButton::~ModButton()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param p_Text         Button text
    /// @param p_OnClick      Action when clicked
    /// @param p_Tooltip      Tooltip text
    /// @param p_Interactable Is interactable?
    ModButton::Ptr ModButton::Make(std::u16string_view p_Text, _v::CActionRef<> p_OnClick, std::u16string_view p_Tooltip, bool p_Interactable)
    {
        return std::make_shared<ModButton>(PRIVATE_TAG_VAL(), p_Text, p_OnClick, p_Tooltip, p_Interactable);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Fire on click event
    void ModButton::FireOnClick()
    {
        try { m_OnClick(); }
        catch (const std::exception& l_Exception)
        {
            ChatPlexSDK::Logger()->Error(u"[CP_SDK.UI][ModButton.FireOnClick] Error:");
            ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

}   ///< namespace CP_SDK::UI