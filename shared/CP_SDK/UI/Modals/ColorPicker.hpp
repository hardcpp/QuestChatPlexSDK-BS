#pragma once

#include "../IModal.hpp"
#include "../../XUI/XUI.hpp"

namespace CP_SDK::UI::Modals {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
        using namespace CP_SDK::XUI;
    }

    /// @brief ColorPicker modal
    class ColorPicker : public IModal
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Modals", ColorPicker, IModal);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(ColorPicker);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(ColorPicker);

        private:
            _v::XUISlider::Ptr      m_H;
            _v::XUISlider::Ptr      m_S;
            _v::XUISlider::Ptr      m_V;
            _v::XUISlider::Ptr      m_O;
            _v::XUIText::Ptr        m_HLabel;
            _v::XUIText::Ptr        m_SLabel;
            _v::XUIText::Ptr        m_VLabel;
            _v::XUIText::Ptr        m_OLabel;
            _v::XUIImage::Ptr       m_Image;
            _v::Action<_u::Color>   m_Callback;
            _v::Action<>            m_CancelCallback;

        private:
            /// @brief On modal show
            void Impl_OnShow();
            /// @brief On modal close
            void Impl_OnClose();

        public:
            /// @brief Init
            /// @param p_Value          Initial value
            /// @param p_Opacity        If opacity supported?
            /// @param p_Callback       Callback
            /// @param p_CancelCallback On cancel callback
            void Init(_u::Color p_Value, bool p_Opacity, _v::CActionRef<_u::Color> p_Callback, _v::CActionRef<> p_CancelCallback);

        private:
            /// @brief On color changed
            void OnColorChanged();

        private:
            /// @brief On cancel button
            void OnCancelButton();
            /// @brief On apply button
            void OnApplyButton();

    };

}   ///< namespace CP_SDK::UI::Modals

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Modals::ColorPicker);