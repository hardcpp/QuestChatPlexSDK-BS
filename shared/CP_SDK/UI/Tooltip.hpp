#pragma once

#include "Components/CImage.hpp"
#include "Components/CText.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/ContentSizeFitter.hpp>
#include <UnityEngine/UI/HorizontalLayoutGroup.hpp>
#include <UnityEngine/UI/Image.hpp>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Tooltip widget
    class CP_SDK_EXPORT_VISIBILITY Tooltip : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", Tooltip, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(Tooltip);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(Tooltip);

        private:
            _v::MonoPtr<_u::RectTransform>          m_RTransform;
            _v::MonoPtr<_u::HorizontalLayoutGroup>  m_HorizontalLayoutGroup;
            _v::MonoPtr<_u::ContentSizeFitter>      m_ContentSizeFitter;
            _v::MonoPtr<_u::Image>                  m_Image;
            _v::MonoPtr<Components::CImage>         m_Border;
            _v::MonoPtr<Components::CText>          m_Text;

        public:
            /// @brief Create a tooltip
            /// @param p_Parent Parent container
            static Tooltip* Create(_u::RectTransform* p_Parent);

        public:
            /// @brief Show the tooltip
            /// @param p_Position World position
            /// @param p_Text     Tooltip text
            void Show(_u::Vector3 p_Position, std::u16string_view p_Text);
            /// @brief Hide the tooltip
            void Hide();

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Tooltip);