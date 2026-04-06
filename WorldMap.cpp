#include "WorldMap.h"
#include "SaveData.h"
#include "GeometryUtils.h"
#include "AudioManager.h"
#include "LevelData.h"
#include <iostream>

using namespace std;

// polygonal region creator
LevelRegion createRegion(const vector<sf::Vector2f>& points, const string& name, int worldIndex, int levelIndexInWorld, int globalLevelIndex) {
    auto shape = make_unique<sf::ConvexShape>();
    shape->setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        shape->setPoint(i, points[i]);

    shape->setFillColor(sf::Color::Transparent);
    shape->setOutlineColor(sf::Color::Yellow);
    shape->setOutlineThickness(2.f);

    LevelRegion region;
    region.shape = move(shape);
    region.levelName = name;
    region.worldIndex = worldIndex;
    region.levelIndexInWorld = levelIndexInWorld;
    region.globalLevelIndex = globalLevelIndex;
    return region;
}

//circular region creator
LevelRegion createRegion(const sf::Vector2f& center, float radius, const string& name, int worldIndex, int levelIndexInWorld, int globalLevelIndex) {
    auto shape = make_unique<sf::CircleShape>(radius);
    shape->setPosition(center - sf::Vector2f(radius, radius));
    shape->setFillColor(sf::Color::Transparent);
    shape->setOutlineColor(sf::Color::Yellow);
    shape->setOutlineThickness(2.f);

    LevelRegion region;
    region.shape = move(shape);
    region.levelName = name;
    region.worldIndex = worldIndex;
    region.levelIndexInWorld = levelIndexInWorld;
    region.globalLevelIndex = globalLevelIndex;
    return region;
}

WorldMap::WorldMap() {
    font.loadFromFile("assets/fonts/chiller.ttf");
    levelNameText.setFont(font);
    levelNameText.setCharacterSize(60);
    levelNameText.setFillColor(sf::Color::White);
    levelNameText.setOutlineColor(sf::Color::Black);
    levelNameText.setOutlineThickness(2.f);

    worldName.setFont(font);
    worldName.setCharacterSize(60);
    worldName.setFillColor(sf::Color::White);
    worldName.setOutlineColor(sf::Color::Black);
    worldName.setOutlineThickness(2.f);

    worldMapPauseButton.setFont(font);
    worldMapPauseButton.setString("||");
    worldMapPauseButton.setCharacterSize(50.f);
    worldMapPauseButton.setPosition(20.f, 20.f);
    worldMapPauseButton.setOutlineThickness(1.f);
    worldMapPauseButton.setOutlineColor(sf::Color::Black);

    setupNavigationButtons();
}

void WorldMap::load(const string& worldMapTexture, const SaveData& data) {
    if (!backgroundTexture.loadFromFile(worldMapTexture)) {
        cerr << "Failed to load world map texture.\n";
        return;
    }
    backgroundSprite.setTexture(backgroundTexture);

    currentWorldIndex = 0;
    int worldCount = 0;
    for (int i = 0; i < data.worldsUnlocked.size(); i++) {
        if (data.worldsUnlocked[i] == true) worldCount++;
    }
    if (worldCount == 0) {
        worldCount = 1;
    }

    worlds.clear();
    worldNames.clear();
    vector<string> allWorldNames = { 
        "book 1: fractured nullspace",
        "book 2: uncanny valley",
        "book 3: bleak mechanica",
        "book 4: reverie threshold",
        "book 5: fading dream"
    };
    for (int i = 0; i < data.worldsUnlocked.size(); ++i) {
        if (data.worldsUnlocked[i]) {
            worldNames.push_back(allWorldNames[i]);
        }
    }
    worlds.resize(worldNames.size());

    if (worldCount >= 1) createRegionsForWorld1();
    if (worldCount >= 2) createRegionsForWorld2();
    if (worldCount >= 3) createRegionsForWorld3();
    if (worldCount >= 4) createRegionsForWorld4();
    if (worldCount >= 5) createRegionsForWorld5();
}

void WorldMap::setupNavigationButtons() {
    leftArrow.setFont(font);
    leftArrow.setString("<");
    leftArrow.setCharacterSize(50.f);
    leftArrow.setFillColor(sf::Color::White);
    leftArrow.setOutlineThickness(1.f);
    leftArrow.setOutlineColor(sf::Color::Black);

    rightArrow.setFont(font);
    rightArrow.setString(">");
    rightArrow.setCharacterSize(50.f);
    rightArrow.setFillColor(sf::Color::White);
    rightArrow.setOutlineThickness(1.f);
    rightArrow.setOutlineColor(sf::Color::Black);
}

void WorldMap::updateArrows(const sf::Vector2f& mousePos) {
    // worldnav arrows hover logic
    if (leftArrow.getGlobalBounds().contains(mousePos) && currentWorldIndex > 0)
        leftArrow.setFillColor(sf::Color::Yellow);
    else
        leftArrow.setFillColor(sf::Color::White);

    if (rightArrow.getGlobalBounds().contains(mousePos) && currentWorldIndex < worlds.size() - 1)
        rightArrow.setFillColor(sf::Color::Yellow);
    else
        rightArrow.setFillColor(sf::Color::White);

    // pause button hover logic
    if (worldMapPauseButton.getGlobalBounds().contains(mousePos)) {
        worldMapPauseButton.setFillColor(sf::Color::Yellow);
        worldMapPauseButton.setOutlineColor(sf::Color::Black);
    }
    else {
        worldMapPauseButton.setFillColor(sf::Color::White);
        worldMapPauseButton.setOutlineColor(sf::Color::Black);
    }
}

