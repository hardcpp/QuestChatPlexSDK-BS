#pragma once

#include "IListCell.hpp"

#include <memory>

namespace CP_SDK::UI::Data {

    namespace _u
    {
        using namespace System;
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief List cell prefabs getter
    /// @tparam t_ListCellType List cell type
    template<class t_ListCellType>
    class CP_SDK_EXPORT_VISIBILITY ListCellPrefabs
    {
        private:
            static _v::MonoPtr<t_ListCellType> m_Prefab;

        public:
            IListCell* Cell();

        public:
            /// @brief Get prefab
            static t_ListCellType* Get()
            {
                if (m_Prefab)
                    return m_Prefab.Ptr();

                m_Prefab = _u::GameObject::New_ctor(std::string(classof(t_ListCellType*)->name) + "ListCellPrefab", ArrayW<_u::Type*>({
                    reinterpret_cast<_u::Type*>(csTypeOf(_u::RectTransform*).convert()),
                    reinterpret_cast<_u::Type*>(csTypeOf(t_ListCellType*).convert())
                }))->GetComponent<t_ListCellType*>();

                _u::GameObject::DontDestroyOnLoad(m_Prefab->get_gameObject());
                m_Prefab->get_gameObject()->SetActive(false);

                return m_Prefab.Ptr();
            }

    };

    template <typename t_ListCellType> _v::MonoPtr<t_ListCellType> ListCellPrefabs<t_ListCellType>::m_Prefab;

}   ///< namespace CP_SDK::UI::Data