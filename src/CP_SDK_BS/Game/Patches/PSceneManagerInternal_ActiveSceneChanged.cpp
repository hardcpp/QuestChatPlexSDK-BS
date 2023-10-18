
#include "CP_SDK/Utils/Il2cpp.hpp"
#include "CP_SDK/Unity/MTThreadInvoker.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"
#include "CP_SDK_BS/Game/Logic.hpp"

#include <GlobalNamespace/GameScenesManager.hpp>
#include <UnityEngine/SceneManagement/SceneManager.hpp>
#include <UnityEngine/SceneManagement/Scene.hpp>
#include <UnityEngine/Resources.hpp>

namespace CP_SDK_BS::Game::Patches {

    CP_SDK_IL2CPP_HOOK_MAKE_AUTO_HOOK_MATCH(
        SceneManagerInternal_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged,
        void, UnityEngine::SceneManagement::Scene p_Current, UnityEngine::SceneManagement::Scene p_Next)
    {
        SceneManagerInternal_ActiveSceneChanged(p_Current, p_Next);
        Logic::SceneManager_activeSceneChanged(p_Current, p_Next);
    }

}   ///< namespace CP_SDK_BS::Game::Patches
