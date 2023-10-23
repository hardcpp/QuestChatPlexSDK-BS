#pragma once

#include "IXUIElement.hpp"

#include <UnityEngine/Transform.hpp>

namespace CP_SDK::XUI {

    /// @brief IXUIElement with children abstract class
    /// @tparam t_Base Return type for daisy chaining
    template<class t_Base>
    class IXUIElementWithChilds
        : public IXUIElement
    {
        CP_SDK_NO_COPYMOVE_CTORS(IXUIElementWithChilds);

        protected:
            std::vector<std::shared_ptr<IXUIElement>> m_Childs;

        protected:

            /// @brief Constructor
            /// @param p_Name   Element name
            /// @param p_TypeID Type ID
            /// @param p_Childs Child XUI Elements
            IXUIElementWithChilds(std::u16string_view p_Name, ETypeID p_TypeID, const std::vector<std::shared_ptr<IXUIElement>>& p_Childs)
                : IXUIElement(p_Name, p_TypeID), m_Childs(p_Childs)
            { }
            /// @brief Destructor
            virtual ~IXUIElementWithChilds() = default;

        protected:
            /// @brief BuildUI for children into p_Parent transform
            /// @param p_Parent Transform to build UI into
            void BuildUIChilds(_u::Transform* p_Parent)
            {
                if (m_Childs.empty())
                    return;

                for (auto l_I = 0; l_I < m_Childs.size(); ++l_I)
                    m_Childs[l_I]->BuildUI(p_Parent);
            }

        public:
            /// @brief For each direct XUIElement child of type t_ChildType
            /// @tparam t_ChildType XUIElement type
            /// @param p_Functor Functor callback
            template<class t_ChildType>
            t_Base* ForEachDirect(_v::CActionRef<t_ChildType*> p_Functor)
            {
                for (auto l_I = 0; l_I < m_Childs.size(); ++l_I)
                {
                    if (m_Childs[l_I]->m_TypeID != t_ChildType::TypeID)
                        continue;

                    p_Functor(reinterpret_cast<t_ChildType*>(m_Childs[l_I].get()));
                }

                return reinterpret_cast<t_Base*>(this);
            }

    };

}   ///< namespace CP_SDK::XUI