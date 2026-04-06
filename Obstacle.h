#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

enum class ObstacleType {
    Ground,
    Wall,
    Platform,
    Ceiling,
    Trigger
};

class Obstacle {
public:
    Obstacle(sf::Vector2f size, sf::Vector2f position, ObstacleType type, sf::Texture* texture = nullptr);
    void draw(sf::RenderWindow& window);
    const sf::FloatRect getBounds() const;
    sf::RectangleShape& getShape(); // if you want to access or move it externally

private:
    sf::RectangleShape shape;
    ObstacleType type;
};