void WorldMap::update(const sf::Vector2f& mousePos, SaveData& data) {
    currentData = data;

    updateArrows(mousePos);

    bool hoveringAny = false;

    if (currentWorldIndex >= 0 && currentWorldIndex < worlds.size()) {
        for (auto& region : worlds[currentWorldIndex]) {
            if (auto convex = dynamic_cast<sf::ConvexShape*>(region.shape.get())) {
                vector<sf::Vector2f> transformedPoints;
                sf::Transform transform = convex->getTransform();
                for (size_t i = 0; i < convex->getPointCount(); ++i) {
                    transformedPoints.push_back(transform.transformPoint(convex->getPoint(i)));
                }
                sf::FloatRect bounds = convex->getGlobalBounds();
                if (!bounds.contains(mousePos)) {
                    region.isHovered = false;
                    continue;
                }
                if (pointInPolygon(transformedPoints, mousePos)) {
                    if (region.globalLevelIndex == 0) {
                        region.isHovered = true;
                        levelNameText.setString(region.levelName);
                        sf::FloatRect levelTextBounds = levelNameText.getGlobalBounds();
                        levelNameText.setOrigin(levelTextBounds.width / 2.f, levelTextBounds.height / 2.f);
                        levelNameText.setPosition(800.f, 740.f);
                        hoveringAny = true;
                        continue;
                    }
                    else if (region.globalLevelIndex - 1 >= 0 && data.levelsCompleted[region.globalLevelIndex - 1] == true) {
                        region.isHovered = true;
                        levelNameText.setString(region.levelName);
                        sf::FloatRect levelTextBounds = levelNameText.getGlobalBounds();
                        levelNameText.setOrigin(levelTextBounds.width / 2.f, levelTextBounds.height / 2.f);
                        levelNameText.setPosition(800.f, 740.f);
                        hoveringAny = true;
                        continue;
                    }
                }
            }
            else if (auto circle = dynamic_cast<sf::CircleShape*>(region.shape.get())) {
                sf::Vector2f pos = circle->getPosition();
                float radius = circle->getRadius();
                sf::Vector2f center = { pos.x + radius, pos.y + radius };

                if (hypot(center.x - mousePos.x, center.y - mousePos.y) <= radius) {
                    if (region.globalLevelIndex == 0) {
                        region.isHovered = true;
                        levelNameText.setString(region.levelName);
                        sf::FloatRect levelTextBounds = levelNameText.getGlobalBounds();
                        levelNameText.setOrigin(levelTextBounds.width / 2.f, levelTextBounds.height / 2.f);
                        levelNameText.setPosition(800.f, 740.f);
                        hoveringAny = true;
                        continue;
                    }
                    else if (region.globalLevelIndex - 1 >= 0 && data.levelsCompleted[region.globalLevelIndex - 1] == true) {
                        region.isHovered = true;
                        levelNameText.setString(region.levelName);
                        sf::FloatRect levelTextBounds = levelNameText.getGlobalBounds();
                        levelNameText.setOrigin(levelTextBounds.width / 2.f, levelTextBounds.height / 2.f);
                        levelNameText.setPosition(800.f, 740.f);
                        hoveringAny = true;
                        continue;
                    }
                }
            }

            region.isHovered = false;
        }

        if (!hoveringAny) {
            levelNameText.setString("");
        }
    }
}

void WorldMap::handleLevelClicks(const sf::Vector2f& mousePos, UIContext& ctx, SaveData& data) {
    if (currentWorldIndex < 0 || currentWorldIndex >= worlds.size()) return;

    for (int i = 0; i < worlds[currentWorldIndex].size(); ++i) {
        auto& region = worlds[currentWorldIndex][i];
        bool clicked = false;

        if (auto convex = dynamic_cast<sf::ConvexShape*>(region.shape.get())) {

            sf::Transform transform = convex->getTransform();
            vector<sf::Vector2f> transformedPoints;
            for (size_t j = 0; j < convex->getPointCount(); ++j) {
                transformedPoints.push_back(transform.transformPoint(convex->getPoint(j)));
            }
            sf::FloatRect bounds = convex->getGlobalBounds();
            bool unlocked = (region.globalLevelIndex == 0) || (region.globalLevelIndex > 0 && data.levelsCompleted[region.globalLevelIndex - 1]);

            if (bounds.contains(mousePos) && pointInPolygon(transformedPoints, mousePos) && unlocked) {
                clicked = true;
            }
        }
        else if (auto circle = dynamic_cast<sf::CircleShape*>(region.shape.get())) {
            sf::Vector2f pos = circle->getPosition();
            float radius = circle->getRadius();
            sf::Vector2f center = { pos.x + radius, pos.y + radius };
            bool unlocked = (region.globalLevelIndex == 0) || (region.globalLevelIndex > 0 && data.levelsCompleted[region.globalLevelIndex - 1]);

            if (hypot(center.x - mousePos.x, center.y - mousePos.y) <= radius && unlocked) {
                clicked = true;
            }
        }

        if (clicked) {
            MusicManager::getInstance().startFadeOut(1.f);
            ctx.setLevelID(currentWorldIndex, i);
            ctx.currentState = UIState::Level;
            return;
        }
    }
}

sf::Vector2f WorldMap::getShapeCenter(const sf::ConvexShape& shape) const {
    float minX = shape.getPoint(0).x;
    float maxX = shape.getPoint(0).x;
    float minY = shape.getPoint(0).y;
    float maxY = shape.getPoint(0).y;

    for (size_t i = 1; i < shape.getPointCount(); ++i) {
        auto pt = shape.getPoint(i);
        minX = min(minX, pt.x);
        maxX = max(maxX, pt.x);
        minY = min(minY, pt.y);
        maxY = max(maxY, pt.y);
    }

    return sf::Vector2f((minX + maxX) / 2.f, (minY + maxY) / 2.f);
}

void WorldMap::render(sf::RenderWindow& window) {
    backgroundSprite.setPosition(-currentWorldIndex * worldWidth, 0);
    window.draw(backgroundSprite);

    if (currentWorldIndex >= 0 && currentWorldIndex < worlds.size()) {
        for (const auto& region : worlds[currentWorldIndex]) {
            if (region.isHovered) {
                region.shape->setOutlineColor(sf::Color::Yellow);
                region.shape->setOutlineThickness(2.f);
                window.draw(*region.shape);
            }
        }

        if (!levelNameText.getString().isEmpty())
            window.draw(levelNameText);
        if (currentWorldIndex >= 0 && currentWorldIndex < worldNames.size()) {
            worldName.setString(worldNames[currentWorldIndex]);

            sf::FloatRect textBounds = worldName.getLocalBounds();
            float titleX = (worldWidth / 2.f) - textBounds.width / 2.f;
            float titleY = 0.f;
            worldName.setPosition(titleX, titleY);

            float arrowOffset = 300.f;

            if (currentWorldIndex > 0) {
                leftArrow.setPosition(800.f - arrowOffset, titleY);
                window.draw(leftArrow);
            }

            if (currentWorldIndex < worlds.size() - 1) {
                rightArrow.setPosition(800.f + (arrowOffset - 20.f), titleY);
                window.draw(rightArrow);
            }

            window.draw(worldName);
        }

        if (currentWorldIndex > 0) {
            window.draw(leftArrow);
        }
        if (currentWorldIndex < worlds.size() - 1) {
            window.draw(rightArrow);
        }
    }
    window.draw(worldMapPauseButton);
}

void WorldMap::handleClickWorldNavArrows(const sf::Vector2f& mousePos) {
    if (leftArrow.getGlobalBounds().contains(mousePos) && currentWorldIndex > 0) {
        currentWorldIndex--;
    }
    else if (rightArrow.getGlobalBounds().contains(mousePos) && currentWorldIndex < worlds.size() - 1) {
        currentWorldIndex++;
    }
}

void WorldMap::goToNextWorld() {
    if (currentWorldIndex < worlds.size() - 1)
        currentWorldIndex++;
}

void WorldMap::goToPreviousWorld() {
    if (currentWorldIndex > 0)
        currentWorldIndex--;
}

