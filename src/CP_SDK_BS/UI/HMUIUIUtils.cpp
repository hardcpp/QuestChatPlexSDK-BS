#include "CP_SDK_BS/UI/HMUIUIUtils.hpp"

#include <UnityEngine/AdditionalCanvasShaderChannels.hpp>
#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Resources.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/RenderMode.hpp>
#include <UnityEngine/CanvasGroup.hpp>
#include <VRUIControls/VRGraphicRaycaster.hpp>

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace VRUIControls;

namespace CP_SDK_BS::UI {

    CP_SDK::Utils::MonoPtr<MainFlowCoordinator>          HMUIUIUtils::m_MainFlowCoordinator;
    CP_SDK::Utils::MonoPtr<Canvas>                       HMUIUIUtils::m_CanvasTemplate;
    CP_SDK::Utils::MonoPtr<PhysicsRaycasterWithCache>    HMUIUIUtils::m_PhysicsRaycaster;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MainFlowCoordinator* HMUIUIUtils::MainFlowCoordinator()
    {
        if (m_MainFlowCoordinator)
            return m_MainFlowCoordinator.Ptr();

        m_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::MainFlowCoordinator*>()->First();

        return m_MainFlowCoordinator.Ptr();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create a flow coordinator
    /// @param p_Type Flow coordinator type
    HMUI::FlowCoordinator* HMUIUIUtils::CreateFlowCoordinator(System::Type* p_Type)
    {
        try
        {
            if (!m_MainFlowCoordinator)
                m_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::MainFlowCoordinator*>()->First();

            auto l_InputModule = m_MainFlowCoordinator->____baseInputModule;
            auto l_Coordinator = GameObject::New_ctor(p_Type->get_Name())->AddComponent(p_Type).try_cast<HMUI::FlowCoordinator>().value_or(nullptr);

            l_Coordinator->____baseInputModule = l_InputModule;

            return l_Coordinator;
        }
        catch(const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[HMUIUIUtils.CreateFlowCoordinator] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);

            throw l_Exception;
        }
    }
    /// @brief Create a view controller
    /// @param p_Type View controller type
    HMUI::ViewController* HMUIUIUtils::CreateViewController(System::Type* p_Type)
    {
        try
        {
            if (!m_CanvasTemplate)
            {
                m_CanvasTemplate = Resources::FindObjectsOfTypeAll<Canvas*>()->FirstOrDefault([](Canvas* x) -> bool {
                    return CP_SDK::Utils::IsUnityPtrValid(x) && x->get_name() == u"DropdownTableView";
                });
            }

            if (!m_PhysicsRaycaster)
                m_PhysicsRaycaster = Resources::FindObjectsOfTypeAll<MainMenuViewController*>()->First()->GetComponent<VRGraphicRaycaster*>()->____physicsRaycaster;

            auto l_GameObject = GameObject::New_ctor(p_Type->get_Name());
            auto l_Canvas     = l_GameObject->AddComponent<Canvas*>();
            l_Canvas->set_renderMode               (m_CanvasTemplate->get_renderMode());
            l_Canvas->set_scaleFactor              (m_CanvasTemplate->get_scaleFactor());
            l_Canvas->set_referencePixelsPerUnit   (m_CanvasTemplate->get_referencePixelsPerUnit());
            //l_Canvas->set_overridePixelPerfect     (m_CanvasTemplate->get_overridePixelPerfect());
            l_Canvas->set_pixelPerfect             (m_CanvasTemplate->get_pixelPerfect());
            //l_Canvas->set_planeDistance            (m_CanvasTemplate->get_planeDistance());
            l_Canvas->set_overrideSorting          (m_CanvasTemplate->get_overrideSorting());
            l_Canvas->set_sortingOrder             (m_CanvasTemplate->get_sortingOrder());
            //l_Canvas->set_targetDisplay            (m_CanvasTemplate->get_targetDisplay());
            l_Canvas->set_sortingLayerID           (m_CanvasTemplate->get_sortingLayerID());
            l_Canvas->set_additionalShaderChannels (m_CanvasTemplate->get_additionalShaderChannels());
            //l_Canvas->set_sortingLayerName         (m_CanvasTemplate->get_sortingLayerName());
            l_Canvas->set_worldCamera              (m_CanvasTemplate->get_worldCamera());
            //l_Canvas->set_normalizedSortingGridSize(m_CanvasTemplate->get_normalizedSortingGridSize());

            l_GameObject->get_gameObject()->AddComponent<VRGraphicRaycaster*>()->____physicsRaycaster = m_PhysicsRaycaster.Ptr();
            l_GameObject->get_gameObject()->AddComponent<CanvasGroup*>();

            auto l_View = l_GameObject->AddComponent(p_Type).try_cast<HMUI::ViewController>().value_or(nullptr);
            l_View->get_rectTransform()->set_anchorMin       (Vector2(0.0f, 0.0f));
            l_View->get_rectTransform()->set_anchorMax       (Vector2(1.0f, 1.0f));
            l_View->get_rectTransform()->set_sizeDelta       (Vector2(0.0f, 0.0f));
            l_View->get_rectTransform()->set_anchoredPosition(Vector2(0.0f, 0.0f));
            l_View->get_gameObject()->SetActive(false);

            return l_View;
        }
        catch(const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[HMUIUIUtils.CreateViewController] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);

            throw l_Exception;
        }

    }

}   ///< namespace CP_SDK_BS::UI