#pragma once

#include "../../ChatPlexSDK.hpp"
#include "../../UI/UIIl2cpp.hpp"
#include "../../UI/UISystem.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/Transform.hpp>

#include <string>

namespace CP_SDK::XUI {

    namespace _u
    {
        using namespace TMPro;
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::UI;
        using namespace CP_SDK::UI::Components;
        using namespace CP_SDK::Utils;
    }

    /// NOTE: It's important to capture by copy to avoid memory issue
    #define XUI_PROXY_IMPL(__Type, __Impl)    {  if (m_Element) { auto x = m_Element.Ptr(false); __Impl; return          this; } else return OnReady([=](__Type* x) __Impl); }
    #define XUI_PROXY_IMPL_EX(__Type, __Impl) {  if (m_Element) { auto x = m_Element.Ptr(false); __Impl; return (t_Base*)this; } else return OnReady([=](__Type* x) __Impl); }

    /// @brief Element interface
    class IXUIElement
    {
        CP_SDK_NO_COPYMOVE_CTORS(IXUIElement);

        public:
            enum class ETypeID
            {
                ColorInput,
                Dropdown,
                FLayout,
                GLayout,
                HLayout,
                HSpacer,
                IconButton,
                Image,
                PrimaryButton,
                SecondaryButton,
                Slider,
                TabControl,
                Text,
                TextInput,
                TextSegmentedControl,
                Toggle,
                VLayout,
                VSpacer,
                VScrollView,
                VVList,
            };

            ETypeID m_TypeID;

        protected:
            std::u16string  m_InitialName;

        public:
            virtual _v::UIFieldRef<_u::RectTransform> RTransform() = 0;

        protected:
            /// @brief Constructor
            /// @param p_Name   Initial name
            /// @param p_TypeID Type ID
            IXUIElement(std::u16string_view p_Name, ETypeID p_TypeID)
                : m_InitialName(p_Name), m_TypeID(p_TypeID)
            { }
            /// @brief Destructor
            virtual ~IXUIElement() = default;

        protected:
            /// @brief Get initial name
            std::u16string_view GetInitialName()
            {
                return m_InitialName;
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) = 0;

    };

}   ///< namespace CP_SDK::XUI