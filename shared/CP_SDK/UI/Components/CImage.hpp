#pragma once

#include "../UIIl2cpp.hpp"
#include "../../Unity/EnhancedImage.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/LayoutElement.hpp>
#include <UnityEngine/UI/Image.hpp>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief CImage component
    class CP_SDK_EXPORT CImage : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Components", CImage, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(CImage);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(CImage);

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_u::LayoutElement>    LElement;
            UIFieldRefDel<_u::Image>            ImageC;

        public:
            /// @brief Set preferred width
            /// @param p_Width Width
            CImage* SetWidth(float p_Width);
            /// @brief Set preferred height
            /// @param p_Height Height
            CImage* SetHeight(float p_Height);

        public:
            /// @brief Set min width
            /// @param p_Width Width
            CImage* SetMinWidth(float p_Width);
            /// @brief Set min height
            /// @param p_Height Height
            CImage* SetMinHeight(float p_Height);

        public:
            /// @brief Set color
            /// @param p_Color New color
            CImage* SetColor(_u::Color p_Color);
            /// @brief Set enhanced image
            /// @param p_EnhancedImage New enhanced image
            CImage* SetEnhancedImage(const Unity::EnhancedImage::Ptr& p_EnhancedImage);
            /// @brief Set pixels per unit multiplier
            /// @param p_Multiplier New multiplier
            CImage* SetPixelsPerUnitMultiplier(float p_Multiplier);
            /// @brief Set sprite
            /// @param p_Sprite New sprite
            CImage* SetSprite(_u::Sprite* p_Sprite);
            /// @brief Set type
            /// @param p_Type Image type
            CImage* SetType(_u::Image::Type p_Type);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CImage);