#include "CP_SDK_BS/UI/LevelDetail.hpp"
#include "CP_SDK_BS/UI/HMUIIconSegmentedControl.hpp"
#include "CP_SDK_BS/UI/HMUITextSegmentedControl.hpp"
#include "CP_SDK_BS/Game/Levels.hpp"
#include "CP_SDK/UI/UISystem.hpp"
#include "CP_SDK/Unity/SpriteU.hpp"
#include "CP_SDK/Unity/Operators.hpp"
#include "assets.hpp"

#include <fmt/core.h>

#include <BeatmapSaveDataVersion3/BeatmapSaveData.hpp>
#include <GlobalNamespace/BeatmapDataLoader.hpp>
#include <GlobalNamespace/BeatmapDataBasicInfo.hpp>
#include <GlobalNamespace/BeatmapDifficultySerializedMethods.hpp>
#include <GlobalNamespace/BeatmapLevelLoader.hpp>
#include <GlobalNamespace/BeatmapLevelSO.hpp>
#include <GlobalNamespace/IBeatmapLevelData.hpp>
#include <GlobalNamespace/IBeatmapLevelLoader.hpp>
#include <GlobalNamespace/LocalizedHoverHint.hpp>
#include <GlobalNamespace/MockBeatmapDataAssetFileModel.hpp>
#include <GlobalNamespace/StandardLevelDetailView.hpp>
#include <GlobalNamespace/StandardLevelInfoSaveData.hpp>
#include <HMUI/HoverHint.hpp>
#include <HMUI/HoverHintController.hpp>
#include <HMUI/SegmentedControl.hpp>
#include <HMUI/ToggleWithCallbacks.hpp>
#include <BGLib/Polyglot/Localization.hpp>
#include <System/Action_1.hpp>
#include <System/Action_2.hpp>
#include <System/Math.hpp>
#include <System/Collections/ObjectModel/ReadOnlyCollection_1.hpp>
#include <System/Collections/Generic/IReadOnlyList_1.hpp>
#include <System/Collections/Generic/IReadOnlyCollection_1.hpp>
#include <System/IO/File.hpp>
#include <System/Text/RegularExpressions/Regex.hpp>
#include <UnityEngine/Resources.hpp>

#include "songcore/shared/SongLoader/CustomBeatmapLevel.hpp"

#include "CP_SDK_BS/UI/DefaultComponentsOverrides/Subs/SubFloatingPanelMover.hpp"

