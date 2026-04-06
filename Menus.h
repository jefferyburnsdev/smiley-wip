#pragma once
#include <SFML/Graphics.hpp>
#include "uiContext.h"
#include "WorldMap.h"
#include "CutscenePlayer.h"


using namespace std;

void handleMainMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font);
void handleNewSaveEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font, CutscenePlayer& cs);
void handleExistingSaveEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font, CutscenePlayer& cs, WorldMap& m);
int handleSplashEvents(sf::Event event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font);
void handleWorldMapPauseMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font);
void handleLevelPauseMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font);
void handleSettingsMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font, int menuCase);
void handleAchievementsMenuEvents(const sf::Event& event, UIContext& ctx, sf::RenderWindow& window, const sf::Font& font);
