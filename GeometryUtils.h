#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

bool pointInPolygon(const std::vector<sf::Vector2f>& points, const sf::Vector2f& test);
