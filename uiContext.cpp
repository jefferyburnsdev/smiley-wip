#include "uiContext.h"
#include "SaveData.h"
#include "SaveManager.h"
#include <iostream>

using namespace std;

UIContext::UIContext(sf::Font& font){
    // splash menu items
    submenu.splashTitle.setFont(font);
    submenu.splashTitle.setString("SMILEY");
    submenu.splashTitle.setCharacterSize(200.f);
    sf::FloatRect titleBounds = submenu.splashTitle.getLocalBounds();
    submenu.splashTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    submenu.splashTitle.setPosition(1600.f / 2.f, 150.f + titleBounds.height / 2.f);

    submenu.splashSmile.setFont(font);
    submenu.splashSmile.setString(".."); // eyes
    submenu.splashSmile.setCharacterSize(200.f);
    sf::FloatRect smileBounds = submenu.splashSmile.getLocalBounds();
    submenu.splashSmile.setOrigin(smileBounds.left + smileBounds.width / 2.f, smileBounds.top + smileBounds.height / 2.f);
    submenu.splashSmile.setPosition(1010.f, 150.f);

    submenu.splashStartButton.setFont(font);
    submenu.splashStartButton.setString("start");
    submenu.splashStartButton.setCharacterSize(75);
    sf::FloatRect splashStartButtonBounds = submenu.splashStartButton.getLocalBounds();
    submenu.splashStartButton.setOrigin(splashStartButtonBounds.left + splashStartButtonBounds.width / 2.f,
        splashStartButtonBounds.top + splashStartButtonBounds.height / 2.f);
    submenu.splashStartButton.setPosition(1600.f / 2.f, 500.f + splashStartButtonBounds.height / 2.f);

    submenu.splashQuitButton.setFont(font);
    submenu.splashQuitButton.setString("quit");
    submenu.splashQuitButton.setCharacterSize(50.f);
    submenu.splashQuitButton.setPosition(20.f, 20.f);

    // existingsave menu items
    submenu.existingSaveNameLabel.setFont(font);
    submenu.existingSaveNameLabel.setString("");
    submenu.existingSaveNameLabel.setCharacterSize(50.f);
    submenu.existingSaveNameLabel.setPosition(300.f, 200.f);
    submenu.existingSaveNameLabel.setFillColor(sf::Color::White);

    submenu.continueSelectedSaveButton.setFont(font);
    submenu.continueSelectedSaveButton.setString("continue");
    submenu.continueSelectedSaveButton.setCharacterSize(50.f);
    sf::FloatRect continueSelectedSaveButtonBounds = submenu.continueSelectedSaveButton.getLocalBounds();
    submenu.continueSelectedSaveButton.setOrigin(continueSelectedSaveButtonBounds.width / 2.f, continueSelectedSaveButtonBounds.height / 2.f);
    submenu.continueSelectedSaveButton.setPosition(centerX - 200.f, 500.f);
    submenu.continueSelectedSaveButton.setFillColor(sf::Color::White);

    submenu.deleteSelectedSaveButton.setFont(font);
    submenu.deleteSelectedSaveButton.setString("delete");
    submenu.deleteSelectedSaveButton.setCharacterSize(50.f);
    sf::FloatRect deleteSelectedSaveButtonBounds = submenu.deleteSelectedSaveButton.getLocalBounds();
    submenu.deleteSelectedSaveButton.setOrigin(deleteSelectedSaveButtonBounds.width / 2.f, deleteSelectedSaveButtonBounds.height / 2.f);
    submenu.deleteSelectedSaveButton.setPosition(centerX + 200.f, 500.f);
    submenu.deleteSelectedSaveButton.setFillColor(sf::Color::White);

    submenu.cancelLoadSelectedSaveButton.setFont(font);
    submenu.cancelLoadSelectedSaveButton.setString("cancel");
    submenu.cancelLoadSelectedSaveButton.setCharacterSize(50.f);
    sf::FloatRect cancelLoadSelectedSaveButtonBounds = submenu.cancelLoadSelectedSaveButton.getLocalBounds();
    submenu.cancelLoadSelectedSaveButton.setOrigin(cancelLoadSelectedSaveButtonBounds.width / 2.f, cancelLoadSelectedSaveButtonBounds.height / 2.f);
    submenu.cancelLoadSelectedSaveButton.setPosition(centerX, 500.f);
    submenu.cancelLoadSelectedSaveButton.setFillColor(sf::Color::White);

    // createnewsave menu items
    submenu.inputNameForNewSaveBox.setSize(sf::Vector2f(600.f, 80.f));
    submenu.inputNameForNewSaveBox.setFillColor(sf::Color(50, 50, 50));
    submenu.inputNameForNewSaveBox.setOutlineThickness(3.f);
    submenu.inputNameForNewSaveBox.setOutlineColor(sf::Color::White);
    submenu.inputNameForNewSaveBox.setOrigin(submenu.inputNameForNewSaveBox.getSize().x / 2.f, submenu.inputNameForNewSaveBox.getSize().y / 2.f);
    submenu.inputNameForNewSaveBox.setPosition(centerX, 260.f);

    submenu.inputNameForNewSaveText.setFont(font);
    submenu.inputNameForNewSaveText.setCharacterSize(80.f);
    submenu.inputNameForNewSaveText.setFillColor(sf::Color::White);
    submenu.inputNameForNewSaveText.setString("");
    submenu.inputNameForNewSaveText.setPosition(submenu.inputNameForNewSaveBox.getPosition().x - submenu.inputNameForNewSaveBox.getSize().x / 2.f + 10.f,
        submenu.inputNameForNewSaveBox.getPosition().y - submenu.inputNameForNewSaveText.getCharacterSize() + 20.f);

    submenu.confirmNewSaveCreationButton.setFont(font);
    submenu.confirmNewSaveCreationButton.setString("confirm");
    submenu.confirmNewSaveCreationButton.setCharacterSize(55.f);
    sf::FloatRect confirmNewSaveCreationButtonBounds = submenu.confirmNewSaveCreationButton.getLocalBounds();
    submenu.confirmNewSaveCreationButton.setOrigin(confirmNewSaveCreationButtonBounds.width / 2.f, confirmNewSaveCreationButtonBounds.height / 2.f);
    submenu.confirmNewSaveCreationButton.setPosition(centerX - 150.f, 370.f);
    submenu.confirmNewSaveCreationButton.setFillColor(sf::Color(100, 100, 100));  // initially gray

    submenu.cancelNewSaveCreationButton.setFont(font);
    submenu.cancelNewSaveCreationButton.setString("cancel");
    submenu.cancelNewSaveCreationButton.setCharacterSize(55);
    sf::FloatRect cancelNewSaveCreationButtonBounds = submenu.cancelNewSaveCreationButton.getLocalBounds();
    submenu.cancelNewSaveCreationButton.setOrigin(cancelNewSaveCreationButtonBounds.width / 2.f, cancelNewSaveCreationButtonBounds.height / 2.f);
    submenu.cancelNewSaveCreationButton.setPosition(centerX + 150.f, 370.f);
    submenu.cancelNewSaveCreationButton.setFillColor(sf::Color::White);

    mainMenuBackButton.setFont(font);
    mainMenuBackButton.setString("<back");
    mainMenuBackButton.setCharacterSize(50.f);
    mainMenuBackButton.setPosition(20.f, 20.f);

    submenu.newSaveNameLabel.setFont(font);
    submenu.newSaveNameLabel.setString("name for this save:");
    submenu.newSaveNameLabel.setCharacterSize(60);
    sf::FloatRect labelBounds = submenu.newSaveNameLabel.getLocalBounds();
    submenu.newSaveNameLabel.setOrigin(labelBounds.width / 2.f, labelBounds.height / 2.f);
    submenu.newSaveNameLabel.setPosition(centerX, 160.f);

    // achievements menu items
    submenu.achievementsDisplayText.setFont(font);
    submenu.achievementsDisplayText.setString("achievements");
    submenu.achievementsDisplayText.setCharacterSize(75.f);
    sf::FloatRect achievementsBounds = submenu.achievementsDisplayText.getLocalBounds();
    submenu.achievementsDisplayText.setOrigin(achievementsBounds.width / 2.f, achievementsBounds.height / 2.f);
    submenu.achievementsDisplayText.setPosition(centerX, 20.f);
    submenu.achievementsDisplayText.setFillColor(sf::Color::White);

    submenu.achievementsMenuTotalOrbsCountDisplay.setFont(font);
    submenu.achievementsMenuTotalOrbsCountDisplay.setString("orbs collected: 0");
    submenu.achievementsMenuTotalOrbsCountDisplay.setCharacterSize(50.f);
    submenu.achievementsMenuTotalOrbsCountDisplay.setPosition(20.f, 700.f);
    submenu.achievementsMenuTotalOrbsCountDisplay.setFillColor(sf::Color::White);

    submenu.achievementsMenuLeftArrow.setFont(font);
    submenu.achievementsMenuLeftArrow.setString("<");
    submenu.achievementsMenuLeftArrow.setCharacterSize(50.f);
    sf::FloatRect leftArrowBounds = submenu.achievementsMenuLeftArrow.getLocalBounds();
    submenu.achievementsMenuLeftArrow.setOrigin(leftArrowBounds.width / 2.f, leftArrowBounds.height / 2.f);
    submenu.achievementsMenuLeftArrow.setPosition(centerX - 200, 20.f);
    submenu.achievementsMenuLeftArrow.setFillColor(sf::Color::White);

    submenu.achievementsMenuRightArrow.setFont(font);
    submenu.achievementsMenuRightArrow.setString(">");
    submenu.achievementsMenuRightArrow.setCharacterSize(50.f);
    sf::FloatRect rightArrowBounds = submenu.achievementsMenuRightArrow.getLocalBounds();
    submenu.achievementsMenuRightArrow.setOrigin(rightArrowBounds.width / 2.f, rightArrowBounds.height / 2.f);
    submenu.achievementsMenuRightArrow.setPosition(centerX + 200, 20.f);
    submenu.achievementsMenuRightArrow.setFillColor(sf::Color::White);

    submenu.achievementsMenuBackButton.setFont(font);
    submenu.achievementsMenuBackButton.setString("<back");
    submenu.achievementsMenuBackButton.setCharacterSize(50.f);
    submenu.achievementsMenuBackButton.setPosition(20.f, 20.f);
    submenu.achievementsMenuBackButton.setFillColor(sf::Color::White);

    float columnStartX = 20.f;
    float columnSpacing = 175.f;
    float orbRadius = 20.f;
    float orbSpacingY = 50.f;
    float bookLabelOffsetY = 120.f;
    submenu.book1Label.setFont(font);
    submenu.book1Label.setString("fractured nullspace");
    submenu.book1Label.setCharacterSize(25.f);
    submenu.book1Label.setFillColor(sf::Color::White);
    submenu.book1Label.setPosition(columnStartX + 0 * columnSpacing, bookLabelOffsetY);

    submenu.book2Label.setFont(font);
    submenu.book2Label.setString("uncanny valley");
    submenu.book2Label.setCharacterSize(25.f);
    submenu.book2Label.setFillColor(sf::Color::White);
    submenu.book2Label.setPosition(columnStartX + 1 * columnSpacing, bookLabelOffsetY);

    submenu.book3Label.setFont(font);
    submenu.book3Label.setString("bleak mechanica");
    submenu.book3Label.setCharacterSize(25.f);
    submenu.book3Label.setFillColor(sf::Color::White);
    submenu.book3Label.setPosition(columnStartX + 2 * columnSpacing, bookLabelOffsetY);

    submenu.book4Label.setFont(font);
    submenu.book4Label.setString("reverie threshold");
    submenu.book4Label.setCharacterSize(25.f);
    submenu.book4Label.setFillColor(sf::Color::White);
    submenu.book4Label.setPosition(columnStartX + 3 * columnSpacing, bookLabelOffsetY);

    submenu.book5Label.setFont(font);
    submenu.book5Label.setString("fading dream");
    submenu.book5Label.setCharacterSize(25.f);
    submenu.book5Label.setFillColor(sf::Color::White);
    submenu.book5Label.setPosition(columnStartX + 4 * columnSpacing, bookLabelOffsetY);

    auto createOrb = [=](float x, float y) {
        sf::CircleShape orb(orbRadius);
        orb.setPosition(x, y);
        orb.setFillColor(sf::Color(10, 10, 10, 100));
        orb.setOutlineColor(sf::Color::Black);
        orb.setOutlineThickness(1.f);
        return orb;
        };

    float baseOrbY = bookLabelOffsetY + 50.f;
    for (int i = 0; i < 10; ++i) {
        float x = columnStartX + 0 * columnSpacing;
        float y = baseOrbY + i * orbSpacingY;
        submenu.book1Orbs.push_back(createOrb(x, y));
    }
    for (int i = 0; i < 8; ++i) {
        float x = columnStartX + 1 * columnSpacing;
        float y = baseOrbY + i * orbSpacingY;
        submenu.book2Orbs.push_back(createOrb(x, y));
    }
    for (int i = 0; i < 11; ++i) {
        float x = columnStartX + 2 * columnSpacing;
        float y = baseOrbY + i * orbSpacingY;
        submenu.book3Orbs.push_back(createOrb(x, y));
    }
    for (int i = 0; i < 11; ++i) {
        float x = columnStartX + 3 * columnSpacing;
        float y = baseOrbY + i * orbSpacingY;
        submenu.book4Orbs.push_back(createOrb(x, y));
    }
    for (int i = 0; i < 12; ++i) {
        float x = columnStartX + 4 * columnSpacing;
        float y = baseOrbY + i * orbSpacingY;
        submenu.book5Orbs.push_back(createOrb(x, y));
    }

    const float boxWidth = 600.f;
    const float boxHeight = 70.f;
    const float leftX = 900.f;
    const float topY = 120.f;
    const float boxSpacingY = 90.f;
    const string achievementTitles[30] = {
        /*1*/"i think, therefore...", /*2*/"no regrets", /*3*/"if (!nullptr)", /*4*/"completely canny", /*5*/"light in the distance", /*6*/"stuck in your head", 
        /*7*/"i think i remember...", /*8*/"...", /*9*/"\\E709w?R7508a70ERr6;\\n .\\UNINSTALL.EXE", /*10*/"truth hurts", /*11*/"the second freefaller",
        /*12*/"long-winded", /*13*/"i've no quarrel with thee", /*14*/"mastermind", /*15*/"where have you been?", /*16*/"housekeeping", 
        /*17*/"wait, i know you...", /*18*/"when there's a will, there's a way", /*19*/"taking a dip", /*20*/"liminal tunnelvision", /*21*/"is this good?",
        /*22*/"seek help", /*23*/"just checkin' in", /*24*/"identity crisis", /*25*/"m-cap", /*26*/"everyone's a critic", /*27*/"i dunno, Lois, i'm scared-",
        /*28*/"that's crazy", /*29*/"life goes on", /*30*/"pretend it's platinum"
    };
    const string achievementDescriptions[30] = {
        /*1*/"you are born.", /*2*/"you remember everything now: and you won't change a thing.", /*3*/"clear the fractured nullspace.", 
        /*4*/"clear the uncanny valley.", /*5*/"clear the bleak mechanica.", /*6*/"clear the reverie threshold.", /*7*/"clear the fading dream.", 
        /*8*/"witness the death of a friend.", /*9*/"YOU WEREN'T SUPPOSED TO DO THAT.", /*10*/"obtain the truthblade.", /*11*/"obtain the hovercape.", 
        /*12*/"exhaust your conversation with Jkmn.", /*13*/"complete \"control room\" without explicitly damaging any lie.", 
        /*14*/"defeat Cerebrumael without taking any damage.", /*15*/"find Lok.", /*16*/"make the house as presentable as possible in \"something seems off\".",
        /*17*/"remember a friend for who he truly is.", /*18*/"find a way to take damage in the fading dream.", /*19*/"decide to swim to shore.",
        /*20*/"complete \"infinite village\" without ever retracing your steps.", /*21*/"collect your first orb and not know its significance.",
        /*22*/"kill Iliette.", /*23*/"visit an old friend.", /*24*/"refuse to smile.", /*25*/"collect every orb in the game.",
        /*26*/"get insulted by a friend.", /*27*/"find a place to hide from Binarney.", /*28*/"kill Mentallison.", /*29*/"choose to forget.",
        /*30*/"unlock all other achievements."
    };
    submenu.achievementBoxes.resize(30);
    submenu.achievementTitles.resize(30);
    submenu.achievementDescriptions.resize(30);
    for (int i = 0; i < 30; ++i) {
        float yPos = topY + (i % 6) * boxSpacingY;

        auto& box = submenu.achievementBoxes[i];
        auto& title = submenu.achievementTitles[i];
        auto& desc = submenu.achievementDescriptions[i];

        box.setSize({ boxWidth, boxHeight });
        box.setPosition(leftX, yPos);
        box.setFillColor(sf::Color(0, 0, 0, 150));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);

        title.setFont(font);
        title.setString(achievementTitles[i]);
        title.setCharacterSize(30.f);
        title.setFillColor(sf::Color::White);
        title.setPosition(leftX + 15.f, yPos + 0.f);

        desc.setFont(font);
        desc.setString(achievementDescriptions[i]);
        desc.setCharacterSize(24.f);
        desc.setFillColor(sf::Color(200, 200, 200));
        desc.setPosition(leftX + 15.f, yPos + 35.f);
    }

    // settings menu items
    submenu.settingsDisplayText.setFont(font);
    submenu.settingsDisplayText.setString("settings");
    submenu.settingsDisplayText.setCharacterSize(75.f);
    sf::FloatRect settingsTitleBounds = submenu.settingsDisplayText.getLocalBounds();
    submenu.settingsDisplayText.setOrigin(settingsTitleBounds.width / 2.f, settingsTitleBounds.height / 2.f);
    submenu.settingsDisplayText.setPosition(centerX, 40.f);
    submenu.settingsDisplayText.setFillColor(sf::Color::White);

    submenu.settingsMenuBackButton.setFont(font);
    submenu.settingsMenuBackButton.setString("<back");
    submenu.settingsMenuBackButton.setCharacterSize(50.f);
    submenu.settingsMenuBackButton.setPosition(20.f, 20.f);
    submenu.settingsMenuBackButton.setFillColor(sf::Color::White);

    submenu.masterVolume.setFont(font);
    submenu.masterVolume.setString("master volume");
    submenu.masterVolume.setCharacterSize(40.f);
    submenu.masterVolume.setPosition(centerX - 300.f, 300.f);
    submenu.masterVolume.setFillColor(sf::Color::White);

    submenu.musicVolume.setFont(font);
    submenu.musicVolume.setString("music volume");
    submenu.musicVolume.setCharacterSize(40.f);
    submenu.musicVolume.setPosition(centerX - 300.f, 400.f);
    submenu.musicVolume.setFillColor(sf::Color::White);

    submenu.resetToDefault.setFont(font);
    submenu.resetToDefault.setString("reset to default");
    submenu.resetToDefault.setCharacterSize(35.f);
    sf::FloatRect resetBounds = submenu.resetToDefault.getLocalBounds();
    submenu.resetToDefault.setOrigin(resetBounds.width / 2.f, resetBounds.height / 2.f);
    submenu.resetToDefault.setPosition(centerX, 720.f);
    submenu.resetToDefault.setFillColor(sf::Color(255, 100, 100));

    float sliderBarWidth = 300.f;
    float sliderBarHeight = 8.f;
    float knobRadius = 10.f;
    float sliderStartX = centerX + 30.f;
    float masterSliderY = 325.f;
    float musicSliderY = 425.f;
    
    submenu.masterVolumeSliderBar.setSize({ sliderBarWidth, sliderBarHeight });
    submenu.masterVolumeSliderBar.setFillColor(sf::Color(150, 150, 150));
    submenu.masterVolumeSliderBar.setPosition(sliderStartX, masterSliderY);

    submenu.masterVolumeKnob.setRadius(knobRadius);
    submenu.masterVolumeKnob.setOrigin(knobRadius, knobRadius);
    submenu.masterVolumeKnob.setFillColor(sf::Color::White);
    submenu.masterVolumeKnob.setPosition(sliderStartX + sliderBarWidth, masterSliderY + sliderBarHeight / 2.f);

    submenu.masterVolumeValueText.setFont(font);
    submenu.masterVolumeValueText.setCharacterSize(24.f);
    submenu.masterVolumeValueText.setFillColor(sf::Color::White);
    submenu.masterVolumeValueText.setOutlineColor(sf::Color::Black);
    submenu.masterVolumeValueText.setOutlineThickness(1.f);
    submenu.masterVolumeValueText.setString("100%");
    submenu.masterVolumeValueText.setPosition(sliderStartX + sliderBarWidth + 20.f, masterSliderY - 10.f);

    submenu.musicVolumeSliderBar.setSize({ sliderBarWidth, sliderBarHeight });
    submenu.musicVolumeSliderBar.setFillColor(sf::Color(150, 150, 150));
    submenu.musicVolumeSliderBar.setPosition(sliderStartX, musicSliderY);

    submenu.musicVolumeKnob.setRadius(knobRadius);
    submenu.musicVolumeKnob.setOrigin(knobRadius, knobRadius);
    submenu.musicVolumeKnob.setFillColor(sf::Color::White);
    submenu.musicVolumeKnob.setPosition(sliderStartX + sliderBarWidth, musicSliderY + sliderBarHeight / 2.f);

    submenu.musicVolumeValueText.setFont(font);
    submenu.musicVolumeValueText.setCharacterSize(24.f);
    submenu.musicVolumeValueText.setFillColor(sf::Color::White);
    submenu.musicVolumeValueText.setOutlineColor(sf::Color::Black);
    submenu.musicVolumeValueText.setOutlineThickness(1.f);
    submenu.musicVolumeValueText.setString("100%");
    submenu.musicVolumeValueText.setPosition(sliderStartX + sliderBarWidth + 20.f, musicSliderY - 10.f);

    // worldmappause menu items
    submenu.worldMapPauseMenuContinueButton.setFont(font);
    submenu.worldMapPauseMenuContinueButton.setString("continue");
    submenu.worldMapPauseMenuContinueButton.setCharacterSize(50.f);
    submenu.worldMapPauseMenuContinueButton.setPosition(400.f, 200.f);
    submenu.worldMapPauseMenuContinueButton.setFillColor(sf::Color::White);
    submenu.worldMapPauseMenuContinueButton.setOutlineColor(sf::Color::Black);
    submenu.worldMapPauseMenuContinueButton.setOutlineThickness(2.f);

    submenu.worldMapPauseMenuReturnToMainMenuButton.setFont(font);
    submenu.worldMapPauseMenuReturnToMainMenuButton.setString("return to main menu");
    submenu.worldMapPauseMenuReturnToMainMenuButton.setCharacterSize(50.f);
    submenu.worldMapPauseMenuReturnToMainMenuButton.setPosition(400.f, 500.f);
    submenu.worldMapPauseMenuReturnToMainMenuButton.setFillColor(sf::Color::White);
    submenu.worldMapPauseMenuReturnToMainMenuButton.setOutlineColor(sf::Color::Black);
    submenu.worldMapPauseMenuReturnToMainMenuButton.setOutlineThickness(2.f);

    submenu.worldMapPauseMenuSettingsButton.setFont(font);
    submenu.worldMapPauseMenuSettingsButton.setString("settings");
    submenu.worldMapPauseMenuSettingsButton.setCharacterSize(50.f);
    submenu.worldMapPauseMenuSettingsButton.setPosition(400.f, 400.f);
    submenu.worldMapPauseMenuSettingsButton.setFillColor(sf::Color::White);
    submenu.worldMapPauseMenuSettingsButton.setOutlineColor(sf::Color::Black);
    submenu.worldMapPauseMenuSettingsButton.setOutlineThickness(2.f);

    submenu.worldMapPauseMenuAchievementsButton.setFont(font);
    submenu.worldMapPauseMenuAchievementsButton.setString("achievements");
    submenu.worldMapPauseMenuAchievementsButton.setCharacterSize(50.f);
    submenu.worldMapPauseMenuAchievementsButton.setPosition(400.f, 300.f);
    submenu.worldMapPauseMenuAchievementsButton.setFillColor(sf::Color::White);
    submenu.worldMapPauseMenuAchievementsButton.setOutlineColor(sf::Color::Black);
    submenu.worldMapPauseMenuAchievementsButton.setOutlineThickness(2.f);

    // levelpause menu items
    submenu.levelPauseMenuContinueButton.setFont(font);
    submenu.levelPauseMenuContinueButton.setString("continue");
    submenu.levelPauseMenuContinueButton.setCharacterSize(50.f);
    submenu.levelPauseMenuContinueButton.setPosition(400.f, 250.f);
    submenu.levelPauseMenuContinueButton.setFillColor(sf::Color::White);
    submenu.levelPauseMenuContinueButton.setOutlineColor(sf::Color::Black);
    submenu.levelPauseMenuContinueButton.setOutlineThickness(2.f);

    submenu.levelPauseMenuReturnToWorldMapButton.setFont(font);
    submenu.levelPauseMenuReturnToWorldMapButton.setString("return to world map");
    submenu.levelPauseMenuReturnToWorldMapButton.setCharacterSize(50.f);
    submenu.levelPauseMenuReturnToWorldMapButton.setPosition(400.f, 350.f);
    submenu.levelPauseMenuReturnToWorldMapButton.setFillColor(sf::Color::White);
    submenu.levelPauseMenuReturnToWorldMapButton.setOutlineColor(sf::Color::Black);
    submenu.levelPauseMenuReturnToWorldMapButton.setOutlineThickness(2.f);

    submenu.levelPauseMenuSettingsButton.setFont(font);
    submenu.levelPauseMenuSettingsButton.setString("settings");
    submenu.levelPauseMenuSettingsButton.setCharacterSize(50.f);
    submenu.levelPauseMenuSettingsButton.setPosition(400.f, 450.f);
    submenu.levelPauseMenuSettingsButton.setFillColor(sf::Color::White);
    submenu.levelPauseMenuSettingsButton.setOutlineColor(sf::Color::Black);
    submenu.levelPauseMenuSettingsButton.setOutlineThickness(2.f);


    // main menu items
    for (int i = 0; i < 3; ++i) {
        SaveFileUIElements elem;

        elem.fileOption.setFont(font);
        elem.fileOption.setString("file " + to_string(i + 1));
        elem.fileOption.setCharacterSize(80.f);
        elem.fileOption.setPosition(300.f, 300.f + i * 80.f);

        elem.worldsStat.setFont(font);
        elem.worldsStat.setCharacterSize(50.f);
        elem.worldsStat.setPosition(1000.f, 302.f + i * 80.f);

        elem.orbsStat.setFont(font);
        elem.orbsStat.setCharacterSize(30.f);
        elem.orbsStat.setPosition(1000.f, 360.f + i * 80.f);

        elem.achievementsStat.setFont(font);
        elem.achievementsStat.setCharacterSize(30.f);
        elem.achievementsStat.setPosition(1300.f, 360.f + i * 80.f);

        submenu.saveFileElements.push_back(elem);
    }
}

