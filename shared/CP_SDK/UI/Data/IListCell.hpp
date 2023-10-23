#pragma once

#include "../UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/EventSystems/IEventSystemHandler.hpp>
#include <UnityEngine/EventSystems/IPointerEnterHandler.hpp>
#include <UnityEngine/EventSystems/IPointerExitHandler.hpp>
#include <UnityEngine/EventSystems/PointerEventData.hpp>
#include <UnityEngine/UI/Button.hpp>
#include <UnityEngine/UI/Image.hpp>

namespace CP_SDK::UI::Components {

    class CVXList;

}   ///< namespace CP_SDK::UI::Components

namespace CP_SDK::UI::Data {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::EventSystems;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::UI::Components;
        using namespace CP_SDK::Utils;
    }

    class IListItem;

    /// @brief Abstract List Cell component
    class IListCell : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.Data", IListCell, _u::MonoBehaviour, CP_SDK_IL2CPP_INTERFACES(
            _u::IEventSystemHandler*,
            _u::IPointerEnterHandler*,
            _u::IPointerExitHandler*
        ));
        CP_SDK_IL2CPP_DECLARE_CTOR(IListCell);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(IListCell);

        private:
            _v::MonoPtr<_u::RectTransform>  m_RTransform;
            _v::MonoPtr<_u::Image>          m_Image;
            _v::MonoPtr<_u::Button>         m_Button;
            _v::MonoPtr<_v::CVXList>        m_OwnerList;
            int                             m_Index;
            std::shared_ptr<IListItem>      m_ListItem;
            bool                            m_Stateless;

        protected:
            _v::Action<>                            m_Build;
            _v::Func<float>                         m_GetCellHeight;
            _v::Func<IListCell*, _u::GameObject*>   m_AddSelfComponent;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            UIFieldRefDel<_v::CVXList>          OwnerList;
            int                                 Index() const;
            const std::shared_ptr<IListItem>&   ListItem();
            std::u16string                      Tooltip;

        public:
            /// @brief Create cell instance
            /// @param p_Parent Parent
            IListCell* Create(_u::RectTransform* p_Parent);
            /// @brief Bind to list
            /// @param p_OwnerList Owner list
            /// @param p_Index List item index
            /// @param p_ListItem List item instance
            void Bind(_v::CMonoPtrRef<_v::CVXList> p_OwnerList, int p_Index, const std::shared_ptr<IListItem>& p_ListItem);

        public:
            /// @brief Build cell
            void Build();
            /// @brief Set list cell state
            /// @param p_State New state
            void SetState(bool p_State);
            /// @brief Set is stateless
            /// @param p_Stateless New value
            void SetStateless(bool p_Stateless);
            /// @brief Set is selectable
            /// @param p_Selectable New state
            void SetSelectable(bool p_Selectable);
            /// @brief Get cell height
            float GetCellHeight();

        private:
            /// @brief On cell click
            void Button_OnClick();

        public:
            /// @brief On pointer enter
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerEnterHandler, void, OnPointerEnter, _u::PointerEventData*);
            /// @brief On pointer exit
            /// @param p_EventData Event data
            CP_SDK_IL2CPP_OVERRIDE_METHOD_EX(_u, IPointerExitHandler,  void, OnPointerExit,  _u::PointerEventData*);

    };

}   ///< namespace CP_SDK::UI::Data

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Data::IListCell);