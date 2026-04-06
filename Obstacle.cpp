#include "Obstacle.h"

using namespace std;

Obstacle::Obstacle(sf::Vector2f size, sf::Vector2f position, ObstacleType type, sf::Texture* texture) : type(type)
{
    shape.setSize(size);
    shape.setPosition(position);

    if (!texture) {
        switch (type) {
        case ObstacleType::Ground:
            shape.setFillColor(sf::Color(139, 69, 19));
            break;
        case ObstacleType::Platform:
            shape.setFillColor(sf::Color(100, 100, 255));
            break;
        case ObstacleType::Wall:
            shape.setFillColor(sf::Color::Cyan);
            break;
        case ObstacleType::Ceiling:
            shape.setFillColor(sf::Color(150, 150, 150));
            break;
        case ObstacleType::Trigger:
            shape.setFillColor(sf::Color(0, 255, 0, 120));
            break;
        }
    }

    if (texture) {
        shape.setTexture(texture);
    }
}

void Obstacle::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

const sf::FloatRect Obstacle::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::RectangleShape& Obstacle::getShape()
{
    return shape;
}
