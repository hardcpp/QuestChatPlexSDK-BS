#pragma once

#include "UIIl2cpp.hpp"

#include <UnityEngine/CanvasGroup.hpp>
#include <UnityEngine/Color.hpp>
#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/Vector3.hpp>

#include <tuple>
#include <vector>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    class IModal;
    class IScreen;

    using KeyboardCustomKey     = std::tuple<std::u16string, _v::Delegate<void()>, std::u16string>;
    using KeyboardCustomKeys    = std::vector<KeyboardCustomKey>;

    /// @brief IViewController interface
    class IViewController : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", IViewController, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(IViewController);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(IViewController);

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::RectTransform>    ModalContainerRTransform;
            UIFieldRefDel<_u::CanvasGroup>      CGroup;
            UIFieldRefDel<IScreen>              CurrentScreen;

        public:
            /// @brief Activate
            /// @param p_Screen Target screen
            void __Activate(IScreen* p_Screen);
            /// @brief Deactivate
            void __Deactivate();

        public:
            /// @brief Show a modal
            /// @param p_Modal Modal to show
            void ShowModal(IModal* p_Modal);
            /// @brief Close a modal
            /// @param p_Modal Modal to close
            void CloseModal(IModal* p_Modal);
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
            void ShowKeyboardModal(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback = nullptr, KeyboardCustomKeys* p_CustomKeys = nullptr);
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
            _v::Action<IScreen*> m___Activate;
            _v::Action<>         m___Deactivate;

            _v::Action<IModal*>  m_ShowModal;
            _v::Action<IModal*>  m_CloseModal;
            _v::Action<>         m_CloseAllModals;

            _v::Action<_u::Color, bool, _v::CActionRef<_u::Color>, _v::CActionRef<>>                                    m_ShowColorPickerModal;
            _v::Action<std::u16string_view, _v::CActionRef<bool>>                                                       m_ShowConfirmationModal;
            _v::Action<const std::vector<std::u16string>&, std::u16string_view, _v::CActionRef<std::u16string_view>>    m_ShowDropdownModal;
            _v::Action<std::u16string_view, _v::CActionRef<std::u16string_view>, _v::CActionRef<>, KeyboardCustomKeys*> m_ShowKeyboardModal;
            _v::Action<std::u16string_view, bool, _v::CActionRef<>>                                                     m_ShowLoadingModal;
            _v::Action<std::u16string_view, _v::CActionRef<>>                                                           m_ShowMessageModal;

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

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::IViewController);