using namespace GlobalNamespace;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace CP_SDK_BS::UI {

    CP_SDK::Utils::MonoPtr<GameObject> LevelDetail::m_SongDetailViewTemplate;

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Init
    void LevelDetail::Init()
    {
        if (m_SongDetailViewTemplate)
            return;

        auto l_Result = Resources::FindObjectsOfTypeAll<StandardLevelDetailView*>()->FirstOrDefault([](StandardLevelDetailView* x) -> bool {
            return x->get_gameObject()->get_name() == u"LevelDetail";
        });

        if (!l_Result)
            return;

        m_SongDetailViewTemplate = GameObject::Instantiate(l_Result->get_gameObject());
        m_SongDetailViewTemplate->set_name(u"CP_SDK_BS_StandardLevelDetailView_Template");

        try
        {
            auto l_Component = m_SongDetailViewTemplate->GetComponent<StandardLevelDetailView*>();
            if (l_Component)
            {
                auto l_Loader = BeatmapLevelLoader::New_ctor(nullptr, MockBeatmapDataAssetFileModel::New_ctor()->i___GlobalNamespace__IBeatmapDataAssetFileModel(), nullptr, BeatmapLevelLoader::InitData::New_ctor(0));
                auto l_Packs = System::Collections::Generic::List_1<::UnityW<PackDefinitionSO>>::New_ctor();
                l_Component->____beatmapLevelsModel = BeatmapLevelsModel::New_ctor(nullptr, l_Loader->i___GlobalNamespace__IBeatmapLevelLoader(), l_Packs->i___System__Collections__Generic__IEnumerable_1_T_());

                GameObject::DestroyImmediate(l_Component);
            }
        }
        catch (const std::exception& l_Exception)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.UI][LevelDetail.Init] Error:");
            CP_SDK::ChatPlexSDK::Logger()->Error(l_Exception);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::u16string                          LevelDetail::Name()
    {
        return m_SongNameText ? m_SongNameText->get_text() : u"";
    }
    void                                    LevelDetail::Name(std::u16string_view p_Value)
    {
        auto l_HTMLStripped = System::Text::RegularExpressions::Regex::Replace(p_Value, u"<.*?>", u"");
        if (l_HTMLStripped->get_Length() < 30)
            m_SongNameText->set_text(p_Value);
        else
            m_SongNameText->set_text(u"" + p_Value.substr(0, 27) + u"...");
    }
    std::u16string                          LevelDetail::AuthorNameText()
    {
        return m_AuthorNameText ? m_AuthorNameText->get_text() : u"";
    }
    void                                    LevelDetail::AuthorNameText(std::u16string_view p_Value)
    {
        auto l_HTMLStripped = System::Text::RegularExpressions::Regex::Replace(p_Value, u"<.*?>", u"");
        if (l_HTMLStripped->get_Length() < 32)
            m_AuthorNameText->set_text(p_Value);
        else
            m_AuthorNameText->set_text(u"" + p_Value.substr(0, 29) + "...");
    }
    Sprite*                                 LevelDetail::Cover()
    {
        return m_SongCoverImage ? m_SongCoverImage->get_sprite() : nullptr;
    }
    void                                    LevelDetail::Cover(Sprite* p_Value)
    {
        m_SongCoverImage->set_sprite(p_Value);
    }
    double                                  LevelDetail::Time()
    {
        return m_Time;
    }
    void                                    LevelDetail::Time(double p_Value)
    {
        m_Time = p_Value;
        m_SongTimeText->set_text(p_Value >= 0.0 ? fmt::format("{:.0f}:{:02.0f}", std::floor(p_Value / 60), std::floor(std::fmod(p_Value, 60))) : "--");
    }
    float                                   LevelDetail::BPM()
    {
        return m_BPM;
    }
    void                                    LevelDetail::BPM(float p_Value)
    {
        m_BPM = p_Value;
        m_SongBPMText->set_text(fmt::format("{:.0f}", p_Value));
    }
    float                                   LevelDetail::NPS()
    {
        return m_NPS;
    }
    void                                    LevelDetail::NPS(float p_Value)
    {
        m_NPS = p_Value;
        m_SongNPSText->set_text(p_Value >= 0.0f ? fmt::format("{:.2f}", p_Value) : "--");
    }
    int                                     LevelDetail::NJS()
    {
        return m_NJS;
    }
    void                                    LevelDetail::NJS(int p_Value)
    {
        m_NJS = p_Value;
        m_SongNJSText->set_text(p_Value > 0 ? std::to_string(p_Value) : "--");
    }
    float                                   LevelDetail::Offset()
    {
        return m_Offset;
    }
    void                                    LevelDetail::Offset(float p_Value)
    {
        m_Offset = p_Value;
        m_SongOffsetText->set_text(!std::isnan(p_Value) ? fmt::format("{:.1f}", p_Value) : "--");
    }
    int                                     LevelDetail::Notes()
    {
        return m_Notes;
    }
    void                                    LevelDetail::Notes(int p_Value)
    {
        m_Notes = p_Value;
        m_SongNotesText->set_text(p_Value > 0 ? std::to_string(p_Value) : "--");
    }
    int                                     LevelDetail::Obstacles()
    {
        return m_Obstacles;
    }
    void                                    LevelDetail::Obstacles(int p_Value)
    {
        m_Obstacles = p_Value;
        m_SongObstaclesText->set_text(p_Value > 0 ? std::to_string(p_Value) : "--");
    }
    int                                     LevelDetail::Bombs()
    {
        return m_Bombs;
    }
    void                                    LevelDetail::Bombs(int p_Value)
    {
        m_Bombs = p_Value;
        m_SongBombsText->set_text(p_Value > 0 ? std::to_string(p_Value) : "--");
    }
    HMUI::IconSegmentedControl::DataItem*   LevelDetail::Characteristic()
    {
        return m_Characteristic.Ptr(false);
    }
    void                                    LevelDetail::Characteristic(HMUI::IconSegmentedControl::DataItem* p_Value)
    {
        m_Characteristic = p_Value;
        HMUIIconSegmentedControl::SetDataNoHoverHint(m_SongCharacteristicSegmentedControl.Ptr(), ::Array<HMUI::IconSegmentedControl::DataItem*>::New({
            p_Value
        }));
    }
    std::u16string                          LevelDetail::Difficulty()
    {
        return m_Difficulty;
    }
    void                                    LevelDetail::Difficulty(std::u16string_view p_Value)
    {
        auto l_List = System::Collections::Generic::List_1<StringW>::New_ctor();
        l_List->Add(p_Value);

        m_Difficulty = p_Value;
        m_SongDiffSegmentedControl->SetTexts(l_List->AsReadOnly()->i___System__Collections__Generic__IReadOnlyList_1_T_());
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Constructor
    /// @param p_Parent Parent transform
    LevelDetail::LevelDetail(Transform* p_Parent)
    {
        SelectedBeatmapDifficulty = BeatmapDifficulty::Easy;

        Init();

        m_GameObject = GameObject::Instantiate(m_SongDetailViewTemplate.Ptr(), p_Parent);

        auto l_BSMLObjects     = m_GameObject->GetComponentsInChildren<RectTransform*>();
        auto l_HoverHints      = m_GameObject->GetComponentsInChildren<HMUI::HoverHint*>(true);
        auto l_LocalHoverHints = m_GameObject->GetComponentsInChildren<LocalizedHoverHint*>(true);

        for (auto l_Current : l_BSMLObjects)
        {
            if (!l_Current->get_gameObject()->get_name()->StartsWith(u"BSML"))
                continue;

            GameObject::Destroy(l_Current->get_gameObject());
        }
        for (auto l_Current : l_HoverHints)         GameObject::Destroy(l_Current);
        for (auto l_Current : l_LocalHoverHints)    GameObject::Destroy(l_Current);

        /// Favorite toggle
        m_FavoriteToggle = m_GameObject->get_transform()->Find(u"FavoriteToggle")->get_gameObject();
        m_FavoriteToggle->SetActive(false);

        /// Find play buttons
        auto l_ActionButtons     = m_GameObject->get_transform()->Find(u"ActionButtons");
        auto l_PracticeButton    = l_ActionButtons->Find(u"PracticeButton");
        auto l_PlayButton        = l_ActionButtons->Find(u"ActionButton");

        /// Re-bind play button
        if (l_PlayButton->GetComponent<Button*>())
        {
            auto l_ActionButtonsRTransform = l_ActionButtons->get_transform().try_cast<RectTransform>().value_or(nullptr);
            l_ActionButtonsRTransform->set_anchoredPosition(Vector2(-0.5f, l_ActionButtonsRTransform->get_anchoredPosition().y));

            auto l_ButtonsParent = l_PlayButton->get_transform()->get_parent();
            GameObject::Destroy(l_PracticeButton->get_gameObject());
            GameObject::Destroy(l_PlayButton->get_gameObject());

            m_SecondaryButton = _v::UISystem::SecondaryButtonFactory->Create(u"Secondary", l_ButtonsParent);
            m_SecondaryButton->SetText(u"Secondary");
            m_SecondaryButton->SetHeight(8.0f)->SetWidth(30.0f);
            m_SecondaryButton->OnClick({this, &LevelDetail::OnSecondaryButtonClicked});

            m_PrimaryButton = _v::UISystem::PrimaryButtonFactory->Create(u"Primary", l_ButtonsParent);
            m_PrimaryButton->SetText(u"Primary");
            m_PrimaryButton->SetHeight(8.0f)->SetWidth(30.0f);
            m_PrimaryButton->OnClick({this, &LevelDetail::OnPrimaryButtonClicked});

            SetSecondaryButtonEnabled(false);
            SetSecondaryButtonText(u"?");
            SetPrimaryButtonEnabled(true);
            SetPrimaryButtonText(u"?");
        }

        m_CharacteristicSegmentedControllerClone    = m_GameObject->get_transform()->Find(u"BeatmapCharacteristic")->Find(u"BeatmapCharacteristicSegmentedControl")->GetComponent<BeatmapCharacteristicSegmentedControlController*>();
        m_SongCharacteristicSegmentedControl        = HMUIIconSegmentedControl::Create(m_CharacteristicSegmentedControllerClone->get_transform().try_cast<RectTransform>().value_or(nullptr), true);

        m_DifficultiesSegmentedControllerClone  = m_GameObject->get_transform()->Find(u"BeatmapDifficulty")->GetComponentInChildren<BeatmapDifficultySegmentedControlController*>();
        m_SongDiffSegmentedControl              = HMUITextSegmentedControl::Create(m_DifficultiesSegmentedControllerClone->get_transform().try_cast<RectTransform>().value_or(nullptr), true);

        auto l_LevelBarBig = m_GameObject->get_transform()->Find(u"LevelBarBig");

        m_SongNameText      = l_LevelBarBig->GetComponentsInChildren<TextMeshProUGUI*>()->First([](auto x) { return x->get_gameObject()->get_name() == u"SongNameText"; });
        m_AuthorNameText    = l_LevelBarBig->GetComponentsInChildren<TextMeshProUGUI*>()->First([](auto x) { return x->get_gameObject()->get_name() == u"AuthorNameText"; });
        m_SongCoverImage    = l_LevelBarBig->Find(u"SongArtwork")->GetComponent<HMUI::ImageView*>();

        m_SongCoverImage->get_rectTransform()->set_anchoredPosition(Vector2( 2.000f, m_SongCoverImage->get_rectTransform()->get_anchoredPosition().y));
        m_SongNameText->get_rectTransform()->set_anchoredPosition  (Vector2(-0.195f, m_SongNameText->get_rectTransform()->get_anchoredPosition().y));
        m_AuthorNameText->set_richText(true);

        /// Disable multiline
        l_LevelBarBig->Find(u"MultipleLineTextContainer")->get_gameObject()->SetActive(false);

        auto l_BeatmapParamsPanel = m_GameObject->get_transform()->Find(u"BeatmapParamsPanel");
        l_BeatmapParamsPanel->get_transform()->set_localPosition(l_BeatmapParamsPanel->get_transform()->get_localPosition() + (2 * Vector3::get_up()));

        l_BeatmapParamsPanel->get_gameObject()->AddComponent<HorizontalLayoutGroup*>()->set_childControlHeight(false);
        l_BeatmapParamsPanel->get_gameObject()->AddComponent<LayoutElement*>();

        m_SongNPSText       = l_BeatmapParamsPanel->GetComponentsInChildren<TextMeshProUGUI*>()->First([](auto x) { return x->get_gameObject()->get_transform()->get_parent()->get_name() == u"NPS";});
        m_SongNotesText     = l_BeatmapParamsPanel->GetComponentsInChildren<TextMeshProUGUI*>()->First([](auto x) { return x->get_gameObject()->get_transform()->get_parent()->get_name() == u"NotesCount";});
        m_SongObstaclesText = l_BeatmapParamsPanel->GetComponentsInChildren<TextMeshProUGUI*>()->First([](auto x) { return x->get_gameObject()->get_transform()->get_parent()->get_name() == u"ObstaclesCount";});
        m_SongBombsText     = l_BeatmapParamsPanel->GetComponentsInChildren<TextMeshProUGUI*>()->First([](auto x) { return x->get_gameObject()->get_transform()->get_parent()->get_name() == u"BombsCount";});

        auto l_SizeDelta = m_SongNPSText->get_transform()->get_parent()->get_transform().try_cast<RectTransform>().value_or(nullptr)->get_sizeDelta();
        l_SizeDelta.y *= 2;

        m_SongNPSText->get_transform()->get_parent()->get_gameObject()->AddComponent<VerticalLayoutGroup*>()->set_padding(RectOffset::New_ctor(0, 0, 0, 3));
        m_SongNPSText->get_transform()->get_parent()->get_gameObject()->AddComponent<LayoutElement*>();
        m_SongNPSText->get_transform()->get_parent()->get_transform().try_cast<RectTransform>().value_or(nullptr)->set_sizeDelta(l_SizeDelta);

        m_SongNotesText->get_transform()->get_parent()->get_gameObject()->AddComponent<VerticalLayoutGroup*>()->set_padding(RectOffset::New_ctor(0, 0, 0, 3));
        m_SongNotesText->get_transform()->get_parent()->get_gameObject()->AddComponent<LayoutElement*>();
        m_SongNotesText->get_transform()->get_parent()->get_transform().try_cast<RectTransform>().value_or(nullptr)->set_sizeDelta(l_SizeDelta);

        m_SongObstaclesText->get_transform()->get_parent()->get_gameObject()->AddComponent<VerticalLayoutGroup*>()->set_padding(RectOffset::New_ctor(0, 0, 0, 3));
        m_SongObstaclesText->get_transform()->get_parent()->get_gameObject()->AddComponent<LayoutElement*>();
        m_SongObstaclesText->get_transform()->get_parent()->get_transform()->get_transform()->get_parent()->get_transform().try_cast<RectTransform>().value_or(nullptr)->set_sizeDelta(l_SizeDelta);

        m_SongBombsText->get_transform()->get_parent()->get_gameObject()->AddComponent<VerticalLayoutGroup*>()->set_padding(RectOffset::New_ctor(0, 0, 0, 3));
        m_SongBombsText->get_transform()->get_parent()->get_gameObject()->AddComponent<LayoutElement*>();
        m_SongBombsText->get_transform()->get_parent()->get_transform().try_cast<RectTransform>().value_or(nullptr)->set_sizeDelta(l_SizeDelta);

        /// Patch
        auto l_OffsetSprite = CP_SDK::Unity::SpriteU::CreateFromRaw(Assets::Offset_png, 100.0f, Vector2::get_one() * 16.0f);
        m_SongOffsetText = GameObject::Instantiate(m_SongNPSText->get_transform()->get_parent()->get_gameObject(), m_SongNPSText->get_transform()->get_parent()->get_parent())->GetComponentInChildren<TextMeshProUGUI*>();
        m_SongOffsetText->get_transform()->get_parent()->SetAsFirstSibling();
        m_SongOffsetText->get_transform()->get_parent()->GetComponentInChildren<HMUI::ImageView*>()->set_sprite(l_OffsetSprite);

        auto l_NJSSprite = CP_SDK::Unity::SpriteU::CreateFromRaw(Assets::NJS_png, 100.0f, Vector2::get_one() * 16.0f);
        m_SongNJSText = GameObject::Instantiate(m_SongNPSText->get_transform()->get_parent()->get_gameObject(), m_SongNPSText->get_transform()->get_parent()->get_parent())->GetComponentInChildren<TextMeshProUGUI*>();
        m_SongNJSText->get_transform()->get_parent()->SetAsFirstSibling();
        m_SongNJSText->get_transform()->get_parent()->GetComponentInChildren<HMUI::ImageView*>()->set_sprite(l_NJSSprite);

        m_SongNPSText->get_transform()->get_parent()->SetAsFirstSibling();

        m_SongBPMText = GameObject::Instantiate(m_SongNPSText->get_transform()->get_parent()->get_gameObject(), m_SongNPSText->get_transform()->get_parent()->get_parent())->GetComponentInChildren<TextMeshProUGUI*>();
        m_SongBPMText->get_transform()->get_parent()->SetAsFirstSibling();
        m_SongBPMText->get_transform()->get_parent()->GetComponentInChildren<HMUI::ImageView*>()->set_sprite(Resources::FindObjectsOfTypeAll<Sprite*>()->First([](auto x) { return x->get_name() == u"MetronomeIcon"; }));

        m_SongTimeText = GameObject::Instantiate(m_SongNPSText->get_transform()->get_parent()->get_gameObject(), m_SongNPSText->get_transform()->get_parent()->get_parent())->GetComponentInChildren<TextMeshProUGUI*>();
        m_SongTimeText->get_transform()->get_parent()->SetAsFirstSibling();
        m_SongTimeText->get_transform()->get_parent()->GetComponentInChildren<HMUI::ImageView*>()->set_sprite(Resources::FindObjectsOfTypeAll<Sprite*>()->First([](auto x) { return x->get_name() == u"ClockIcon"; }));

        /// Bind events
        m_SongCharacteristicSegmentedControl->add_didSelectCellEvent(
            custom_types::MakeDelegate<::System::Action_2<UnityW<::HMUI::SegmentedControl>, int>*>(std::function([this](UnityW<::HMUI::SegmentedControl> __a, int __b) -> void {
                OnCharacteristicChanged(__a, __b);
            }))
        );
        m_SongDiffSegmentedControl->add_didSelectCellEvent(
            custom_types::MakeDelegate<::System::Action_2<UnityW<::HMUI::SegmentedControl>, int>*>(std::function([this](UnityW<HMUI::SegmentedControl> __a, int __b) -> void {
                OnDifficultyChanged(__a, __b);
            }))
        );

        try
        {
            for (auto l_Text : m_GameObject->GetComponentsInChildren<TextMeshProUGUI*>(true))
                l_Text->set_fontStyle(l_Text->get_fontStyle().value__ & ~FontStyles::Italic.value__);

            for (auto l_Image : m_GameObject->GetComponentsInChildren<HMUI::ImageView*>(true))
            {
                m_SongCoverImage->____skew = 0.0f;
                m_SongCoverImage->SetAllDirty();
            }
        }
        catch (const std::exception&)
        {

        }

        m_GameObject->SetActive(true);
    }
    /// @brief Destructor
    LevelDetail::~LevelDetail()
    {

    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set if the game object is active
    /// @param p_Active New state
    void LevelDetail::SetActive(bool p_Active)
    {
        m_GameObject->SetActive(p_Active);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set from game
    /// @param p_BeatMap        BeatMap
    /// @param p_Cover          Cover texture
    /// @param p_Characteristic Game mode
    /// @param p_Difficulty     Difficulty
    bool LevelDetail::FromGame(BeatmapLevel* p_BeatMap, Sprite* p_Cover, BeatmapCharacteristicSO* p_Characteristic, BeatmapDifficulty p_Difficulty)
    {
        m_LocalBeatMap      = nullptr;
        m_BeatMap           = nullptr;

        if (p_BeatMap == nullptr)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.UI][LevelDetail.FromGame] Null Beatmap provided!");
            return false;
        }

        /// Display mode
        Characteristic(HMUI::IconSegmentedControl::DataItem::New_ctor(p_Characteristic->____icon, BGLib::Polyglot::Localization::Get(p_Characteristic->____descriptionLocalizationKey)));

        auto l_DifficultyBeatmap = p_BeatMap->GetDifficultyBeatmapData(p_Characteristic, p_Difficulty);

        /// Display difficulty
        Difficulty(Game::Levels::BeatmapDifficultySerializedNameToDifficultyName(BeatmapDifficultySerializedMethods::SerializedName(p_Difficulty)));

        Name          (p_BeatMap->___songName);
        AuthorNameText(u"Mapped by <b><u>" + p_BeatMap->___allMappers->FirstOrDefault() + u"</b></u>");
        Cover         (p_Cover ? p_Cover : Game::Levels::GetDefaultPackCover());
        Time          (p_BeatMap->___songDuration);
        BPM           (p_BeatMap->___beatsPerMinute);
        NJS           ((int)l_DifficultyBeatmap->___noteJumpMovementSpeed);
        Offset        (l_DifficultyBeatmap->___noteJumpStartBeatOffset);
        NPS           (l_DifficultyBeatmap->___notesCount / p_BeatMap->___songDuration);
        Notes         (l_DifficultyBeatmap->___notesCount);
        Obstacles     (l_DifficultyBeatmap->___obstaclesCount);
        Bombs         (l_DifficultyBeatmap->___bombsCount);

        return true;
    }
    /// @brief Set from BeatSaver
    /// @param p_BeatMap BeatMap
    /// @param p_Cover   Cover texture
    bool LevelDetail::FromBeatSaver(Game::BeatMaps::MapDetail::Ptr& p_BeatMap, Sprite* p_Cover)
    {
        m_LocalBeatMap  = nullptr;
        m_BeatMap       = nullptr;

        if (p_BeatMap == nullptr)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.UI][LevelDetail.FromBeatSaver2] Null Beatmap provided!");
            return false;
        }

        auto l_Version = p_BeatMap->SelectMapVersion();
        if (l_Version == nullptr)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.UI][LevelDetail.FromBeatSaver2] Invalid version!");
            return false;
        }

        /// Display modes
        auto l_Characteristics = ListW<HMUI::IconSegmentedControl::DataItem*>();
        for (auto& l_Current : l_Version->GetBeatmapCharacteristicSOSerializedNamesInOrder())
        {
            auto l_BeatmapCharacteristicSO = (BeatmapCharacteristicSO*)nullptr;
            if (Game::Levels::TryGetBeatmapCharacteristicSOBySerializedName(l_Current, &l_BeatmapCharacteristicSO))
                l_Characteristics->Add(HMUI::IconSegmentedControl::DataItem::New_ctor(l_BeatmapCharacteristicSO->____icon, BGLib::Polyglot::Localization::Get(l_BeatmapCharacteristicSO->____descriptionLocalizationKey)));
        }

        if (l_Characteristics->get_Count() == 0)
        {
            CP_SDK::ChatPlexSDK::Logger()->Error(u"[CP_SDK_BS.UI][LevelDetail.FromBeatSaver2] No valid characteristics found for map \"{p_BeatMap.id}\"!");
            return false;
        }

        /// Store beatmap
        m_BeatMap = p_BeatMap;

        HMUIIconSegmentedControl::SetDataNoHoverHint(m_SongCharacteristicSegmentedControl.Ptr(), l_Characteristics->ToArray().operator Array<HMUI::IconSegmentedControl::DataItem *> *());
        m_SongCharacteristicSegmentedControl->SelectCellWithNumber(0);
        OnCharacteristicChanged(nullptr, 0);

        /// Display informations
        Name          (p_BeatMap->metadata.value().songName);
        AuthorNameText(u"Mapped by <b><u>" + p_BeatMap->metadata.value().levelAuthorName + u"</b></u>");
        Cover         (p_Cover ? p_Cover : Game::Levels::GetDefaultPackCover());
        BPM           (p_BeatMap->metadata.value().bpm);

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set favorite toggle enabled
    /// @param p_Value
    void LevelDetail::SetFavoriteToggleEnabled(bool p_Value)
    {
        m_FavoriteToggle->SetActive(p_Value);
    }
    /// @brief Set favorite toggle images
    /// @param p_Default Default image
    /// @param p_Enabled Enable image
    void LevelDetail::SetFavoriteToggleImage(Sprite* p_Default, Sprite* p_Enabled)
    {
        auto l_IVDefault = m_FavoriteToggle->get_transform()->GetChild(0)->GetComponent<HMUI::ImageView*>();
        auto l_IVMarked  = m_FavoriteToggle->get_transform()->GetChild(1)->GetComponent<HMUI::ImageView*>();

        l_IVDefault->set_sprite(p_Default);
        l_IVMarked->set_sprite(p_Enabled);
    }
    /// @brief Set favorite toggle hover hint
    /// @param p_Hint New hint
    void LevelDetail::SetFavoriteToggleHoverHint(std::u16string_view p_Hint)
    {
        auto l_HoverHint = m_FavoriteToggle->GetComponent<HMUI::HoverHint*>();
        if (l_HoverHint == nullptr || !l_HoverHint)
        {
            l_HoverHint = m_FavoriteToggle->AddComponent<HMUI::HoverHint*>();
            l_HoverHint->____hoverHintController = Resources::FindObjectsOfTypeAll<HMUI::HoverHintController*>()->First();
        }

        l_HoverHint->set_text(p_Hint);
    }
    /// @brief Set favorite toggle value
    /// @param p_Value New value
    void LevelDetail::SetFavoriteToggleValue(bool p_Value)
    {
        m_FavoriteToggle->GetComponent<HMUI::ToggleWithCallbacks*>()->set_isOn(p_Value);
    }
    /// @brief Set favorite toggle callback
    /// @param p_Action New callback
    void LevelDetail::SetFavoriteToggleCallback(CP_SDK::Utils::CActionRef<> p_Action)
    {
        m_FavoriteToggle->GetComponent<HMUI::ToggleWithCallbacks*>()->add_stateDidChangeEvent(
            custom_types::MakeDelegate<::System::Action_1<::HMUI::ToggleWithCallbacks::SelectionState>*>(std::function([=](HMUI::ToggleWithCallbacks::SelectionState x) {
                if (x == HMUI::ToggleWithCallbacks::SelectionState::Pressed)
                    p_Action();
            }))
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Reverse button order
    void LevelDetail::ReverseButtonsOrder()
    {
        m_SecondaryButton->get_transform()->SetAsLastSibling();
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Set button enabled state
    /// @param p_Value New value
    void LevelDetail::SetSecondaryButtonEnabled(bool p_Value)
    {
        m_SecondaryButton->get_gameObject()->SetActive(p_Value);
    }
    /// @brief Set button enabled state
    /// @param p_Value New value
    void LevelDetail::SetPrimaryButtonEnabled(bool p_Value)
    {
        m_PrimaryButton->get_gameObject()->SetActive(p_Value);
    }
    /// @brief Set button enabled interactable
    /// @param p_Value New value
    void LevelDetail::SetPracticeButtonInteractable(bool p_Value)
    {
        m_SecondaryButton->SetInteractable(p_Value);
    }
    /// @brief Set button enabled interactable
    /// @param p_Value New value
    void LevelDetail::SetPrimaryButtonInteractable(bool p_Value)
    {
        m_PrimaryButton->SetInteractable(p_Value);
    }
    /// @brief Set button text
    /// @param p_Value New value
    void LevelDetail::SetSecondaryButtonText(std::u16string_view p_Value)
    {
        m_SecondaryButton->SetText(p_Value);
    }
    /// @brief Set button text
    /// @param p_Value New value
    void LevelDetail::SetPrimaryButtonText(std::u16string_view p_Value)
    {
        m_PrimaryButton->SetText(p_Value);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    /// @brief When the characteristic changed
    /// @param p_SegmentControl Control instance
    /// @param p_Index New selected index
    void LevelDetail::OnCharacteristicChanged(HMUI::SegmentedControl* p_SegmentControl, int p_Index)
    {
        if (m_LocalBeatMap)
        {
            std::vector<BeatmapCharacteristicSO*> l_Characs;
            Game::Levels::BeatmapLevel_ForEachBeatmapKey(m_LocalBeatMap.Ptr(), [&](const BeatmapKey& p_Current) -> bool
            {
                auto l_It = std::find(l_Characs.begin(), l_Characs.end(), p_Current.beatmapCharacteristic.unsafePtr());
                if (l_It != l_Characs.end())
                    return true;    ///< Continue

                l_Characs.push_back(const_cast<BeatmapCharacteristicSO*>(p_Current.beatmapCharacteristic.unsafePtr()));
                return true;    ///< Continue
            });

            if (p_Index > l_Characs.size())
                return;

            SelectedBeatmapCharacteristicSO = l_Characs[p_Index];

            auto l_Difficulties = System::Collections::Generic::List_1<StringW>::New_ctor();
            Game::Levels::BeatmapLevel_ForEachBeatmapKey(m_LocalBeatMap.Ptr(), [&](const BeatmapKey& p_Current) -> bool
            {
                auto l_Name = Game::Levels::BeatmapDifficultySerializedNameToDifficultyName(BeatmapDifficultySerializedMethods::SerializedName(p_Current.difficulty));
                if (l_Difficulties->Contains(l_Name))
                    return true;    ///< Continue

                l_Difficulties->Add(l_Name);
                return true;    ///< Continue
            });

            m_SongDiffSegmentedControl->SetTexts(l_Difficulties->AsReadOnly()->i___System__Collections__Generic__IReadOnlyList_1_T_());
            m_SongDiffSegmentedControl->SelectCellWithNumber(l_Difficulties->get_Count() - 1);
            OnDifficultyChanged(nullptr, l_Difficulties->get_Count() - 1);
        }
        else if (m_BeatMap != nullptr)
        {
            auto l_Version = m_BeatMap->SelectMapVersion();
            auto l_Characs = l_Version->GetBeatmapCharacteristicSOSerializedNamesInOrder();

            if (p_Index > l_Characs.size())
                return;

            auto l_BeatmapCharacteristicSO = (BeatmapCharacteristicSO*)nullptr;
            Game::Levels::TryGetBeatmapCharacteristicSOBySerializedName(Game::Levels::SanitizeBeatmapCharacteristicSOSerializedName(l_Characs[p_Index]), &l_BeatmapCharacteristicSO);
            SelectedBeatmapCharacteristicSO = l_BeatmapCharacteristicSO;

            auto l_Difficulties = System::Collections::Generic::List_1<StringW>::New_ctor();
            for (auto& l_Current : l_Version->GetDifficultiesPerBeatmapCharacteristicSOSerializedName(l_Characs[p_Index]))
                l_Difficulties->Add(Game::Levels::BeatmapDifficultySerializedNameToDifficultyName(l_Current->difficulty));

            m_SongDiffSegmentedControl->SetTexts(l_Difficulties->AsReadOnly()->i___System__Collections__Generic__IReadOnlyList_1_T_());
            m_SongDiffSegmentedControl->SelectCellWithNumber(l_Difficulties->get_Count() - 1);
            OnDifficultyChanged(nullptr, l_Difficulties->get_Count() - 1);
        }
    }
    /// @brief When the difficulty is changed
    /// @param p_SegmentControl Control instance
    /// @param p_Index          New selected index
    void LevelDetail::OnDifficultyChanged(HMUI::SegmentedControl* p_SegmentControl, int p_Index)
    {
        if (m_LocalBeatMap)
        {
            std::vector<BeatmapKey> l_Difficulties;
            Game::Levels::BeatmapLevel_ForEachBeatmapKey(m_LocalBeatMap.Ptr(), [&](const BeatmapKey& p_Current) -> bool
            {
                if (p_Current.beatmapCharacteristic.unsafePtr() != SelectedBeatmapCharacteristicSO.Ptr(false))
                    return true;    ///< Continue

                auto l_It = std::find_if(l_Difficulties.begin(), l_Difficulties.end(), [&](const BeatmapKey& p_A) -> bool {
                    return p_A.beatmapCharacteristic.unsafePtr() == p_Current.beatmapCharacteristic.unsafePtr() && p_A.difficulty == p_Current.difficulty && p_A.levelId == p_Current.levelId;
                });
                if (l_It != l_Difficulties.end())
                    return true;    ///< Continue

                l_Difficulties.push_back(p_Current);
                return true;    ///< Continue
            });

            if (p_Index < 0 || p_Index >= l_Difficulties.size())
            {
                Time     (-1.0f);
                NPS      (-1.0f);
                NJS      (-1);
                Offset   (std::numeric_limits<float>::quiet_NaN());
                Notes    (-1);
                Obstacles(-1);
                Bombs    (-1);
                return;
            }

            auto& l_BeatmapKey        = l_Difficulties[p_Index];
            auto  l_DifficultyBeatmap = m_LocalBeatMap->GetDifficultyBeatmapData(SelectedBeatmapCharacteristicSO.Ptr(), l_BeatmapKey.difficulty);

            Time          (m_LocalBeatMap->___songDuration);
            BPM           (m_LocalBeatMap->___beatsPerMinute);
            NJS           ((int)l_DifficultyBeatmap->___noteJumpMovementSpeed);
            Offset        (l_DifficultyBeatmap->___noteJumpStartBeatOffset);
            NPS           (l_DifficultyBeatmap->___notesCount / m_LocalBeatMap->___songDuration);
            Notes         (l_DifficultyBeatmap->___notesCount);
            Obstacles     (l_DifficultyBeatmap->___obstaclesCount);
            Bombs         (l_DifficultyBeatmap->___bombsCount);

            OnActiveDifficultyChanged(l_BeatmapKey);
        }
        else if (m_BeatMap)
        {
            auto l_Version = m_BeatMap->SelectMapVersion();
            auto l_Characs = l_Version->GetBeatmapCharacteristicSOSerializedNamesInOrder();

            if (m_SongCharacteristicSegmentedControl->get_selectedCellNumber() > l_Characs.size())
                return;

            auto l_Difficulties = l_Version->GetDifficultiesPerBeatmapCharacteristicSOSerializedName(l_Characs[m_SongCharacteristicSegmentedControl->get_selectedCellNumber()]);
            if (p_Index < 0 || p_Index >= l_Difficulties.size())
            {
                Time     (-1.0f);
                NPS      (-1.0f);
                NJS      (-1);
                Offset   (std::numeric_limits<float>::quiet_NaN());
                Notes    (-1);
                Obstacles(-1);
                Bombs    (-1);
                return;
            }

            auto& l_SelectedBeatmapCharacteristicDifficulty  = l_Difficulties[p_Index];
            SelectedBeatmapDifficulty                        = Game::Levels::BeatmapDifficultySerializedNameToBeatmapDifficulty(l_SelectedBeatmapCharacteristicDifficulty->difficulty);

            /// Display informations
            Time     ((double)m_BeatMap->metadata.value().duration);
            NPS      ((float)l_SelectedBeatmapCharacteristicDifficulty->nps);
            NJS      ((int)l_SelectedBeatmapCharacteristicDifficulty->njs);
            Offset   (l_SelectedBeatmapCharacteristicDifficulty->offset);
            Notes    (l_SelectedBeatmapCharacteristicDifficulty->notes);
            Obstacles(l_SelectedBeatmapCharacteristicDifficulty->obstacles);
            Bombs    (l_SelectedBeatmapCharacteristicDifficulty->bombs);

            std::u16string l_LevelID;
            if (Game::Levels::TryGetLevelIDFromHash(l_Version->hash, &l_LevelID))
            {
                auto l_BeatmapKey = BeatmapKey(SelectedBeatmapCharacteristicSO.Ptr(), SelectedBeatmapDifficulty, l_LevelID);
                OnActiveDifficultyChanged(l_BeatmapKey);
            }
        }
    }
    /// @brief Secondary button on click
    void LevelDetail::OnSecondaryButtonClicked()
    {
        OnSecondaryButton();
    }
    /// @brief Primary button on click
    void LevelDetail::OnPrimaryButtonClicked()
    {
        OnPrimaryButton();
    }

}   ///< namespace CP_SDK_BS::UI