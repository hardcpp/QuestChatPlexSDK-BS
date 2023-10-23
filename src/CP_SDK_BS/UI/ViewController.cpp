#include "CP_SDK_BS/UI/ViewController.hpp"
#include "CP_SDK_BS/UI/CP_SDK_UI_IViewControllerBridge.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "CP_SDK/UI/UISystem.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK_BS::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(ViewController);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::map<System::Type*, _v::MonoPtr<ViewController>> ViewController::m_Instances;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(ViewController)
    {
        m_LogPrefix = u"[CP_SDK_BS.UI][ViewController<" + GetType()->get_FullName().operator std::__ndk1::u16string_view() + u">";

        m_ModalShowCount    = 0;
        m_UICreated         = false;

        CP_SDK_UI_IL2CPP_BIND_FIELD(RTransform,                 m_RTransform);
        CP_SDK_UI_IL2CPP_BIND_FIELD(ModalContainerRTransform,   m_ModalContainer->RTransform());
        CP_SDK_UI_IL2CPP_BIND_FIELD(CGroup,                     m_CGroup);

        m_ShowModal                  = {this, &ViewController::ShowModal_Impl};
        m_CloseModal                 = {this, &ViewController::CloseModal_Impl};
        m_CloseAllModals             = {this, &ViewController::CloseAllModals_Impl};

        m_ShowColorPickerModal       = {this, &ViewController::ShowColorPickerModal_Impl};
        m_ShowConfirmationModal      = {this, &ViewController::ShowConfirmationModal_Impl};
        m_ShowDropdownModal          = {this, &ViewController::ShowDropdownModal_Impl};
        m_ShowKeyboardModal          = {this, &ViewController::ShowKeyboardModal_Impl};
        m_ShowLoadingModal           = {this, &ViewController::ShowLoadingModal_Impl};
        m_ShowMessageModal           = {this, &ViewController::ShowMessageModal_Impl};

        m_KeyboardModal_GetValue     = {this, &ViewController::KeyboardModal_GetValue_Impl};
        m_KeyboardModal_SetValue     = {this, &ViewController::KeyboardModal_SetValue_Impl};
        m_KeyboardModal_Append       = {this, &ViewController::KeyboardModal_Append_Impl};
        m_LoadingModal_SetMessage    = {this, &ViewController::LoadingModal_SetMessage_Impl};

        m_CloseColorPickerModal      = {this, &ViewController::CloseColorPickerModal_Impl};
        m_CloseConfirmationModal     = {this, &ViewController::CloseConfirmationModal_Impl};
        m_CloseDropdownModal         = {this, &ViewController::CloseDropdownModal_Impl};
        m_CloseKeyboardModal         = {this, &ViewController::CloseKeyboardModal_Impl};
        m_CloseLoadingModal          = {this, &ViewController::CloseLoadingModal_Impl};
        m_CloseMessageModal          = {this, &ViewController::CloseMessageModal_Impl};

        m_ShowTooltip                = {this, &ViewController::ShowTooltip_Impl};
        m_HideTooltip                = {this, &ViewController::HideTooltip_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(ViewController)
    {
        try
        {
            /// Call implementation
            OnViewDestruction();
        }
        catch(const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".DidDeactivate] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        /// Forward event
        CP_SDK_IL2CPP_CALL_BASE_METHOD(HMUI::ViewController, "OnDestroy");

        /// Unbind singleton
        m_Instances[GetType()] = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Can UI be updated
    bool ViewController::CanBeUpdated()
    {
        return get_isInViewControllerHierarchy() && get_isActiveAndEnabled() && m_UICreated;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    bool ViewController::UICreated()
    {
        return m_UICreated;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On activation
    /// @param p_FirstActivation      Is the first activation?
    /// @param p_AddedToHierarchy     Activation type
    /// @param p_ScreenSystemEnabling Is the screen system enabling?
    void ViewController::ViewController__DidActivate(bool p_FirstActivation, bool p_AddedToHierarchy, bool p_ScreenSystemEnabling)
    {
        try
        {
            /// Bind singleton
            m_Instances[GetType()] = this;

            if (p_FirstActivation)
            {
                if (!GetComponent<CP_SDK_UI_IViewControllerBridge*>())
                    get_gameObject()->AddComponent<CP_SDK_UI_IViewControllerBridge*>();

                /// Get components
                m_RTransform    = GetComponent<RectTransform*>();
                m_CGroup        = GetComponent<CanvasGroup*>();

                /// Create modal container
                m_ModalContainer = CP_SDK::UI::UISystem::HLayoutFactory->Create(u"ModalContainer", get_transform());
                m_ModalContainer->HOrVLayoutGroup()->set_enabled(false);
                m_ModalContainer->CSizeFitter()->set_enabled    (false);
                m_ModalContainer->RTransform()->set_sizeDelta   (Vector2(-10.0f, 0.0f));
                m_ModalContainer->get_gameObject()->SetActive(false);

                auto l_ModalContainerCanvasGroup = m_ModalContainer->get_gameObject()->AddComponent<CanvasGroup*>();
                l_ModalContainerCanvasGroup->set_ignoreParentGroups(true);

                m_Tooltip = CP_SDK::UI::Tooltip::Create(m_RTransform.Ptr());

                ////////////////////////////////////////////////////////////////////////////

                /// Call implementation
                OnViewCreation();

                /// Change state
                m_UICreated = true;
            }

            /// Call implementation
            OnViewActivation();
        }
        catch(const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".DidActivate] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }
    /// @brief On activation
    /// @param p_RemovedFromHierarchy  Desactivation type
    /// @param p_ScreenSystemDisabling Is screen system disabling
    void ViewController::ViewController__DidDeactivate(bool p_RemovedFromHierarchy, bool p_ScreenSystemDisabling)
    {
        try
        {
            /// Close all remaining modals
            CloseAllModals();

            /// Call implementation
            OnViewDeactivation();
        }
        catch(const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".DidDeactivate] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create a modal of type t_ModalType
    /// @param p_Type Modal type
    _v::IModal* ViewController::CreateModal_Impl(System::Type* p_Type)
    {
        auto l_GameObject = GameObject::New_ctor(p_Type->get_Name(), ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)),
            p_Type,
            _v::UISystem::Override_UnityComponent_Image.ptr()
        }));
        auto l_Modal = l_GameObject->GetComponent<_v::IModal*>();

        l_Modal->RTransform()->SetParent(m_ModalContainer->RTransform().Ptr(), false);
        l_Modal->RTransform()->set_anchorMin       (Vector2(0.0f, 0.0f));
        l_Modal->RTransform()->set_anchorMax       (Vector2(1.0f, 1.0f));
        l_Modal->RTransform()->set_pivot           (Vector2(0.5f, 0.5f));
        l_Modal->RTransform()->set_anchoredPosition(Vector2(0.0f, 0.0f));
        l_Modal->RTransform()->set_sizeDelta       (Vector2(0.0f, 0.0f));

        auto l_Background = reinterpret_cast<Image*>(l_GameObject->GetComponent(_v::UISystem::Override_UnityComponent_Image.ptr()));
        l_Background->set_material                (_v::UISystem::Override_GetUIMaterial());
        l_Background->set_raycastTarget           (true);
        l_Background->set_pixelsPerUnitMultiplier (1);
        l_Background->set_type                    (Image::Type::Sliced);
        l_Background->set_sprite                  (_v::UISystem::GetUIRoundBGSprite().Ptr());
        l_Background->set_color                   (_v::ColorU::WithAlpha(Color::get_black(), 0.80f));

        l_Modal->get_gameObject()->SetActive(false);

        return l_Modal;
    }
    /// @brief Show a modal
    /// @param p_Modal Modal to show
    void ViewController::ShowModal_Impl(_v::IModal* p_Modal)
    {
        if (!p_Modal || p_Modal->RTransform()->get_parent() != m_ModalContainer->get_transform())
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".ShowModal] Null or invalid parented modal, not showing!");
            return;
        }

        if (!p_Modal->get_gameObject()->get_activeSelf())
        {
            m_ModalShowCount++;

            if (m_ModalShowCount == 1)
            {
                m_ModalContainer->RTransform()->SetAsLastSibling();
                m_ModalContainer->get_gameObject()->SetActive(true);

                m_CGroup->set_enabled       (true);
                m_CGroup->set_blocksRaycasts(false);
            }

            p_Modal->get_transform()->SetAsLastSibling();
            p_Modal->VController = GetComponent<CP_SDK_UI_IViewControllerBridge*>();
            p_Modal->get_gameObject()->SetActive(true);

            try { p_Modal->OnShow(); }
            catch (const std::exception& l_Exception)
            {
                CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".ShowModal] Error:");
                CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
            }
        }
    }
    /// @brief Close a modal
    /// @param p_Modal Modal to close
    void ViewController::CloseModal_Impl(_v::IModal* p_Modal)
    {
        if (!p_Modal || p_Modal->RTransform()->get_parent() != m_ModalContainer->get_transform())
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".CloseModal] Null or invalid parented modal, not closing!");
            return;
        }

        if (p_Modal->get_gameObject()->get_activeSelf())
        {
            try { p_Modal->OnClose(); }
            catch (const std::exception& l_Exception)
            {
                CP_SDK::ChatPlexSDK::Logger()->Error(m_LogPrefix + u".CloseModal] Error:");
                CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
            }

            p_Modal->get_gameObject()->SetActive(false);

            m_ModalShowCount--;

            if (m_ModalShowCount <= 0)
                CloseAllModals();
        }
    }
    /// @brief Close all modals
    void ViewController::CloseAllModals_Impl()
    {
        auto l_ModalContainerRTransform = m_ModalContainer->RTransform();
        auto l_ChildCount               = l_ModalContainerRTransform->get_childCount();

        for (int l_I = 0; l_I < l_ChildCount; ++l_I)
        {
            auto l_Modal = l_ModalContainerRTransform->GetChild(l_I)->GetComponent<_v::IModal*>();
            if (!l_Modal || !l_Modal->get_gameObject()->get_activeSelf())
                continue;

            l_Modal->OnClose();
            l_Modal->get_gameObject()->SetActive(false);
        }

        m_ModalShowCount = 0;

        m_ModalContainer->get_gameObject()->SetActive(false);

        m_CGroup->set_blocksRaycasts(true);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show color picker modal
    /// @param p_Value          Base value
    /// @param p_Opacity        Support opacity?
    /// @param p_Callback       On changed callback
    /// @param p_CancelCallback On cancel callback
    void ViewController::ShowColorPickerModal_Impl(Color p_Value, bool p_Opacity, const _v::Delegate<void(Color)>& p_Callback, const _v::Delegate<void()>&  p_CancelCallback)
    {
        if (!m_ColorPicker)
            m_ColorPicker = CreateModal<_v::Modals::ColorPicker>();

        ShowModal(m_ColorPicker.Ptr());
        m_ColorPicker->Init(p_Value, p_Opacity, p_Callback, p_CancelCallback);
    }
    /// @brief Show the confirmation modal
    /// @param p_Message  Message to display
    /// @param p_Callback Callback
    void ViewController::ShowConfirmationModal_Impl(std::u16string_view p_Message, const _v::Delegate<void(bool)>& p_Callback)
    {
        if (!m_ConfirmationModal)
            m_ConfirmationModal = CreateModal<_v::Modals::Confirmation>();

        ShowModal(m_ConfirmationModal.Ptr());
        m_ConfirmationModal->Init(p_Message, p_Callback);
    }
    /// @brief Show the dropdown modal
    /// @param p_Options  Available options
    /// @param p_Selected Selected option
    /// @param p_Callback Callback
    void ViewController::ShowDropdownModal_Impl(const std::vector<std::u16string>& p_Options, std::u16string_view p_Selected, const _v::Delegate<void(std::u16string_view)>& p_Callback)
    {
        if (!m_DropdownModal)
            m_DropdownModal = CreateModal<_v::Modals::Dropdown>();

        ShowModal(m_DropdownModal.Ptr());
        m_DropdownModal->Init(p_Options, p_Selected, p_Callback);
    }
    /// @brief Show the keyboard modal
    /// @param p_Value          Value
    /// @param p_Callback       Callback
    /// @param p_CancelCallback On cancel callback
    /// @param p_CustomKeys     Custom keys
    void ViewController::ShowKeyboardModal_Impl(std::u16string_view p_Value, const _v::Delegate<void(std::u16string_view)>& p_Callback, const _v::Delegate<void()>& p_CancelCallback, _v::KeyboardCustomKeys* p_CustomKeys)
    {
        if (!m_KeyboardModal)
            m_KeyboardModal = CreateModal<_v::Modals::Keyboard>();

        ShowModal(m_KeyboardModal.Ptr());
        m_KeyboardModal->Init(p_Value, p_Callback, p_CancelCallback, p_CustomKeys);
    }
    /// @brief Show the loading modal
    /// @param p_Message        Message to show
    /// @param p_CancelButton   Show cancel button
    /// @param p_CancelCallback On cancel callback
    void ViewController::ShowLoadingModal_Impl(std::u16string_view p_Message, bool p_CancelButton, const _v::Delegate<void()>& p_CancelCallback)
    {
        if (!m_LoadingModal)
            m_LoadingModal = CreateModal<_v::Modals::Loading>();

        ShowModal(m_LoadingModal.Ptr());
        m_LoadingModal->Init(p_Message, p_CancelButton, p_CancelCallback);
    }
    /// @brief Show the message modal
    /// @param p_Message  Message to display
    /// @param p_Callback Callback
    void ViewController::ShowMessageModal_Impl(std::u16string_view p_Message, const _v::Delegate<void()>& p_Callback)
    {
        if (!m_MessageModal)
            m_MessageModal = CreateModal<_v::Modals::Message>();

        ShowModal(m_MessageModal.Ptr());
        m_MessageModal->Init(p_Message, p_Callback);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get current value
    std::u16string_view ViewController::KeyboardModal_GetValue_Impl()
    {
        return m_KeyboardModal->GetValue();
    }
    /// @brief Set value
    /// @param p_Value New value
    void ViewController::KeyboardModal_SetValue_Impl(std::u16string_view p_Value)
    {
        m_KeyboardModal->SetValue(p_Value);
    }
    /// @brief Append
    /// @param p_ToAppend Value to append
    void ViewController::KeyboardModal_Append_Impl(std::u16string_view p_ToAppend)
    {
        m_KeyboardModal->Append(p_ToAppend);
    }
    /// @brief Set message
    /// @param p_Message New message
    void ViewController::LoadingModal_SetMessage_Impl(std::u16string_view p_Message)
    {
        m_LoadingModal->SetMessage(p_Message);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Close color picker modal
    void ViewController::CloseColorPickerModal_Impl()
    {
        CloseModal_Impl(m_ColorPicker.Ptr());
    }
    /// @brief Close the confirmation modal
    void ViewController::CloseConfirmationModal_Impl()
    {
        CloseModal_Impl(m_ConfirmationModal.Ptr());
    }
    /// @brief Close the dropdown modal
    void ViewController::CloseDropdownModal_Impl()
    {
        CloseModal_Impl(m_DropdownModal.Ptr());
    }
    /// @brief Close the keyboard modal
    void ViewController::CloseKeyboardModal_Impl()
    {
        CloseModal_Impl(m_KeyboardModal.Ptr());
    }
    /// @brief Close the loading modal
    void ViewController::CloseLoadingModal_Impl()
    {
        CloseModal_Impl(m_LoadingModal.Ptr());
    }
    /// @brief Close the message modal
    void ViewController::CloseMessageModal_Impl()
    {
        CloseModal_Impl(m_MessageModal.Ptr());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show the tooltip
    /// @param p_Position World position
    /// @param p_Text     Tooltip text
    void ViewController::ShowTooltip_Impl(Vector3 p_Position, std::u16string_view p_Text)
    {
        m_Tooltip->get_transform()->SetAsLastSibling();
        m_Tooltip->Show(p_Position, p_Text);
    }
    /// @brief Hide the tooltip
    void ViewController::HideTooltip_Impl()
    {
        m_Tooltip->Hide();
    }

}   ///< namespace CP_SDK_BS::UI

