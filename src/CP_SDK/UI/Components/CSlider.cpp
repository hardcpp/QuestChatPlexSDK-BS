#include "CP_SDK/UI/Components/CSlider.hpp"
#include "CP_SDK/Animation/AnimationStateUpdater.hpp"

#include <UnityEngine/GameObject.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CSlider);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CSlider)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CSlider)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief  On value changed event
    /// @param p_Functor Functor to add/remove
    /// @param p_Add     Should add
    CSlider* CSlider::OnValueChanged(_v::CActionRef<float> p_Functor, bool p_Add)
    {
        m_OnValueChanged(p_Functor, p_Add);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get min value
    float CSlider::GetMinValue()
    {
        return m_GetMinValue();
    }
    /// @brief Get max value
    float CSlider::GetMaxValue()
    {
        return m_GetMaxValue();
    }
    /// @brief Get increments
    float CSlider::GetIncrements()
    {
        return m_GetIncrements();
    }
    /// @brief Get value
    float CSlider::GetValue()
    {
        return m_GetValue();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set theme color
    /// @param p_Color New color
    CSlider* CSlider::SetColor(Color p_Color)
    {
        m_SetColor(p_Color);
        return this;
    }
    /// @brief Set value formatter
    /// @param p_CustomFormatter Custom value formatter
    CSlider* CSlider::SetFormatter(_v::CFuncRef<std::u16string, float> p_CustomFormatter)
    {
        m_SetFormatter(p_CustomFormatter);
        return this;
    }
    /// @brief Set integer mode
    /// @param p_IsInteger Is integer?
    CSlider* CSlider::SetInteger(bool p_IsInteger)
    {
        m_SetInteger(p_IsInteger);
        return this;
    }
    /// @brief Set button interactable state
    /// @param p_Interactable New state
    CSlider* CSlider::SetInteractable(bool p_Interactable)
    {
        set_interactable(p_Interactable);
        m_SetInteractable(p_Interactable);
        return this;
    }
    /// @brief Set min value
    /// @param p_MinValue New value
    CSlider* CSlider::SetMinValue(float p_MinValue)
    {
        m_SetMinValue(p_MinValue);
        return this;
    }
    /// @brief Set max value
    /// @param p_MaxValue New value
    CSlider* CSlider::SetMaxValue(float p_MaxValue)
    {
        m_SetMaxValue(p_MaxValue);
        return this;
    }
    /// @brief Set increments
    /// @param p_Increments New value
    CSlider* CSlider::SetIncrements(float p_Increments)
    {
        m_SetIncrements(p_Increments);
        return this;
    }
    /// @brief Set value
    /// @param p_Value Value
    /// @param p_Notify Notify?
    CSlider* CSlider::SetValue(float p_Value, bool p_Notify)
    {
        m_SetValue(p_Value, p_Notify);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Switch to color mode
    /// @param p_H Is Hue mode?
    /// @param p_S Is saturation mode?
    /// @param p_V Is value mode?
    /// @param p_O Is opacity mode?
    CSlider* CSlider::SwitchToColorMode(bool p_H, bool p_S, bool p_V, bool p_O)
    {
        m_SwitchToColorMode(p_H, p_S, p_V, p_O);
        return this;
    }
    /// @brief Color mode set H
    /// @param p_H Is Hue mode?
    CSlider* CSlider::ColorModeSetHue(float p_H)
    {
        m_ColorModeSetHue(p_H);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components