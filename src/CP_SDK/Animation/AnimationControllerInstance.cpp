#include "CP_SDK/Animation/AnimationControllerInstance.hpp"
#include "CP_SDK/ChatPlexSDK.hpp"

#include <UnityEngine/SpriteMeshType.hpp>
#include <UnityEngine/Time.hpp>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK::Animation {

    /// @brief Constructor
    /// @param p_Texture Texture atlas
    /// @param p_UVs     UVs of sub sprites
    /// @param p_Delays  Delays of frames
    AnimationControllerInstance::AnimationControllerInstance(CP_SDK_PRIV_TAG_ARG(), _v::CMonoPtrRef<Texture2D> p_Texture, const std::vector<Rect>& p_UVs, const std::vector<uint16_t>& p_Delays)
        : m_IsDelayConsistent(true), m_ActiveCount(0), Frames(p_UVs.size()), CurrentFrameIndex(0)
    {
        int l_FirstDelay = -1;

        m_UVs   = p_UVs;
        Frames.resize(p_UVs.size());
        Delays  = p_Delays;

        auto l_Width     = const_cast<_v::MonoPtr<Texture2D>&>(p_Texture)->get_width();
        auto l_Height    = const_cast<_v::MonoPtr<Texture2D>&>(p_Texture)->get_height();
        for (int l_Frame = 0; l_Frame < p_UVs.size(); ++l_Frame)
        {
            auto& l_CurrentUV = p_UVs[l_Frame];
            Frames[l_Frame] = Sprite::Create(
                p_Texture.Ptr(),
                Rect(l_CurrentUV.m_XMin* l_Width, l_CurrentUV.m_YMin * l_Height, l_CurrentUV.m_Width * l_Width, l_CurrentUV.m_Height * l_Height),
                Vector2(0.0f, 0.0f),
                100.0f,
                0,
                SpriteMeshType::FullRect,
                Vector4(0.0f, 0.0f, 0.0f, 0.0f),
                false
            );

            if (l_Frame == 0)
                l_FirstDelay = p_Delays[l_Frame];

            if (p_Delays[l_Frame] != l_FirstDelay)
                m_IsDelayConsistent = false;
        }

        FirstFrame = Frames[0];

        m_LastFrameChange = (long)(Time::get_realtimeSinceStartup() * 1000.0f);
    }
    /// @brief Destructor
    AnimationControllerInstance::~AnimationControllerInstance()
    {
        m_ActiveImages.clear();
        Frames.clear();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param p_Texture Texture atlas
    /// @param p_UVs     UVs of sub sprites
    /// @param p_Delays  Delays of frames
    AnimationControllerInstance::Ptr AnimationControllerInstance::Make(_v::CMonoPtrRef<Texture2D> p_Texture, const std::vector<Rect>& p_UVs, const std::vector<uint16_t>& p_Delays)
    {
        return std::make_shared<AnimationControllerInstance>(CP_SDK_PRIV_TAG_VAL(), p_Texture, p_UVs, p_Delays);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Register an image
    /// @param p_TargetImage Target
    void AnimationControllerInstance::Register(Image* p_TargetImage)
    {
        const auto& l_It = std::find(m_ActiveImages.begin(), m_ActiveImages.end(), p_TargetImage);
        if (l_It == m_ActiveImages.end())
            m_ActiveImages.push_back(p_TargetImage);

        m_ActiveCount = m_ActiveImages.size();
    }
    /// @brief Unregister an image
    /// @param p_TargetImage Target
    void AnimationControllerInstance::Unregister(Image* p_TargetImage)
    {
        const auto& l_It = std::find(m_ActiveImages.begin(), m_ActiveImages.end(), p_TargetImage);
        if (l_It != m_ActiveImages.end())
            m_ActiveImages.erase(l_It);

        m_ActiveCount = m_ActiveImages.size();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Check if we should move to the next frame
    /// @param p_Now Current time in MS
    void AnimationControllerInstance::CheckForNextFrame(int64_t p_Now)
    {
        if (m_ActiveCount == 0)
            return;

        int64_t l_DeltaT = p_Now - m_LastFrameChange;
        if (l_DeltaT < Delays[CurrentFrameIndex])
            return;

        /// Bump animations with consistently 10ms or lower frame timings to 100ms
        if (m_IsDelayConsistent && Delays[CurrentFrameIndex] <= 10 && l_DeltaT < 100)
            return;

        m_LastFrameChange = p_Now;
        do
        {
            CurrentFrameIndex++;
            if (CurrentFrameIndex >= m_UVs.size())
                CurrentFrameIndex = 0;
        }
        while (!m_IsDelayConsistent && Delays[CurrentFrameIndex] == 0);

        auto& l_NewSprite = Frames[CurrentFrameIndex];
        for (int l_I = 0; l_I < m_ActiveCount; ++l_I)
        {
            if (!m_ActiveImages[l_I])
            {
                ChatPlexSDK::Logger()->Error(u"Invalid image");
                m_ActiveImages.erase(std::next(m_ActiveImages.begin(), l_I));
                m_ActiveCount = m_ActiveImages.size();
                l_I--;
                continue;
            }

            m_ActiveImages[l_I]->set_sprite(l_NewSprite.Ptr());
        }
    }

}   ///< namespace CP_SDK::Animation