#pragma once

#include "IViewController.hpp"
#include "Modals/ColorPicker.hpp"
#include "Modals/Confirmation.hpp"
#include "Modals/Dropdown.hpp"
#include "Modals/Keyboard.hpp"
#include "Modals/Loading.hpp"
#include "Modals/Message.hpp"
#include "Tooltip.hpp"

#include <map>

#define CP_SDK_UI_VIEW_CONTROLLER_INSTANCE() public: static CP_SDK::Utils::MonoPtr<___TargetType>& Instance() { return _Instance<___TargetType>(); }

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace System;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief IViewController interface
    class ViewController : public IViewController
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", ViewController, IViewController);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(ViewController);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(ViewController);

        private:
            std::u16string                      m_LogPrefix;
            _v::MonoPtr<_u::RectTransform>      m_RTransform;
            _v::MonoPtr<_u::CanvasGroup>        m_CGroup;
            _v::MonoPtr<IScreen>                m_CurrentScreen;
            int                                 m_ModalShowCount;
            _v::MonoPtr<Components::CHLayout>   m_ModalContainer;
            _v::MonoPtr<Modals::ColorPicker>    m_ColorPicker;
            _v::MonoPtr<Modals::Confirmation>   m_ConfirmationModal;
            _v::MonoPtr<Modals::Dropdown>       m_DropdownModal;
            _v::MonoPtr<Modals::Keyboard>       m_KeyboardModal;
            _v::MonoPtr<Modals::Loading>        m_LoadingModal;
            _v::MonoPtr<Modals::Message>        m_MessageModal;
            _v::MonoPtr<Tooltip>                m_Tooltip;
            bool                                m_UICreated;

            static std::map<_u::Type*, _v::MonoPtr<ViewController>> m_Instances;

        public:
            template<class t_Base> requires(std::is_assignable_v<ViewController*&, t_Base*>)
            static _v::MonoPtr<t_Base>& _Instance()
            {
                auto l_Type = reinterpret_cast<_u::Type*>(csTypeOf(t_Base*));
                if (l_Type && m_Instances.contains(l_Type))
                    return *reinterpret_cast<_v::MonoPtr<t_Base>*>(&m_Instances[l_Type]);

                return UIFieldDefault<t_Base>::Value;
            }
            static ViewController* _Instance(_u::Type* p_Type)
            {
                if (p_Type && m_Instances.contains(p_Type))
                    return m_Instances[p_Type].Ptr(false);

                return nullptr;
            }

        public:
            /// @brief Can UI be updated
            bool CanBeUpdated();

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::RectTransform>    ModalContainerRTransform;
            UIFieldRefDel<_u::CanvasGroup>      CGroup;
            UIFieldRefDel<IScreen>              CurrentScreen;
            bool                                UICreated();

        private:
            /// @brief On component creation
            DECLARE_INSTANCE_METHOD(void, Awake);
            /// @brief Init
            void Init();

        private:
            /// @brief Activate
            /// @param p_Screen Target screen
            void __Activate_Impl(IScreen* p_Screen);
            /// @brief Deactivate
            void __Deactivate_Impl();

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
                return reinterpret_cast<t_ModalType*>(CreateModal_Impl(reinterpret_cast<_u::Type*>(csTypeOf(t_ModalType*))));
            }

        private:
            /// @brief Create a modal of type t_ModalType
            /// @param p_Type Modal type
            IModal* CreateModal_Impl(_u::Type* p_Type);
            /// @brief Show a modal
            /// @param p_Modal Modal to show
            void ShowModal_Impl(IModal* p_Modal);
            /// @brief Close a modal
            /// @param p_Modal Modal to close
            void CloseModal_Impl(IModal* p_Modal);
            /// @brief Close all modals
            void CloseAllModals_Impl();

        private:
            /// @brief Show color picker modal
            /// @param p_Value          Base value
            /// @param p_Opacity        Support opacity?
            /// @param p_Callback       On changed callback
            /// @param p_CancelCallback On cancel callback
            void ShowColorPickerModal_Impl(_u::Color p_Value, bool p_Opacity, _v::CActionRef<_u::Color> p_Callback, _v::CActionRef<>&  p_CancelCallback = nullptr);
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
            void ShowKeyboardModal_Impl(std::u16string_view p_Value, _v::CActionRef<std::u16string_view> p_Callback, _v::CActionRef<> p_CancelCallback = nullptr, KeyboardCustomKeys* p_CustomKeys = nullptr);
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

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::ViewController);