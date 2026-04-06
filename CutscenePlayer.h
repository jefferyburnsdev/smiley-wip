#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "uiContext.h"
#include "WorldMap.h"

using namespace std;

class CutscenePlayer {
public:
    CutscenePlayer();
    void play(UIContext& ctx);
    void update(UIContext& ctx, WorldMap& m);
    void render(sf::RenderWindow& window, UIContext& ctx);
    bool finished() const;

private:
    bool isPlaying = false;
    bool isFinished = false;
    sf::Font font;
    sf::Clock timer;
    // add animation state later
};
