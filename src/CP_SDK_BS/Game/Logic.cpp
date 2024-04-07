#include "CP_SDK_BS/Game/Logic.hpp"
#include "CP_SDK_BS/Game/Levels.hpp"
#include "CP_SDK_BS/Game/Scoring.hpp"
#include "CP_SDK_BS/Game/Patches/PMissionLevelScenesTransitionSetupDataSO.hpp"
#include "CP_SDK_BS/Game/Patches/PMultiplayerLevelScenesTransitionSetupDataSO.hpp"
#include "CP_SDK_BS/Game/Patches/PStandardLevelScenesTransitionSetupDataSO.hpp"
#include "CP_SDK_BS/UI/LevelDetail.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <custom-types/shared/delegate.hpp>
#include <GlobalNamespace/GameScenesManager.hpp>
#include <GlobalNamespace/ScoreModel.hpp>
#include <System/Action_2.hpp>
#include <UnityEngine/Resources.hpp>

static bool m_WasChatPlexUnityInitialized = false;

namespace CP_SDK_BS::Game {

    Logic::t_Delegate1          Logic::m_Delegate1                  = nullptr;
    Logic::ESceneType           Logic::m_ActiveScene                = Logic::ESceneType::None;
    LevelData::Ptr              Logic::m_LevelData                  = nullptr;
    LevelCompletionData::Ptr    Logic::m_LevelCompletionData        = nullptr;
    bool                        Logic::m_LastMainSceneWasNotMenu    = false;
    bool                        Logic::m_WasInReplay                = false;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    _v::Event<>                                   Logic::_OnUnityActiveSceneChanged;
    _v::Event<Logic::ESceneType>                  Logic::OnSceneChange;
    _v::Event<>                                   Logic::OnMenuSceneLoaded;
    _v::Event<const LevelData::Ptr&>              Logic::OnLevelStarted;
    _v::Event<const LevelCompletionData::Ptr&>    Logic::OnLevelEnded;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    const Logic::ESceneType Logic::ActiveScene()
    {
        return m_ActiveScene;
    }
    const LevelData::Ptr& Logic::LevelData()
    {
        return m_LevelData;
    }
    const LevelCompletionData::Ptr& Logic::LevelCompletionData()
    {
        return m_LevelCompletionData;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On Unity scene change
    /// @param p_Current Current scene
    /// @param p_Next    Next scene
    void Logic::SceneManager_activeSceneChanged(_u::Scene p_Current, _u::Scene p_Next)
    {
#if DEBUG_SCENES
        CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.SceneManager_activeSceneChanged] " + p_Next.get_name() + u" ======");
#endif

        try
        {
            _OnUnityActiveSceneChanged();

            if (p_Next.get_name() == u"GameCore")
                OnGameSceneActive();
            else if (p_Next.get_name() == u"MainMenu")
            {
                if (ActiveScene() != ESceneType::Menu)
                    OnMenuSceneActive();

                auto l_GameScenesManager = _u::Resources::FindObjectsOfTypeAll<_u::GameScenesManager*>()->FirstOrDefault();
                if (l_GameScenesManager != nullptr)
                {
                    if (p_Current.get_name() == "EmptyTransition" && !m_LastMainSceneWasNotMenu)
                    {
                        if (!m_Delegate1)
                            m_Delegate1 = custom_types::MakeDelegate<t_Delegate1>(std::function(OnMenuSceneLoadedFresh));

                        l_GameScenesManager->remove_transitionDidFinishEvent(m_Delegate1);
                        l_GameScenesManager->add_transitionDidFinishEvent(m_Delegate1);
                    }
                }

                m_LastMainSceneWasNotMenu = false;
            }

            if (p_Next.get_name() == "GameCore" || p_Next.get_name() == "Credits" || p_Next.get_name() == "BeatmapEditor")
                m_LastMainSceneWasNotMenu = true;
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Logic.SceneManager_activeSceneChanged] Error :");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief  On menu scene active
    void Logic::OnMenuSceneActive()
    {
#if DEBUG_SCENES
        CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.OnMenuSceneActive] ======");
#endif

        try
        {
            m_ActiveScene = ESceneType::Menu;
            m_LevelData   = nullptr;

            CP_SDK::ChatPlexSDK::Fire_OnGenericMenuScene();

            OnSceneChange.Invoke(m_ActiveScene);

            if (m_LevelCompletionData != nullptr)
                OnLevelEnded.Invoke(m_LevelCompletionData);
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Logic.OnMenuSceneActive] Error :");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);
        }
    }
    /// @brief On menu scene loaded
    /// @param p_Object      Transition object
    /// @param p_DiContainer Container
    void Logic::OnMenuSceneLoadedFresh(_u::ScenesTransitionSetupDataSO* p_Object, Zenject::DiContainer* p_DiContainer)
    {
#if DEBUG_SCENES
        CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.OnMenuSceneLoadedFresh] ======");
