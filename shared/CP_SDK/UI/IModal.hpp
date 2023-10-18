#pragma once

#include "IViewController.hpp"
#include "UIIl2cpp.hpp"

#include <UnityEngine/MonoBehaviour.hpp>
#include <UnityEngine/RectTransform.hpp>

namespace CP_SDK::UI {

    namespace _u
    {
        using namespace UnityEngine;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Abstract base modal component
    class IModal : public _u::MonoBehaviour
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI", IModal, _u::MonoBehaviour);
        CP_SDK_IL2CPP_DECLARE_CTOR(IModal);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR(IModal);

        private:
            _v::MonoPtr<_u::RectTransform> m_RTransform;

        public:
            UIFieldRefDel<_u::RectTransform>    RTransform;
            _v::MonoPtr<IViewController>        VController;

        public:
            /// On component creation
            DECLARE_INSTANCE_METHOD(void, Awake);

        public:
            /// @brief On modal show
            _v::Action<> OnShow;
            /// @brief On modal close
            _v::Action<> OnClose;

    };

}   ///< namespace CP_SDK::UI

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::IModal);