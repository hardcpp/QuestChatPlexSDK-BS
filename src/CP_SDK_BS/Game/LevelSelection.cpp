#include "CP_SDK_BS/Game/LevelSelection.hpp"
#include "CP_SDK_BS/Game/Logic.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"
#include <custom-types/shared/delegate.hpp>
#include <beatsaber-hook/shared/utils/byref.hpp>

#include <HMUI/ViewController.hpp>
#include <HMUI/IconSegmentedControl.hpp>
#include <HMUI/InputFieldView.hpp>
#include <GlobalNamespace/LevelFilteringNavigationController.hpp>
#include <GlobalNamespace/LevelFilter.hpp>
#include <GlobalNamespace/SelectLevelCategoryViewController.hpp>
#include <System/Collections/Generic/HashSet_1.hpp>
#include <UnityEngine/Resources.hpp>

namespace CP_SDK_BS::Game {

    _v::MonoPtr<_u::BeatmapLevel>   LevelSelection::m_PendingFilterSong;
    bool                            LevelSelection::m_PreventLevelSearchViewController_didStartLoadingEvent;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    static bool m_CatchNextLevelSelectionNavigationController_DidActivate = false;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Filter to specific song
    /// @param p_SongToFilter Song to filter
    bool LevelSelection::FilterToSpecificSong(_u::BeatmapLevel* p_SongToFilter)
    {
        m_PendingFilterSong = p_SongToFilter;

        try
        {
            auto l_LevelFilteringNavigationController = _u::Resources::FindObjectsOfTypeAll<_u::LevelSelectionNavigationController*>()->FirstOrDefault();
            if (l_LevelFilteringNavigationController)
            {
                if (l_LevelFilteringNavigationController->get_gameObject()->get_activeInHierarchy())
                {
                    m_CatchNextLevelSelectionNavigationController_DidActivate = false;
                    LevelSelectionNavigationController_didActivateEvent(l_LevelFilteringNavigationController);
                }
                else
                    m_CatchNextLevelSelectionNavigationController_DidActivate = true;

                return true;
            }
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][LevelSelection.FilterToSpecificSong] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Change current song view to all songs view
    /// @param p_LevelSelectionNavigationController instance
    void LevelSelection::LevelSelectionNavigationController_didActivateEvent(_u::LevelSelectionNavigationController* p_LevelSelectionNavigationController)
    {
        CP_SDK::Unity::MTCoroutineStarter::Start(custom_types::Helpers::CoroutineHelper::New(
            LevelSelection_SelectLevelCategory(p_LevelSelectionNavigationController)
        ));
    }
    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        LevelSelectionNavigationController_DidActivate, &_u::LevelSelectionNavigationController::DidActivate,
        void, _u::LevelSelectionNavigationController* __Instance,
        bool __a, bool __b, bool __c)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter LevelSelectionNavigationController_DidActivate");

        LevelSelectionNavigationController_DidActivate(__Instance, __a, __b, __c);

