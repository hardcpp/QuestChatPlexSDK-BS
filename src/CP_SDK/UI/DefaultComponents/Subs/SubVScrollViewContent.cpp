#include "CP_SDK/UI/DefaultComponents/Subs/SubVScrollViewContent.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/Transform.hpp>
#include <UnityEngine/Rect.hpp>
#include <UnityEngine/RectTransform.hpp>
#include <UnityEngine/UI/LayoutRebuilder.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI::DefaultComponents::Subs {

    CP_SDK_IL2CPP_INHERIT_INIT(SubVScrollViewContent);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(SubVScrollViewContent)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(SubVScrollViewContent)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Component first frame
    void SubVScrollViewContent::Start()
    {
        LayoutRebuilder::ForceRebuildLayoutImmediate(get_transform().try_cast<RectTransform>().value_or(nullptr));

        StopAllCoroutines();
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(Coroutine_DisableCanvas(this)));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On frame
    void SubVScrollViewContent::Update()
    {
        if (!m_IsDirty)
            return;

        UpdateScrollView();
        m_IsDirty = false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On rect transform dimensions changed
    void SubVScrollViewContent::OnRectTransformDimensionsChange()
    {
        m_IsDirty = true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Timed canvas disabler
    /// @param p_Time Time in seconds
    custom_types::Helpers::Coroutine SubVScrollViewContent::Coroutine_DisableCanvas(SubVScrollViewContent* p_Self)
    {
        co_yield nullptr;

        auto l_RTransform = p_Self->get_transform()->GetChild(0).try_cast<RectTransform>().value_or(nullptr);

        while (l_RTransform->get_sizeDelta().y == -1.0f)
            co_yield nullptr;

        p_Self->UpdateScrollView();
    }
    /// @brief Update scroll view content size & buttons
    void SubVScrollViewContent::UpdateScrollView()
    {
        VScrollView->SetContentSize(get_transform()->GetChild(0).try_cast<RectTransform>().value_or(nullptr)->get_rect().m_Height);
        VScrollView->RefreshScrollButtons();
    }

}   ///< namespace CP_SDK::UI::DefaultComponents::Subs