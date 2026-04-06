#include <iostream>
#include "CutscenePlayer.h"
#include "WorldMap.h"
#include "SaveManager.h"

using namespace std;

CutscenePlayer::CutscenePlayer() {
    if (!font.loadFromFile("assets/fonts/chiller.ttf")) {
        cerr << "Failed to load font\n";
    }
}

void CutscenePlayer::play(UIContext& ctx) {
    if (isPlaying) return;
    isFinished = false;
    isPlaying = true;
    timer.restart();
}

void CutscenePlayer::update(UIContext& ctx, WorldMap& m) {
    if (!isFinished && timer.getElapsedTime().asSeconds() > 5.f) {
        isFinished = true;

        ctx.getCurrentSaveData().worldsUnlocked[0] = true;
        ctx.getCurrentSaveData().lastWorldMap = 1;
        SaveManager::saveToFile(ctx.getCurrentSaveData(), ctx.getCurrentFilename());
        m.load("assets/images/grandworldmap.png", ctx.getCurrentSaveData());
        ctx.currentState = UIState::WorldMap;
    }
}

void CutscenePlayer::render(sf::RenderWindow& window, UIContext& ctx) {
    sf::RectangleShape bg(sf::Vector2f(window.getSize()));
    bg.setFillColor(sf::Color::Black);
    window.draw(bg);

    sf::Text text;
    text.setFont(font);
    text.setString("Playing cutscene...");
    text.setCharacterSize(80.f);
    text.setFillColor(sf::Color::White);
    text.setPosition(800.f, 400.f);
    window.draw(text);
}

bool CutscenePlayer::finished() const {
    return isFinished;
}
