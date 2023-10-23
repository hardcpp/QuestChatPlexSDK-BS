#include "CP_SDK_BS/UI/IHMUIViewController.hpp"
#include "CP_SDK_BS/UI/CP_SDK_UI_IViewControllerBridge.hpp"

using namespace UnityEngine;

namespace CP_SDK_BS::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(CP_SDK_UI_IViewControllerBridge);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CP_SDK_UI_IViewControllerBridge)
    {
        m_RTransform                = nullptr;
        m_ModalContainerRTransform  = nullptr;
        m_CGroup                    = nullptr;
        m_CurrentScreen             = nullptr;

        m_IHMUIViewController       = nullptr;

        ////////////////////////////////////////////////////////////////////////////

        RTransform = [this]() -> CP_SDK::UI::UIFieldRefDelExtractor<typeof(RTransform)>::t_PtrType {
            if (!m_RTransform && CheckIHMUIViewController()) m_RTransform = m_IHMUIViewController->RTransform().Ptr();
            return m_RTransform;
        };
        ModalContainerRTransform = [this]() -> CP_SDK::UI::UIFieldRefDelExtractor<typeof(ModalContainerRTransform)>::t_PtrType {
            if (!m_ModalContainerRTransform && CheckIHMUIViewController()) m_ModalContainerRTransform = m_IHMUIViewController->ModalContainerRTransform().Ptr();
            return m_ModalContainerRTransform;
        };
        CGroup = [this]() -> CP_SDK::UI::UIFieldRefDelExtractor<typeof(CGroup)>::t_PtrType {
            if (!m_CGroup && CheckIHMUIViewController()) m_CGroup = m_IHMUIViewController->CGroup().Ptr();
            return m_CGroup;
        };
        CurrentScreen = [this]() -> CP_SDK::UI::UIFieldRefDelExtractor<typeof(CurrentScreen)>::t_PtrType {
            return CP_SDK::UI::UIFieldDefault<CP_SDK::UI::IScreen>::Value;
        };

        ////////////////////////////////////////////////////////////////////////////

        m_ShowModal = [this](_v::IModal* a) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowModal(a);
        };
        m_CloseModal = [this](_v::IModal* a) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseModal(a);
        };
        m_CloseAllModals = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseAllModals();
        };

        ////////////////////////////////////////////////////////////////////////////

        m_ShowColorPickerModal = [this](Color a, bool b, _v::CActionRef<Color> c, _v::CActionRef<> d) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowColorPickerModal(a, b, c, d);
        };
        m_ShowConfirmationModal = [this](std::u16string_view a, _v::CActionRef<bool> b) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowConfirmationModal(a, b);
        };
        m_ShowDropdownModal = [this](const std::vector<std::u16string>& a, std::u16string_view b, _v::CActionRef<std::u16string_view> c) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowDropdownModal(a, b, c);
        };
        m_ShowKeyboardModal = [this](std::u16string_view a, _v::CActionRef<std::u16string_view> b, _v::CActionRef<> c, _v::KeyboardCustomKeys* d) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowKeyboardModal(a, b, c, d);
        };
        m_ShowLoadingModal = [this](std::u16string_view a, bool b, _v::CActionRef<> c) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowLoadingModal(a, b, c);
        };
        m_ShowMessageModal = [this](std::u16string_view a, _v::CActionRef<> b) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowMessageModal(a, b);
        };

        ////////////////////////////////////////////////////////////////////////////

        m_KeyboardModal_GetValue = [this]() -> std::u16string_view {
            if (CheckIHMUIViewController()) return m_IHMUIViewController->KeyboardModal_GetValue();
            return u"";
        };
        m_KeyboardModal_SetValue = [this](std::u16string_view a) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->KeyboardModal_SetValue(a);
        };
        m_KeyboardModal_Append = [this](std::u16string_view a) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->KeyboardModal_Append(a);
        };
        m_LoadingModal_SetMessage = [this](std::u16string_view a) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->LoadingModal_SetMessage(a);
        };

        ////////////////////////////////////////////////////////////////////////////

        m_CloseColorPickerModal = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseColorPickerModal();
        };
        m_CloseConfirmationModal = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseConfirmationModal();
        };
        m_CloseDropdownModal = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseDropdownModal();
        };
        m_CloseKeyboardModal = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseKeyboardModal();
        };
        m_CloseLoadingModal = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseLoadingModal();
        };
        m_CloseMessageModal = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->CloseMessageModal();
        };

        ////////////////////////////////////////////////////////////////////////////

        m_ShowTooltip = [this](Vector3 a, std::u16string_view b) -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->ShowTooltip(a, b);
        };
        m_HideTooltip = [this]() -> void {
            if (CheckIHMUIViewController()) m_IHMUIViewController->HideTooltip();
        };
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CP_SDK_UI_IViewControllerBridge)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Check IHMUIViewController instance
    bool CP_SDK_UI_IViewControllerBridge::CheckIHMUIViewController()
    {
        if (m_IHMUIViewController)
            return true;

        m_IHMUIViewController = GetComponent<IHMUIViewController*>();
        return m_IHMUIViewController;
    }

}   ///< namespace CP_SDK_BS::UI