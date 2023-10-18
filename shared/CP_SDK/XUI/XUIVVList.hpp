#pragma once

#include "../UI/Components/CVVList.hpp"
#include "Generics/IXUIElement.hpp"
#include "Generics/IXUIBindable.hpp"
#include "Generics/IXUIElementReady.hpp"

namespace CP_SDK::XUI {

    /// @brief CVVList XUI Element
    class XUIVVList :
        public IXUIElement,
        public IXUIElementReady<XUIVVList, _v::CVVList>,
        public IXUIBindable<XUIVVList>,
        public std::enable_shared_from_this<XUIVVList>
    {
        DISABLE_COPY_MOVECONSTRUCTORS(XUIVVList);
        PRIVATE_TAG();

        private:
            _v::MonoPtr<_v::CVVList> m_Element;

            _v::Event<_v::CVVList*> m_OnReady;

        public:
            std::shared_ptr<XUIVVList> AsShared() { return std::enable_shared_from_this<XUIVVList>::shared_from_this(); }

            virtual _v::UIFieldRef<_u::RectTransform> RTransform() override
            {
                return m_Element ? m_Element->RTransform() : _v::UIFieldDefault<_u::RectTransform>::Value;
            }
            virtual _v::UIFieldRef<_v::CVVList> Element()
            {
                return m_Element;
            }

        public:
            static constexpr ETypeID TypeID = ETypeID::VVList;
            using Ptr = std::shared_ptr<XUIVVList>;

            XUIVVList(PRIVATE_TAG_ARG(), std::u16string_view p_Name)
                : IXUIElement(p_Name, TypeID)
            { }

        public:
            /// @brief Constructor
            static Ptr Make()
            {
                return std::make_shared<XUIVVList>(PRIVATE_TAG_VAL(), u"XUIVVList");
            }
            /// @brief Constructor
            /// @param p_Name Element name
            static Ptr Make(std::u16string_view p_Name)
            {
                return std::make_shared<XUIVVList>(PRIVATE_TAG_VAL(), p_Name);
            }

        public:
            /// @brief BuildUI for this element into p_Parent transform
            /// @param p_Parent Transform to build UI into
            virtual void BuildUI(_u::Transform* p_Parent) override
            {
                m_Element = _v::UISystem::VVListFactory->Create(IXUIElement::GetInitialName(), p_Parent);

                try { m_OnReady(m_Element.Ptr()); m_OnReady.Clear(); }
                catch (const std::exception& l_Exception)
                {
                    ChatPlexSDK::Logger()->Error(u"[CP_SDK.XUI][XUIVVList.BuildUI] Error OnReady:");
                    ChatPlexSDK::Logger()->Error(l_Exception);
                }
            }

        public:
            /// @brief IXUIElement OnReady functor interface
            /// @param p_Functor Functor to add
            virtual XUIVVList* OnReady(_v::CActionRef<_v::CVVList*> p_Functor) override
            {
                if (m_Element)           p_Functor(m_Element.Ptr());
                else m_OnReady        += p_Functor;
                return this;
            }
            /// @brief On ready, bind
            /// @param p_Target Bind target
            virtual XUIVVList* Bind(std::shared_ptr<XUIVVList>* p_Target) override
            {
                *p_Target = std::enable_shared_from_this<XUIVVList>::shared_from_this();
                return this;
            }

        public:
            /// @brief On list item selected event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add Should add
            XUIVVList* OnListItemSelected(_v::Action<const _v::Data::IListItem::Ptr&> p_Functor, bool p_Add = true) XUI_PROXY_IMPL(_v::CVVList, { x->OnListItemSelected(p_Functor, p_Add); })

        public:
            /// @brief Set game object active state
            /// @param p_Active New state
            XUIVVList* SetActive(bool p_Active) XUI_PROXY_IMPL(_v::CVVList, { x->get_gameObject()->SetActive(p_Active); })

        public:
            /// @brief Scroll to position
            /// @param p_TargetPosition New target position
            /// @param p_Animated Is animated?
            XUIVVList* ScrollTo(float p_TargetPosition, bool p_Animated) XUI_PROXY_IMPL(_v::CVVList, { x->ScrollTo(p_TargetPosition, p_Animated); })

        public:
            /// @brief Set list cell prefab
            /// @param p_Prefab New prefab
            XUIVVList* SetListCellPrefab(_v::Data::IListCell* p_Prefab) XUI_PROXY_IMPL(_v::CVVList, { x->SetListCellPrefab(p_Prefab); })
            /// @brief Set list items
            /// @param p_ListItems New items
            XUIVVList* SetListItems(const std::vector<_v::Data::IListItem::Ptr>& p_ListItems) XUI_PROXY_IMPL(_v::CVVList, { x->SetListItems(p_ListItems); })
            /// @brief Set selected list item
            /// @param p_ListItem Selected list item
            /// @param p_Notify Should notify?
            XUIVVList* SetSelectedListItem(const _v::Data::IListItem::Ptr& p_ListItem, bool p_Notify = true) XUI_PROXY_IMPL(_v::CVVList, { x->SetSelectedListItem(p_ListItem, p_Notify); })

        public:
            /// @brief Add a list item
            /// @param p_ListItem Item to add
            XUIVVList* AddListItem(const _v::Data::IListItem::Ptr& p_ListItem) XUI_PROXY_IMPL(_v::CVVList, { x->AddListItem(p_ListItem); })
            /// @brief Sort list items by a functor
            /// @param p_Functor
            XUIVVList* SortListItems(_v::Func<int, const _v::Data::IListItem::Ptr&, const _v::Data::IListItem::Ptr&> p_Functor) XUI_PROXY_IMPL(_v::CVVList, { x->SortListItems(p_Functor); })
            /// @brief Remove a list item
            /// @param p_ListItem Item to remove
            XUIVVList* RemoveListItem(const _v::Data::IListItem::Ptr& p_ListItem)XUI_PROXY_IMPL(_v::CVVList, { x->RemoveListItem(p_ListItem); })

    };

}   ///< namespace CP_SDK::XUI