#include "CP_SDK/Utils/Il2cpp.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

namespace CP_SDK::Utils {

    std::vector<void (*)(Paper::LoggerContext& logger)> Hooks::m_InstalledFuncs;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// Register a hook
    /// @param p_Function Function to register
    void Hooks::Register(void (*p_Function)(Paper::LoggerContext&))
    {
        m_InstalledFuncs.push_back(p_Function);
    }
    /// Install all hooks
    void Hooks::InstallHooks()
    {
        auto l_Logger = reinterpret_cast<Logging::PaperLogger*>(ChatPlexSDK::Logger())->GetPaperLogger();
        for (auto& l_Current : m_InstalledFuncs)
            l_Current(*l_Logger);
    }

}   ///< namespace CP_SDK::Utils