// hardcoding level names to bypass unknown errors
void WorldMap::createRegionsForWorld1() {
    vector<LevelRegion> world1Levels;
    const int worldIndex = 0;
    world1Levels.push_back(createRegion({ 707.f, 405.f }, 70.f, "chapter 1: who am i?", worldIndex, 0, 0));

    world1Levels.push_back(createRegion({ 868.f, 266.f }, 70.f, "chapter 2: what is this place?", worldIndex, 1, 1));

    world1Levels.push_back(createRegion({ 1043.f, 158.f }, 70.f, "chapter 3: i'm just trying to live", worldIndex, 2, 2));

    world1Levels.push_back(createRegion({ 1237.f, 177.f }, 70.f, "chapter 4: locals", worldIndex, 3, 3));

    world1Levels.push_back(createRegion({ 1372.f, 316.f }, 70.f, "chapter 5: history of nullspace", worldIndex, 4, 4));
    
    world1Levels.push_back(createRegion({ 1249.f, 422.f }, 70.f, "chapter 6: i'm not alone", worldIndex, 5, 5));

    world1Levels.push_back(createRegion({ 1089.f, 496.f }, 70.f, "chapter 7: starting our journey", worldIndex, 6, 6));

    world1Levels.push_back(createRegion({ 1205.f, 625.f }, 70.f, "chapter 8: bit by bit", worldIndex, 7, 7));

    world1Levels.push_back(createRegion({ 1342.f, 697.f }, 70.f, "chapter 9: what lies beyond", worldIndex, 8, 8));

    world1Levels.push_back(createRegion({ 1505.f, 593.f }, 70.f, "chapter 10: the firewall's executable", worldIndex, 9, 9));

    if (worlds.size() > 0) {
        worlds[0] = move(world1Levels);
    }
}

