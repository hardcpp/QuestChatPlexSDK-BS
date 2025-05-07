#pragma once

#include "IListCell.hpp"

#include <memory>

namespace CP_SDK::UI::Data {

    /// @brief Abstract List Item
    class CP_SDK_EXPORT IListItem : public std::enable_shared_from_this<IListItem>
    {
        CP_SDK_NO_COPYMOVE_CTORS(IListItem);

        public:
            using Ptr = std::shared_ptr<IListItem>;

        private:
            Utils::MonoPtr<IListCell> m_Cell;

        public:
            IListCell* Cell();

        public:
            /// @brief Constructor
            IListItem();
            /// @brief Destructor
            virtual ~IListItem();

        public:
            /// @brief Set displayed Cell
            /// @param p_Cell New display cell
            void SetCell(IListCell* p_Cell);

        public:
            /// @brief On show
            virtual void OnShow();
            /// @brief On hide
            virtual void OnHide();

        public:
            /// @brief On select
            virtual void OnSelect();
            /// @brief On Unselect
            virtual void OnUnselect();

        protected:
            /// @brief Convert and test Cell to t_CellType
            /// @tparam t_CellType Cell type
            /// @return Converted or nullptr
            template<class t_CellType>
            t_CellType GetCellAsClassOf()
            {
                if (Cell() && classof(t_CellType) == Cell()->klass)
                    return reinterpret_cast<t_CellType>(Cell());

                return nullptr;
            }

    };

}   ///< namespace CP_SDK::UI::Data