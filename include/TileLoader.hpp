#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <string>

class TileLoader
{
private:
    std::vector<std::string> tile_names;
    std::map<std::string, std::vector<uint32_t>> tiles;
    void loadTileset(const std::string &file);
    int TileSize;
    int resize;

public:
    TileLoader(std::vector<std::string> files, int TileSize, int resize = 1);
    std::vector<uint32_t> GetTile(std::string name);
    int GetTileSize();
};
