#pragma once

#include "CIconButton.hpp"
#include "../IScreen.hpp"
#include "../../ChatPlexSDK.hpp"

#include <map>

#include <UnityEngine/Vector2.hpp>
#include <UnityEngine/Vector3.hpp>

namespace CP_SDK::UI::Components {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Floating Panel component
    class CFloatingPanel : public IScreen
    {
        CP_SDK_IL2CPP_INHERIT_CUSTOM("CP_SDK.UI.Components", CFloatingPanel, IScreen);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHAIN(CFloatingPanel, IScreen);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHAIN(CFloatingPanel, IScreen);

        private:
            _v::MonoPtr<IViewController>                                                m_CurrentViewController;
            bool                                                                        m_AllowMovement;
            bool                                                                        m_AutoLockOnSceneSwitch;
            bool                                                                        m_AlignWithFloor;
            std::map<ChatPlexSDK::EGenericScene, std::tuple<_u::Vector3, _u::Vector3>>  m_SceneTransforms;
            std::map<ChatPlexSDK::EGenericScene, _v::Action<_u::Vector3, _u::Vector3>>  m_OnSceneRelease;
            _v::MonoPtr<_u::Image>                                                      m_Background;
            _v::MonoPtr<CIconButton>                                                    m_LockIcon;
            _v::MonoPtr<CIconButton>                                                    m_GearIcon;

            _v::Event<_v::CMonoPtrRef<CFloatingPanel>>              m_OnSceneRelocated;
            _v::Event<_v::CMonoPtrRef<CFloatingPanel>, _u::Vector2> m_OnSizeChanged;
            _v::Event<_v::CMonoPtrRef<CFloatingPanel>>              m_OnGearIcon;

        protected:
            _v::Action<_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>>, bool> m_OnGrab;
            _v::Action<_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>>, bool> m_OnRelease;

            _v::Action<bool>        m_SetAllowMovement;
            _v::Action<float>       m_SetRadius;
            _v::Action<_u::Vector2> m_SetSize;

        public:
            enum class ECorner
            {
                None,
                TopLeft,
                TopRight,
                BottomLeft,
                BottomRight,
            };

        public:
            UIFieldRefDel<IViewController>      CurrentViewController;
            UIFieldRefDel<_u::RectTransform>    RTransform;

        public:
            /// @brief Replace active view controller
            /// @param p_ViewController New view controller
            void SetViewController_Impl(_v::CMonoPtrRef<IViewController> p_ViewController);

        public:
            /// @brief On grab event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CFloatingPanel* OnGrab(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add = true);
            /// @brief On release event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CFloatingPanel* OnRelease(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add = true);
            /// @brief On scene relocated icon event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CFloatingPanel* OnSceneRelocated(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add = true);
            /// @brief On scene relocated icon event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CFloatingPanel* OnSizeChanged(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>, _u::Vector2> p_Functor, bool p_Add = true);
            /// @brief On gear icon event
            /// @param p_Functor Functor to add/remove
            /// @param p_Add     Should add
            CFloatingPanel* OnGearIcon(_v::CActionRef<_v::CMonoPtrRef<CFloatingPanel>> p_Functor, bool p_Add = true);

        public:
            /// @brief Get allow movement
            bool GetAllowMovement();

        public:
            /// @brief Set align with floor
            /// @param p_Align Align
            CFloatingPanel* SetAlignWithFloor(bool p_Align);
            /// @brief Set allow movements
            /// @param p_Allow Is allowed?
            CFloatingPanel* SetAllowMovement(bool p_Allow);
            /// @brief Set background state
            /// @param p_Enabled Is enabled?
            /// @param p_Color Optional color
            CFloatingPanel* SetBackground(bool p_Enabled, std::optional<_u::Color> p_Color = std::nullopt);
            /// @brief Set background color
            /// @param p_Color New background color
            CFloatingPanel* SetBackgroundColor(_u::Color p_Color);
            /// @brief Set background sprite
            /// @param p_Sprite New sprite
            /// @param p_Type Image type
            CFloatingPanel* SetBackgroundSprite(_u::Sprite* p_Sprite, _u::Image::Type p_Type = _u::Image::Type::Simple);
            /// @brief Set lock icon mode
            /// @param p_Corner Corner or none
            CFloatingPanel* SetLockIcon(ECorner p_Corner);
            /// @brief Set gear icon mode
            /// @param p_Corner Corner or none
            CFloatingPanel* SetGearIcon(ECorner p_Corner);
            /// @brief Set radius on supported games
            /// @param p_Radius Canvas radius
            CFloatingPanel* SetRadius(float p_Radius);
            /// @brief Set on scene release
            /// @param p_Scene Target scene
            /// @param p_Callback Callback
            CFloatingPanel* OnSceneRelease(ChatPlexSDK::EGenericScene p_Scene, _v::CActionRef<_u::Vector3, _u::Vector3> p_Callback);
            /// @brief Set scene transform
            /// @param p_Scene Target scene
            /// @param p_LocalPosition Local position
            /// @param p_LocalEulerAngles Local euler angles
            CFloatingPanel* SetSceneTransform(ChatPlexSDK::EGenericScene p_Scene, _u::Vector3 p_LocalPosition, _u::Vector3 p_LocalEulerAngles);
            /// @brief Set size
            /// @param p_Size New size
            CFloatingPanel* SetSize(_u::Vector2 p_Size);
            /// @brief Set transform direct
            /// @param p_LocalPosition Local position
            /// @param p_LocalEulerAngles Local euler angles
            CFloatingPanel* SetTransformDirect(_u::Vector3 p_LocalPosition, _u::Vector3 p_LocalEulerAngles);

        protected:
            /// @brief On component creation
            DECLARE_INSTANCE_METHOD(void, Awake);

        private:
            /// @brief On generic scene change
            /// @param p_ActiveScene New active scene
            void ChatPlexSDK_OnGenericSceneChange(ChatPlexSDK::EGenericScene p_ActiveScene);

    };

}   ///< namespace CP_SDK::UI::Components

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::Components::CFloatingPanel);