#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultColorInputFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultDropdownFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultFLayoutFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultFloatingPanelFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultGLayoutFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultHLayoutFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultIconButtonFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultImageFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultPrimaryButtonFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultSecondaryButtonFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultSliderFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultTabControlFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultTextFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultTextInputFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultTextSegmentedControlFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultToggleFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultVLayoutFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultVScrollViewFactory.hpp"
#include "CP_SDK/UI/DefaultFactories/DefaultVVListFactory.hpp"
#include "CP_SDK/UI/FlowCoordinators/MainFlowCoordinator.hpp"
#include "CP_SDK/UI/IViewController.hpp"
#include "CP_SDK/UI/ModMenu.hpp"
#include "CP_SDK/UI/ScreenSystem.hpp"
#include "CP_SDK/UI/ViewController.hpp"
#include "CP_SDK/Unity/Extensions/ColorU.hpp"
#include "CP_SDK/Unity/FontManager.hpp"
#include "CP_SDK/Unity/Texture2DU.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "assets.hpp"

#include <UnityEngine/Color32.hpp>
#include <UnityEngine/FilterMode.hpp>
#include <UnityEngine/UI/Image.hpp>
#include <TMPro/TextMeshProUGUI.hpp>

using namespace CP_SDK::Unity::Extensions;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI {

    Unity::EnhancedImage::Ptr UISystem::m_LoadingAnimation = nullptr;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    _v::MonoPtr<Sprite> UISystem::m_UIButtonSprite              = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIColorPickerFBGSprite      = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIColorPickerHBGSprite      = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIColorPickerSBGSprite      = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIColorPickerVBGSprite      = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIDownArrowSprite           = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIIconGear                  = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIIconLocked                = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIIconUnlocked              = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIRectBGSprite              = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIRoundBGSprite             = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIRoundRectLeftBGSprite     = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIRoundRectRightBGSprite    = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UIRoundSmoothFrameSprite    = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UISliderBGSprite            = nullptr;
    _v::MonoPtr<Sprite> UISystem::m_UISliderHandleSprite        = nullptr;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<FactoryInterfaces::IColorInputFactory>           UISystem::ColorInputFactory           = std::make_shared<DefaultFactories::DefaultColorInputFactory>();
    std::shared_ptr<FactoryInterfaces::IDropdownFactory>             UISystem::DropdownFactory             = std::make_shared<DefaultFactories::DefaultDropdownFactory>();
    std::shared_ptr<FactoryInterfaces::IFLayoutFactory>              UISystem::FLayoutFactory              = std::make_shared<DefaultFactories::DefaultFLayoutFactory>();
    std::shared_ptr<FactoryInterfaces::IFloatingPanelFactory>        UISystem::FloatingPanelFactory        = std::make_shared<DefaultFactories::DefaultFloatingPanelFactory>();
    std::shared_ptr<FactoryInterfaces::IGLayoutFactory>              UISystem::GLayoutFactory              = std::make_shared<DefaultFactories::DefaultGLayoutFactory>();
    std::shared_ptr<FactoryInterfaces::IHLayoutFactory>              UISystem::HLayoutFactory              = std::make_shared<DefaultFactories::DefaultHLayoutFactory>();
    std::shared_ptr<FactoryInterfaces::IIconButtonFactory>           UISystem::IconButtonFactory           = std::make_shared<DefaultFactories::DefaultIconButtonFactory>();
    std::shared_ptr<FactoryInterfaces::IImageFactory>                UISystem::ImageFactory                = std::make_shared<DefaultFactories::DefaultImageFactory>();
    std::shared_ptr<FactoryInterfaces::IPrimaryButtonFactory>        UISystem::PrimaryButtonFactory        = std::make_shared<DefaultFactories::DefaultPrimaryButtonFactory>();
    std::shared_ptr<FactoryInterfaces::ISecondaryButtonFactory>      UISystem::SecondaryButtonFactory      = std::make_shared<DefaultFactories::DefaultSecondaryButtonFactory>();
    std::shared_ptr<FactoryInterfaces::ISliderFactory>               UISystem::SliderFactory               = std::make_shared<DefaultFactories::DefaultSliderFactory>();
    std::shared_ptr<FactoryInterfaces::ITabControlFactory>           UISystem::TabControlFactory           = std::make_shared<DefaultFactories::DefaultTabControlFactory>();
    std::shared_ptr<FactoryInterfaces::ITextFactory>                 UISystem::TextFactory                 = std::make_shared<DefaultFactories::DefaultTextFactory>();
    std::shared_ptr<FactoryInterfaces::ITextInputFactory>            UISystem::TextInputFactory            = std::make_shared<DefaultFactories::DefaultTextInputFactory>();
    std::shared_ptr<FactoryInterfaces::ITextSegmentedControlFactory> UISystem::TextSegmentedControlFactory = std::make_shared<DefaultFactories::DefaultTextSegmentedControlFactory>();
    std::shared_ptr<FactoryInterfaces::IToggleFactory>               UISystem::ToggleFactory               = std::make_shared<DefaultFactories::DefaultToggleFactory>();
    std::shared_ptr<FactoryInterfaces::IVLayoutFactory>              UISystem::VLayoutFactory              = std::make_shared<DefaultFactories::DefaultVLayoutFactory>();
    std::shared_ptr<FactoryInterfaces::IVScrollViewFactory>          UISystem::VScrollViewFactory          = std::make_shared<DefaultFactories::DefaultVScrollViewFactory>();
    std::shared_ptr<FactoryInterfaces::IVVListFactory>               UISystem::VVListFactory               = std::make_shared<DefaultFactories::DefaultVVListFactory>();

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    int   UISystem::UILayer     = 5;
    float UISystem::FontScale   = 0.875f;

    Color UISystem::DefaultBGColor          = ColorU::WithAlpha("#000000", 0.5000f);
    Color UISystem::NavigationBarBGColor    = ColorU::WithAlpha("#727272", 0.6500f);
    Color UISystem::PrimaryColor            = ColorU::WithAlpha("#258CFF", 1.0000f);
    Color UISystem::SecondaryColor          = ColorU::WithAlpha("#858587", 1.0000f);
    Color UISystem::TitleBlockBGColor       = ColorU::WithAlpha("#727272", 0.5000f);
    Color UISystem::ModalBGColor            = ColorU::WithAlpha("#202021", 0.9750f);
    Color UISystem::ListBGColor             = ColorU::WithAlpha("#000000", 0.7500f);
    Color UISystem::KeyboardTextBGColor     = ColorU::WithAlpha("#7F7F7F", 0.5000f);
    Color UISystem::TooltipBGColor          = ColorU::WithAlpha("#3A3A3B", 0.9875f);

    Color UISystem::TextColor               = ColorU::WithAlpha("#FFFFFF", 1.0000f);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    SafePtr<System::Type> UISystem::Override_UnityComponent_Image            = csTypeOf(Image*);
    SafePtr<System::Type> UISystem::Override_UnityComponent_TextMeshProUGUI  = csTypeOf(TextMeshProUGUI*);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    _v::Func<TMP_FontAsset*>     UISystem::Override_GetUIFont                = []()                  -> TMP_FontAsset*   { return Unity::FontManager::GetMainFont(); };
    _v::Func<Material*>          UISystem::Override_GetUIFontSharedMaterial  = []()                  -> Material*        { return nullptr;                           };
    _v::Func<Material*>          UISystem::Override_GetUIMaterial            = []()                  -> Material*        { return nullptr;                           };
    _v::Action<MonoBehaviour*>   UISystem::Override_OnClick                  = [](MonoBehaviour*)    -> void             {                                           };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    _v::Delegate<void(IScreen*)> UISystem::OnScreenCreated;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void UISystem::Init()
    {
        Unity::EnhancedImage::FromRawAnimated(
            u"CP_SDK._Resources.ChatPlexLogoLoading.webp",
            Animation::EAnimationType::WEBP,
            reinterpret_cast<::Array<uint8_t>*>(IncludedAssets::ChatPlexLogoLoading_webp.operator ArrayW<uint8_t, Array<uint8_t> *>().convert()),
            [](const Unity::EnhancedImage::Ptr& x) -> void {
                m_LoadingAnimation = x;
            }
        );

        ScreenSystem::Create();
        ModMenu::Create();
    }
    void UISystem::Destroy()
    {
        ModMenu::Destroy();
        ScreenSystem::Destroy();

        FlowCoordinators::MainFlowCoordinator::Destroy();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Unity::EnhancedImage::Ptr& UISystem::GetLoadingAnimation()
    {
        return m_LoadingAnimation;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    #define AUTO_SPRITE_GETTER(__Field, __Variable, __Asset, __Borders) \
        UIFieldRefDelExtractor<typeof(__Field)>::t_DelType __Field = []() -> UIFieldRefDelExtractor<typeof(__Field)>::t_PtrType { return GetXSprite(__Variable, __Asset, __Borders); };

    AUTO_SPRITE_GETTER(UISystem::GetUIButtonSprite,             m_UIButtonSprite,            IncludedAssets::UIButton_png.Raw(),             Vector4(10, 10, 10, 10));
    AUTO_SPRITE_GETTER(UISystem::GetUIColorPickerFBGSprite,     m_UIColorPickerFBGSprite,    IncludedAssets::UIColorPickerFBG_png.Raw(),     Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIColorPickerHBGSprite,     m_UIColorPickerHBGSprite,    IncludedAssets::UIColorPickerHBG_png.Raw(),     Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIColorPickerSBGSprite,     m_UIColorPickerSBGSprite,    IncludedAssets::UIColorPickerSBG_png.Raw(),     Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIColorPickerVBGSprite,     m_UIColorPickerVBGSprite,    IncludedAssets::UIColorPickerVBG_png.Raw(),     Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIDownArrowSprite,          m_UIDownArrowSprite,         IncludedAssets::UIDownArrow_png.Raw(),          Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIIconGearSprite,           m_UIIconGear,                IncludedAssets::UIIconGear_png.Raw(),           Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIIconLockedSprite,         m_UIIconLocked,              IncludedAssets::UIIconLocked_png.Raw(),         Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIIconUnlockedSprite,       m_UIIconUnlocked,            IncludedAssets::UIIconUnlocked_png.Raw(),       Vector4::get_zero());
    AUTO_SPRITE_GETTER(UISystem::GetUIRectBGSprite,             m_UIRectBGSprite,            IncludedAssets::UIRectBG_png.Raw(),             Vector4(15, 15, 15, 15));
    AUTO_SPRITE_GETTER(UISystem::GetUIRoundBGSprite,            m_UIRoundBGSprite,           IncludedAssets::UIRoundBG_png.Raw(),            Vector4(15, 15, 15, 15));
    AUTO_SPRITE_GETTER(UISystem::GetUIRoundRectLeftBGSprite,    m_UIRoundRectLeftBGSprite,   IncludedAssets::UIRoundRectLeftBG_png.Raw(),    Vector4(15, 15, 15, 15));
    AUTO_SPRITE_GETTER(UISystem::GetUIRoundRectRightBGSprite,   m_UIRoundRectRightBGSprite,  IncludedAssets::UIRoundRectRightBG_png.Raw(),   Vector4(15, 15, 15, 15));
    AUTO_SPRITE_GETTER(UISystem::GetUIRoundSmoothFrameSprite,   m_UIRoundSmoothFrameSprite,  IncludedAssets::UIRoundSmoothFrame_png.Raw(),   Vector4(15, 15, 15, 15));
    AUTO_SPRITE_GETTER(UISystem::GetUISliderBGSprite,           m_UISliderBGSprite,          IncludedAssets::UISliderBG_png.Raw(),           Vector4(15, 15, 15, 15));
    AUTO_SPRITE_GETTER(UISystem::GetUISliderHandleSprite,       m_UISliderHandleSprite,      IncludedAssets::UISliderHandle_png.Raw(),       Vector4::get_zero());

    #undef AUTO_SPRITE_GETTER

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    IViewController* UISystem::_CreateViewController(System::Type* p_Type)
    {
        if (ViewController::_Instance(p_Type))
            return ViewController::_Instance(p_Type);

        auto l_GameObject = GameObject::New_ctor(p_Type->get_Name(), ArrayW<System::Type*>({
            reinterpret_cast<System::Type*>(csTypeOf(RectTransform*)),
            reinterpret_cast<System::Type*>(csTypeOf(CanvasGroup*))
        }));
        GameObject::DontDestroyOnLoad(l_GameObject);

        auto l_ViewController = reinterpret_cast<ViewController*>(l_GameObject->AddComponent(p_Type));
        l_ViewController->RTransform()->set_anchorMin       (Vector2::get_zero());
        l_ViewController->RTransform()->set_anchorMax       (Vector2::get_one());
        l_ViewController->RTransform()->set_sizeDelta       (Vector2::get_zero());
        l_ViewController->RTransform()->set_anchoredPosition(Vector2::get_zero());
        l_ViewController->get_gameObject()->SetActive(false);

        return l_ViewController;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void UISystem::_DestroyUI(_v::MonoPtr<IViewController>* p_View)
    {
        if (*p_View)
        {
            GameObject::DestroyImmediate((*p_View)->get_gameObject());
            *p_View = nullptr;
        }
    }
    void UISystem::DestroyUI(_v::MonoPtr<Components::CFloatingPanel>* p_Panel)
    {
        if (*p_Panel)
        {
            GameObject::DestroyImmediate((*p_Panel)->get_gameObject());
            *p_Panel = nullptr;
        }
    }
    void UISystem::_DestroyUI(_v::MonoPtr<Components::CFloatingPanel>* p_Panel, _v::MonoPtr<IViewController>* p_View)
    {
        if (*p_Panel)
        {
            GameObject::DestroyImmediate((*p_Panel)->get_gameObject());
            *p_Panel = nullptr;
        }

        if (*p_View)
        {
            GameObject::DestroyImmediate((*p_View)->get_gameObject());
            *p_View = nullptr;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    UIFieldRef<Sprite> UISystem::GetXSprite(UIFieldRef<Sprite> p_Sprite, ::Array<uint8_t>* p_Data, Vector4 p_Borders)
    {
        if (p_Sprite)
            return p_Sprite;

        auto l_Texture = Unity::Texture2DU::CreateFromRaw(p_Data);
        l_Texture->set_filterMode(FilterMode::Trilinear);

        auto l_NewSprite = Unity::SpriteU::CreateFromTexture(
            l_Texture,
            100.0f,
            Vector2(0.5f, 0.5f),
            0,
            SpriteMeshType::FullRect,
            p_Borders
        );

        if (l_NewSprite)
            p_Sprite = l_NewSprite;

        return p_Sprite;
    }

}   ///< namespace CP_SDK::UI