#endif

        try
        {
            if (!m_WasChatPlexUnityInitialized)
            {
                CP_SDK::ChatPlexSDK::OnUnityReady();
                m_WasChatPlexUnityInitialized = true;
            }

            auto l_GameScenesManager = _u::Resources::FindObjectsOfTypeAll<_u::GameScenesManager*>()->FirstOrDefault();
            if (l_GameScenesManager != nullptr)
                l_GameScenesManager->remove_transitionDidFinishEvent(m_Delegate1);

            UI::LevelDetail::Init();
            Levels::GetDefaultPackCover();

            m_ActiveScene           = ESceneType::Menu;
            m_LevelData             = nullptr;
            m_LevelCompletionData   = nullptr;
            m_WasInReplay           = false;

            CP_SDK::ChatPlexSDK::Fire_OnGenericMenuSceneLoaded();

            OnMenuSceneLoaded.Invoke();

            OnMenuSceneActive();
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Logic.OnMenuSceneLoadedFresh] Error :");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);
        }
    }
    /// @brief On game scene active
    void Logic::OnGameSceneActive()
    {
#if DEBUG_SCENES
        CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.OnGameSceneActive] ======");
#endif
        try
        {
            /// Catch new map restart mechanic
            if (m_ActiveScene == ESceneType::Playing && m_LevelCompletionData != nullptr)
            {
                OnLevelEnded(m_LevelCompletionData);

                if (m_LevelData != nullptr)
                {
                    switch (m_LevelData->Type)
                    {
                        case LevelType::Solo:
                            Patches::PStandardLevelScenesTransitionSetupDataSO::RestoreLevelData(m_LevelData);
                            Patches::PMissionLevelScenesTransitionSetupDataSO::RestoreLevelData(m_LevelData);
                            break;

                        case LevelType::Multiplayer:
                            Patches::PStandardLevelScenesTransitionSetupDataSO::RestoreLevelData(m_LevelData);
                            break;

                        case LevelType::None:
                        default:
                            break;

                    }
                }
            }

            m_ActiveScene           = ESceneType::Playing;
            m_WasInReplay           = Scoring::IsInReplay();

            CP_SDK::ChatPlexSDK::Fire_OnGenericPlayingScene();

            OnSceneChange(m_ActiveScene);

            if (m_LevelData != nullptr)
            {
                m_LevelData->IsReplay = m_WasInReplay;

                if (m_LevelData->Data && m_LevelData->Data->transformedBeatmapData)
                    m_LevelData->MaxMultipliedScore = _u::ScoreModel::ComputeMaxMultipliedScoreForBeatmap(m_LevelData->Data->transformedBeatmapData);

                m_LevelCompletionData = nullptr;

#if DEBUG_SCENES
                CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.OnGameSceneActive] OnLevelStarted ======");
#endif

                OnLevelStarted(m_LevelData);
            }
        }
        catch (const std::exception& p_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.Game][Logic.OnGameSceneActive] Error :");
            CP_SDK::ChatPlexSDK::Logger()->Error(p_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On level started
    /// @param p_LevelData Level data
    void Logic::FireLevelStarted(const LevelData::Ptr& p_LevelData)
    {
#if DEBUG_SCENES
        CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.FireLevelStarted] ======");
#endif
        m_LevelCompletionData     = nullptr;
        m_LevelData               = p_LevelData;

        if (m_LevelData != nullptr && m_LevelData->Data && m_LevelData->Data->transformedBeatmapData)
            m_LevelData->MaxMultipliedScore = _u::ScoreModel::ComputeMaxMultipliedScoreForBeatmap(m_LevelData->Data->transformedBeatmapData);
    }
    /// @brief On level ended
    /// @param p_LevelCompletionData Level completion data
    void Logic::FireLevelEnded(const LevelCompletionData::Ptr& p_LevelCompletionData)
    {
#if DEBUG_SCENES
        CP_SDK::ChatPlexSDK::Logger()->Error(u"====== [CP_SDK_BS.Game][Logic.FireLevelEnded] ======");
#endif

        m_LevelCompletionData             = p_LevelCompletionData;
        m_LevelCompletionData->IsReplay   = m_WasInReplay;

        if (m_LevelCompletionData != nullptr && m_LevelCompletionData->Data && m_LevelCompletionData->Data->transformedBeatmapData)
            m_LevelCompletionData->MaxMultipliedScore = _u::ScoreModel::ComputeMaxMultipliedScoreForBeatmap(m_LevelCompletionData->Data->transformedBeatmapData);
    }

}   ///< namespace CP_SDK_BS::Game
