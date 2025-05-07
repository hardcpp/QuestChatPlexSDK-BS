#pragma once

#include "../Components/CImage.hpp"
#include "../../Utils/Event.hpp"

namespace CP_SDK::UI::DefaultComponents {

    namespace _u
    {
        using namespace UnityEngine;
        using namespace UnityEngine::UI;
    }
    namespace _v
    {
        using namespace CP_SDK::Utils;
    }

    /// @brief Default CImage component
    class CP_SDK_EXPORT DefaultCImage : public Components::CImage
    {
        CP_SDK_IL2CPP_INHERIT("CP_SDK.UI.DefaultComponents", DefaultCImage, Components::CImage);
        CP_SDK_IL2CPP_DECLARE_CTOR_CHILD(DefaultCImage);
        CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_CHILD(DefaultCImage);

        private:
            _v::MonoPtr<_u::RectTransform>   m_RTransform;
            _v::MonoPtr<_u::LayoutElement>   m_LElement;
            _v::MonoPtr<_u::Image>           m_ImageC;

        public:
            /// @brief On component creation
            _v::Event<> Init;

            /// @brief On component creation
            void Init_DefaultCImage();

    };

}   ///< namespace CP_SDK::UI::DefaultComponents

CP_SDK_IL2CPP_INHERIT_HELPERS(CP_SDK::UI::DefaultComponents::DefaultCImage);