#include "CP_SDK_BS/UI/DefaultFactoriesOverrides/BS_FloatingPanelFactory.hpp"
#include "CP_SDK_BS/UI/DefaultComponentsOverrides/BS_CFloatingPanel.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <System/Array.hpp>

namespace CP_SDK_BS::UI::DefaultFactoriesOverrides {

    /// @brief Create an CFloatingPanel into the parent
    /// @param p_Name   Name
    /// @param p_Parent Parent transform
    CP_SDK::UI::Components::CFloatingPanel* BS_FloatingPanelFactory::Create(std::u16string_view p_Name, _u::Transform* p_Parent)
    {
        auto l_GameObject = _u::GameObject::New_ctor(p_Name, ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(_u::RectTransform*).convert()) }));
        l_GameObject->get_transform()->SetParent(p_Parent, false);

        auto l_Element = l_GameObject->AddComponent<DefaultComponentsOverrides::BS_CFloatingPanel*>();
        l_Element->Init();

        return l_Element;
    }

}   ///< namespace CP_SDK_BS::UI::DefaultFactoriesOverrides