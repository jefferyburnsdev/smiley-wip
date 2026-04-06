#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Menus.h"
#include "CutscenePlayer.h"
#include "SaveManager.h"
#include "WorldMap.h"
#include "AudioManager.h"

using namespace std;

void handleMainMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font) {
    // quit via escape key
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            ctx.currentState = UIState::Splash;
            return;
        }
    }

    // identify which file is being hovered over for hover highlight call
    if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool hoveredAny = false;

        for (int i = 0; i < ctx.submenu.saveFileElements.size(); ++i) {
            const auto& elem = ctx.submenu.saveFileElements[i];
            if (elem.fileOption.getGlobalBounds().contains(mousePos)) {
                ctx.submenu.hoveredSaveFileIndex = i;
                hoveredAny = true;

                if (event.type == sf::Event::MouseButtonPressed) {
                    ctx.submenu.selectedSaveFileIndex = i;
                    string filename = ctx.getCurrentFilename();
                    if (SaveManager::isSaveFileEmpty(filename)) {
                        ctx.currentState = UIState::NewSaveSubmenu;
                    }
                    else {
                        ctx.currentState = UIState::ExistingSaveSubmenu;
                    }
                }
                break;
            }
        }

        if (!hoveredAny && ctx.mainMenuBackButton.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.hoveredSaveFileIndex = 100;
            hoveredAny = true;

            if (event.type == sf::Event::MouseButtonPressed) {
                ctx.currentState = UIState::Splash;
            }
        }

        if (!hoveredAny) {
            ctx.submenu.hoveredSaveFileIndex = -1;
        }
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    ctx.updateHoverState(ctx.submenu.hoveredSaveFileIndex, mousePos);

    window.clear();
    ctx.draw(window);  // call UIContext::draw() to render function elements
    window.display();
}

void handleNewSaveEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font, CutscenePlayer& cs) {
    sf::FloatRect boxBounds = ctx.submenu.inputNameForNewSaveBox.getGlobalBounds();
    sf::Vector2f boxPos(boxBounds.left, boxBounds.width);

    // user types input event
    if (event.type == sf::Event::TextEntered) {
        sf::FloatRect textBounds = ctx.submenu.inputNameForNewSaveText.getGlobalBounds();
        sf::Vector2f textPos(textBounds.left, textBounds.width);

        if (event.text.unicode >= 32 && event.text.unicode <= 126 && textBounds.left + textBounds.width + 15.f < boxBounds.left + boxBounds.width - 15.f) {
            ctx.submenu.typedInputForSaveFileName += static_cast<char>(event.text.unicode);
        }
        else if (event.text.unicode == 8 && !ctx.submenu.typedInputForSaveFileName.empty()) { // backspace
            ctx.submenu.typedInputForSaveFileName.pop_back();
        }
        ctx.submenu.inputNameForNewSaveText.setString(ctx.submenu.typedInputForSaveFileName);
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool confirmEnabled = !ctx.submenu.typedInputForSaveFileName.empty();
    int selected = -1;

    // user pressed escape while in createnewsave submenu
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            ctx.currentState = UIState::MainMenu;
            ctx.submenu.typedInputForSaveFileName.clear();
            ctx.submenu.inputNameForNewSaveText.setString("");
            return;
        }
    }

    // user clicked confirm button in createnewsave submenu
    if (ctx.submenu.confirmNewSaveCreationButton.getGlobalBounds().contains(mousePos) && confirmEnabled) {
        selected = 99;
        if (event.type == sf::Event::MouseButtonPressed && confirmEnabled) {
            MusicManager::getInstance().startFadeOut(2.f);
            SaveData data;

            data.saveName = ctx.submenu.typedInputForSaveFileName;
            data.worldsUnlocked[0] = false;
            
            ctx.setCurrentSaveData(data);
            SaveManager::saveToFile(ctx.getCurrentSaveData(), ctx.getCurrentFilename());
            ctx.updateSaveFileText();
            ctx.submenu.typedInputForSaveFileName.clear();
            ctx.submenu.inputNameForNewSaveText.setString("");

            ctx.currentState = UIState::Cutscene;
            cs.play(ctx);

            return;
        }
    }

    // user clicked cancel button in createnewsave submenu
    if (ctx.submenu.cancelNewSaveCreationButton.getGlobalBounds().contains(mousePos)) {
        selected = 98;
        if (event.type == sf::Event::MouseButtonPressed) {
            ctx.currentState = UIState::MainMenu;
            ctx.submenu.typedInputForSaveFileName.clear();
            ctx.submenu.inputNameForNewSaveText.setString("");
            return;
        }
    }
    ctx.updateHoverState(selected, mousePos, confirmEnabled);

    window.clear();
    ctx.draw(window);  // call UIContext::draw() to render function elements
    window.display();
}

void handleExistingSaveEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font, CutscenePlayer& cs, WorldMap& m) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    int selected = -1;

    // user pressed cancel via escape key in loadsave submenu
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            ctx.currentState = UIState::MainMenu;
            return;
        }
    }

    // user pressed continue button in loadsave submenu
    if (ctx.submenu.continueSelectedSaveButton.getGlobalBounds().contains(mousePos)) {
        selected = 97;
        if (event.type == sf::Event::MouseButtonPressed) {
            MusicManager::getInstance().startFadeOut(2.f);
            SaveData data;
            int i = ctx.submenu.selectedSaveFileIndex; // choose fileindex for save file
            string filename = ctx.getCurrentFilename();
            if (SaveManager::loadFromFile(data, filename)) {
                ctx.setCurrentSaveData(data);
                
                if (!data.worldsUnlocked[0]) { // if cutscene never completed upon save creation
                    data.lastWorldMap = 1;
                    SaveManager::saveToFile(data, filename); // persist the update

                    ctx.setCurrentSaveData(data);

                    ctx.currentState = UIState::Cutscene;
                    cs.play(ctx);
                    return;
                }
                else {
                    ctx.setCurrentSaveData(data);
                    int worldCount = 1;
                    for (int i = 0; i < data.worldsUnlocked.size() - 1; i++) {
                        if (data.worldsUnlocked[i] == true) worldCount++;
                    }
                    m.load("assets/images/grandworldmap.png", data);
                    SaveManager::saveToFile(data, filename); // persist the update
                    ctx.currentState = UIState::WorldMap;
                }
            }
            else {
                cerr << "Failed to load save file: " << filename << "\n";
            }

        }
    }

    // user pressed delete button in loadsave submenu
    if (ctx.submenu.deleteSelectedSaveButton.getGlobalBounds().contains(mousePos)) {
        selected = 96;
        if (event.type == sf::Event::MouseButtonPressed) {
            SaveManager::deleteSaveData(ctx.submenu.selectedSaveFileIndex);

            ctx.updateSaveFileText();
            ctx.currentState = UIState::MainMenu;
        }
    }

    // user pressed cancel button in loadsave submenu
    if (ctx.submenu.cancelLoadSelectedSaveButton.getGlobalBounds().contains(mousePos)) {
        selected = 95;
        if (event.type == sf::Event::MouseButtonPressed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            ctx.currentState = UIState::MainMenu;
        }
    }
    ctx.updateHoverState(selected, mousePos);

    window.clear();
    ctx.draw(window);  // call UIContext::draw() to render function elements
    window.display();
}

int handleSplashEvents(sf::Event event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font) {
    // handle events
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    int selected = -1;

    // start button
    if (ctx.submenu.splashStartButton.getGlobalBounds().contains(mousePos)) {
        selected = 1;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            ctx.currentState = UIState::MainMenu;
            return selected;
        }
    }

    // quit button
    if (ctx.submenu.splashQuitButton.getGlobalBounds().contains(mousePos)) {
        selected = 2;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            window.close();
            return selected;
        }
    }

    // quit via escape key
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        window.close();
        return 0;
    }

    // hover highlight call
    ctx.updateHoverState(selected, mousePos);

    window.clear();
    ctx.draw(window);  // call UIContext::draw() to render function elements
    window.display();

    return 0;
}

void handleWorldMapPauseMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto highlight = [&](sf::Text& text) {
        text.setFillColor(text.getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
        };

    highlight(ctx.submenu.worldMapPauseMenuContinueButton);
    highlight(ctx.submenu.worldMapPauseMenuSettingsButton);
    highlight(ctx.submenu.worldMapPauseMenuAchievementsButton);
    highlight(ctx.submenu.worldMapPauseMenuReturnToMainMenuButton);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (ctx.submenu.worldMapPauseMenuContinueButton.getGlobalBounds().contains(mousePos)) {
            ctx.currentState = UIState::WorldMap;
        }
        else if (ctx.submenu.worldMapPauseMenuSettingsButton.getGlobalBounds().contains(mousePos)) {
            ctx.currentState = UIState::Settings;
        }
        else if (ctx.submenu.worldMapPauseMenuAchievementsButton.getGlobalBounds().contains(mousePos)) {
            ctx.currentState = UIState::Achievements;
        }
        else if (ctx.submenu.worldMapPauseMenuReturnToMainMenuButton.getGlobalBounds().contains(mousePos)) {
            MusicManager::getInstance().startFadeOut(1.f);
            MusicManager::getInstance().playMusic("assets/music/smiley_theme.ogg", 50.f, true);

            ctx.currentState = UIState::MainMenu;
        }
    }
}

void handleLevelPauseMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

}

void handleSettingsMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font, int menuCase) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed) {
        if (ctx.submenu.settingsMenuBackButton.getGlobalBounds().contains(mousePos)) {
            SaveManager::saveToFile(ctx.getCurrentSaveData(), ctx.getCurrentFilename());
            ctx.currentState = (menuCase == 1) ? UIState::WorldMapPauseMenu : UIState::LevelPauseMenu;
        }

        if (ctx.submenu.masterVolumeKnob.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.draggingMasterVol = true;
        }

        if (ctx.submenu.musicVolumeKnob.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.draggingMusicVol = true;
        }

        if (ctx.submenu.resetToDefault.getGlobalBounds().contains(mousePos)) {
            float sliderStartX = ctx.submenu.masterVolumeSliderBar.getPosition().x;
            float sliderBarWidth = ctx.submenu.masterVolumeSliderBar.getSize().x;
            float knobX = sliderStartX + 1.f * sliderBarWidth;

            ctx.submenu.masterVolumeKnob.setPosition(knobX, ctx.submenu.masterVolumeKnob.getPosition().y);
            ctx.submenu.masterVolumeValueText.setString("100%");
            MusicManager::getInstance().setMasterVol(100.f);

            ctx.submenu.musicVolumeKnob.setPosition(knobX, ctx.submenu.musicVolumeKnob.getPosition().y);
            ctx.submenu.musicVolumeValueText.setString("100%");
            MusicManager::getInstance().setMusicVol(100.f);
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (ctx.submenu.settingsMenuBackButton.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.settingsMenuBackButton.setFillColor(sf::Color::Yellow);
        }
        else {
            ctx.submenu.settingsMenuBackButton.setFillColor(sf::Color::White);
        }

        if (ctx.submenu.resetToDefault.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.resetToDefault.setFillColor(sf::Color(255, 100, 100));
        }
        else {
            ctx.submenu.resetToDefault.setFillColor(sf::Color::White);
        }

        if (ctx.submenu.masterVolumeKnob.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.masterVolumeKnob.setFillColor(sf::Color(255, 255, 100));
            ctx.submenu.masterVolumeKnob.setScale(1.2f, 1.2f);
        }
        else if (!ctx.submenu.draggingMasterVol) {
            ctx.submenu.masterVolumeKnob.setFillColor(sf::Color::White);
            ctx.submenu.masterVolumeKnob.setScale(1.f, 1.f);
        }

        if (ctx.submenu.musicVolumeKnob.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.musicVolumeKnob.setFillColor(sf::Color(255, 255, 100));
            ctx.submenu.musicVolumeKnob.setScale(1.2f, 1.2f);
        }
        else if (!ctx.submenu.draggingMusicVol) {
            ctx.submenu.musicVolumeKnob.setFillColor(sf::Color::White);
            ctx.submenu.musicVolumeKnob.setScale(1.f, 1.f);
        }

        float sliderStartX = ctx.submenu.masterVolumeSliderBar.getPosition().x;
        float sliderBarWidth = ctx.submenu.masterVolumeSliderBar.getSize().x;

        if (ctx.submenu.draggingMasterVol) {
            float clampedX = std::clamp(mousePos.x, sliderStartX, sliderStartX + sliderBarWidth);
            ctx.submenu.masterVolumeKnob.setPosition(clampedX, ctx.submenu.masterVolumeKnob.getPosition().y);

            float percent = (clampedX - sliderStartX) / sliderBarWidth;
            int volumeValue = static_cast<int>(percent * 100);
            ctx.submenu.masterVolumeValueText.setString(std::to_string(volumeValue) + "%");

            MusicManager::getInstance().setMasterVol(static_cast<float>(volumeValue));
            ctx.getCurrentSaveData().sfxvolume = static_cast<float>(volumeValue);
        }

        if (ctx.submenu.draggingMusicVol) {
            float clampedX = std::clamp(mousePos.x, sliderStartX, sliderStartX + sliderBarWidth);
            ctx.submenu.musicVolumeKnob.setPosition(clampedX, ctx.submenu.musicVolumeKnob.getPosition().y);

            float percent = (clampedX - sliderStartX) / sliderBarWidth;
            int volumeValue = static_cast<int>(percent * 100);
            ctx.submenu.musicVolumeValueText.setString(std::to_string(volumeValue) + "%");

            MusicManager::getInstance().setMusicVol(static_cast<float>(volumeValue));
            ctx.getCurrentSaveData().musicvolume = static_cast<float>(volumeValue);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        ctx.submenu.draggingMasterVol = false;
        ctx.submenu.draggingMusicVol = false;
    }

    if (event.type == sf::Event::MouseMoved) {
        float sliderStartX = ctx.submenu.masterVolumeSliderBar.getPosition().x;
        float sliderBarWidth = ctx.submenu.masterVolumeSliderBar.getSize().x;

        if (ctx.submenu.draggingMasterVol) {
            float clampedX = clamp(mousePos.x, sliderStartX, sliderStartX + sliderBarWidth);
            ctx.submenu.masterVolumeKnob.setPosition(clampedX, ctx.submenu.masterVolumeKnob.getPosition().y);

            float percent = (clampedX - sliderStartX) / sliderBarWidth;
            int volumeValue = static_cast<int>(percent * 100.f);
            ctx.submenu.masterVolumeValueText.setString(to_string(volumeValue) + "%");

            MusicManager::getInstance().setMasterVol(static_cast<float>(volumeValue));
        }

        if (ctx.submenu.draggingMusicVol) {
            float clampedX = clamp(mousePos.x, sliderStartX, sliderStartX + sliderBarWidth);
            ctx.submenu.musicVolumeKnob.setPosition(clampedX, ctx.submenu.musicVolumeKnob.getPosition().y);

            float percent = (clampedX - sliderStartX) / sliderBarWidth;
            int volumeValue = static_cast<int>(percent * 100.f);
            ctx.submenu.musicVolumeValueText.setString(to_string(volumeValue) + "%");

            MusicManager::getInstance().setMusicVol(static_cast<float>(volumeValue));
        }
    }
}


void handleAchievementsMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (ctx.submenu.achievementsMenuRightArrow.getGlobalBounds().contains(mousePos)) {
            if (ctx.submenu.currentAchievementsPage < 4)  // 5 pages, 6 achievements per page
                ctx.submenu.currentAchievementsPage++;
        }

        if (ctx.submenu.achievementsMenuLeftArrow.getGlobalBounds().contains(mousePos)) {
            if (ctx.submenu.currentAchievementsPage > 0)
                ctx.submenu.currentAchievementsPage--;
        }

        if (ctx.submenu.achievementsMenuBackButton.getGlobalBounds().contains(mousePos)) {
            ctx.currentState = UIState::WorldMapPauseMenu;
        }
    }

    // hover highlight logic for arrows
    else if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (ctx.submenu.currentAchievementsPage > 0 && ctx.submenu.achievementsMenuLeftArrow.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.achievementsMenuLeftArrow.setFillColor(sf::Color::Yellow);
        }
        else {
            ctx.submenu.achievementsMenuLeftArrow.setFillColor(sf::Color::White);
        }

        if (ctx.submenu.currentAchievementsPage < 4 && ctx.submenu.achievementsMenuRightArrow.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.achievementsMenuRightArrow.setFillColor(sf::Color::Yellow);
        }
        else {
            ctx.submenu.achievementsMenuRightArrow.setFillColor(sf::Color::White);
        }

        if (ctx.submenu.achievementsMenuBackButton.getGlobalBounds().contains(mousePos)) {
            ctx.submenu.achievementsMenuBackButton.setFillColor(sf::Color::Yellow);
        }
        else {
            ctx.submenu.achievementsMenuBackButton.setFillColor(sf::Color::White);
        }
    }
}
