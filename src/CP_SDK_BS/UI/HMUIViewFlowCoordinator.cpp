#include "CP_SDK_BS/UI/HMUIViewFlowCoordinator.hpp"
#include "CP_SDK_BS/UI/ViewController.hpp"

#include "CP_SDK/ChatPlexSDK.hpp"
#include "CP_SDK/Unity/MTCoroutineStarter.hpp"

#include <custom-types/shared/delegate.hpp>

#include <HMUI/ViewController.hpp>
#include <System/Action.hpp>
#include <UnityEngine/Resources.hpp>
#include <UnityEngine/WaitUntil.hpp>

namespace CP_SDK_BS::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(HMUIViewFlowCoordinator);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::map<System::Type*, _v::MonoPtr<HMUIViewFlowCoordinator>> HMUIViewFlowCoordinator::m_Instances;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(HMUIViewFlowCoordinator)
    {
        m_IsDequeueEngaged      = false;
        m_BackupFlowCoordinator = nullptr;
    };
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(HMUIViewFlowCoordinator)
    {

    };

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On activation
    /// @param p_FirstActivation      Is the first activation?
    /// @param p_AddedToHierarchy     Activation type
    /// @param p_ScreenSystemEnabling Is the screen system enabling?
    void HMUIViewFlowCoordinator::FlowCoordinator__DidActivate(bool p_FirstActivation, bool p_AddedToHierarchy, bool p_ScreenSystemEnabling)
    {
        if (p_FirstActivation)
        {
            auto l_Title = Title();
            SetTitle(!l_Title.empty() ? l_Title : u"DEFAULT", HMUI::ViewController::AnimationType::None);
            set_showBackButton(true);
        }

        if (p_FirstActivation)
        {
            if (!m_IsDequeueEngaged && m_SwitchQueue.size() != 0)
            {
                auto l_Current = m_SwitchQueue.front();
                m_SwitchQueue.pop();

                ProvideInitialViewControllers(l_Current.Main.Ptr(), l_Current.Left.Ptr(false), l_Current.Right.Ptr(false), nullptr, nullptr);
            }
            else
            {
                auto l_InitialViewsController = GetInitialViewsController.Invoke();
                ProvideInitialViewControllers(l_InitialViewsController.Main.Ptr(), l_InitialViewsController.Left.Ptr(false), l_InitialViewsController.Right.Ptr(false), nullptr, nullptr);
            }
        }
    }
    /// @brief When the back button is pressed
    /// @param p_TopViewController Controller instance
    void HMUIViewFlowCoordinator::FlowCoordinator__BackButtonWasPressed(HMUI::ViewController * p_TopViewController)
    {
        if (OnBackButtonPressed.Invoke(p_TopViewController))
            return;

        Dismiss();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Present this HMUIViewFlowCoordinator
    /// @param p_IgnoreBackuping Ignore existing flow coordinator
    void HMUIViewFlowCoordinator::Present(bool p_IgnoreBackuping)
    {
        if (IsFlowCoordinatorInHierarchy(this))
        {
            Present_Override(p_IgnoreBackuping);
            return;
        }

        m_BackupFlowCoordinator = nullptr;

        auto l_BackupFlowCoordinator = p_IgnoreBackuping ? nullptr : UnityEngine::Resources::FindObjectsOfTypeAll<HMUI::FlowCoordinator*>()->LastOrDefault([this](auto x) {
            return x->get_isActivated() && x != this && IsFlowCoordinatorInHierarchy(x);
        });

        /// Look for existing flow coordinator
        if (l_BackupFlowCoordinator != nullptr && l_BackupFlowCoordinator)
        {
            l_BackupFlowCoordinator->PresentFlowCoordinator(this, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
            m_BackupFlowCoordinator = l_BackupFlowCoordinator;
        }
        /// Present main view controller
        else
            HMUIUIUtils::MainFlowCoordinator()->PresentFlowCoordinator(this, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);

        Present_Override(p_IgnoreBackuping);
    }
    /// @brief Dismiss this HMUIViewFlowCoordinator
    void HMUIViewFlowCoordinator::Dismiss()
    {
        Dismiss_Override();

        while (!m_SwitchQueue.empty())
            m_SwitchQueue.pop();

        m_IsDequeueEngaged = false;

        SetLeftScreenViewController(nullptr,  HMUI::ViewController::AnimationType::None);
        SetRightScreenViewController(nullptr, HMUI::ViewController::AnimationType::None);

        /// Restore original flow coordinator
        if (m_BackupFlowCoordinator)
        {
            while (!m_SwitchQueue.empty())
                m_SwitchQueue.pop();

            m_IsDequeueEngaged = false;

            m_BackupFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
            m_BackupFlowCoordinator = nullptr;

            return;
        }

        /// Back to game main menu
        HMUIUIUtils::MainFlowCoordinator()->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Hide the left screen
    void HMUIViewFlowCoordinator::HideLeftScreen()
    {
        SetLeftScreenViewController(nullptr, HMUI::ViewController::AnimationType::None);
    }
    /// @brief Hide the right screen
    void HMUIViewFlowCoordinator::HideRightScreen()
    {
        SetRightScreenViewController(nullptr, HMUI::ViewController::AnimationType::None);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Enqueue a main view controller change
    /// @param p_NewView New main view controller
    void HMUIViewFlowCoordinator::ChangeView(HMUI::ViewController * p_NewView, HMUI::ViewController * p_Left, HMUI::ViewController * p_Right)
    {
        auto l_TopViewController = get_topViewController();
        if (IsFlowCoordinatorInHierarchy(this) && l_TopViewController.unsafePtr() == p_NewView)
            return;

        m_SwitchQueue.push({p_NewView, p_Left, p_Right});

        if (!IsFlowCoordinatorInHierarchy(this))
        {
            Present();
            return;
        }

        if (!m_IsDequeueEngaged && l_TopViewController->get_isActiveAndEnabled() && !l_TopViewController->get_isInTransition())
            DequeueViewController();
        if (!m_IsDequeueEngaged && (!l_TopViewController->get_isActiveAndEnabled() || l_TopViewController->get_isInTransition()))
        {
            m_IsDequeueEngaged = true;
            CP_SDK::Unity::MTCoroutineStarter::Start(
                custom_types::Helpers::CoroutineHelper::New(DequeueViewControllerWhileOldInTransition(this))
            );
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Dequeue a controller change
    void HMUIViewFlowCoordinator::DequeueViewController()
    {
        if (m_SwitchQueue.empty())
        {
            m_IsDequeueEngaged = false;
            return;
        }

        auto l_Current = m_SwitchQueue.front();
        m_SwitchQueue.pop();

        m_IsDequeueEngaged = true;

        SetLeftScreenViewController(l_Current.Left.Ptr(false),    HMUI::ViewController::AnimationType::None);
        SetRightScreenViewController(l_Current.Right.Ptr(false),  HMUI::ViewController::AnimationType::None);

        ReplaceTopViewController(
            l_Current.Main.Ptr(false),
            custom_types::MakeDelegate<System::Action*>((std::function<void()>)[this]() {
                if (IsFlowCoordinatorInHierarchy(this) && get_isActivated())
                    DequeueViewController();
            }),
            HMUI::ViewController::AnimationType::In,
            HMUI::ViewController::AnimationDirection::Horizontal
        );
    }
    /// @brief Dequeue a controller change but wait for old one to finish transition
    custom_types::Helpers::Coroutine HMUIViewFlowCoordinator::DequeueViewControllerWhileOldInTransition(HMUIViewFlowCoordinator* p_Self)
    {
        co_yield nullptr;

        if (!p_Self->IsFlowCoordinatorInHierarchy(p_Self))
            co_return;

        ///yield return new WaitUntil(() => isActiveAndEnabled);
        while (!p_Self->get_isActiveAndEnabled()) co_yield nullptr;

        if (!p_Self->IsFlowCoordinatorInHierarchy(p_Self))
            co_return;

        /// yield return new WaitUntil(() => topViewController.isActiveAndEnabled);
        while (!p_Self->get_topViewController()->get_isActiveAndEnabled()) co_yield nullptr;

        if (!p_Self->IsFlowCoordinatorInHierarchy(p_Self))
            co_return;

        /// yield return new WaitUntil(() => !topViewController.isInTransition);
        while (p_Self->get_topViewController()->get_isInTransition()) co_yield nullptr;

        if (!p_Self->IsFlowCoordinatorInHierarchy(p_Self))
            co_return;

        /// yield return new WaitForEndOfFrame();
        co_yield nullptr;

        if (!p_Self->IsFlowCoordinatorInHierarchy(p_Self))
            co_return;

        p_Self->DequeueViewController();
    }

}   ///< namespace CP_SDK_BS::UI

