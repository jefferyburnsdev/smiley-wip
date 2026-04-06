#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Obstacle.h"
#include "SaveData.h"
#include "Level.h"

using namespace std;

class World5 : public Level {
public:
    World5(int levelID, const string& levelName, const string& musicPath);

    void load(SaveData& saveData);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    bool isComplete() const;

private:
    void handleInput(float deltaTime);
    void applyGravity(float deltaTime);
    void resolveCollisions();

    int levelID;
    string levelName;
    string musicPath;

    SaveData* saveDataRef = nullptr;

    sf::Sprite player;
    sf::Texture playerTexture;

    sf::RectangleShape ground;
    sf::Texture groundTexture;

    sf::Music bgMusic;

    float velocityY = 0.f;
    bool isJumping = false;

    float screenW = 1600.f;
    float screenH = 800.f;
    float moveSpeed = 200.f;
    float jumpVelocity = -500.f;
    float gravity = 1600.f;

    vector<Obstacle> obstacles;

    bool levelCompleted = false;
};