void WorldMap::createRegionsForWorld2() {
    vector<LevelRegion> world2Levels;
    const int worldIndex = 1;
    world2Levels.push_back(createRegion(
        { { 15.f, 433.f },{ 15.f, 465.f },{ 20.f, 485.f },{ 24.f, 488.f },{ 20.f, 495.f },{ 11.f, 507.f },{ 11.f, 523.f },{ 8.f, 538.f },{ 10.f, 553.f },
        { 16.f, 561.f },{ 23.f, 572.f },{ 20.f, 578.f },{ 26.f, 583.f },{ 30.f, 581.f },{ 28.f, 603.f },{ 43.f, 628.f },{ 53.f, 623.f },{ 59.f, 616.f },
        { 67.f, 625.f },{ 88.f, 613.f },{ 85.f, 593.f },{ 83.f, 580.f },{ 85.f, 568.f },{ 89.f, 574.f },{ 98.f, 572.f },{ 103.f, 564.f },{ 108.f, 567.f },
        { 116.f, 569.f },{ 120.f, 567.f },{ 122.f, 562.f },{ 124.f, 567.f },{ 123.f, 602.f },{ 127.f, 606.f },{ 119.f, 606.f },{ 118.f, 612.f },{ 118.f, 620.f },
        { 120.f, 625.f },{ 124.f, 627.f },{ 113.f, 631.f },{ 112.f, 644.f },{ 110.f, 651.f },{ 117.f, 665.f },{ 121.f, 673.f },{ 132.f, 681.f },{ 144.f, 680.f },
        { 151.f, 680.f },{ 157.f, 680.f },{ 160.f, 690.f },{ 163.f, 694.f },{ 169.f, 687.f },{ 171.f, 701.f },{ 171.f, 712.f },{ 180.f, 703.f },{ 191.f, 703.f },
        { 200.f, 719.f },{ 210.f, 702.f },{ 217.f, 708.f },{224.f, 717.f },{ 228.f, 715.f },{ 228.f, 696.f },{ 238.f, 690.f },{ 238.f, 678.f },{ 230.f, 677.f },
        { 225.f, 666.f },{ 227.f, 663.f },{ 239.f, 665.f },{ 250.f, 664.f },{ 258.f, 661.f },{ 266.f, 650.f },{ 267.f, 642.f },{ 265.f, 634.f },{ 262.f, 631.f },
        { 282.f, 631.f },{ 290.f, 613.f },{ 286.f, 600.f },{ 293.f, 606.f },{ 298.f, 610.f },{ 304.f, 612.f },{ 314.f, 607.f },{ 317.f, 604.f },{ 324.f, 599.f },
        { 329.f, 599.f },{ 334.f, 597.f },{ 336.f, 588.f },{ 339.f, 585.f },{ 340.f, 582.f },{ 340.f, 568.f },{ 342.f, 558.f },{ 348.f, 547.f },{ 337.f, 549.f },
        { 349.f, 537.f },{ 349.f, 529.f },{ 359.f, 522.f },{ 368.f, 514.f },{ 368.f, 507.f },{ 369.f, 499.f },{ 366.f, 491.f },{ 362.f, 488.f },{ 361.f, 485.f },
        { 356.f, 477.f },{ 352.f, 479.f },{ 346.f, 476.f },{ 348.f, 465.f },{ 342.f, 455.f },{ 337.f, 450.f },{ 335.f, 446.f },{ 331.f, 443.f },{ 339.f, 431.f },
        { 347.f, 424.f },{ 346.f, 416.f },{ 338.f, 411.f },{ 337.f, 402.f },{ 337.f, 389.f },{ 330.f, 380.f },{ 322.f, 376.f },{ 315.f, 365.f },{ 306.f, 362.f },
        { 296.f, 364.f },{ 294.f, 353.f },{ 287.f, 349.f },{ 279.f, 347.f },{ 285.f, 340.f },{ 284.f, 328.f },{ 281.f, 321.f },{ 277.f, 319.f },{ 279.f, 312.f },
        { 281.f, 300.f },{ 268.f, 306.f },{ 266.f, 300.f },{ 255.f, 292.f },{ 246.f, 290.f },{ 240.f, 290.f },{ 232.f, 292.f },{ 228.f, 282.f },{ 213.f, 282.f },
        { 212.f, 282.f },{ 212.f, 283.f },{ 209.f, 282.f },{ 221.f, 277.f },{ 221.f, 271.f },{ 212.f, 266.f },{ 197.f, 264.f },{ 197.f, 254.f },{ 186.f, 244.f },
        { 177.f, 238.f },{ 169.f, 233.f },{ 160.f, 231.f },{ 148.f, 237.f },{ 140.f, 246.f },{ 131.f, 247.f },{ 122.f, 255.f },{ 122.f, 259.f },{ 117.f, 264.f },
        { 115.f, 265.f },{ 104.f, 270.f },{ 103.f, 272.f },{ 105.f, 275.f },{ 97.f, 273.f },{ 89.f, 279.f },{ 81.f, 279.f },{ 69.f, 292.f },{ 59.f, 305.f },
        { 59.f, 315.f },{ 62.f, 320.f },{ 52.f, 323.f },{ 46.f, 327.f },{ 41.f, 320.f },{ 28.f, 317.f },{ 28.f, 326.f },{ 22.f, 331.f },{ 15.f, 345.f },
        { 15.f, 355.f },{ 19.f, 367.f },{ 24.f, 375.f },{ 27.f, 386.f },{ 19.f, 393.f },{ 9.f, 402.f },{ 4.f, 416.f },{ 10.f, 424.f },{ 13.f, 435.f },
        { 15.f, 433.f } }, "chapter 1: the forest", worldIndex, 0, 10));

    world2Levels.push_back(createRegion(
        { { 394.f, 371.f },{ 375.f, 394.f },{ 375.f, 430.f },{ 384.f, 461.f },{ 391.f, 478.f },{ 405.f, 485.f },{ 423.f, 490.f },{ 436.f, 482.f },
        { 439.f, 497.f },{ 443.f, 509.f },{ 458.f, 520.f },{ 474.f, 524.f },{ 503.f, 522.f },{ 515.f, 517.f },{ 521.f, 510.f },{ 525.f, 498.f },{ 528.f, 489.f },
        { 532.f, 475.f },{ 527.f, 462.f },{ 525.f, 447.f },{ 518.f, 436.f },{ 512.f, 428.f },{ 502.f, 417.f },{ 505.f, 405.f },{ 505.f, 395.f },{ 505.f, 390.f },
        { 505.f, 374.f },{ 504.f, 365.f },{ 488.f, 345.f },{ 479.f, 344.f },{ 462.f, 342.f },{ 444.f, 352.f },{ 433.f, 364.f },{ 418.f, 359.f },{ 402.f, 363.f },
        { 394.f, 371.f } }, "chapter 2: runic shore", worldIndex, 1, 11));

    world2Levels.push_back(createRegion(
        { { 532.f, 411.f },{ 528.f, 434.f },{ 540.f, 454.f },{ 547.f, 454.f },{ 552.f, 430.f },{ 701.f, 410.f },{ 713.f, 433.f },{ 719.f, 435.f },
        { 725.f, 409.f },{ 711.f, 388.f },{ 532.f, 411.f } }, "chapter 3: the bridge", worldIndex, 2, 12));

    world2Levels.push_back(createRegion(
        { { 769.f, 374.f },{ 752.f, 384.f },{ 751.f, 386.f },{ 756.f, 386.f },{ 759.f, 409.f },{ 778.f, 408.f },{ 778.f, 417.f },{ 770.f, 413.f },
        { 749.f, 424.f },{ 758.f, 425.f },{ 759.f, 447.f },{ 778.f, 445.f },{ 778.f, 467.f },{ 807.f, 466.f },{ 806.f, 446.f },{ 825.f, 445.f },{ 825.f, 425.f },
        { 832.f, 423.f },{ 813.f, 410.f },{ 805.f, 414.f },{ 805.f, 406.f },{ 823.f, 407.f },{ 823.f, 386.f },{ 831.f, 386.f },{ 810.f, 373.f },{ 805.f, 376.f },
        { 804.f, 366.f },{ 813.f, 364.f },{ 791.f, 352.f },{ 771.f, 365.f },{ 779.f, 366.f },{ 779.f, 379.f },{ 769.f, 374.f } }, "chapter 4: infinite village",
        worldIndex, 3, 13));

    world2Levels.push_back(createRegion(
        { { 928.f, 411.f },{ 938.f, 428.f },{ 930.f, 434.f },{ 957.f, 436.f },{ 971.f, 413.f },{ 963.f, 415.f },{ 954.f, 398.f },{ 928.f, 411.f } }, "chapter 5: something seems off",
        worldIndex, 4, 14));

    world2Levels.push_back(createRegion({ 1130.f, 538.f }, 30.f, "chapter 6: [REDACTED] plains", worldIndex, 5, 15));

    world2Levels.push_back(createRegion({ 1350.f, 305.f }, 50.f, "chapter 7: memory meadow", worldIndex, 6, 16));

    world2Levels.push_back(createRegion(
        { { 1455.f, 8.f },{ 1456.f, 10.f },{ 1465.f, 17.f },{ 1457.f, 96.f },{ 1526.f, 95.f },{ 1526.f, 81.f },{ 1518.f, 76.f },{ 1483.f, 77.f },{ 1483.f, 22.f },
        { 1540.f, 41.f },{ 1542.f, 41.f },{ 1543.f, 76.f },{ 1538.f, 74.f },{ 1538.f, 79.f },{ 1544.f, 78.f },{ 1547.f, 74.f },{ 1547.f, 40.f },{ 1550.f, 37.f },
        { 1544.f, 29.f },{ 1486.f, 8.f },{ 1471.f, 6.f },{ 1459.f, 2.f },{ 1455.f, 8.f } }, "chapter 8: crane-ium", worldIndex, 7, 17));

    if (worlds.size() > 1) {
        worlds[1] = move(world2Levels);
    }
}

