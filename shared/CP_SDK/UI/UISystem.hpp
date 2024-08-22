#pragma once

#include "UIIl2cpp.hpp"
#include "FactoryInterfaces/IColorInputFactory.hpp"
#include "FactoryInterfaces/IDropdownFactory.hpp"
#include "FactoryInterfaces/IFLayoutFactory.hpp"
#include "FactoryInterfaces/IFloatingPanelFactory.hpp"
#include "FactoryInterfaces/IGLayoutFactory.hpp"
#include "FactoryInterfaces/IHLayoutFactory.hpp"
#include "FactoryInterfaces/IIconButtonFactory.hpp"
#include "FactoryInterfaces/IImageFactory.hpp"
#include "FactoryInterfaces/IPrimaryButtonFactory.hpp"
#include "FactoryInterfaces/ISecondaryButtonFactory.hpp"
#include "FactoryInterfaces/ISliderFactory.hpp"
#include "FactoryInterfaces/ITabControlFactory.hpp"
#include "FactoryInterfaces/ITextFactory.hpp"
#include "FactoryInterfaces/ITextInputFactory.hpp"
#include "FactoryInterfaces/ITextSegmentedControlFactory.hpp"
#include "FactoryInterfaces/IToggleFactory.hpp"
#include "FactoryInterfaces/IVLayoutFactory.hpp"
#include "FactoryInterfaces/IVScrollViewFactory.hpp"
#include "FactoryInterfaces/IVVListFactory.hpp"
#include "../Unity/EnhancedImage.hpp"

