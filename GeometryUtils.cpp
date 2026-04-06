#include "GeometryUtils.h"
#include <iostream>
#include <vector>

using namespace std;

bool pointInPolygon(const vector<sf::Vector2f>& points, const sf::Vector2f& test) {
    bool inside = false;
    size_t j = points.size() - 1;
    for (size_t i = 0; i < points.size(); i++) {
        const auto& pi = points[i];
        const auto& pj = points[j];
        if ((pi.y > test.y) != (pj.y > test.y)) {
            float slope = (pj.x - pi.x) * (test.y - pi.y) / (pj.y - pi.y) + pi.x;
            if (test.x < slope) {
                inside = !inside;
            }
        }
        j = i;
    }
    return inside;
}
