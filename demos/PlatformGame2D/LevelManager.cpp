//
// Created by Prajwal Prakash on 24/06/20.
//

#include "LevelManager.h"
#include <filesystem>
#include <fstream>


LevelManager::LevelManager(const char* assets_directory) : m_AssetsDirectory(assets_directory)
{
    LoadLevels();
}

void LevelManager::LoadLevels()
{
    for (const auto& entry : std::filesystem::directory_iterator(m_AssetsDirectory))
    {
        AM_INFO("Reading level from {}", entry.path().c_str());

        std::string line; // to read the file contents line by line
        std::ifstream level_file(entry.path());

        // Initialize data for the level
        std::string map;
        uint32_t width = 0, height = 0;

        while (std::getline(level_file, line))
        {
            map += line;
            height++;
        }

        width = line.size();

        Ambient::Ref<Level> level = std::make_shared<Level>();

        level->Map = map;
        level->Width = width;
        level->Height = height;

        m_Levels.push_back(level);
    }
}

