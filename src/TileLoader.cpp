#include "TileLoader.hpp"
#include <fstream>
#include <iostream>
#include "json.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using json = nlohmann::json;

TileLoader::TileLoader(std::vector<std::string> files, int TileSize)
{
    this->TileSize = TileSize;
    std::vector<uint32_t> nullTile(TileSize * TileSize * 4);
    for (int i = 0; i < TileSize * TileSize; i++)
    {
        nullTile[i] = 0xff0000ff;
    }
    tiles["NULL"] = nullTile;
    tile_names.push_back("NULL");
    for (std::string &file : files)
    {
        loadTileset(file);
    }
}

void TileLoader::loadTileset(const std::string &file)
{
    std::ifstream f("tiles/" + file);
    if (!f.is_open())
    {
        //! JSON文件加载失败
    }
    json data = json::parse(f);
    f.close();
    std::string filePath = "tiles/" + data["file"].get<std::string>();
    int h = data["height"];
    int w = data["width"];
    int count = data["count"];
    this->tile_names = data["tiles"].get<std::vector<std::string>>();
    int imgWidth, imgHeight, channels;
    unsigned char *gTexture = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &channels, 4);
    if (!gTexture)
    {
        //! 图像加载失败
    }
    int index = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (index >= count)
                break;
            std::vector<uint32_t> tileData(TileSize * TileSize);
            for (int ty = 0; ty < TileSize; ty++)
            {
                for (int tx = 0; tx < TileSize; tx++)
                {
                    int src_x = x * TileSize + tx;
                    int src_y = y * TileSize + ty;
                    int srcIndex = (src_y * imgWidth + src_x) * 4;
                    uint32_t pixel =
                        (gTexture[srcIndex + 0] << 0) |
                        (gTexture[srcIndex + 1] << 8) |
                        (gTexture[srcIndex + 2] << 16) |
                        (gTexture[srcIndex + 3] << 24);
                    int dstIndex = ty * TileSize + tx;
                    tileData[dstIndex] = pixel;
                }
            }
            this->tiles[tile_names[index]] = tileData;
            index++;
        }
    }
    stbi_image_free(gTexture);
}

std::vector<uint32_t> TileLoader::GetTile(std::string name)
{
    if (std::find(tile_names.begin(), tile_names.end(), name) == tile_names.end())
    {
        //! 应改为提示图像
    }
    return tiles[name];
}

int TileLoader::GetTileSize()
{
    return TileSize;
}