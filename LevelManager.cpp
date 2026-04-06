#include "LevelManager.h"
#include "AudioManager.h"
#include "World1.h"
#include "World2.h"
#include "World3.h"
#include "World4.h"
#include "world5.h"
#include <vector>

using namespace std;

vector<vector<string>> LevelNames = {
    {"chapter 1: who am i?", "chapter 2: what is this place?", "chapter 3: i'm just trying to live", "chapter 4: locals", "chapter 5: history of nullspace", "chapter 6: i'm not alone", "chapter 7: starting our journey", "chapter 8: bit by bit", "chapter 9: what lies beyond", "chapter 10: the firewall's executable"},
    {"chapter 1: the forest", "chapter 2: runic shore", "chapter 3: the bridge", "chapter 4: infinite village", "chapter 5: something seems off", "chapter 6: [REDACTED] plains", "chapter 7: memory meadow", "chapter 8: crane-ium"},
    {"chapter 1: following in your footsteps", "chapter 2: night shift scaling", "chapter 3: sky-high business", "chapter 4: the building over", "chapter 5: clocking out", "chapter 6: out of service", "chapter 7: checkpoint", "chapter 8: rooftop access", "chapter 9: control room", "chapter 10: the observatory", "chapter 11: i see a way out"},
    {"chapter 1: leaving the city", "chapter 2: that's the spirit", "chapter 3: uncanny memory", "chapter 4: synaptic flow", "chapter 5: mountain climber", "chapter 6: mind gap", "chapter 7: my highest point", "chapter 8: growth", "chapter 9: manifestation", "chapter 10: pseudonym: Jkmn", "chapter 11: stuck in my head"},
    {"chapter 1: certified amnesiac", "chapter 2: who Jkmn was", "chapter 3: who Jkmn is", "chapter 4: the creator", "chapter 5: hospitalization", "chapter 6: january 28th, 2067", "chapter 7: who i was", "chapter 8: who i am", "chapter 9: questionnaire", "chapter 10: how is this possible?", "chapter 11: the fog is clearing", "chapter 12: neuron activation", "chapter 13: muerte es la vida"}
};

void LevelManager::loadLevel(const UIContext& ctx, SaveData& saveData) {
    int world = ctx.getWorldIndex();
    int level = ctx.getLevelIndex();
    string levelName = LevelNames[world][level];
    string musicPath;

    switch (world) {
    case 0: 
        musicPath = "assets/music/world1bg.ogg"; 
        currentLevel = make_unique<World1>(level, levelName, musicPath);
        break;
    case 1: 
        musicPath = "assets/music/world2bg.ogg"; 
        currentLevel = make_unique<World2>(level, levelName, musicPath);
        break;
    case 2: 
        musicPath = "assets/music/world3bg.ogg"; 
        currentLevel = make_unique<World3>(level, levelName, musicPath);
        break;
    case 3: 
        musicPath = "assets/music/world4bg.ogg"; 
        currentLevel = make_unique<World4>(level, levelName, musicPath);
        break;
    case 4: 
        musicPath = "assets/music/world5bg.ogg"; 
        currentLevel = make_unique<World5>(level, levelName, musicPath);
        break;
    default: 
        musicPath = "assets/music/world1bg.ogg"; 
        currentLevel = make_unique<World1>(level, levelName, musicPath);
        break;
    }

    MusicManager::getInstance().playMusic(musicPath, 60.f, true);
    currentLevel->load(saveData);
}

void LevelManager::handleEvent(const sf::Event& event) {
    if (currentLevel)
        currentLevel->handleEvent(event);
}

void LevelManager::update(float deltaTime) {
    if (currentLevel)
        currentLevel->update(deltaTime);
}

void LevelManager::render(sf::RenderWindow& window) {
    if (currentLevel)
        currentLevel->render(window);
}

bool LevelManager::isLevelComplete() const {
    return currentLevel && currentLevel->isComplete();
}
