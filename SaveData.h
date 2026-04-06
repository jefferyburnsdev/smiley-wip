#pragma once
#include <vector>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct SaveData {
    int saveVersion = 1;
    string saveName;
    int lastWorldMap = 1;

    vector<bool> worldsUnlocked = vector<bool>(5, false);

    bool levelsCompleted[53] = {};
    bool world1Progress[10] = {};
    bool world2Progress[8] = {};
    bool world3Progress[11] = {};
    bool world4Progress[11] = {};
    bool world5Progress[13] = {};

    bool hovercape = false;
    bool truthblade = false;
    bool riley = false;

    vector<bool> universalTotalOrbsCollected = vector<bool>(52, false);
    int universalTotalOrbsExisting = 52;

    vector<bool> achievementsUnlocked = vector<bool>(30, false);
    int totalAchievementsUnlocked = 0;
    int totalAchievementsPossible = 30;

    float sfxvolume = 100.f;
    float musicvolume = 100.f;
    bool fullscreen = false;
    string currentCharacter = "smiley";

    SaveData()
    {
        for (int i = 0; i < 53; i++) levelsCompleted[i] = false;

        for (int i = 0; i < 10; i++) world1Progress[i] = false;
        for (int i = 0; i < 8; i++) world2Progress[i] = false;
        for (int i = 0; i < 11; i++) world3Progress[i] = false;
        for (int i = 0; i < 11; i++) world4Progress[i] = false;
        for (int i = 0; i < 13; i++) world5Progress[i] = false;
    }
};

inline void to_json(json& j, const SaveData& d)
{
    vector<int> achievementsAsInts(d.achievementsUnlocked.begin(), d.achievementsUnlocked.end());

    j = json{
        {"saveVersion", d.saveVersion},

        {"saveName", d.saveName},
        {"lastWorldMap", d.lastWorldMap},

        {"worldsUnlocked", d.worldsUnlocked},

        {"levelsCompleted", vector<bool>(begin(d.levelsCompleted), end(d.levelsCompleted))},
        {"world1Progress", vector<bool>(begin(d.world1Progress), end(d.world1Progress))},
        {"world2Progress", vector<bool>(begin(d.world2Progress), end(d.world2Progress))},
        {"world3Progress", vector<bool>(begin(d.world3Progress), end(d.world3Progress))},
        {"world4Progress", vector<bool>(begin(d.world4Progress), end(d.world4Progress))},
        {"world5Progress", vector<bool>(begin(d.world5Progress), end(d.world5Progress))},

        {"hovercape", d.hovercape},
        {"truthblade", d.truthblade},
        {"riley", d.riley},

        {"universalTotalOrbsCollected", d.universalTotalOrbsCollected},
        {"universalTotalOrbsExisting", d.universalTotalOrbsExisting},

        {"achievementsUnlocked", achievementsAsInts},
        {"totalAchievementsUnlocked", d.totalAchievementsUnlocked},
        {"totalAchievementsPossible", d.totalAchievementsPossible},

        {"sfxvolume", d.sfxvolume},
        {"musicvolume", d.musicvolume},
        {"fullscreen", d.fullscreen},
        {"currentCharacter", d.currentCharacter}
    };
}

inline void from_json(const json& j, SaveData& d)
{
    if (j.contains("saveVersion"))
        j.at("saveVersion").get_to(d.saveVersion);

    if (j.contains("saveName"))
        j.at("saveName").get_to(d.saveName);

    if (j.contains("lastWorldMap"))
        j.at("lastWorldMap").get_to(d.lastWorldMap);

    if (j.contains("worldsUnlocked"))
        d.worldsUnlocked = j.at("worldsUnlocked").get<vector<bool>>();

    auto copy_bool_array = [](const json& js, auto& arr)
        {
            for (size_t i = 0; i < js.size() && i < size(arr); ++i)
                arr[i] = js[i];
        };

    if (j.contains("levelsCompleted")) copy_bool_array(j.at("levelsCompleted"), d.levelsCompleted);
    if (j.contains("world1Progress")) copy_bool_array(j.at("world1Progress"), d.world1Progress);
    if (j.contains("world2Progress")) copy_bool_array(j.at("world2Progress"), d.world2Progress);
    if (j.contains("world3Progress")) copy_bool_array(j.at("world3Progress"), d.world3Progress);
    if (j.contains("world4Progress")) copy_bool_array(j.at("world4Progress"), d.world4Progress);
    if (j.contains("world5Progress")) copy_bool_array(j.at("world5Progress"), d.world5Progress);

    if (j.contains("hovercape")) j.at("hovercape").get_to(d.hovercape);
    if (j.contains("truthblade")) j.at("truthblade").get_to(d.truthblade);
    if (j.contains("riley")) j.at("riley").get_to(d.riley);

    if (j.contains("universalTotalOrbsCollected"))
        d.universalTotalOrbsCollected = j.at("universalTotalOrbsCollected").get<vector<bool>>();

    if (j.contains("universalTotalOrbsExisting"))
        j.at("universalTotalOrbsExisting").get_to(d.universalTotalOrbsExisting);

    if (j.contains("achievementsUnlocked"))
    {
        vector<int> loadedInts = j.at("achievementsUnlocked").get<vector<int>>();
        d.achievementsUnlocked.clear();
        for (int val : loadedInts)
            d.achievementsUnlocked.push_back(val != 0);
    }

    if (j.contains("totalAchievementsUnlocked"))
        j.at("totalAchievementsUnlocked").get_to(d.totalAchievementsUnlocked);

    if (j.contains("totalAchievementsPossible"))
        j.at("totalAchievementsPossible").get_to(d.totalAchievementsPossible);

    if (j.contains("sfxvolume")) j.at("sfxvolume").get_to(d.sfxvolume);
    if (j.contains("musicvolume")) j.at("musicvolume").get_to(d.musicvolume);
    if (j.contains("fullscreen")) j.at("fullscreen").get_to(d.fullscreen);
    if (j.contains("currentCharacter")) j.at("currentCharacter").get_to(d.currentCharacter);
}

