#include "CP_SDK/UI/DefaultFactories/DefaultDropdownFactory.hpp"
#include "CP_SDK/UI/DefaultComponents/DefaultCDropdown.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <System/Array.hpp>

using namespace UnityEngine;

namespace CP_SDK::UI::DefaultFactories {

    /// @brief Create an CDropdown into the parent
    /// @param p_Name   Name
    /// @param p_Parent Parent transform
    Components::CDropdown* DefaultDropdownFactory::Create(std::u16string_view p_Name, Transform* p_Parent)
    {
        auto l_GameObject = GameObject::New_ctor(p_Name, ArrayW<System::Type*>({ reinterpret_cast<System::Type*>(csTypeOf(RectTransform*).convert()) }));
        l_GameObject->get_transform()->SetParent(p_Parent, false);

        auto l_Element = l_GameObject->AddComponent<DefaultComponents::DefaultCDropdown*>();
        l_Element->Init();

        return l_Element;
    }

}   ///< namespace CP_SDK::UI::DefaultFactories