Quest version of ChatPlex SDK for BeatSaber

This SDK was created to simplify and streamline mods creation for multiple games. It mostly provides generic components and utilities for Unity modding and also specific components and utilities for BeatSaber

Each mods using ChatPlexSDK-BS can declare a module (optional, described below), this module is like an interface for the SDK to manage your mod, show settings UI and a enable/disable toggle if the module `Type` is defined as `Integrated`

Components (Full list in documentation):
- **CP_SDK** *ChatPlex SDK game agnostic namepace*
  * **Animation** *Animated image loading and processing*
  * **Chat** *Chat service for connecting to various live streaming platforms chat*
  * **Config** *Json configuration utilities*
  * **Logging** *Logging utilities*
  * **Network** *Network and HTTP utilities*
  * **Pool** *Memory management & pools utilities*
  * **UI** *User interface components, views, flow coordinator, builders and factories*
  * **Unity** *Tools and extensions to interact with Unity on different layer & threads, load fonts/sprites/textures*
  * **Utils** *Various platform utils like Delegate/Action/Function/Event system*
  * **XUI** *Tree like syntax CP_SDK::UI builder*
- **CP_SDK_BS** *ChatPlex SDK BeatSaber specific*
  * **Game** *Game specific helper and utilities*
  * **UI** *BeatSaber specific UI compoenents and utilities*

# Documentation
[Doxygen Documentation](https://documentation.chatplex.org/QuestChatPlexSDK-BS/namespaces.html)

# Getting the SDK
To get the library, simply head over to the [releases](https://github.com/hardcpp/QuestChatPlexSDK-BS/releases) section, or build it yourself.
If you want to develop with the SDK simply follow the instructions below in `# How to use (As a dependency)`

# Building the library
To build the library, make sure you have QPM, CMake, Ninja installed
```sh
git clone https://github.com/hardcpp/QuestChatPlexSDK-BS.git --recursive
cd QuestChatPlexSDK-BS
qpm restore
qpm qmod build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -B build
cmake --build ./build
```

# How to use (As a dependency)
Add this project as a dependence with QPM
```
qpm dependency add chatplex-sdk-bs
qpm restore
qpm qmod build
```

Add include path in `CMakeLists.txt` after `# codegen includes`
```cmake
# chatplex-sdk-bs includes
target_include_directories(${COMPILE_ID} PRIVATE ${EXTERN_DIR}/includes/chatplex-sdk-bs/shared)
```

# How to use (As a module)
Add this project as a dependence with QPM
```
qpm dependency add chatplex-sdk-bs
qpm restore
qpm qmod build
```

Add defines in `CMakeLists.txt` after `include(qpm_defines.cmake)`
```cmake
add_definitions(-DCP_SDK_BMBF)
```

Add include path in `CMakeLists.txt` after `# codegen includes`
```cmake
# chatplex-sdk-bs includes
target_include_directories(${COMPILE_ID} PRIVATE ${EXTERN_DIR}/includes/chatplex-sdk-bs/shared)
```

Create your logger handler
```cpp
#pragma once

#include <CP_SDK/Logging/ILogger.hpp>

namespace MyModNamespace {

    /// @brief Logger instance holder
    class Logger
    {
        public:
            /// @brief Logger instance
            static CP_SDK::Logging::ILogger* Instance;

    };

}   ///< namespace MyModNamespace
```

Create your module declaration
`MyModule.hpp`
```cpp
#pragma once

#include "UI/SettingsLeftView.hpp"
#include "UI/SettingsMainView.hpp"
#include "MPConfig.hpp"

#include <CP_SDK/ModuleBase.hpp>
#include <CP_SDK/ChatPlexSDK.hpp>

namespace MyModNamespace {

    /// @brief My Module
    class MyModule : public CP_SDK::ModuleBase<Multiplayer>
    {
        CP_SDK_MODULE_BASE_INSTANCE_DECL(Multiplayer);

        public:
            CP_SDK::EIModuleBaseType            Type()                      const override { return CP_SDK::EIModuleBaseType::External;                     }
            std::u16string_view                 Name()                      const override { return u"MyModule";                                            }
            std::u16string_view                 FancyName()                 const override { return u"MyModule";                                            }
            std::u16string_view                 Description()               const override { return u"My description";                                      }
            std::u16string_view                 DocumentationURL()          const override { return u"MyModule documentation URL";                          }
            bool                                UseChatFeatures()           const override { return false;                                                  }
            bool                                IsEnabled()                 const override { return true;                                                   }
            void                                IsEnabled(bool p_Enabled)         override {                                                                }
            CP_SDK::EIModuleBaseActivationType  ActivationType()            const override { return CP_SDK::EIModuleBaseActivationType::OnMenuSceneLoaded;  }

        public:
            /// @brief Constructor
            MyModule();

        protected:
            /// @brief Enable the Module
            void OnEnable() override;
            /// @brief Disable the Module
            void OnDisable() override;

        public:
            /// @brief Get Module settings UI
            t_InitialViews GetSettingsViewControllers() override;

    };

}   ///< namespace MyModNamespace
```

Create your module implementation
`MyModule.cpp`
```cpp
#include "Multiplayer.hpp"
#include "Logger.hpp"

namespace MyModNamespace {

    CP_SDK_MODULE_BASE_INSTANCE_IMPL(MyModule);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Constructor
    MyModule::MyModule()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enable the Module
    void MyModule::OnEnable()
    {

    }
    /// @brief Disable the Module
    void MyModule::OnDisable()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Get Module settings UI
    MyModule::t_InitialViews MyModule::GetSettingsViewControllers()
    {
        return { nullptr, nullptr, nullptr};
    }

}   ///< namespace MyModNamespace
```

And to initilize the module
`main.cpp`
```cpp
#include "Logger.hpp"
#include "MyModule.hpp"

#include <CP_SDK/ChatPlexSDK.hpp>
#include <CP_SDK/Logging/BMBFLogger.hpp>

#include <beatsaber-hook/shared/utils/il2cpp-functions.hpp>
#include <custom-types/shared/register.hpp>

ModInfo s_ModInfo;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Called at the early stages of game loading
extern "C" void setup(ModInfo& p_ModInfo)
{
    p_ModInfo.id        = MOD_ID;
    p_ModInfo.version   = VERSION;

    s_ModInfo = p_ModInfo;

    auto l_Logger = new CP_SDK::Logging::BMBFLogger(new Logger(p_ModInfo, LoggerOptions(false, true)));

    l_Logger->Error(u"MyModule Setuping!");

    MyModNamespace::Logger::Instance = l_Logger;
    CP_SDK::ChatPlexSDK::RegisterModule(new MyModNamespace::MyModule());

    l_Logger->Error(u"MyModule Completed setup!");
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Called later on in the game loading - a good time to install function hooks
extern "C" void load()
{
    il2cpp_functions::Init();

    MyModNamespace::Logger::Instance->Error(u"Registering custom types.");
    custom_types::Register::AutoRegister();
}
```

# Credits / Thanks
- Fern
- Pink
- RedBrumbler
- Sc2ad
