#pragma once

#include "IHMUIViewController.hpp"
#include "../../CP_SDK/UI/Components/CHLayout.hpp"
#include "../../CP_SDK/UI/Modals/ColorPicker.hpp"
#include "../../CP_SDK/UI/Modals/Confirmation.hpp"
#include "../../CP_SDK/UI/Modals/Dropdown.hpp"
#include "../../CP_SDK/UI/Modals/Keyboard.hpp"
#include "../../CP_SDK/UI/Modals/Loading.hpp"
#include "../../CP_SDK/UI/Modals/Message.hpp"
#include "../../CP_SDK/UI/Tooltip.hpp"
#include "../../CP_SDK/UI/UISystem.hpp"
#include "../../CP_SDK/Unity/Extensions/ColorU.hpp"

#include <string>
#include <map>

#define SDK_UI_VIEW_CONTROLLER_INSTANCE() public: static CP_SDK::Utils::MonoPtr<___TargetType>& Instance() { return _Instance<___TargetType>(); }

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::UI;
        using namespace CP_SDK::Unity::Extensions;
        using namespace CP_SDK::Utils;
    }

    /// @brief View flow coordinator base class
    class CP_SDK_EXPORT ViewController : public IHMUIViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK_BS.UI", ViewController, IHMUIViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(ViewController);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(ViewController);

        private:
            std::u16string                          m_LogPrefix;
            _v::MonoPtr<_u::RectTransform>          m_RTransform;
            _v::MonoPtr<_u::CanvasGroup>            m_CGroup;
            int                                     m_ModalShowCount;
            _v::MonoPtr<_v::Components::CHLayout>   m_ModalContainer;
            _v::MonoPtr<_v::Modals::ColorPicker>    m_ColorPicker;
            _v::MonoPtr<_v::Modals::Confirmation>   m_ConfirmationModal;
            _v::MonoPtr<_v::Modals::Dropdown>       m_DropdownModal;
            _v::MonoPtr<_v::Modals::Keyboard>       m_KeyboardModal;
            _v::MonoPtr<_v::Modals::Loading>        m_LoadingModal;
            _v::MonoPtr<_v::Modals::Message>        m_MessageModal;
            _v::MonoPtr<_v::Tooltip>                m_Tooltip;
            bool                                    m_UICreated;

            static std::map<System::Type*, _v::MonoPtr<ViewController>> m_Instances;

        public:
            template<class t_Base> requires(std::is_assignable_v<ViewController*&, t_Base*>)
            static _v::MonoPtr<t_Base>& _Instance()
            {
                auto l_Type = reinterpret_cast<System::Type*>(csTypeOf(t_Base*).convert());
                if (l_Type && m_Instances.contains(l_Type))
                    return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&m_Instances[l_Type]);

                return _v::UIFieldDefault<t_Base>::Value;
            }

        public:
            /// @brief Can UI be updated
            bool CanBeUpdated();

        public:
            bool UICreated();

        private:
            /// @brief On activation
            /// @param p_FirstActivation      Is the first activation?
            /// @param p_AddedToHierarchy     Activation type
            /// @param p_ScreenSystemEnabling Is the screen system enabling?
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(HMUI, ViewController, void, DidActivate, bool p_FirstActivation, bool p_AddedToHierarchy, bool p_ScreenSystemEnabling);
            /// @brief On activation
            /// @param p_RemovedFromHierarchy  Desactivation type
            /// @param p_ScreenSystemDisabling Is screen system disabling
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(HMUI, ViewController, void, DidDeactivate, bool p_RemovedFromHierarchy, bool p_ScreenSystemDisabling);

        protected:
            /// @brief On view creation
            _v::Action<> OnViewCreation;
            /// @brief On view activation
            _v::Action<> OnViewActivation;
            /// @brief On view deactivation
            _v::Action<> OnViewDeactivation;
            /// @brief On view destruction
            _v::Action<> OnViewDestruction;

        public:
            /// @brief Create a modal of type t_ModalType
            /// @tparam t_ModalType Modal type
            template<class t_ModalType>
            t_ModalType* CreateModal()
            {
                return reinterpret_cast<t_ModalType*>(CreateModal_Impl(reinterpret_cast<System::Type*>(csTypeOf(t_ModalType*).convert())));
            }

        private:
            /// @brief Create a modal of type t_ModalType
            /// @param p_Type Modal type
            _v::IModal* CreateModal_Impl(System::Type* p_Type);
            /// @brief Show a modal
            /// @param p_Modal Modal to show
            void ShowModal_Impl(_v::IModal* p_Modal);
            /// @brief Close a modal
            /// @param p_Modal Modal to close
            void CloseModal_Impl(_v::IModal* p_Modal);
            /// @brief Close all modals
            void CloseAllModals_Impl();

        private:
            /// @brief Show color picker modal
            /// @param p_Value          Base value
            /// @param p_Opacity        Support opacity?
            /// @param p_Callback       On changed callback
            /// @param p_CancelCallback On cancel callback
            void ShowColorPickerModal_Impl(_u::Color p_Value, bool p_Opacity, _v::CActionRef<_u::Color> p_Callback, _v::CActionRef<>  p_CancelCallback = nullptr);
            /// @brief Show the confirmation modal
            /// @param p_Message  Message to display
            /// @param p_Callback Callback
            void ShowConfirmationModal_Impl(std::u16string_view p_Message, _v::CActionRef<bool> p_Callback);
            /// @brief Show the dropdown modal
            /// @param p_Options  Available options
            /// @param p_Selected Selected option
            /// @param p_Callback Callback
            void ShowDropdownModal_Impl(const std::vector<std::u16string>& p_Options, std::u16string_view p_Selected, _v::CActionRef<std::u16string_view> p_Callback);
            /// @brief Show the keyboard modal
            /// @param p_Value          Value
            /// @param p_Callback       Callback
            /// @param p_CancelCallback On cancel callback
            /// @param p_CustomKeys     Custom keys
            void ShowKeyboardModal_Impl(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback = nullptr, _v::KeyboardCustomKeys* p_CustomKeys = nullptr);
            /// @brief Show the loading modal
            /// @param p_Message        Message to show
            /// @param p_CancelButton   Show cancel button
            /// @param p_CancelCallback On cancel callback
            void ShowLoadingModal_Impl(std::u16string_view p_Message = {}, bool p_CancelButton = false, _v::CActionRef<> p_CancelCallback = nullptr);
            /// @brief Show the message modal
            /// @param p_Message  Message to display
            /// @param p_Callback Callback
            void ShowMessageModal_Impl(std::u16string_view p_Message, _v::CActionRef<> p_Callback = nullptr);

        private:
            /// @brief Get current value
            std::u16string_view KeyboardModal_GetValue_Impl();
            /// @brief Set value
            /// @param p_Value New value
            void KeyboardModal_SetValue_Impl(std::u16string_view p_Value);
            /// @brief Append
            /// @param p_ToAppend Value to append
            void KeyboardModal_Append_Impl(std::u16string_view p_ToAppend);
            /// @brief Set message
            /// @param p_Message New message
            void LoadingModal_SetMessage_Impl(std::u16string_view p_Message);

        private:
            /// @brief Close color picker modal
            void CloseColorPickerModal_Impl();
            /// @brief Close the confirmation modal
            void CloseConfirmationModal_Impl();
            /// @brief Close the dropdown modal
            void CloseDropdownModal_Impl();
            /// @brief Close the keyboard modal
            void CloseKeyboardModal_Impl();
            /// @brief Close the loading modal
            void CloseLoadingModal_Impl();
            /// @brief Close the message modal
            void CloseMessageModal_Impl();

        private:
            /// @brief Show the tooltip
            /// @param p_Position World position
            /// @param p_Text     Tooltip text
            void ShowTooltip_Impl(_u::Vector3 p_Position, std::u16string_view p_Text);
            /// @brief Hide the tooltip
            void HideTooltip_Impl();

    };

}   ///< namespace CP_SDK_BS::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK_BS::UI::ViewController);