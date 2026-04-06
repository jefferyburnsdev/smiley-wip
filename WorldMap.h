#pragma once
#include <SFML/Graphics.hpp>
#include "SaveData.h"
#include "uiContext.h"
#include <vector>
#include <string>

using namespace std;

struct LevelRegion {
    unique_ptr<sf::Shape> shape; // irregular-shaped region
    string levelName;
    bool isHovered = false;
    int worldIndex = 0;
    int levelIndexInWorld = 0;
    int globalLevelIndex = 0;
};

class WorldMap {
public:
    SaveData currentData;
    WorldMap();
    sf::Text worldMapPauseButton;
    sf::Text worldName;
    vector<string> worldNames = {
        "book 1: fractured nullspace",
        "book 2: uncanny valley",
        "book 3: bleak mechanica",
        "book 4: reverie threshold",
        "book 5: fading dream"
    };
    bool worldMapMusicStarted = false;

    void load(const string& worldMapTexture, const SaveData& data);
    void update(const sf::Vector2f& mousePos, SaveData& data);
    void render(sf::RenderWindow& window);
    void handleClickWorldNavArrows(const sf::Vector2f& mousePos);
    void handleLevelClicks(const sf::Vector2f& mousePos, UIContext& ctx, SaveData& data);
    sf::Vector2f getShapeCenter(const sf::ConvexShape& shape) const;

    void createRegionsForWorld1();
    void createRegionsForWorld2();
    void createRegionsForWorld3();
    void createRegionsForWorld4();
    void createRegionsForWorld5();

    void goToNextWorld();
    void goToPreviousWorld();

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    int currentWorldIndex = 0;
    const int worldWidth = 1600;  // fixed width per world
    const int screenHeight = 800;

    sf::Text leftArrow, rightArrow;

    vector<vector<LevelRegion>> worlds; // each world has a set of level regions (vector of vectors)
    sf::Font font;
    sf::Text levelNameText;

    void setupNavigationButtons();
    void updateArrows(const sf::Vector2f& mousePos);
};
