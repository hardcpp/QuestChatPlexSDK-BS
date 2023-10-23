#include "CP_SDK_BS/UI/IHMUIViewController.hpp"
#include "CP_SDK/UI/IScreen.hpp"
#include "CP_SDK/UI/IModal.hpp"

using namespace UnityEngine;

namespace CP_SDK_BS::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(IHMUIViewController);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(IHMUIViewController)
    {

    };
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(IHMUIViewController)
    {

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show a modal
    /// @param p_Modal Modal to show
    void IHMUIViewController::ShowModal(_v::IModal* p_Modal)
    {
        m_ShowModal(p_Modal);
    }
    /// @brief Close a modal
    /// @param p_Modal Modal to close
    void IHMUIViewController::CloseModal(_v::IModal* p_Modal)
    {
        m_CloseModal(p_Modal);
    }
    /// @brief Close all modals
    void IHMUIViewController::CloseAllModals()
    {
        m_CloseAllModals();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show color picker modal
    /// @param p_Value          Base value
    /// @param p_Opacity        Support opacity?
    /// @param p_Callback       On changed callback
    /// @param p_CancelCallback On cancel callback
    void IHMUIViewController::ShowColorPickerModal(Color p_Value, bool p_Opacity, _v::CActionRef<Color> p_Callback, _v::CActionRef<> p_CancelCallback)
    {
        m_ShowColorPickerModal(p_Value, p_Opacity, p_Callback, p_CancelCallback);
    }
    /// @brief Show the confirmation modal
    /// @param p_Message  Message to display
    /// @param p_Callback Callback
    void IHMUIViewController::ShowConfirmationModal(std::u16string_view p_Message, _v::CActionRef<bool> p_Callback)
    {
        m_ShowConfirmationModal(p_Message, p_Callback);
    }
    /// @brief Show the dropdown modal
    /// @param p_Options  Available options
    /// @param p_Selected Selected option
    /// @param p_Callback Callback
    void IHMUIViewController::ShowDropdownModal(const std::vector<std::u16string>& p_Options, std::u16string_view p_Selected, _v::CActionRef<std::u16string_view> p_Callback)
    {
        m_ShowDropdownModal(p_Options, p_Selected, p_Callback);
    }
    /// @brief Show the keyboard modal
    /// @param p_Value          Value
    /// @param p_Callback       Callback
    /// @param p_CancelCallback On cancel callback
    /// @param p_CustomKeys     Custom keys
    void IHMUIViewController::ShowKeyboardModal(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback, _v::KeyboardCustomKeys* p_CustomKeys)
    {
        m_ShowKeyboardModal(p_Value, p_Callback, p_CancelCallback, p_CustomKeys);
    }
    /// @brief Show the loading modal
    /// @param p_Message        Message to show
    /// @param p_CancelButton   Show cancel button
    /// @param p_CancelCallback On cancel callback
    void IHMUIViewController::ShowLoadingModal(std::u16string_view p_Message, bool p_CancelButton, _v::CActionRef<> p_CancelCallback)
    {
        m_ShowLoadingModal(p_Message, p_CancelButton, p_CancelCallback);
    }
    /// @brief Show the message modal
    /// @param p_Message  Message to display
    /// @param p_Callback Callback
    void IHMUIViewController::ShowMessageModal(std::u16string_view p_Message, _v::CActionRef<> p_Callback)
    {
        m_ShowMessageModal(p_Message, p_Callback);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get current value
    std::u16string_view IHMUIViewController::KeyboardModal_GetValue()
    {
        return m_KeyboardModal_GetValue();
    }
    /// @brief Set value
    /// @param p_Value New value
    void IHMUIViewController::KeyboardModal_SetValue(std::u16string_view p_Value)
    {
        m_KeyboardModal_SetValue(p_Value);
    }
    /// @brief Append
    /// @param p_ToAppend Value to append
    void IHMUIViewController::KeyboardModal_Append(std::u16string_view p_ToAppend)
    {
        m_KeyboardModal_Append(p_ToAppend);
    }
    /// @brief Set message
    /// @param p_Message New message
    void IHMUIViewController::LoadingModal_SetMessage(std::u16string_view p_Message)
    {
        m_LoadingModal_SetMessage(p_Message);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Close color picker modal
    void IHMUIViewController::CloseColorPickerModal()
    {
        m_CloseColorPickerModal();
    }
    /// @brief Close the confirmation modal
    void IHMUIViewController::CloseConfirmationModal()
    {
        m_CloseConfirmationModal();
    }
    /// @brief Close the dropdown modal
    void IHMUIViewController::CloseDropdownModal()
    {
        m_CloseDropdownModal();
    }
    /// @brief Close the keyboard modal
    void IHMUIViewController::CloseKeyboardModal()
    {
        m_CloseKeyboardModal();
    }
    /// @brief Close the loading modal
    void IHMUIViewController::CloseLoadingModal()
    {
        m_CloseLoadingModal();
    }
    /// @brief Close the message modal
    void IHMUIViewController::CloseMessageModal()
    {
        m_CloseMessageModal();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show the tooltip
    /// @param p_Position World position
    /// @param p_Text     Tooltip text
    void IHMUIViewController::ShowTooltip(Vector3 p_Position, std::u16string_view p_Text)
    {
        m_ShowTooltip(p_Position, p_Text);
    }
    /// @brief Hide the tooltip
    void IHMUIViewController::HideTooltip()
    {
        m_HideTooltip();
    }

}   ///< namespace CP_SDK_BS::UI

