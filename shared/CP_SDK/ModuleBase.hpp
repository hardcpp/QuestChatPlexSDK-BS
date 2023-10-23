#pragma once

#include "UI/UIIl2cpp.hpp"
#include "UI/ViewController.hpp"

#include <string>

#define CP_SDK_MODULE_BASE_INSTANCE_DECL(__Type)        \
    CP_SDK_NO_COPYMOVE_CTORS(__Type)                    \
    private:                                            \
        static __Type* m_Instance;                      \
    public:                                             \
        static __Type* Instance();                      \
    protected:                                          \
        void SetInstance(__Type* p_Instance) override;

#define CP_SDK_MODULE_BASE_INSTANCE_IMPL(__Type)                             \
    __Type* __Type::m_Instance = nullptr;                                    \
    __Type* __Type::Instance() { return m_Instance; }                        \
    void __Type::SetInstance(__Type* p_Instance) { m_Instance = p_Instance; }

namespace CP_SDK {

    /// @brief Module type
    enum class EIModuleBaseType
    {
        Integrated,
        External
    };
    /// @brief Activation type kind
    enum class EIModuleBaseActivationType
    {
        Never,
        OnStart,
        OnMenuSceneLoaded
    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Module base interface
    class IModuleBase
    {
        CP_SDK_NO_COPYMOVE_CTORS(IModuleBase);

        public:
            struct t_InitialViews
            {
                Utils::MonoPtr<UI::IViewController> Main;
                Utils::MonoPtr<UI::IViewController> Left;
                Utils::MonoPtr<UI::IViewController> Right;
            };

        public:
            virtual EIModuleBaseType            Type() const = 0;
            virtual std::u16string_view         Name() const = 0;
            virtual std::u16string_view         FancyName() const = 0;
            virtual std::u16string_view         Description() const = 0;
            virtual std::u16string_view         DocumentationURL() const = 0;
            virtual bool                        UseChatFeatures() const = 0;
            virtual bool                        IsEnabled() const = 0;
            virtual void                        IsEnabled(bool p_Enabled) = 0;
            virtual EIModuleBaseActivationType  ActivationType() const = 0;

        protected:
            /// @brief Constructor
            IModuleBase() = default;
            /// @brief Destructor
            virtual ~IModuleBase() { }

        public:
            /// @brief Set enabled
            /// @param p_Enabled New state
            virtual void SetEnabled(bool p_Enabled) = 0;

        public:
            /// @brief Check for module activation
            /// @param p_Kind Activation type
            virtual void CheckForActivation(EIModuleBaseActivationType p_Kind) = 0;
            /// @brief On application exit
            virtual void OnApplicationExit() = 0;

        public:
            /// @brief Get Module settings UI
            /// @return Main, Left, Right
            virtual t_InitialViews GetSettingsViewControllers() { return {}; }

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Module base interface
    template<class t_Type> class ModuleBase :
        public IModuleBase
    {
        CP_SDK_NO_COPYMOVE_CTORS(ModuleBase);

        protected:
            /// @brief Constructor
            ModuleBase()
                : m_WasEnabled(false)
            {

            }
            /// @brief Destructor
            virtual ~ModuleBase()
            {

            }

        public:
            /// @brief Set enabled
            /// @param p_Enabled New state
            void SetEnabled(bool p_Enabled) override
            {
                IsEnabled(p_Enabled);

                if (IsEnabled() && !m_WasEnabled) Enable();
                if (!IsEnabled() && m_WasEnabled) Disable();
            }

        public:
            /// @brief Check for module activation
            /// @param p_Kind Activation type
            void CheckForActivation(EIModuleBaseActivationType p_Kind) override
            {
                if (!m_WasEnabled && IsEnabled() && ActivationType() == p_Kind)
                    Enable();
            }
            /// @brief On application exit
            void OnApplicationExit() override
            {
                if (m_WasEnabled && IsEnabled())
                    Disable();
            }

        private:
            /// @brief Enable the Module
            void Enable()
            {
                if (m_WasEnabled)
                    return;

                m_WasEnabled = true;
                SetInstance(reinterpret_cast<t_Type*>(this));
                OnEnable();
            }
            /// @brief Disable the Module
            void Disable()
            {
                if (!m_WasEnabled)
                    return;

                OnDisable();
                SetInstance(nullptr);
                m_WasEnabled = false;
            }

        protected:
            /// @brief Set singleton instance
            virtual void SetInstance(t_Type* p_Instance) = 0;

            /// @brief Enable the Module
            virtual void OnEnable() = 0;
            /// @brief Disable the Module
            virtual void OnDisable() = 0;

        private:
            bool m_WasEnabled;

    };

}   ///< namespace CP_SDK