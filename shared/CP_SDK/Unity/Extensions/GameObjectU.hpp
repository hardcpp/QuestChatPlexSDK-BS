#pragma once

#include "../../Utils/MonoPtr.hpp"

#include <UnityEngine/GameObject.hpp>
#include <UnityEngine/Transform.hpp>

namespace CP_SDK::Unity::Extensions {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Unity GameObject tools
    class CP_SDK_EXPORT_VISIBILITY GameObjectU
    {
        public:
            /// @brief Change the layer of a GameObject and all his childs
            /// @param p_This Source object
            /// @param p_Layer Target layer
            inline static void ChangerLayerRecursive(_u::GameObject* p_This, int p_Layer)
            {
                if (!_v::IsUnityPtrValid(p_This))
                    return;

                p_This->set_layer(p_Layer);

                auto l_ChildCount = p_This->get_transform()->get_childCount();
                for (auto l_I = 0; l_I < l_ChildCount; ++l_I)
                {
                    auto l_Child = p_This->get_transform()->GetChild(l_I)->get_gameObject();
                    if (l_Child.isAlive())
                        ChangerLayerRecursive(l_Child, p_Layer);
                }
            }
            /// @brief Find common root between 2 game objects
            /// @param p_Left
            /// @param p_Right
            inline static _u::GameObject* FindCommonRoot(_u::GameObject* p_Left, _u::GameObject* p_Right)
            {
                if (!_v::IsUnityPtrValid(p_Left) || !_v::IsUnityPtrValid(p_Right))
                    return nullptr;

                auto l_LeftTransform = p_Left->get_transform();
                while (l_LeftTransform.isAlive())
                {
                    auto l_RightTransform = p_Right->get_transform();
                    while (l_RightTransform.isAlive())
                    {
                        if (l_LeftTransform == l_RightTransform)
                            return l_LeftTransform->get_gameObject();

                        l_RightTransform = l_RightTransform->get_parent();
                    }

                    l_LeftTransform = l_LeftTransform->get_parent();
                }

                return nullptr;
            }
            /// @brief Destroy child gameobjects
            /// @param p_This Instance
            inline static void DestroyChilds(_u::GameObject* p_This)
            {
                if (!_v::IsUnityPtrValid(p_This))
                    return;

                auto l_ChildCount = p_This->get_transform()->get_childCount();
                for (auto l_I = (l_ChildCount - 1); l_I >= 0; l_I--)
                {
                    _u::GameObject::Destroy(p_This->get_transform()->GetChild(l_I)->get_gameObject());
                }
            }

    };

}   ///< namespace CP_SDK::Unity::Extensions