void UIContext::updateSaveFileText() {
    for (int i = 0; i < submenu.saveFileElements.size(); ++i) {
        string filename = "saves/save" + to_string(i + 1) + ".json";
        SaveData data;
        bool loaded = SaveManager::loadFromFile(data, filename);

        auto& elem = submenu.saveFileElements[i];
        if (!loaded) {
            elem.fileOption.setString("{null}");
            elem.worldsStat.setString("no data");
            elem.orbsStat.setString("");
            elem.achievementsStat.setString("");
        }
        else {
            elem.fileOption.setString(data.saveName);
            int count = 0, orb = 0;
            for (int j = 0; j < 5; ++j) {
                if (data.worldsUnlocked[j]) {
                    ++count;
                }
            }
            for (int i = 0; i < data.universalTotalOrbsCollected.size(); i++) {
                if (data.universalTotalOrbsCollected[i]) {
                    orb++;
                }
            }
            elem.worldsStat.setString("book " + to_string(count));
            elem.orbsStat.setString("total orbs collected: " + to_string(orb));
            elem.achievementsStat.setString("achievements: " + to_string(data.totalAchievementsUnlocked));
        }
    }
}

void UIContext::updateHoverState(int selected, sf::Vector2f mousePos, bool confirmEnabled) {
    if (currentState == UIState::Splash) {
        submenu.splashStartButton.setFillColor((selected == 1) ? sf::Color::Yellow : sf::Color::White);
        submenu.splashQuitButton.setFillColor((selected == 2) ? sf::Color::Yellow : sf::Color::White);
    }

    if (currentState == UIState::MainMenu) {
        for (int i = 0; i < submenu.saveFileElements.size(); ++i) {
            auto& elem = submenu.saveFileElements[i];
            sf::Color color = (i == selected) ? sf::Color::Yellow : sf::Color::White;
            elem.fileOption.setFillColor(color);
            elem.worldsStat.setFillColor(color);
            elem.orbsStat.setFillColor(color);
            elem.achievementsStat.setFillColor(color);
        }
        mainMenuBackButton.setFillColor((selected == 100) ? sf::Color::Yellow : sf::Color::White);
    }

    if (currentState == UIState::NewSaveSubmenu) {
        sf::Color cancelColor = (selected == 98) ? sf::Color::Yellow : sf::Color::White;
        submenu.cancelNewSaveCreationButton.setFillColor(cancelColor);

        if (confirmEnabled) {
            sf::Color confirmColor = (selected == 99) ? sf::Color::Yellow : sf::Color::White;
            submenu.confirmNewSaveCreationButton.setFillColor(confirmColor);
        }
        else {
            submenu.confirmNewSaveCreationButton.setFillColor(sf::Color(150, 150, 150)); // grayed out
        }
    }

    if (currentState == UIState::ExistingSaveSubmenu) {
        submenu.cancelLoadSelectedSaveButton.setFillColor((selected == 95) ? sf::Color::Yellow : sf::Color::White);
        submenu.continueSelectedSaveButton.setFillColor((selected == 97) ? sf::Color::Yellow : sf::Color::White);
        submenu.deleteSelectedSaveButton.setFillColor((selected == 96) ? sf::Color::Yellow : sf::Color::White);
    }
}

