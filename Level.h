#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Obstacle.h"
#include "SaveData.h"

enum class CharacterType {
    smiley,
    hovercape,
    truthblade,
    riley
};

struct CharacterState {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    sf::Vector2f position;

    bool facingright = true;
    bool isgrounded = false;
};

class Level {
public:
    virtual ~Level() = default;

    virtual void load(SaveData& data) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    bool isComplete() const { return levelComplete; }
protected:
    CharacterState currentCharacter;
    CharacterState smileyCharacter;
    CharacterState hovercapeCharacter;
    CharacterState truthbladeCharacter;
    CharacterState rileyCharacter;

    sf::Texture smileyTexture;
    sf::Texture hovercapeTexture;
    sf::Texture truthbladeTexture;
    sf::Texture rileyTexture;

    CharacterState& getCharacterStateFromName(const std::string& name)
    {
        if (name == "hovercape") return hovercapeCharacter;
        if (name == "truthblade") return truthbladeCharacter;
        if (name == "riley") return rileyCharacter;

        return smileyCharacter;
    }

    virtual void loadCharacters(SaveData& data) {}
    void applyCharacters(CharacterState& state);

    sf::Sprite bg;
    sf::Texture bgtexture;

    bool levelComplete = false;
    float gravity = 1400.f;
    float moveSpeed = 300.f;
    float jumpStrength = 600.f;

    sf::FloatRect groundRect;

    struct ParallaxLayer {
        sf::Sprite sprite;
        float speedMultiplier;
    };

    vector<ParallaxLayer> parallaxLayers;
    vector<Obstacle> obstacles;

    void updateParallax(float anchorX);
};

inline void Level::updateParallax(float anchorX)
{
    for (auto& layer : parallaxLayers)
        layer.sprite.setPosition(-anchorX * layer.speedMultiplier, 0.f);
}
