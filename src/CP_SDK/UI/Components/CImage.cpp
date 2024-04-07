#include "CP_SDK/UI/Components/CImage.hpp"
#include "CP_SDK/Animation/AnimationStateUpdater.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Vector2.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::Components {

    CP_SDK_IL2CPP_INHERIT_INIT(CImage);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(CImage)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(CImage)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set preferred width
    /// @param p_Width Width
    CImage* CImage::SetWidth(float p_Width)
    {
        RTransform()->set_sizeDelta({p_Width, RTransform()->get_sizeDelta().y});
        LElement()->set_preferredWidth(p_Width);
        return this;
    }
    /// @brief Set preferred height
    /// @param p_Height Height
    CImage* CImage::SetHeight(float p_Height)
    {
        RTransform()->set_sizeDelta(Vector2(RTransform()->get_sizeDelta().x, p_Height));
        LElement()->set_preferredHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set min width
    /// @param p_Width Width
    CImage* CImage::SetMinWidth(float p_Width)
    {
        LElement()->set_minWidth(p_Width);
        return this;
    }
    /// @brief Set min height
    /// @param p_Height Height
    CImage* CImage::SetMinHeight(float p_Height)
    {
        LElement()->set_minHeight(p_Height);
        return this;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set color
    /// @param p_Color New color
    CImage* CImage::SetColor(Color p_Color)
    {
        ImageC()->set_color(p_Color);
        return this;
    }
    /// @brief Set enhanced image
    /// @param p_EnhancedImage New enhanced image
    CImage* CImage::SetEnhancedImage(const Unity::EnhancedImage::Ptr& p_EnhancedImage)
    {
        auto l_Updater = GetComponent<Animation::AnimationStateUpdater*>();
        if (!l_Updater)
            l_Updater = get_gameObject()->AddComponent<Animation::AnimationStateUpdater*>();

        if (l_Updater && !p_EnhancedImage)
            GameObject::Destroy(l_Updater);
        else
        {
            l_Updater->TargetImage = ImageC().Ptr();
            l_Updater->SetControllerDataInstance(p_EnhancedImage->AnimControllerData());
        }

        return this;
    }
    /// @brief Set pixels per unit multiplier
    /// @param p_Multiplier New multiplier
    CImage* CImage::SetPixelsPerUnitMultiplier(float p_Multiplier)
    {
        ImageC()->set_pixelsPerUnitMultiplier(p_Multiplier);
        return this;
    }
    /// @brief Set sprite
    /// @param p_Sprite New sprite
    CImage* CImage::SetSprite(UnityEngine::Sprite* p_Sprite)
    {
        ImageC()->set_sprite(p_Sprite);
        return this;
    }
    /// @brief Set type
    /// @param p_Type Image type
    CImage* CImage::SetType(Image::Type p_Type)
    {
        ImageC()->set_type(p_Type);
        return this;
    }

}   ///< namespace CP_SDK::UI::Components