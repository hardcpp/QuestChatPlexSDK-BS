#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/Event.hpp"
#include "LevelData.hpp"
#include "LevelCompletionData.hpp"

#include <System/Action_2.hpp>
#include <UnityEngine/SceneManagement/Scene.hpp>
#include <GlobalNamespace/ScenesTransitionSetupDataSO.hpp>
#include <Zenject/DiContainer.hpp>

namespace CP_SDK_BS::Game {

    namespace _u
    {
        using namespace GlobalNamespace;
        using namespace UnityEngine;
        using namespace UnityEngine::SceneManagement;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Game helper
    class Logic
    {
        CP_SDK_NO_DEF_CTORS(Logic);

        using t_Delegate1 = System::Action_2<UnityW<_u::ScenesTransitionSetupDataSO>, Zenject::DiContainer*>*;

        public:
            /// @brief Scenes
            enum class ESceneType
            {
                None,
                Menu,
                Playing
            };

        private:
            static t_Delegate1              m_Delegate1;
            static ESceneType               m_ActiveScene;
            static LevelData::Ptr           m_LevelData;
            static LevelCompletionData::Ptr m_LevelCompletionData;
            static bool                     m_LastMainSceneWasNotMenu;
            static bool                     m_WasInReplay;

        public:
            static const ESceneType                 ActiveScene();
            static const LevelData::Ptr&            LevelData();
            static const LevelCompletionData::Ptr&  LevelCompletionData();

        public:
            static _v::Event<>                                 _OnUnityActiveSceneChanged;
            static _v::Event<ESceneType>                       OnSceneChange;
            static _v::Event<>                                 OnMenuSceneLoaded;
            static _v::Event<const LevelData::Ptr&>            OnLevelStarted;
            static _v::Event<const LevelCompletionData::Ptr&>  OnLevelEnded;

        public:
            /// @brief On Unity scene change
            /// @param p_Current Current scene
            /// @param p_Next    Next scene
            static void SceneManager_activeSceneChanged(_u::Scene p_Current, _u::Scene p_Next);

        private:
            /// @brief On menu scene active
            static void OnMenuSceneActive();
            /// @brief On menu scene loaded
            /// @param p_Object      Transition object
            /// @param p_DiContainer Container
            static void OnMenuSceneLoadedFresh(_u::ScenesTransitionSetupDataSO* p_Object, Zenject::DiContainer* p_DiContainer);
            /// @brief On game scene active
            static void OnGameSceneActive();

        public:
            /// @brief On level started
            /// @param p_LevelData Level data
            static void FireLevelStarted(const LevelData::Ptr& p_LevelData);
            /// @brief On level ended
            /// @param p_LevelCompletionData Level completion data
            static void FireLevelEnded(const LevelCompletionData::Ptr& p_LevelCompletionData);

    };

}   ///< namespace CP_SDK_BS::Game

