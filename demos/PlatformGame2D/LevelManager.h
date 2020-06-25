//
// Created by Prajwal Prakash on 24/06/20.
//

#ifndef SANDBOX_LEVELMANAGER_H
#define SANDBOX_LEVELMANAGER_H

#include "Ambient.h"
#include "LevelManager.h"


struct Level
{
    std::string Map;
    uint32_t Width, Height;

    template <typename T>
    char GetTile(T arg1, T arg2) const
    {
        int x = (int)arg1;
        int y = (int)arg2;

        if (x >= 0 && x < Width && y >= 0 && y < Height)
        {
            return Map[y * Width + x];
        }
        else
        {
            return ' ';
        }
    }
};


class LevelManager
{
public:
    LevelManager() {}

    LevelManager(const char* assets_directory);
    ~LevelManager() = default;

    // Load all the levels in the assets directory
    void LoadLevels();
    Ambient::Ref<Level> GetLevel(uint32_t level_index) { return m_Levels[level_index]; }

private:
    const char* m_AssetsDirectory;
    std::vector<Ambient::Ref<Level>> m_Levels;
};


#endif //SANDBOX_LEVELMANAGER_H
