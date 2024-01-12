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

        m_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::MainFlowCoordinator*>().First();

        return m_MainFlowCoordinator.Ptr();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Create a flow coordinator
    /// @param p_Type Flow coordinator type
    HMUI::FlowCoordinator* HMUIUIUtils::CreateFlowCoordinator(System::Type* p_Type)
    {
        if (!m_MainFlowCoordinator)
            m_MainFlowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::MainFlowCoordinator*>().First();

        auto l_InputModule = m_MainFlowCoordinator->_baseInputModule;
        auto l_Coordinator = reinterpret_cast<HMUI::FlowCoordinator*>(GameObject::New_ctor(p_Type->get_Name())->AddComponent(p_Type));

        l_Coordinator->_baseInputModule = l_InputModule;

        return l_Coordinator;
    }
    /// @brief Create a view controller
    /// @param p_Type View controller type
    HMUI::ViewController* HMUIUIUtils::CreateViewController(System::Type* p_Type)
    {
        if (!m_CanvasTemplate)
            m_CanvasTemplate = Resources::FindObjectsOfTypeAll<Canvas*>().First([](Canvas* x) -> bool { return x->get_name() == u"DropdownTableView"; });

        if (!m_PhysicsRaycaster)
            m_PhysicsRaycaster = Resources::FindObjectsOfTypeAll<MainMenuViewController*>().First()->GetComponent<VRGraphicRaycaster*>()->_physicsRaycaster;

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

        l_GameObject->get_gameObject()->AddComponent<VRGraphicRaycaster*>()->_physicsRaycaster = m_PhysicsRaycaster.Ptr();
        l_GameObject->get_gameObject()->AddComponent<CanvasGroup*>();

        auto l_View = reinterpret_cast<HMUI::ViewController*>(l_GameObject->AddComponent(p_Type));
        l_View->get_rectTransform()->set_anchorMin       (Vector2(0.0f, 0.0f));
        l_View->get_rectTransform()->set_anchorMax       (Vector2(1.0f, 1.0f));
        l_View->get_rectTransform()->set_sizeDelta       (Vector2(0.0f, 0.0f));
        l_View->get_rectTransform()->set_anchoredPosition(Vector2(0.0f, 0.0f));
        l_View->get_gameObject()->SetActive(false);

        return l_View;
    }

}   ///< namespace CP_SDK_BS::UI