        try
        {
            if (m_CatchNextLevelSelectionNavigationController_DidActivate)
            {
                LevelSelection::LevelSelectionNavigationController_didActivateEvent(__Instance);
                m_CatchNextLevelSelectionNavigationController_DidActivate = false;
            }
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][LevelSelection.LevelSelectionNavigationController_DidActivate] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit LevelSelectionNavigationController_DidActivate");
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Level selection, select level category
    /// @param p_LevelSelectionNavigationController LevelSelectionNavigationController instance
    custom_types::Helpers::Coroutine LevelSelection::LevelSelection_SelectLevelCategory(_u::LevelSelectionNavigationController* p_LevelSelectionNavigationController)
    {
        while (!_v::IsUnityPtrValid(p_LevelSelectionNavigationController) || p_LevelSelectionNavigationController->____isInTransition)
        {
            if (!_v::IsUnityPtrValid(p_LevelSelectionNavigationController))
                co_return;

            co_yield nullptr;
        }

        if (Logic::ActiveScene() != Logic::ESceneType::Menu)
            co_return;

        if (!p_LevelSelectionNavigationController || !p_LevelSelectionNavigationController->get_isInViewControllerHierarchy() || !p_LevelSelectionNavigationController->get_isActiveAndEnabled())
            co_return;

        auto l_LevelFilteringNavigationController = p_LevelSelectionNavigationController->____levelFilteringNavigationController;
        if (!l_LevelFilteringNavigationController.isAlive())
            co_return;

        if (l_LevelFilteringNavigationController->get_selectedLevelCategory() != _u::SelectLevelCategoryViewController::LevelCategory::All)
        {
            auto l_Selector = l_LevelFilteringNavigationController->____selectLevelCategoryViewController;
            if (l_Selector.isAlive())
            {
                auto l_SegmentControl    = l_Selector->____levelFilterCategoryIconSegmentedControl;
                auto l_Tags              = l_Selector->____levelCategoryInfos;
                auto l_IndexToSelect     = -1;


                for (auto l_I = 0; l_I < l_Tags->get_Length(); ++l_I)
                {
                    if (l_Tags[l_I]->___levelCategory != _u::SelectLevelCategoryViewController::LevelCategory::All)
                        continue;

                    l_IndexToSelect = l_I;
                    break;
                }

                /// Multiplayer : missing extension
                if (l_IndexToSelect == -1)
                    co_return;

                l_SegmentControl->SelectCellWithNumber(l_IndexToSelect);
                l_Selector->LevelFilterCategoryIconSegmentedControlDidSelectCell(l_SegmentControl, l_IndexToSelect);

                CP_SDK::Unity::MTCoroutineStarter::Start(custom_types::Helpers::CoroutineHelper::New(
                    LevelSelection_FilterLevel(
                        l_LevelFilteringNavigationController->____levelSearchViewController,
                        true
                    )
                ));
            }
        }
        else
        {
            CP_SDK::Unity::MTCoroutineStarter::Start(custom_types::Helpers::CoroutineHelper::New(
                LevelSelection_FilterLevel(
                    l_LevelFilteringNavigationController->____levelSearchViewController,
                    false
                )
            ));
        }
    }
    /// @brief Level selection, filter
    /// @param p_LevelSearchViewController LevelSearchViewController instance
    /// @param p_Wait Should wait for any transition
    custom_types::Helpers::Coroutine LevelSelection::LevelSelection_FilterLevel(_u::LevelSearchViewController* p_LevelSearchViewController, bool p_Wait)
    {
        if (Logic::ActiveScene() != Logic::ESceneType::Menu)
            co_return;

        if (!_v::IsUnityPtrValid(p_LevelSearchViewController) || !m_PendingFilterSong)
            co_return;

        if (p_Wait)
        {
            while (!_v::IsUnityPtrValid(p_LevelSearchViewController) || p_LevelSearchViewController->____isInTransition)
            {
                if (!_v::IsUnityPtrValid(p_LevelSearchViewController))
                    co_return;

                co_yield nullptr;
            }

            if (!_v::IsUnityPtrValid(p_LevelSearchViewController) || !p_LevelSearchViewController->get_isInViewControllerHierarchy() || !p_LevelSearchViewController->get_isActiveAndEnabled())
                co_return;

            if (Logic::ActiveScene() != Logic::ESceneType::Menu)
                co_return;
        }

        try
        {
            m_PreventLevelSearchViewController_didStartLoadingEvent = true;
            p_LevelSearchViewController->ResetAllFilterSettings(false);

            auto l_Set = System::Collections::Generic::HashSet_1<::StringW>::New_ctor();
            l_Set->Add(m_PendingFilterSong->___levelID);

            auto filter = GlobalNamespace::LevelFilter(false, false, false, GlobalNamespace::BeatmapDifficultyMask::All, GlobalNamespace::SongPackMask::get_all(), "", 0.0f, 0.0f, 0, "", l_Set);
            p_LevelSearchViewController->Refresh(
                byref(filter)
            );
            m_PreventLevelSearchViewController_didStartLoadingEvent = false;
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][LevelSelection.LevelSelection_FilterLevel] coroutine failed : ");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);

            m_PreventLevelSearchViewController_didStartLoadingEvent = false;
            LevelSearchViewController_didStartLoadingEvent(p_LevelSearchViewController);
        }
    }
    /// @brief LevelSearchViewController didStartLoadingEvent
    /// @param p_LevelSearchViewController LevelSearchViewController instance
    void LevelSelection::LevelSearchViewController_didStartLoadingEvent(_u::LevelSearchViewController* p_LevelSearchViewController)
    {
        if (m_PreventLevelSearchViewController_didStartLoadingEvent)
            return;

        try
        {
            auto l_Filter = p_LevelSearchViewController->____currentSearchFilter;
            if (l_Filter.limitIds && l_Filter.limitIds->get_Length() == 1)
            {
                p_LevelSearchViewController->ResetAllFilterSettings(false);

                auto l_InputFieldView = p_LevelSearchViewController->____searchTextInputFieldView;
                if (l_InputFieldView)
                {
                    l_InputFieldView->UpdateClearButton();
                    l_InputFieldView->UpdatePlaceholder();
                }
            }
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game.Patches][LevelSearchViewController_didStartLoadingEvent] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        LevelSearchViewController_RefreshAsync, &_u::LevelSearchViewController::RefreshAsync,
        void, _u::LevelSearchViewController* __Instance)
    {
        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Enter LevelSearchViewController_RefreshAsync");

        LevelSearchViewController_RefreshAsync(__Instance);

        try
        {
            LevelSelection::LevelSearchViewController_didStartLoadingEvent(__Instance);
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][LevelSelection.LevelSearchViewController_RefreshAsync] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }

        //CP_SDK::ChatPlexSDK::Logger()->Error(u"Exit LevelSearchViewController_RefreshAsync");
    }

}   ///< namespace CP_SDK_BS::Game
