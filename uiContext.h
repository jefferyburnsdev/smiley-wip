#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "SaveData.h"

using namespace std;

struct SaveFileUIElements {
    sf::Text fileOption;
    sf::Text name;
    sf::Text worldsStat;
    sf::Text orbsStat;
    sf::Text achievementsStat;
};

enum class UIState {
    MainMenu,
    MainMenuSettings,
    NewSaveSubmenu,
    ExistingSaveSubmenu,
    Splash,

    Cutscene,
    WorldMap,
    Level,
    WorldMapPauseMenu,
    LevelPauseMenu,
    Settings,
    Achievements,
    Exit
};

class UIContext {
public:
    UIContext(sf::Font& font);
    const float windowWidth = 1600.0f;
    const float centerX = windowWidth / 2.0f;

    UIState currentState = UIState::MainMenu;
    
    struct SubMenuElements {
        sf::Text splashTitle;
        sf::Text splashSmile;
        sf::Text splashStartButton;
        sf::Text splashQuitButton;

        int hoveredSaveFileIndex = -1;
        int selectedSaveFileIndex = -1;
        vector<SaveFileUIElements> saveFileElements;
        sf::Text newSaveNameLabel;
        sf::Text inputNameForNewSaveText;
        sf::RectangleShape inputNameForNewSaveBox;
        sf::Text confirmNewSaveCreationButton;
        sf::Text cancelNewSaveCreationButton;
        string typedInputForSaveFileName;

        sf::Text existingSaveNameLabel;
        sf::Text continueSelectedSaveButton;
        sf::Text deleteSelectedSaveButton;
        sf::Text cancelLoadSelectedSaveButton;

        sf::Text worldMapPauseMenuContinueButton;
        sf::Text worldMapPauseMenuReturnToMainMenuButton;
        sf::Text worldMapPauseMenuSettingsButton;
        sf::Text worldMapPauseMenuAchievementsButton;

        sf::Text levelPauseMenuContinueButton;
        sf::Text levelPauseMenuReturnToWorldMapButton;
        sf::Text levelPauseMenuSettingsButton;

        sf::Text settingsDisplayText;
        bool draggingMasterVol = false;
        bool draggingMusicVol = false;
        sf::Text settingsMenuBackButton;
        sf::Text masterVolume;
        sf::Text musicVolume;
        sf::Text resetToDefault;
        sf::RectangleShape masterVolumeSliderBar;
        sf::CircleShape masterVolumeKnob;
        sf::Text masterVolumeValueText;
        sf::RectangleShape musicVolumeSliderBar;
        sf::CircleShape musicVolumeKnob;
        sf::Text musicVolumeValueText;

        sf::Text achievementsDisplayText;
        int currentAchievementsPage = 0;
        sf::Text achievementsMenuLeftArrow;
        sf::Text achievementsMenuRightArrow;
        sf::Text achievementsMenuBackButton;
        sf::Text achievementsMenuTotalOrbsCountDisplay;
        vector<sf::CircleShape> book1Orbs;
        vector<sf::CircleShape> book2Orbs;
        vector<sf::CircleShape> book3Orbs;
        vector<sf::CircleShape> book4Orbs;
        vector<sf::CircleShape> book5Orbs;
        sf::Text book1Label;
        sf::Text book2Label;
        sf::Text book3Label;
        sf::Text book4Label;
        sf::Text book5Label;
        vector<sf::RectangleShape> achievementBoxes{ 30 };
        vector<sf::Text> achievementTitles{ 30 };
        vector<sf::Text> achievementDescriptions{ 30 };
    };

    SubMenuElements submenu;

    string worldMapTexture = "assets/images/grandworldmap.png";

    sf::Text mainMenuBackButton;

    void updateHoverState(int selected, sf::Vector2f mousePos, bool confirmEnabled = false);
    void updateSaveFileText();
    void draw(sf::RenderWindow& window);

    SaveData& getCurrentSaveData();
    string getCurrentFilename();
    bool loadSaveConfirmed();
    void setCurrentSaveData(const SaveData& data);
    void setLevelID(int worldIndex, int levelIndex);
    int getWorldIndex() const;
    int getLevelIndex() const;

private:
    SaveData currentSaveData;
    bool confirmInCreateNewSaveClicked = false;
    int currentWorldIndex = 0;
    int currentLevelIndex = 0;
};
