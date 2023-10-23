#include "CP_SDK/UI/Data/TextListCell.hpp"
#include "CP_SDK/UI/UISystem.hpp"

namespace CP_SDK::UI::Data {

    using namespace UnityEngine;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    CP_SDK_IL2CPP_INHERIT_INIT(TextListCell);

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    CP_SDK_IL2CPP_DECLARE_CTOR_IMPL(TextListCell)
    {
        CP_SDK_UI_IL2CPP_BIND_FIELD(Text, m_Text);

        m_Build         = {this, &TextListCell::Build_Impl};
        m_GetCellHeight = {this, &TextListCell::GetCellHeight_Impl};

        m_AddSelfComponent = {this, &TextListCell::AddSelfComponent_Impl};
    }
    /// @brief Destructor
    CP_SDK_IL2CPP_DECLARE_DTOR_MONOBEHAVIOUR_IMPL(TextListCell)
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Build cell
    void TextListCell::Build_Impl()
    {
        /// This code is handled in the super method
        //if (RTransform)
        //    return;
        //
        //base.Build();

        m_Text = UISystem::TextFactory->Create(u"Text", RTransform().Ptr());
        m_Text->SetAlign(TMPro::TextAlignmentOptions::CaplineLeft);
        m_Text->SetMargins(2.0f, 0.0f, 2.0f, 0.0f);
        m_Text->RTransform()->set_anchorMin(Vector2::get_zero());
        m_Text->RTransform()->set_anchorMax(Vector2::get_one());
        m_Text->RTransform()->set_sizeDelta(Vector2::get_zero());
    }
    /// @brief Get cell height
    float TextListCell::GetCellHeight_Impl()
    {
        return 5.0f;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Add self component
    /// @param p_Target Target gameobject
    IListCell* TextListCell::AddSelfComponent_Impl(GameObject* p_Target)
    {
        auto l_Existing = p_Target->GetComponent<TextListCell*>();
        if (!l_Existing)
            l_Existing = p_Target->AddComponent<TextListCell*>();

        return l_Existing;
    }

}   ///< namespace CP_SDK::UI::Data