void WorldMap::createRegionsForWorld3() {
    vector<LevelRegion> world3Levels;
    const int worldIndex = 2;
    world3Levels.push_back(createRegion(
        { { 224.167f, 580.377f }, { 238.333f, 581.169f }, { 238.333f, 600.198f }, { 225.833f, 600.198f }, { 224.167f, 580.377f } }, "chapter 1: following in your footsteps",
        worldIndex, 0, 18));

    world3Levels.push_back(createRegion(
        { { 386.667f, 537.562f }, { 395.f, 535.976f }, { 395.f, 560.555f }, { 385.833f, 560.555f }, { 386.667f, 537.562f } }, "chapter 2: night shift scaling",
        worldIndex, 1, 19));

    world3Levels.push_back(createRegion(
        { { 402.5f, 44.4004f }, { 408.333f, 45.1933f }, { 408.333f, 69.7721f }, { 403.333f, 69.7721f }, { 402.5f, 44.4004f } }, "chapter 3: sky-high business",
        worldIndex, 2, 20));

    world3Levels.push_back(createRegion(
        { { 445.833f, 204.559f }, { 459.167f, 204.559f }, { 459.167f, 219.623f }, { 445.f, 219.623f }, { 445.833f, 204.559f } }, "chapter 4: the building over",
        worldIndex, 3, 21));

    world3Levels.push_back(createRegion(
        { { 590.833f, 148.266f }, { 590.833f, 172.844f }, { 601.667f, 172.844f }, { 602.5f, 150.644f }, { 590.833f, 148.266f } }, "chapter 5: clocking out",
        worldIndex, 4, 22));

    world3Levels.push_back(createRegion(
        { { 628.333f, 292.567f }, { 643.333f, 292.567f }, { 644.167f, 317.146f }, { 628.333f, 316.353f }, { 628.333f, 292.567f } }, "chapter 6: out of service",
        worldIndex, 5, 23));

    world3Levels.push_back(createRegion(
        { { 709.167f, 550.248f }, { 720.833f, 550.248f }, { 720.833f, 577.998f }, { 707.5f, 578.791f }, { 709.167f, 550.248f } }, "chapter 7: checkpoint",
        worldIndex, 6, 24));

    world3Levels.push_back(createRegion(
        { { 786.667f, 211.695f }, { 815.833f, 211.695f }, { 815.833f, 263.231f }, { 786.667f, 263.231f }, { 786.667f, 211.695f } }, "chapter 8: rooftop access",
        worldIndex, 7, 25));

    world3Levels.push_back(createRegion(
        { { 945.833f, 240.238f }, { 945.833f, 259.267f }, { 955.833f, 258.474f }, { 955.833f, 239.445f }, { 946.667f, 239.445f } }, "chapter 9: control room",
        worldIndex, 8, 26));

    world3Levels.push_back(createRegion(
        { { 1096.f, 712.f }, { 1086.f, 712.f }, { 1086.f, 766.f }, { 1096.f, 766.f }, { 1096.f, 712.f } }, "chapter 10: the observatory",
        worldIndex, 9, 27));

    world3Levels.push_back(createRegion(
        { { 1596.f, 198.f }, { 1586.f, 192.f }, { 1571.f, 203.f }, { 1567.f, 203.f }, { 1548.f, 195.f }, { 1557.f, 189.f }, { 1559.f, 183.f }, { 1557.f, 183.f },
        { 1554.f, 182.f }, { 1551.f, 181.f }, { 1544.f, 178.f }, { 1544.f, 178.f }, { 1543.f, 178.f }, { 1539.f, 180.f }, { 1539.f, 181.f }, { 1538.f, 183.f },
        { 1530.f, 178.f }, { 1530.f, 178.f }, { 1526.f, 176.f }, { 1518.f, 176.f }, { 1518.f, 176.f }, { 1515.f, 176.f }, { 1513.f, 176.f }, { 1510.f, 179.f }, 
        { 1509.f, 181.f }, { 1505.f, 182.f }, { 1502.f, 175.f }, { 1500.f, 172.f }, { 1499.f, 166.f }, { 1487.f, 166.f }, { 1481.f, 170.f },
        { 1476.f, 174.f }, { 1470.f, 179.f }, { 1470.f, 184.f }, { 1475.f, 197.f }, { 1476.f, 201.f }, { 1476.f, 209.f }, { 1472.f, 218.f }, { 1458.f, 223.f },
        { 1455.f, 238.f }, { 1454.f, 255.f }, { 1455.f, 269.f }, { 1459.f, 278.f }, { 1459.f, 287.f }, { 1458.f, 298.f }, { 1454.f, 306.f }, { 1451.f, 308.f },
        { 1425.f, 303.f }, { 1390.f, 319.f }, { 1383.f, 326.f }, { 1383.f, 341.f }, { 1373.f, 354.f }, { 1366.f, 368.f }, { 1361.f, 381.f }, { 1360.f, 393.f },
        { 1360.f, 406.f }, { 1373.f, 422.f }, { 1377.f, 429.f }, { 1375.f, 438.f }, { 1373.f, 445.f }, { 1389.f, 450.f }, { 1395.f, 452.f }, { 1402.f, 452.f },
        { 1386.f, 461.f }, { 1368.f, 475.f }, { 1368.f, 483.f }, { 1367.f, 501.f }, { 1367.f, 515.f }, { 1371.f, 527.f }, { 1373.f, 538.f }, { 1374.f, 552.f },
        { 1369.f, 563.f }, { 1369.f, 572.f }, { 1375.f, 587.f }, { 1372.f, 594.f }, { 1362.f, 600.f }, { 1342.f, 611.f }, { 1327.f, 632.f }, { 1326.f, 640.f },
        { 1329.f, 658.f }, { 1333.f, 669.f }, { 1359.f, 693.f }, { 1371.f, 716.f }, { 1376.f, 718.f }, { 1394.f, 725.f }, { 1385.f, 748.f }, { 1382.f, 762.f },
        { 1398.f, 770.f }, { 1419.f, 772.f }, { 1454.f, 772.f }, { 1464.f, 774.f }, { 1464.f, 781.f }, { 1461.f, 795.f }, { 1464.f, 799.f }, { 1599.f, 799.f },
        { 1599.f, 200.f } }, "chapter 11: i see a way out", worldIndex, 10, 28));

    if (worlds.size() > 2) {
        worlds[2] = move(world3Levels);
    }
}

