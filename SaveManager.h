#pragma once
#include <string>
#include <fstream>
#include "SaveData.h"

using namespace std;

class SaveManager {
public:
    static bool saveToFile(const SaveData& data, const string& filename);
    static bool loadFromFile(SaveData& data, const string& filename);
    static bool isSaveFileEmpty(const string& filename);
    static bool deleteSaveData(int fileIndex);
};
