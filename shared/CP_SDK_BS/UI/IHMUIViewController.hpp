#pragma once

#include "../../CP_SDK/UI/IViewController.hpp"

#include <HMUI/ViewController.hpp>
#include <UnityEngine/CanvasGroup.hpp>
#include <UnityEngine/Color.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/Vector3.hpp>
#include "System/Reflection/MemberInfo.hpp"
#include "System/Type.hpp"

#include <string>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::UI;
        using namespace CP_SDK::Utils;
    }

    /// @brief IHMUIViewController interface
    class CP_SDK_EXPORT IHMUIViewController : public HMUI::ViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI", IHMUIViewController, HMUI::ViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(IHMUIViewController);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(IHMUIViewController);
        CP_SDK_IL2CPP_CTOR_FIELD();
        CP_SDK_IL2CPP_DTOR_FIELD();

        public:
            _v::UIFieldRefDel<_u::RectTransform>    RTransform;
            _v::UIFieldRefDel<_u::RectTransform>    ModalContainerRTransform;
            _v::UIFieldRefDel<_u::CanvasGroup>      CGroup;

        public:
            /// @brief Show a modal
            /// @param p_Modal Modal to show
            void ShowModal(_v::IModal* p_Modal);
            /// @brief Close a modal
            /// @param p_Modal Modal to close
            void CloseModal(_v::IModal* p_Modal);
            /// @brief Close all modals
            void CloseAllModals();

        public:
            /// @brief Show color picker modal
            /// @param p_Value          Base value
            /// @param p_Opacity        Support opacity?
            /// @param p_Callback       On changed callback
            /// @param p_CancelCallback On cancel callback
            void ShowColorPickerModal(_u::Color p_Value, bool p_Opacity, _v::CActionRef<_u::Color> p_Callback, _v::CActionRef<>  p_CancelCallback = nullptr);
            /// @brief Show the confirmation modal
            /// @param p_Message  Message to display
            /// @param p_Callback Callback
            void ShowConfirmationModal(std::u16string_view p_Message, _v::CActionRef<bool> p_Callback);
            /// @brief Show the dropdown modal
            /// @param p_Options  Available options
            /// @param p_Selected Selected option
            /// @param p_Callback Callback
            void ShowDropdownModal(const std::vector<std::u16string>& p_Options, std::u16string_view p_Selected, _v::CActionRef<std::u16string_view> p_Callback);
            /// @brief Show the keyboard modal
            /// @param p_Value          Value
            /// @param p_Callback       Callback
            /// @param p_CancelCallback On cancel callback
            /// @param p_CustomKeys     Custom keys
            void ShowKeyboardModal(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback = nullptr, _v::KeyboardCustomKeys* p_CustomKeys = nullptr);
            /// @brief Show the loading modal
            /// @param p_Message        Message to show
            /// @param p_CancelButton   Show cancel button
            /// @param p_CancelCallback On cancel callback
            void ShowLoadingModal(std::u16string_view p_Message = {}, bool p_CancelButton = false, _v::CActionRef<> p_CancelCallback = nullptr);
            /// @brief Show the message modal
            /// @param p_Message  Message to display
            /// @param p_Callback Callback
            void ShowMessageModal(std::u16string_view p_Message, _v::CActionRef<> p_Callback = nullptr);

        public:
            /// @brief Get current value
            std::u16string_view KeyboardModal_GetValue();
            /// @brief Set value
            /// @param p_Value New value
            void KeyboardModal_SetValue(std::u16string_view p_Value);
            /// @brief Append
            /// @param p_ToAppend Value to append
            void KeyboardModal_Append(std::u16string_view p_ToAppend);
            /// @brief Set message
            /// @param p_Message New message
            void LoadingModal_SetMessage(std::u16string_view p_Message);

        public:
            /// @brief Close color picker modal
            void CloseColorPickerModal();
            /// @brief Close the confirmation modal
            void CloseConfirmationModal();
            /// @brief Close the dropdown modal
            void CloseDropdownModal();
            /// @brief Close the keyboard modal
            void CloseKeyboardModal();
            /// @brief Close the loading modal
            void CloseLoadingModal();
            /// @brief Close the message modal
            void CloseMessageModal();

        public:
            /// @brief Show the tooltip
            /// @param p_Position World position
            /// @param p_Text     Tooltip text
            void ShowTooltip(_u::Vector3 p_Position, std::u16string_view p_Text);
            /// @brief Hide the tooltip
            void HideTooltip();

        protected:
            _v::Action<_v::IModal*> m_ShowModal;
            _v::Action<_v::IModal*> m_CloseModal;
            _v::Action<>            m_CloseAllModals;

            _v::Action<_u::Color, bool, _v::CActionRef<_u::Color>, _v::CActionRef<>>                                        m_ShowColorPickerModal;
            _v::Action<std::u16string_view, _v::CActionRef<bool>>                                                           m_ShowConfirmationModal;
            _v::Action<const std::vector<std::u16string>&, std::u16string_view, _v::CActionRef<std::u16string_view>>        m_ShowDropdownModal;
            _v::Action<std::u16string_view, _v::CActionRef<std::u16string_view>, _v::CActionRef<>, _v::KeyboardCustomKeys*> m_ShowKeyboardModal;
            _v::Action<std::u16string_view, bool, _v::CActionRef<>>                                                         m_ShowLoadingModal;
            _v::Action<std::u16string_view, _v::CActionRef<>>                                                               m_ShowMessageModal;

            _v::Func<std::u16string_view>    m_KeyboardModal_GetValue;
            _v::Action<std::u16string_view>  m_KeyboardModal_SetValue;
            _v::Action<std::u16string_view>  m_KeyboardModal_Append;
            _v::Action<std::u16string_view>  m_LoadingModal_SetMessage;

            _v::Action<> m_CloseColorPickerModal;
            _v::Action<> m_CloseConfirmationModal;
            _v::Action<> m_CloseDropdownModal;
            _v::Action<> m_CloseKeyboardModal;
            _v::Action<> m_CloseLoadingModal;
            _v::Action<> m_CloseMessageModal;

            _v::Action<_u::Vector3, std::u16string_view>    m_ShowTooltip;
            _v::Action<>                                    m_HideTooltip;

    };

}   ///< namespace CP_SDK_BS::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::IHMUIViewController);