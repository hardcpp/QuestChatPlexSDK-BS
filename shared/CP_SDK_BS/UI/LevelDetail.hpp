#pragma once

#include "../../CP_SDK/Utils/Il2cpp.hpp"
#include "../../CP_SDK/Utils/MonoPtr.hpp"
#include "../../CP_SDK/Utils/Event.hpp"
#include "../../CP_SDK/UI/Components/CPrimaryButton.hpp"
#include "../../CP_SDK/UI/Components/CSecondaryButton.hpp"
#include "../Game/BeatMaps/MapDetail.hpp"

#include <GlobalNamespace/BeatmapDifficultySegmentedControlController.hpp>
#include <GlobalNamespace/BeatmapCharacteristicSegmentedControlController.hpp>
#include <GlobalNamespace/BeatmapCharacteristicSO.hpp>
#include <GlobalNamespace/BeatmapDifficulty.hpp>
#include <GlobalNamespace/BeatmapKey.hpp>
#include <GlobalNamespace/BeatmapLevel.hpp>
#include <HMUI/ImageView.hpp>
#include <HMUI/TextSegmentedControl.hpp>
#include <HMUI/IconSegmentedControl.hpp>
#include <TMPro/TextMeshProUGUI.hpp>

namespace CP_SDK_BS::UI {

    namespace _u
    {
        using namespace GlobalNamespace;
        using namespace UnityEngine;
        using namespace TMPro;
    }
    namespace _v
    {
        using namespace CP_SDK::UI;
        using namespace CP_SDK::UI::Components;
        using namespace CP_SDK::Utils;
    }

    /// @brief Song detail widget
    class LevelDetail
    {
        CP_SDK_NO_COPYMOVE_CTORS(LevelDetail);

        private:
            static _v::MonoPtr<_u::GameObject> m_SongDetailViewTemplate;

        public:
            /// @brief Init
            static void Init();

        private:
            _v::MonoPtr<_u::GameObject>                                         m_GameObject;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongNameText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_AuthorNameText;
            _v::MonoPtr<HMUI::ImageView>                                        m_SongCoverImage;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongTimeText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongBPMText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongNPSText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongNJSText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongOffsetText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongNotesText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongObstaclesText;
            _v::MonoPtr<_u::TextMeshProUGUI>                                    m_SongBombsText;
            _v::MonoPtr<_u::BeatmapDifficultySegmentedControlController>        m_DifficultiesSegmentedControllerClone;
            _v::MonoPtr<_u::BeatmapCharacteristicSegmentedControlController>    m_CharacteristicSegmentedControllerClone;
            _v::MonoPtr<HMUI::TextSegmentedControl>                             m_SongDiffSegmentedControl;
            _v::MonoPtr<HMUI::IconSegmentedControl>                             m_SongCharacteristicSegmentedControl;
            _v::MonoPtr<_v::CSecondaryButton>                                   m_SecondaryButton;
            _v::MonoPtr<_v::CPrimaryButton>                                     m_PrimaryButton;
            _v::MonoPtr<_u::GameObject>                                         m_FavoriteToggle;
            _v::MonoPtr<_u::BeatmapLevel>                                       m_LocalBeatMap;
            Game::BeatMaps::MapDetail::Ptr                                      m_BeatMap;

        private:
            double          m_Time      = 0;
            float           m_BPM       = 0;
            float           m_NPS       = 0;
            int             m_NJS       = 0;
            float           m_Offset    = 0;
            int             m_Notes     = 0;
            int             m_Obstacles = 0;
            int             m_Bombs     = 0;
            std::u16string  m_Difficulty;

            _v::MonoPtr<HMUI::IconSegmentedControl::DataItem> m_Characteristic;

        public:
            _v::MonoPtr<_u::BeatmapCharacteristicSO>    SelectedBeatmapCharacteristicSO;
            _u::BeatmapDifficulty                       SelectedBeatmapDifficulty;

            _v::Event<_u::BeatmapKey&> OnActiveDifficultyChanged;

            _v::Action<> OnSecondaryButton;
            _v::Action<> OnPrimaryButton;