void WorldMap::createRegionsForWorld4() {
    vector<LevelRegion> world4Levels;
    const int worldIndex = 3;
    world4Levels.push_back(createRegion(
        { { 207.f, 592.f }, { 200.f, 573.f }, { 197.f, 567.f }, { 186.f, 556.f }, { 176.f, 539.f }, { 158.f, 535.f }, { 156.f, 531.f }, { 148.f, 524.f },
        { 128.f, 516.f }, { 113.f, 510.f }, { 94.f, 510.f }, { 72.f, 510.f }, { 72.f, 510.f }, { 54.f, 510.f }, { 43.f, 511.f }, { 25.f, 514.f },
        { 19.f, 517.f }, { 6.f, 520.f }, { 1.f, 526.f }, { 1.f, 605.f }, { 26.f, 608.f }, { 42.f, 603.f }, { 74.f, 603.f }, { 80.f, 603.f },
        { 91.f, 604.f }, { 104.f, 606.f }, { 117.f, 605.f }, { 133.f, 600.f }, { 153.f, 600.f }, { 175.f, 598.f }, { 189.f, 595.f }, { 207.f, 592.f } }, "chapter 1: leaving the city",
        worldIndex, 0, 29));

    world4Levels.push_back(createRegion({ 184.f, 647.f }, 9.f, "chapter 2: that's the spirit", worldIndex, 1, 30));

    world4Levels.push_back(createRegion({ 352.f, 542.f }, 9.f, "chapter 3: uncanny memory", worldIndex, 2, 31));

    world4Levels.push_back(createRegion({ 484.f, 603.f }, 9.f, "chapter 4: synaptic flow", worldIndex, 3, 32));

    world4Levels.push_back(createRegion({ 624.f, 439.f }, 9.f, "chapter 5: mountain climber", worldIndex, 4, 33));

    world4Levels.push_back(createRegion({ 724.f, 246.f }, 11.f, "chapter 6: mind gap", worldIndex, 5, 34));

    world4Levels.push_back(createRegion({ 903.f, 158.f }, 13.f, "chapter 7: my highest point", worldIndex, 6, 35));

    world4Levels.push_back(createRegion({ 1017.f, 286.f }, 41.f, "chapter 8: growth", worldIndex, 7, 36));

    world4Levels.push_back(createRegion({ 903.f, 603.f }, 85.f, "chapter 9: manifestation", worldIndex, 8, 37));

    world4Levels.push_back(createRegion(
        { { 1241.f, 585.f }, { 1228.f, 588.f }, { 1220.f, 599.f }, { 1212.f, 610.f }, { 1209.f, 616.f }, { 1209.f, 626.f }, { 1207.f, 633.f }, { 1207.f, 641.f },
        { 1208.f, 645.f }, { 1212.f, 649.f }, { 1218.f, 655.f }, { 1222.f, 661.f }, { 1222.f, 670.f }, { 1223.f, 684.f }, { 1224.f, 695.f }, { 1237.f, 702.f },
        { 1248.f, 710.f }, { 1255.f, 719.f }, { 1265.f, 710.f }, { 1276.f, 702.f }, { 1285.f, 698.f }, { 1286.f, 680.f }, { 1287.f, 664.f }, { 1286.f, 660.f },
        { 1298.f, 656.f }, { 1302.f, 654.f }, { 1306.f, 650.f }, { 1308.f, 642.f }, { 1308.f, 639.f }, { 1305.f, 627.f }, { 1301.f, 613.f }, { 1298.f, 603.f },
        { 1292.f, 595.f }, { 1288.f, 584.f }, { 1277.f, 582.f }, { 1269.f, 579.f }, { 1270.f, 571.f }, { 1273.f, 565.f }, { 1274.f, 564.f }, { 1277.f, 561.f },
        { 1283.f, 558.f }, { 1284.f, 550.f }, { 1280.f, 546.f }, { 1283.f, 545.f }, { 1287.f, 539.f }, { 1289.f, 531.f }, { 1289.f, 527.f }, { 1284.f, 526.f },
        { 1283.f, 520.f }, { 1270.f, 510.f }, { 1269.f, 509.f }, { 1257.f, 509.f }, { 1250.f, 509.f }, { 1241.f, 512.f }, { 1235.f, 514.f }, { 1236.f, 517.f },
        { 1229.f, 520.f }, { 1233.f, 521.f }, { 1228.f, 523.f }, { 1224.f, 528.f }, { 1219.f, 530.f }, { 1215.f, 531.f }, { 1217.f, 536.f }, { 1221.f, 538.f },
        { 1222.f, 541.f }, { 1222.f, 545.f }, { 1227.f, 545.f }, { 1225.f, 550.f }, { 1229.f, 552.f }, { 1231.f, 563.f }, { 1234.f, 566.f }, { 1236.f, 567.f },
        { 1239.f, 570.f }, { 1239.f, 575.f }, { 1243.f, 579.f }, { 1241.f, 585.f } }, "chapter 10: pseudonym: Jkmn", worldIndex, 9, 38));

    world4Levels.push_back(createRegion(
        { { 1433.f, 567.f }, { 1430.f, 555.f }, { 1433.f, 552.f }, { 1435.f, 550.f }, { 1434.f, 545.f }, { 1434.f, 536.f }, { 1436.f, 534.f }, { 1442.f, 531.f },
        { 1449.f, 530.f }, { 1449.f, 523.f }, { 1460.f, 514.f }, { 1467.f, 512.f }, { 1472.f, 504.f }, { 1478.f, 505.f }, { 1486.f, 504.f }, { 1487.f, 506.f },
        { 1490.f, 500.f }, { 1510.f, 496.f }, { 1516.f, 499.f }, { 1521.f, 501.f }, { 1523.f, 505.f }, { 1533.f, 503.f }, { 1539.f, 506.f }, { 1547.f, 512.f },
        { 1551.f, 519.f }, { 1546.f, 521.f }, { 1557.f, 523.f }, { 1564.f, 527.f }, { 1564.f, 533.f }, { 1562.f, 542.f }, { 1571.f, 547.f }, { 1571.f, 550.f },
        { 1571.f, 556.f }, { 1566.f, 559.f }, { 1572.f, 563.f }, { 1573.f, 563.f }, { 1573.f, 569.f }, { 1573.f, 573.f }, { 1569.f, 576.f }, { 1572.f, 577.f },
        { 1572.f, 585.f }, { 1572.f, 587.f }, { 1567.f, 590.f }, { 1562.f, 591.f }, { 1565.f, 596.f }, { 1565.f, 599.f }, { 1567.f, 602.f }, { 1564.f, 603.f },
        { 1562.f, 606.f }, { 1565.f, 607.f }, { 1566.f, 613.f }, { 1562.f, 620.f }, { 1553.f, 625.f }, { 1550.f, 627.f }, { 1551.f, 630.f }, { 1548.f, 633.f },
        { 1552.f, 637.f }, { 1553.f, 642.f }, { 1545.f, 646.f }, { 1535.f, 648.f }, { 1522.f, 649.f }, { 1510.f, 656.f }, { 1498.f, 658.f }, { 1484.f, 658.f },
        { 1470.f, 658.f }, { 1459.f, 653.f }, { 1446.f, 647.f }, { 1446.f, 645.f }, { 1433.f, 640.f }, { 1431.f, 632.f }, { 1433.f, 625.f }, { 1437.f, 624.f },
        { 1429.f, 621.f }, { 1431.f, 617.f }, { 1434.f, 611.f }, { 1439.f, 611.f }, { 1436.f, 603.f }, { 1435.f, 595.f }, { 1428.f, 591.f }, { 1427.f, 586.f },
        { 1431.f, 582.f }, { 1428.f, 579.f }, { 1427.f, 573.f }, { 1430.f, 567.f }, { 1433.f, 567.f } }, "chapter 11: stuck in my head", worldIndex, 10, 39));

    if (worlds.size() > 3) {
        worlds[3] = move(world4Levels);
    }
}

