#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Obstacle.h"
#include "SaveData.h"
#include "Level.h"

using namespace std;

class World1 : public Level {
public:
    World1(int levelID, const string& levelName, const string& musicPath);

    void load(SaveData& data);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);

private:
    int currentLevelIndex;
    std::string currentLevelName;
    std::string musicPath;

    sf::RectangleShape bg;
    sf::Texture playerTexture;
    sf::Sprite currentCharacterSprite;

    vector<Obstacle> obstacles;
    sf::Vector2f spawnPoint;

    void loadLevelLayout(int index);
    void loadLevel1();
    void loadLevel2();
    void loadLevel3();
    void loadLevel4();
    void loadLevel5();
    void loadLevel6();
    void loadLevel7();
    void loadLevel8();
    void loadLevel9();
    void loadLevel10();
    void handlePlayerMovement(float deltaTime);
    void handleCollisions();
};