        public:
            std::u16string                          Name();
            void                                    Name(std::u16string_view p_Value);
            std::u16string                          AuthorNameText();
            void                                    AuthorNameText(std::u16string_view p_Value);
            _u::Sprite*                             Cover();
            void                                    Cover(_u::Sprite* p_Value);
            double                                  Time();
            void                                    Time(double p_Value);
            float                                   BPM();
            void                                    BPM(float p_Value);
            float                                   NPS();
            void                                    NPS(float p_Value);
            int                                     NJS();
            void                                    NJS(int p_Value);
            float                                   Offset();
            void                                    Offset(float p_Value);
            int                                     Notes();
            void                                    Notes(int p_Value);
            int                                     Obstacles();
            void                                    Obstacles(int p_Value);
            int                                     Bombs();
            void                                    Bombs(int p_Value);
            HMUI::IconSegmentedControl::DataItem*   Characteristic();
            void                                    Characteristic(HMUI::IconSegmentedControl::DataItem* p_Value);
            std::u16string                          Difficulty();
            void                                    Difficulty(std::u16string_view p_Value);

        public:
            /// @brief Constructor
            /// @param p_Parent Parent transform
            LevelDetail(_u::Transform* p_Parent);
            /// @brief Destructor
            ~LevelDetail();

        public:
            /// @brief Set if the game object is active
            /// @param p_Active New state
            void SetActive(bool p_Active);

        public:
            /// @brief Set from game
            /// @param p_BeatMap        BeatMap
            /// @param p_Cover          Cover texture
            /// @param p_Characteristic Game mode
            /// @param p_Difficulty     Difficulty
            bool FromGame(_u::BeatmapLevel* p_BeatMap, _u::Sprite* p_Cover, _u::BeatmapCharacteristicSO* p_Characteristic, _u::BeatmapDifficulty p_Difficulty);
            /// @brief Set from BeatSaver
            /// @param p_BeatMap BeatMap
            /// @param p_Cover   Cover texture
            bool FromBeatSaver(Game::BeatMaps::MapDetail::Ptr& p_BeatMap, _u::Sprite* p_Cover);

        public:
            /// @brief Set favorite toggle enabled
            /// @param p_Value
            void SetFavoriteToggleEnabled(bool p_Value);
            /// @brief Set favorite toggle images
            /// @param p_Default Default image
            /// @param p_Enabled Enable image
            void SetFavoriteToggleImage(_u::Sprite* p_Default, _u::Sprite* p_Enabled);
            /// @brief Set favorite toggle hover hint
            /// @param p_Hint New hint
            void SetFavoriteToggleHoverHint(std::u16string_view p_Hint);
            /// @brief Set favorite toggle value
            /// @param p_Value New value
            void SetFavoriteToggleValue(bool p_Value);
            /// @brief Set favorite toggle callback
            /// @param p_Action New callback
            void SetFavoriteToggleCallback(_v::CActionRef<> p_Action);

        public:
            /// @brief Reverse button order
            void ReverseButtonsOrder();

        public:
            /// @brief Set button enabled state
            /// @param p_Value New value
            void SetSecondaryButtonEnabled(bool p_Value);
            /// @brief Set button enabled state
            /// @param p_Value New value
            void SetPrimaryButtonEnabled(bool p_Value);
            /// @brief Set button enabled interactable
            /// @param p_Value New value
            void SetPracticeButtonInteractable(bool p_Value);
            /// @brief Set button enabled interactable
            /// @param p_Value New value
            void SetPrimaryButtonInteractable(bool p_Value);
            /// @brief Set button text
            /// @param p_Value New value
            void SetSecondaryButtonText(std::u16string_view p_Value);
            /// @brief Set button text
            /// @param p_Value New value
            void SetPrimaryButtonText(std::u16string_view p_Value);

        private:
            /// @brief When the characteristic changed
            /// @param p_SegmentControl Control instance
            /// @param p_Index New selected index
            void OnCharacteristicChanged(HMUI::SegmentedControl* p_SegmentControl, int p_Index);
            /// @brief When the difficulty is changed
            /// @param p_SegmentControl Control instance
            /// @param p_Index          New selected index
            void OnDifficultyChanged(HMUI::SegmentedControl* p_SegmentControl, int p_Index);
            /// @brief Secondary button on click
            void OnSecondaryButtonClicked();
            /// @brief Primary button on click
            void OnPrimaryButtonClicked();

    };

}   ///< namespace CP_SDK_BS::UI