void WorldMap::createRegionsForWorld5() {
    vector<LevelRegion> world5Levels;
    const int worldIndex = 4;
    world5Levels.push_back(createRegion(
        { { 281.f, 54.f }, { 254.f, 67.f }, { 242.f, 83.f }, { 226.f, 81.f }, { 215.f, 79.f }, { 196.f, 79.f }, { 184.f, 99.f }, { 189.f, 123.f }, 
        { 203.f, 151.f }, { 232.f, 177.f }, { 273.f, 190.f }, { 314.f, 202.f }, { 349.f, 200.f }, { 381.f, 187.f }, { 392.f, 167.f }, { 399.f, 144.f }, 
        { 411.f, 121.f }, { 424.f, 97.f }, { 429.f, 71.f }, { 420.f, 47.f }, { 398.f, 47.f }, { 366.f, 55.f }, { 357.f, 67.f }, { 345.f, 56.f }, { 327.f, 56.f },
        { 317.f, 63.f }, { 316.f, 69.f }, { 306.f, 62.f }, { 292.f, 55.f }, { 284.f, 55.f }, { 281.f, 54.f } }, "chapter 1: certified amnesiac", worldIndex,
        0, 40));

    world5Levels.push_back(createRegion(
        { { 461.f, 73.f }, { 464.f, 110.f }, { 468.f, 125.f }, { 504.f, 160.f }, { 523.f, 175.f }, { 554.f, 191.f }, { 580.f, 201.f }, { 613.f, 201.f }, 
        { 631.f, 189.f }, { 654.f, 165.f }, { 668.f, 148.f }, { 685.f, 120.f }, { 691.f, 96.f }, { 691.f, 77.f }, { 678.f, 65.f }, { 670.f, 65.f },
        { 666.f, 65.f }, { 651.f, 67.f }, { 637.f, 66.f }, { 612.f, 63.f }, { 597.f, 58.f }, { 597.f, 58.f }, { 578.f, 58.f }, { 577.f, 58.f },
        { 558.f, 59.f }, { 544.f, 64.f }, { 536.f, 65.f }, { 528.f, 66.f }, { 517.f, 66.f }, { 504.f, 68.f }, { 486.f, 73.f }, { 481.f, 74.f }, { 470.f, 72.f },
        { 465.f, 74.f }, { 464.f, 82.f }, { 461.f, 73.f } }, "chapter 2: who Jkmn was", worldIndex, 1, 41));

    world5Levels.push_back(createRegion(
        { { 749.f, 86.f }, { 739.f, 105.f }, { 744.f, 134.f }, { 765.f, 162.f }, { 797.f, 177.f }, { 825.f, 184.f }, { 867.f, 193.f }, { 888.f, 193.f },
        { 908.f, 189.f }, { 925.f, 173.f }, { 937.f, 145.f }, { 955.f, 123.f }, { 965.f, 108.f }, { 971.f, 87.f }, { 971.f, 66.f }, { 963.f, 54.f },
        { 945.f, 54.f }, { 932.f, 58.f }, { 927.f, 66.f }, { 916.f, 72.f }, { 900.f, 71.f }, { 888.f, 69.f }, { 868.f, 68.f }, { 847.f, 72.f },
        { 843.f, 75.f }, { 836.f, 75.f }, { 816.f, 75.f }, { 799.f, 75.f }, { 794.f, 75.f }, { 764.f, 80.f }, { 749.f, 86.f } }, "chapter 3: who Jkmn is",
        worldIndex, 2, 42));

    world5Levels.push_back(createRegion(
        { { 1011.f, 83.f }, { 1008.f, 109.f }, { 1026.f, 152.f }, { 1057.f, 176.f }, { 1092.f, 188.f }, { 1120.f, 193.f }, { 1155.f, 195.f }, { 1178.f, 190.f },
        { 1202.f, 173.f }, { 1222.f, 155.f }, { 1235.f, 131.f }, { 1248.f, 98.f }, { 1254.f, 76.f }, { 1244.f, 58.f }, { 1228.f, 50.f }, { 1217.f, 48.f },
        { 1200.f, 56.f }, { 1198.f, 65.f }, { 1185.f, 69.f }, { 1168.f, 68.f }, { 1148.f, 68.f }, { 1142.f, 72.f }, { 1138.f, 74.f }, { 1125.f, 74.f },
        { 1117.f, 71.f }, { 1088.f, 66.f }, { 1076.f, 67.f }, { 1070.f, 70.f }, { 1063.f, 76.f }, { 1053.f, 77.f }, { 1049.f, 77.f }, { 1028.f, 77.f },
        { 1019.f, 79.f }, { 1019.f, 79.f }, { 1014.f, 81.f }, { 1011.f, 83.f } }, "chapter 4: the creator", worldIndex, 3, 43));

    world5Levels.push_back(createRegion(
        { { 190.f, 353.f }, { 174.f, 360.f }, { 186.f, 405.f }, { 205.f, 436.f }, { 235.f, 460.f }, { 279.f, 470.f }, { 318.f, 477.f }, { 343.f, 477.f },
        { 366.f, 469.f }, { 410.f, 443.f }, { 422.f, 419.f }, { 427.f, 396.f }, { 428.f, 374.f }, { 424.f, 347.f }, { 418.f, 330.f }, { 405.f, 329.f },
        { 379.f, 333.f }, { 354.f, 334.f }, { 340.f, 335.f }, { 307.f, 337.f }, { 296.f, 342.f }, { 276.f, 348.f }, { 263.f, 350.f }, { 243.f, 351.f },
        { 236.f, 351.f }, { 205.f, 351.f }, { 194.f, 351.f }, { 192.f, 352.f }, { 190.f, 353.f } }, "chapter 5: hospitalization", worldIndex, 4, 44));

    world5Levels.push_back(createRegion(
        { { 472.f, 350.f }, { 454.f, 380.f }, { 459.f, 397.f }, { 488.f, 424.f }, { 494.f, 437.f }, { 532.f, 463.f }, { 556.f, 474.f }, { 577.f, 480.f },
        { 606.f, 474.f }, { 645.f, 460.f }, { 659.f, 452.f }, { 665.f, 422.f }, { 672.f, 399.f }, { 682.f, 384.f }, { 690.f, 369.f }, { 692.f, 345.f },
        { 686.f, 333.f }, { 676.f, 328.f }, { 660.f, 328.f }, { 646.f, 335.f }, { 628.f, 346.f }, { 627.f, 346.f }, { 616.f, 338.f }, { 595.f, 338.f },
        { 583.f, 341.f }, { 579.f, 345.f }, { 555.f, 350.f }, { 547.f, 350.f }, { 543.f, 350.f }, { 529.f, 352.f }, { 511.f, 353.f }, { 505.f, 353.f },
        { 496.f, 349.f }, { 477.f, 348.f }, { 471.f, 350.f }, { 472.f, 350.f } }, "chapter 6: january 28th, 2067", worldIndex, 5, 45));

    world5Levels.push_back(createRegion(
        { { 732.f, 353.f }, { 725.f, 371.f }, { 725.f, 391.f }, { 736.f, 413.f }, { 752.f, 426.f }, { 770.f, 442.f }, { 787.f, 458.f }, { 805.f, 469.f },
        { 834.f, 476.f }, { 856.f, 480.f }, { 881.f, 480.f }, { 899.f, 472.f }, { 914.f, 460.f }, { 928.f, 445.f }, { 942.f, 419.f }, { 947.f, 404.f },
        { 955.f, 387.f }, { 958.f, 367.f }, { 958.f, 351.f }, { 952.f, 343.f }, { 934.f, 341.f }, { 912.f, 341.f }, { 901.f, 347.f }, { 894.f, 350.f },
        { 884.f, 350.f }, { 865.f, 350.f }, { 851.f, 353.f }, { 840.f, 360.f }, { 824.f, 356.f }, { 808.f, 349.f }, { 795.f, 347.f }, { 780.f, 350.f },
        { 768.f, 351.f }, { 745.f, 351.f }, { 740.f, 351.f }, { 732.f, 353.f } }, "chapter 7: who i was", worldIndex, 6, 46));

    world5Levels.push_back(createRegion(
        { { 1003.f, 388.f }, { 1007.f, 411.f }, { 1030.f, 443.f }, { 1051.f, 463.f }, { 1075.f, 476.f }, { 1122.f, 487.f }, { 1147.f, 487.f }, { 1180.f, 478.f },
        { 1196.f, 474.f }, { 1216.f, 456.f }, { 1221.f, 430.f }, { 1227.f, 410.f }, { 1235.f, 388.f }, { 1240.f, 370.f }, { 1236.f, 351.f }, { 1221.f, 347.f },
        { 1192.f, 348.f }, { 1186.f, 350.f }, { 1182.f, 353.f }, { 1178.f, 356.f }, { 1167.f, 356.f }, { 1138.f, 356.f }, { 1108.f, 355.f }, { 1105.f, 355.f },
        { 1088.f, 360.f }, { 1074.f, 365.f }, { 1052.f, 365.f }, { 1025.f, 358.f }, { 1023.f, 358.f }, { 1008.f, 370.f }, { 1007.f, 385.f }, { 1003.f, 388.f } },
        "chapter 8: who i am", worldIndex, 7, 47));

    world5Levels.push_back(createRegion(
        { { 192.f, 646.f }, { 185.f, 674.f }, { 194.f, 702.f }, { 206.f, 717.f }, { 212.f, 728.f }, { 250.f, 741.f }, { 265.f, 749.f }, { 281.f, 760.f },
        { 310.f, 762.f }, { 338.f, 761.f }, { 354.f, 757.f }, { 384.f, 738.f }, { 403.f, 726.f }, { 410.f, 709.f }, { 417.f, 687.f }, { 422.f, 661.f },
        { 423.f, 643.f }, { 421.f, 630.f }, { 409.f, 621.f }, { 396.f, 621.f }, { 384.f, 622.f }, { 350.f, 622.f }, { 334.f, 622.f }, { 302.f, 622.f },
        { 283.f, 628.f }, { 268.f, 633.f }, { 247.f, 634.f }, { 247.f, 634.f }, { 225.f, 634.f }, { 213.f, 635.f }, { 197.f, 637.f }, { 192.f, 646.f } },
        "chapter 9: questionnaire", worldIndex, 8, 48));

    world5Levels.push_back(createRegion(
        { { 466.f, 655.f }, { 465.f, 689.f }, { 476.f, 713.f }, { 494.f, 731.f }, { 522.f, 746.f }, { 565.f, 758.f }, { 592.f, 763.f }, { 630.f, 761.f },
        { 647.f, 743.f }, { 667.f, 724.f }, { 669.f, 709.f }, { 684.f, 678.f }, { 687.f, 666.f }, { 692.f, 641.f }, { 689.f, 625.f }, { 680.f, 619.f },
        { 666.f, 619.f }, { 656.f, 623.f }, { 648.f, 625.f }, { 632.f, 625.f }, { 606.f, 623.f }, { 576.f, 626.f }, { 569.f, 629.f }, { 550.f, 633.f },
        { 537.f, 633.f }, { 521.f, 634.f }, { 494.f, 635.f }, { 478.f, 641.f }, { 468.f, 648.f }, { 466.f, 655.f } }, "chapter 10: how is this possible?",
        worldIndex, 9, 49));

    world5Levels.push_back(createRegion(
        { { 730.f, 639.f }, { 714.f, 656.f }, { 727.f, 675.f }, { 743.f, 698.f }, { 763.f, 721.f }, { 785.f, 734.f }, { 801.f, 741.f }, { 824.f, 751.f },
        { 860.f, 753.f }, { 883.f, 753.f }, { 903.f, 749.f }, { 921.f, 735.f }, { 928.f, 720.f }, { 935.f, 698.f }, { 944.f, 674.f }, { 948.f, 662.f },
        { 949.f, 643.f }, { 946.f, 626.f }, { 930.f, 620.f }, { 915.f, 625.f }, { 915.f, 639.f }, { 913.f, 644.f }, { 908.f, 641.f }, { 895.f, 631.f },
        { 872.f, 631.f }, { 861.f, 636.f }, { 861.f, 648.f }, { 852.f, 654.f }, { 846.f, 651.f }, { 841.f, 645.f }, { 827.f, 630.f }, { 820.f, 627.f },
        { 803.f, 628.f }, { 797.f, 643.f }, { 793.f, 654.f }, { 775.f, 652.f }, { 767.f, 645.f }, { 754.f, 637.f }, { 738.f, 637.f }, { 730.f, 639.f } },
        "chapter 11: the fog is clearing", worldIndex, 10, 50));

    world5Levels.push_back(createRegion(
        { { 1004.f, 666.f }, { 1028.f, 707.f }, { 1056.f, 735.f }, { 1070.f, 746.f }, { 1096.f, 759.f }, { 1128.f, 762.f }, { 1156.f, 753.f }, { 1175.f, 741.f },
        { 1194.f, 725.f }, { 1205.f, 699.f }, { 1213.f, 673.f }, { 1215.f, 647.f }, { 1208.f, 641.f }, { 1202.f, 641.f }, { 1200.f, 645.f }, { 1192.f, 661.f },
        { 1185.f, 657.f }, { 1172.f, 655.f }, { 1170.f, 651.f }, { 1162.f, 645.f }, { 1151.f, 646.f }, { 1145.f, 651.f }, { 1141.f, 657.f }, { 1140.f, 665.f },
        { 1138.f, 668.f }, { 1136.f, 678.f }, { 1127.f, 680.f }, { 1123.f, 679.f }, { 1117.f, 676.f }, { 1114.f, 673.f }, { 1105.f, 666.f }, { 1105.f, 663.f },
        { 1100.f, 658.f }, { 1096.f, 656.f }, { 1082.f, 655.f }, { 1080.f, 660.f }, { 1075.f, 666.f }, { 1075.f, 669.f }, { 1070.f, 673.f }, { 1062.f, 675.f },
        { 1053.f, 675.f }, { 1039.f, 675.f }, { 1028.f, 670.f }, { 1021.f, 662.f }, { 1014.f, 659.f }, { 1007.f, 662.f }, { 1007.f, 668.f }, { 1004.f, 666.f } },
        "chapter 12: neuron activation", worldIndex, 11, 51));

    world5Levels.push_back(createRegion({ 1413.f, 407.f }, 130.f, "chapter 13: muerte es la vida", worldIndex, 12, 52));

    if (worlds.size() > 4) {
        worlds[4] = move(world5Levels);
    }
}
