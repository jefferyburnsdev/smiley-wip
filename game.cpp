#include <SFML/Graphics.hpp>
#include "UIContext.h"
#include "AudioManager.h"
#include "World1.h"
#include "Menus.h"
#include "CutscenePlayer.h"
#include "WorldMap.h"
#include "SaveManager.h"
#include "LevelManager.h"
#include <iostream>

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 800), "Smiley");

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/chiller.ttf")) {
        cerr << "Failed to load font\n";
        return -1;
    }

    // load splash menu
    // load music
    string musicLoaded = "assets/music/smiley_theme.ogg";
    float musicVol = 50.f;
    bool musicLoop = true;
    MusicManager::getInstance().playMusic(musicLoaded, musicVol, musicLoop);
    sf::Clock deltaClock;

    // run ui logic, declare objs
    UIContext ctx(font);
    CutscenePlayer cs;
    WorldMap m;
    LevelManager lm;
    SaveData data;
    m.load("assets/images/grandworldmap.png", data);
    int menuCase = 1;
    bool loaded = false;

    ctx.currentState = UIState::Splash;
    ctx.updateSaveFileText();

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        MusicManager::getInstance().updateFadeOut(deltaTime);
        if (ctx.currentState == UIState::Level) {
            lm.update(deltaTime);

            if (lm.isLevelComplete()) {
                ctx.currentState = UIState::WorldMap;
                MusicManager::getInstance().playMusic("assets/music/world_map_music.ogg", 60.f, true);
            }
        }

        if (ctx.currentState != UIState::Level) {
            loaded = false;
        }

        sf::Event event;

        // poll events for uistates
        while (window.pollEvent(event)) {
            window.setFramerateLimit(60);

            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }

            switch (ctx.currentState) {
            case UIState::Splash:
                handleSplashEvents(event, ctx, window, font);
                break;
            case UIState::MainMenu:
                handleMainMenuEvents(event, ctx, window, font);
                break;
            case UIState::NewSaveSubmenu:
                handleNewSaveEvents(event, ctx, window, font, cs);
                break;
            case UIState::ExistingSaveSubmenu:
                handleExistingSaveEvents(event, ctx, window, font, cs, m);
                break;
            case UIState::Cutscene:
                cs.update(ctx, m);
                break;
            case UIState::WorldMap:
                MusicManager::getInstance().playMusic("assets/music/world_map_music.ogg", 60.f, true);
                menuCase = 1;
                if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    data = ctx.getCurrentSaveData();
                    m.update(mousePos, data);
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    m.handleLevelClicks(mousePos, ctx, data);
                    m.handleClickWorldNavArrows(mousePos);

                    if (m.worldMapPauseButton.getGlobalBounds().contains(mousePos)) {
                        ctx.currentState = UIState::WorldMapPauseMenu;
                    }
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    ctx.currentState = UIState::WorldMapPauseMenu;
                }
                break;
            case UIState::WorldMapPauseMenu:
                handleWorldMapPauseMenuEvents(event, ctx, window, font);
                break;
            case UIState::Level:
                if (!loaded) {
                    lm.loadLevel(ctx, ctx.getCurrentSaveData());
                    loaded = true;
                }
                window.setFramerateLimit(120);
                lm.handleEvent(event);
                break;
            case UIState::LevelPauseMenu:
                menuCase = 2;
                handleLevelPauseMenuEvents(event, ctx, window, font);
                break;
            case UIState::Settings:
                handleSettingsMenuEvents(event, ctx, window, font, menuCase);
                break;
            case UIState::Achievements:
                handleAchievementsMenuEvents(event, ctx, window, font);
                break;
            case UIState::Exit:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear(sf::Color::Black);

        if (ctx.currentState == UIState::Cutscene) {
            cs.render(window, ctx);
        }
        else if (ctx.currentState == UIState::WorldMap) {
            m.render(window);
        }
        else if (ctx.currentState == UIState::Level) {
            lm.render(window);
        }
        else {
            ctx.draw(window); // only draw menus/UI when NOT in these states
        }
        window.display();
    }
    return 0;
}
