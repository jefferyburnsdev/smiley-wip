#include "World2.h"
#include <iostream>

using namespace std;

World2::World2(int id, const string& name, const string& music)
    : levelID(id), levelName(name), musicPath(music) {}

void World2::load(SaveData& saveData) {
    saveDataRef = &saveData;

    if (!playerTexture.loadFromFile("assets/images/sprite.png") ||
        !groundTexture.loadFromFile("assets/images/ground.png")) {
        cerr << "Failed to load textures\n";
        return;
    }

    player.setTexture(playerTexture);
    player.setPosition(50.f, screenH - 150.f);

    ground.setSize(sf::Vector2f(screenW, 100.f));
    ground.setTexture(&groundTexture);
    ground.setPosition(0.f, screenH - ground.getSize().y);

    if (!bgMusic.openFromFile(musicPath)) {
        cerr << "Failed to load level music\n";
    }
    else {
        bgMusic.setVolume(50.f);
        bgMusic.setLoop(true);
        bgMusic.play();
    }

}

void World2::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        bgMusic.stop();
        levelCompleted = true;
    }
}

void World2::update(float deltaTime) {
    handleInput(deltaTime);
    applyGravity(deltaTime);
    resolveCollisions();
}

void World2::render(sf::RenderWindow& window) {
    window.draw(ground);
    for (auto& obs : obstacles)
        obs.draw(window);
    window.draw(player);
}

bool World2::isComplete() const {
    return levelCompleted;
}

void World2::handleInput(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.move(-moveSpeed * dt, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.move(moveSpeed * dt, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
        velocityY = jumpVelocity;
        isJumping = true;
    }
}

void World2::applyGravity(float dt) {
    velocityY += gravity * dt;
    player.move(0.f, velocityY * dt);
}

void World2::resolveCollisions() {
    const auto& playerBounds = player.getGlobalBounds();
    const auto& groundBounds = ground.getGlobalBounds();

    if (playerBounds.intersects(groundBounds)) {
        player.setPosition(player.getPosition().x, groundBounds.top - playerBounds.height);
        velocityY = 0.f;
        isJumping = false;
    }

    for (auto& obs : obstacles) {
        sf::FloatRect obsBounds = obs.getBounds();
        sf::FloatRect futureBounds = playerBounds;
        futureBounds.top += velocityY;

        if (futureBounds.intersects(obsBounds)) {
            if (velocityY > 0.f)
                player.setPosition(player.getPosition().x, obsBounds.top - playerBounds.height);
            else if (velocityY < 0.f)
                player.setPosition(player.getPosition().x, obsBounds.top + obsBounds.height);

            velocityY = 0.f;
            isJumping = false;
        }
    }
}