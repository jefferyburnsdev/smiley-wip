#include "SaveManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = filesystem;
using namespace std;
using json = nlohmann::json;

bool SaveManager::saveToFile(const SaveData& data, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) return false;

    json j = data;
    out << j.dump(4); // pretty print with 4 spaces indentation
    out.close();
    return true;
}

bool SaveManager::loadFromFile(SaveData& data, const string& filename) {
    ifstream in(filename);
    if (!in || in.peek() == ifstream::traits_type::eof()) {
        return false;
    }

    json j;
    try {
        in >> j;
    }
    catch (const nlohmann::json::parse_error& e) {
        cerr << "JSON parse error: " << e.what() << '\n';
        return false;
    }
    data = j.get<SaveData>();
    in.close();
    return true;
}

bool SaveManager::isSaveFileEmpty(const string& filename) {
    SaveData data;
    if (!SaveManager::loadFromFile(data, filename)) {
        return true;
    }
    return data.saveName.empty();
}

bool SaveManager::deleteSaveData(int fileIndex) {
    string filename = "saves/save" + to_string(fileIndex + 1) + ".json";

    try {
        if (fs::exists(filename)) {
            fs::remove(filename);
        }

        return true;
    }
    catch (const exception& e) {
        cerr << "Failed to delete save file: " << e.what() << endl;
        return false;
    }
}
