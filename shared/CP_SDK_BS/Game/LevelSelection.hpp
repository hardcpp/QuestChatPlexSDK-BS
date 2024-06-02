#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"

#include <custom-types/shared/coroutine.hpp>

#include <GlobalNamespace/BeatmapLevel.hpp>
#include <GlobalNamespace/LevelSelectionNavigationController.hpp>
#include <GlobalNamespace/LevelSearchViewController.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace GlobalNamespace;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Level selection filter
    class CP_SDK_EXPORT_VISIBILITY LevelSelection
    {
        CP_SDK_NO_DEF_CTORS(LevelSelection);

        private:
            static _v::MonoPtr<_u::BeatmapLevel>   m_PendingFilterSong;
            static bool                                         m_PreventLevelSearchViewController_didStartLoadingEvent;

        public:
            /// @brief Filter to specific song
            /// @param p_SongToFilter Song to filter
            static bool FilterToSpecificSong(_u::BeatmapLevel* p_SongToFilter);

        public:
            /// @brief Change current song view to all songs view
            /// @param p_LevelSelectionNavigationController instance
            static void LevelSelectionNavigationController_didActivateEvent(_u::LevelSelectionNavigationController* p_LevelSelectionNavigationController);

        private:
            /// @brief Level selection, select level category
            /// @param p_LevelSelectionNavigationController LevelSelectionNavigationController instance
            static custom_types::Helpers::Coroutine LevelSelection_SelectLevelCategory(_u::LevelSelectionNavigationController* p_LevelSelectionNavigationController);
            /// @brief Level selection, filter
            /// @param p_LevelSearchViewController LevelSearchViewController instance
            /// @param p_Wait Should wait for any transition
            static custom_types::Helpers::Coroutine LevelSelection_FilterLevel(_u::LevelSearchViewController* p_LevelSearchViewController, bool p_Wait);

        public:
            /// @brief LevelSearchViewController didStartLoadingEvent
            /// @param p_LevelSearchViewController LevelSearchViewController instance
            static void LevelSearchViewController_didStartLoadingEvent(_u::LevelSearchViewController* p_LevelSearchViewController);

    };

}   ///< namespace CP_SDK_BS::Game

