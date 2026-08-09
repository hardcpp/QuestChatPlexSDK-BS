#include "CP_SDK/UI/LoadingProgressBar.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/Operators.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/WaitForSecondsRealtime.hpp"

using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::UI {

    CP_SDK_IL2CPP_INHERIT_INIT(LoadingProgressBar);
    CP_SDK_UNITY_PERSISTANT_SINGLETON_IMPL(CP_SDK::UI::LoadingProgressBar);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(LoadingProgressBar)
    {

    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(LoadingProgressBar)
    {
        WasDestroyed();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On component creation
    void LoadingProgressBar::Awake()
    {
        m_Canvas = UISystem::FloatingPanelFactory->Create(u"", transform);
        m_Canvas->transform->localScale = UnityEngine::Vector3(0.01f, 0.01f, 0.01f);
        m_Canvas->SetTransformDirect(Vector3(0, 2.5f, 4.25f), Vector3(0, 0, 0));
        m_Canvas->SetSize(Vector2(150, 40));
        m_Canvas->SetBackground(false);

        m_HeaderText = UISystem::TextFactory->Create(u"", m_Canvas->get_transform());
        if (m_HeaderText)
        {
            m_HeaderText->RTransform()->set_anchoredPosition(Vector2(0.0f, 15.0f));
            m_HeaderText->RTransform()->set_sizeDelta(Vector2(100.0f, 20.0f));
            m_HeaderText->SetFontSize(10.0f);
            m_HeaderText->SetAlign(TextAlignmentOptions::Midline);
            m_HeaderText->SetText(u"...");
        }



        m_LoadingBackground = UISystem::ImageFactory->Create(u"", m_Canvas->get_transform());
        m_LoadingBackground->SetWidth(100);
        m_LoadingBackground->SetHeight(10);
        m_LoadingBackground->SetSprite(
            Sprite::Create(
                Texture2D::get_whiteTexture(),
                Rect(0, 0, Texture2D::get_whiteTexture()->get_width(), Texture2D::get_whiteTexture()->get_height()),
                Vector2::get_one() * 0.5f,
                100,
                1,
                SpriteMeshType::FullRect,
                Vector4::get_zero(),
                false
            )
        );
        m_LoadingBackground->SetColor(Color(0, 0, 0, 0.8f));
        m_LoadingBackground->ImageC()->preserveAspect = false;

        m_LoadingBar =  UISystem::ImageFactory->Create(u"", m_Canvas->get_transform());
        m_LoadingBar->SetWidth(100);
        m_LoadingBar->SetHeight(10);
        m_LoadingBar->SetSprite(
            Sprite::Create(
                Texture2D::get_whiteTexture(),
                Rect(0, 0, Texture2D::get_whiteTexture()->get_width(), Texture2D::get_whiteTexture()->get_height()),
                Vector2::get_one() * 0.5f,
                100,
                1,
                SpriteMeshType::FullRect,
                Vector4::get_zero(),
                false
            )
        );
        m_LoadingBar->SetType(Image::Type::Filled);
        m_LoadingBar->SetColor(Color(0.1f, 1, 0.1f, 0.5f));
        m_LoadingBar->ImageC()->fillMethod = Image::FillMethod::Horizontal;
        m_LoadingBar->ImageC()->fillAmount = 0.5f;
        m_LoadingBar->ImageC()->preserveAspect = false;

        m_Canvas->GetComponent<Canvas*>()->enabled = false;

        ChatPlexSDK::OnGenericSceneChange += {this, &LoadingProgressBar::ChatPlexSDK_OnGenericSceneChange};
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Show a message with a hide timer
    /// @param p_Message Message to display
    /// @param p_Time    Time before disapearing
    void LoadingProgressBar::ShowTimedMessage(std::u16string_view p_Message, float p_Time)
    {
        StopAllCoroutines();

        if (m_HeaderText)
            m_HeaderText->SetText(p_Message);

        m_LoadingBar->ImageC()->enabled = false;
        m_LoadingBackground->ImageC()->enabled = false;
        m_LoadingBar->ImageC()->fillAmount = 0.0f;
        m_Canvas->GetComponent<Canvas*>()->enabled = true;

        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(Coroutine_DisableCanvas(this, p_Time)));
    }
    /// @brief Show loading progress bar with a message
    /// @param p_Message  Message to display
    /// @param p_Progress Loading progress
    void LoadingProgressBar::ShowLoadingProgressBar(std::u16string_view p_Message, float p_Progress)
    {
        StopAllCoroutines();

        if (m_HeaderText)
            m_HeaderText->SetText(p_Message);

        m_LoadingBar->ImageC()->enabled = true;
        m_LoadingBar->ImageC()->fillAmount = p_Progress;
        m_LoadingBackground->ImageC()->enabled = true;
        m_Canvas->GetComponent<Canvas*>()->enabled = true;
    }
    /// @brief Set current progress and displayed message
    /// @param p_Message  Message to display
    /// @param p_Progress Loading progress
    void LoadingProgressBar::SetProgress(std::u16string_view p_Message, float p_Progress)
    {
        StopAllCoroutines();

        if (m_HeaderText)
            m_HeaderText->SetText(p_Message);

        m_LoadingBar->ImageC()->fillAmount = p_Progress;
        m_Canvas->GetComponent<Canvas*>()->enabled = true;
    }
    /// @brief Set hide timer
    /// @param p_Time Time in seconds
    void LoadingProgressBar::HideTimed(float p_Time)
    {
        StopAllCoroutines();
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(Coroutine_DisableCanvas(this, p_Time)));
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief On scene changed
    /// @param p_NewScene New scene type
    void LoadingProgressBar::ChatPlexSDK_OnGenericSceneChange(EGenericScene p_NewScene)
    {
        if (p_NewScene != EGenericScene::Menu)
        {
            StopAllCoroutines();
            m_Canvas->GetComponent<Canvas*>()->enabled = false;
        }
    }
    /// @brief Timed canvas disabler
    /// @param p_Time Time in seconds
    custom_types::Helpers::Coroutine LoadingProgressBar::Coroutine_DisableCanvas(LoadingProgressBar* p_Self, float p_Time)
    {
        co_yield WaitForSecondsRealtime::New_ctor(p_Time)->i___System__Collections__IEnumerator();
        p_Self->m_Canvas->GetComponent<Canvas*>()->enabled = false;
    }

}   ///< namespace CP_SDK::UI