#include <UnityEngine/Color.hpp>
#include <UnityEngine/Material.hpp>
#include <UnityEngine/Sprite.hpp>
#include <UnityEngine/Vector4.hpp>
#include <UnityEngine/CanvasGroup.hpp>
#include <TMPro/TextMeshProUGUI.hpp>
#include <stdint.h>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace System;
        using namespace TMPro;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief UI system main class
    class CP_SDK_EXPORT_VISIBILITY UISystem
    {
        CP_SDK_NO_DEF_CTORS(UISystem);

        private:
            static Unity::EnhancedImage::Ptr m_LoadingAnimation;

        private:
            static _v::MonoPtr<_u::Sprite> m_UIButtonSprite;
            static _v::MonoPtr<_u::Sprite> m_UIColorPickerFBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIColorPickerHBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIColorPickerSBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIColorPickerVBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIDownArrowSprite;
            static _v::MonoPtr<_u::Sprite> m_UIIconGear;
            static _v::MonoPtr<_u::Sprite> m_UIIconLocked;
            static _v::MonoPtr<_u::Sprite> m_UIIconUnlocked;
            static _v::MonoPtr<_u::Sprite> m_UIRectBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIRoundBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIRoundRectLeftBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIRoundRectRightBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UIRoundSmoothFrameSprite;
            static _v::MonoPtr<_u::Sprite> m_UISliderBGSprite;
            static _v::MonoPtr<_u::Sprite> m_UISliderHandleSprite;

        public:
            static std::shared_ptr<FactoryInterfaces::IColorInputFactory>           ColorInputFactory;
            static std::shared_ptr<FactoryInterfaces::IDropdownFactory>             DropdownFactory;
            static std::shared_ptr<FactoryInterfaces::IFLayoutFactory>              FLayoutFactory;
            static std::shared_ptr<FactoryInterfaces::IFloatingPanelFactory>        FloatingPanelFactory;
            static std::shared_ptr<FactoryInterfaces::IGLayoutFactory>              GLayoutFactory;
            static std::shared_ptr<FactoryInterfaces::IHLayoutFactory>              HLayoutFactory;
            static std::shared_ptr<FactoryInterfaces::IIconButtonFactory>           IconButtonFactory;
            static std::shared_ptr<FactoryInterfaces::IImageFactory>                ImageFactory;
            static std::shared_ptr<FactoryInterfaces::IPrimaryButtonFactory>        PrimaryButtonFactory;
            static std::shared_ptr<FactoryInterfaces::ISecondaryButtonFactory>      SecondaryButtonFactory;
            static std::shared_ptr<FactoryInterfaces::ISliderFactory>               SliderFactory;
            static std::shared_ptr<FactoryInterfaces::ITabControlFactory>           TabControlFactory;
            static std::shared_ptr<FactoryInterfaces::ITextFactory>                 TextFactory;
            static std::shared_ptr<FactoryInterfaces::ITextInputFactory>            TextInputFactory;
            static std::shared_ptr<FactoryInterfaces::ITextSegmentedControlFactory> TextSegmentedControlFactory;
            static std::shared_ptr<FactoryInterfaces::IToggleFactory>               ToggleFactory;
            static std::shared_ptr<FactoryInterfaces::IVLayoutFactory>              VLayoutFactory;
            static std::shared_ptr<FactoryInterfaces::IVScrollViewFactory>          VScrollViewFactory;
            static std::shared_ptr<FactoryInterfaces::IVVListFactory>               VVListFactory;

        public:
            static int   UILayer;
            static float FontScale;

            static _u::Color DefaultBGColor;
            static _u::Color NavigationBarBGColor;
            static _u::Color PrimaryColor;
            static _u::Color SecondaryColor;
            static _u::Color TitleBlockBGColor;
            static _u::Color ModalBGColor;
            static _u::Color ListBGColor;
            static _u::Color KeyboardTextBGColor;
            static _u::Color TooltipBGColor;

            static _u::Color TextColor;
            static _u::Color TextColorDisabled;

        public:
            static SafePtr<_u::Type> Override_UnityComponent_Image;
            static SafePtr<_u::Type> Override_UnityComponent_TextMeshProUGUI;

        public:
            static _v::Func<_u::TMP_FontAsset*>      Override_GetUIFont;
            static _v::Func<_u::Material*>           Override_GetUIFontSharedMaterial;
            static _v::Func<_u::Material*>           Override_GetUIMaterial;
            static _v::Action<_u::MonoBehaviour*>    Override_OnClick;

        public:
            static _v::Action<IScreen*> OnScreenCreated;

        public:
            static void Init();
            static void Destroy();

        public:
            static Unity::EnhancedImage::Ptr& GetLoadingAnimation();

        public:
            static UIFieldRefDel<_u::Sprite> GetUIButtonSprite;
            static UIFieldRefDel<_u::Sprite> GetUIColorPickerFBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIColorPickerHBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIColorPickerSBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIColorPickerVBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIDownArrowSprite;
            static UIFieldRefDel<_u::Sprite> GetUIIconGearSprite;
            static UIFieldRefDel<_u::Sprite> GetUIIconLockedSprite;
            static UIFieldRefDel<_u::Sprite> GetUIIconUnlockedSprite;
            static UIFieldRefDel<_u::Sprite> GetUIRectBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIRoundBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIRoundRectLeftBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIRoundRectRightBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUIRoundSmoothFrameSprite;
            static UIFieldRefDel<_u::Sprite> GetUISliderBGSprite;
            static UIFieldRefDel<_u::Sprite> GetUISliderHandleSprite;

        public:
            template<class t_ViewController> requires(std::is_assignable_v<IViewController*&, t_ViewController>)
            static t_ViewController CreateViewController()
            {
                return reinterpret_cast<t_ViewController>(_CreateViewController(reinterpret_cast<_u::Type*>(csTypeOf(t_ViewController).convert())));
            }
            static IViewController* _CreateViewController(_u::Type* p_Type);

        public:
            template<class t_ViewController> requires(std::is_assignable_v<IViewController*&, t_ViewController*>)
            static void DestroyUI(_v::MonoPtr<t_ViewController>* p_View)
            {
                _DestroyUI(reinterpret_cast<_v::MonoPtr<IViewController>*>(p_View));
            }
            static void _DestroyUI(_v::MonoPtr<IViewController>* p_View);
            static void DestroyUI(_v::MonoPtr<Components::CFloatingPanel>* p_Panel);
            template<class t_ViewController> requires(std::is_assignable_v<IViewController*&, t_ViewController*>)
            static void DestroyUI(_v::MonoPtr<Components::CFloatingPanel>* p_Panel, _v::MonoPtr<t_ViewController>* p_View)
            {
                _DestroyUI(p_Panel, reinterpret_cast<_v::MonoPtr<IViewController>*>(p_View));
            }
            static void _DestroyUI(_v::MonoPtr<Components::CFloatingPanel>* p_Panel, _v::MonoPtr<IViewController>* p_View);

        private:
            static UIFieldRef<_u::Sprite> GetXSprite(UIFieldRef<_u::Sprite> p_Sprite, ::Array<uint8_t>* p_Data, _u::Vector4 p_Borders);

    };

}   ///< namespace CP_SDK::UI