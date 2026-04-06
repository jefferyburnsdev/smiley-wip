#pragma once

#include "World1.h"
#include "UIContext.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

extern vector<vector<string>> LevelNames;

class LevelManager {
public:
    void loadLevel(const UIContext& ctx, SaveData& saveData);
    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isLevelComplete() const;

private:
    unique_ptr<Level> currentLevel = nullptr;
};
