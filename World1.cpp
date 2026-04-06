#include "World1.h"
#include "AudioManager.h"
#include <iostream>

using namespace std;

World1::World1(int levelIndex, const string& levelName, const string& music) : currentLevelIndex(levelIndex), currentLevelName(levelName), musicPath(music) {}

void World1::load(SaveData& data)
{
    bg.setSize(sf::Vector2f(1600, 800));
    bg.setFillColor(sf::Color(50, 50, 150));
    bg.setPosition(0, 0);

    if (!smileyTexture.loadFromFile("assets/images/sprite.png"))
        cerr << "Failed to load smiley\n";

    currentCharacter.sprite.setTexture(smileyTexture);
    currentCharacter.position = sf::Vector2f(100, 600);
    currentCharacter.sprite.setPosition(currentCharacter.position);

    loadLevelLayout(currentLevelIndex);
}

void World1::loadLevelLayout(int index)
{
    obstacles.clear();

    switch (index)
    {
    case 0:
        loadLevel1();
        break;

    case 1:
        loadLevel2();
        break;

    case 2:
        loadLevel3();
        break;

    case 3:
        loadLevel4();
        break;

    case 4:
        loadLevel5();
        break;

    case 5:
        loadLevel6();
        break;

    case 6:
        loadLevel7();
        break;

    case 7:
        loadLevel8();
        break;

    case 8:
        loadLevel9();
        break;

    case 9:
        loadLevel10();
        break;

    default:
        loadLevel1();
        break;
    }

    currentCharacter.position = spawnPoint;
}


void World1::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space && currentCharacter.isgrounded)
        {
            currentCharacter.velocity.y = -jumpStrength;
            currentCharacter.isgrounded = false;
        }
    }
}


void World1::update(float dt)
{
    handlePlayerMovement(dt);
    handleCollisions();
    updateParallax(currentCharacter.position.x);

    if (currentCharacter.position.x > 1850.f)
        levelComplete = true;
}


void World1::handlePlayerMovement(float dt)
{
    float move = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        move -= moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        move += moveSpeed;

    currentCharacter.velocity.x = move;

    // gravity
    currentCharacter.velocity.y += gravity * dt;

    // move the player
    currentCharacter.position += currentCharacter.velocity * dt;

    // update sprite position
    currentCharacter.sprite.setPosition(currentCharacter.position);
}


void World1::handleCollisions()
{
    currentCharacter.isgrounded = false;

    sf::FloatRect playerBounds = currentCharacter.sprite.getGlobalBounds();

    for (auto& o : obstacles)
    {
        sf::FloatRect obs = o.getBounds();

        if (!playerBounds.intersects(obs))
            continue;

        float overlapLeft = (playerBounds.left + playerBounds.width) - obs.left;
        float overlapRight = (obs.left + obs.width) - playerBounds.left;
        float overlapTop = (playerBounds.top + playerBounds.height) - obs.top;
        float overlapBottom = (obs.top + obs.height) - playerBounds.top;

        float overlapX = std::min(overlapLeft, overlapRight);
        float overlapY = std::min(overlapTop, overlapBottom);

        if (overlapX < overlapY)
        {
            if (overlapLeft < overlapRight)
                currentCharacter.position.x -= overlapLeft;
            else
                currentCharacter.position.x += overlapRight;

            currentCharacter.velocity.x = 0;
        }
        else
        {
            if (overlapTop < overlapBottom)
            {
                currentCharacter.position.y -= overlapTop;
                currentCharacter.velocity.y = 0;
                currentCharacter.isgrounded = true;
            }
            else
            {
                currentCharacter.position.y += overlapBottom;
                currentCharacter.velocity.y = 0;
            }
        }

        playerBounds = currentCharacter.sprite.getGlobalBounds();
    }

    currentCharacter.sprite.setPosition(currentCharacter.position);
}

void World1::render(sf::RenderWindow& window)
{
    window.draw(bg);

    for (auto& o : obstacles)
        o.draw(window);

    window.draw(currentCharacter.sprite);
}

void World1::loadLevel1() {
    // --- Flat ground ---
    obstacles.emplace_back(
        sf::Vector2f(2000, 200),
        sf::Vector2f(0, 700),
        ObstacleType::Ground
    );

    // --- Raised platform ---
    obstacles.emplace_back(
        sf::Vector2f(300, 50),
        sf::Vector2f(500, 550),
        ObstacleType::Platform
    );

    // --- Another ground segment ---
    obstacles.emplace_back(
        sf::Vector2f(500, 200),
        sf::Vector2f(1000, 650),
        ObstacleType::Ground
    );

    // --- Wall the player cannot pass ---
    obstacles.emplace_back(
        sf::Vector2f(50, 400),
        sf::Vector2f(850, 300),
        ObstacleType::Wall
    );

    // --- Trigger at the far right that ends the level ---
    obstacles.emplace_back(
        sf::Vector2f(100, 800),
        sf::Vector2f(1600, 0),
        ObstacleType::Trigger
    );

    spawnPoint = sf::Vector2f(80.f, 600.f);
}

void World1::loadLevel2() {

}

void World1::loadLevel3() {

}

void World1::loadLevel4() {

}

void World1::loadLevel5() {

}

void World1::loadLevel6() {

}

void World1::loadLevel7() {

}

void World1::loadLevel8() {

}

void World1::loadLevel9() {

}

void World1::loadLevel10() {

}