void UIContext::draw(sf::RenderWindow& window) {
    if (currentState == UIState::Splash) {
        window.draw(submenu.splashTitle);
        window.draw(submenu.splashSmile);
        window.draw(submenu.splashStartButton);
        window.draw(submenu.splashQuitButton);
    }
    else if (currentState == UIState::MainMenu) {
        for (auto& elem : submenu.saveFileElements) {
            window.draw(elem.fileOption);
            window.draw(elem.worldsStat);
            window.draw(elem.orbsStat);
            window.draw(elem.achievementsStat);
        }
        window.draw(mainMenuBackButton);
    }
    else if (currentState == UIState::NewSaveSubmenu) {
        window.draw(submenu.newSaveNameLabel);
        window.draw(submenu.inputNameForNewSaveBox);
        window.draw(submenu.inputNameForNewSaveText);
        window.draw(submenu.confirmNewSaveCreationButton);
        window.draw(submenu.cancelNewSaveCreationButton);
    }
    else if (currentState == UIState::ExistingSaveSubmenu) {
        window.draw(submenu.cancelLoadSelectedSaveButton);
        window.draw(submenu.existingSaveNameLabel);
        window.draw(submenu.continueSelectedSaveButton);
        window.draw(submenu.deleteSelectedSaveButton);
    }
    else if (currentState == UIState::WorldMapPauseMenu) {
        window.draw(submenu.worldMapPauseMenuContinueButton);
        window.draw(submenu.worldMapPauseMenuReturnToMainMenuButton);
        window.draw(submenu.worldMapPauseMenuSettingsButton);
        window.draw(submenu.worldMapPauseMenuAchievementsButton);
    }
    else if (currentState == UIState::LevelPauseMenu) {
        window.draw(submenu.levelPauseMenuContinueButton);
        window.draw(submenu.levelPauseMenuReturnToWorldMapButton);
        window.draw(submenu.levelPauseMenuSettingsButton);
    }
    else if (currentState == UIState::Settings) {
        window.draw(submenu.settingsDisplayText);
        window.draw(submenu.settingsMenuBackButton);
        window.draw(submenu.masterVolume);
        window.draw(submenu.musicVolume);
        window.draw(submenu.resetToDefault);
        window.draw(submenu.masterVolumeSliderBar);
        window.draw(submenu.masterVolumeKnob);
        window.draw(submenu.masterVolumeValueText);
        window.draw(submenu.musicVolumeSliderBar);
        window.draw(submenu.musicVolumeKnob);
        window.draw(submenu.musicVolumeValueText);
    }
    else if (currentState == UIState::Achievements) {
        SaveData data = getCurrentSaveData();
        window.draw(submenu.achievementsDisplayText);
        window.draw(submenu.achievementsMenuBackButton);
        if (submenu.currentAchievementsPage > 0)
            window.draw(submenu.achievementsMenuLeftArrow);
        if (submenu.currentAchievementsPage < 4)
            window.draw(submenu.achievementsMenuRightArrow);
        int count = 0;
        for (int i = 0; i < data.universalTotalOrbsCollected.size(); i++) {
            if (data.universalTotalOrbsCollected.at(i)) {
                count++;
            }
        }
        submenu.achievementsMenuTotalOrbsCountDisplay.setString("orbs collected: " + to_string(count) + "/" + 
            to_string(data.universalTotalOrbsExisting));

        int startIdx = submenu.currentAchievementsPage * 6;
        int endIdx = min(startIdx + 6, 30);

        for (const auto& orb : submenu.book1Orbs) window.draw(orb);
        for (const auto& orb : submenu.book2Orbs) window.draw(orb);
        for (const auto& orb : submenu.book3Orbs) window.draw(orb);
        for (const auto& orb : submenu.book4Orbs) window.draw(orb);
        for (const auto& orb : submenu.book5Orbs) window.draw(orb);

        window.draw(submenu.book1Label);
        window.draw(submenu.book2Label);
        window.draw(submenu.book3Label);
        window.draw(submenu.book4Label);
        window.draw(submenu.book5Label);

        window.draw(submenu.achievementsMenuTotalOrbsCountDisplay);

        for (int i = startIdx; i < endIdx; ++i) {
            const bool unlocked = data.achievementsUnlocked.at(i);

            if (unlocked) {
                submenu.achievementBoxes[i].setFillColor(sf::Color(255, 255, 255, 150));  // bright white transparent
                submenu.achievementTitles[i].setFillColor(sf::Color::White);
                submenu.achievementDescriptions[i].setFillColor(sf::Color(230, 230, 230));
            }
            else {
                submenu.achievementBoxes[i].setFillColor(sf::Color(50, 50, 50, 100));  // gray, low alpha
                submenu.achievementTitles[i].setFillColor(sf::Color(120, 120, 120));
                submenu.achievementDescriptions[i].setFillColor(sf::Color(100, 100, 100));
            }

            window.draw(submenu.achievementBoxes[i]);
            window.draw(submenu.achievementTitles[i]);
            window.draw(submenu.achievementDescriptions[i]);
        }
    }
}


SaveData& UIContext::getCurrentSaveData() {
    return currentSaveData;
}

string UIContext::getCurrentFilename() {
    return "saves/save" + to_string(submenu.selectedSaveFileIndex + 1) + ".json";
}

bool UIContext::loadSaveConfirmed() {
    return confirmInCreateNewSaveClicked;
}

void UIContext::setCurrentSaveData(const SaveData& data) {
    currentSaveData = data;
}

void UIContext::setLevelID(int worldIndex, int levelIndex) {
    currentWorldIndex = worldIndex;
    currentLevelIndex = levelIndex;
}

int UIContext::getWorldIndex() const {
    return currentWorldIndex;
}

int UIContext::getLevelIndex() const {
    return currentLevelIndex;
}

