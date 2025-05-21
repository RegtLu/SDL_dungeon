#include "SDLWindows.hpp"
#include "SDL.h"
#include "TileLoader.hpp"
#include <string>
using namespace std;

void SDLWindows::SetTile(int x, int y, string name)
{
    vector<uint32_t> tileData = TileSet->GetTile(name);
    for (int i = 0; i < TileSize * TileResize; i++)
    {
        for (int j = 0; j < TileSize * TileResize; j++)
        {
            int dst_x = x * TileSize * TileResize + i;
            int dst_y = y * TileSize * TileResize + j;
            if (dst_x >= WindowWidth || dst_y >= WindowHeight)
                continue;
            int srcIndex = j * TileSize * TileResize + i;
            uint32_t pixel = tileData[srcIndex];
            FrameBuffer[dst_y * WindowWidth + dst_x] = pixel;
        }
    }
}

void SDLWindows::UpdateFrameBuffer()
{
    fill(FrameBuffer, FrameBuffer + WindowWidth * WindowHeight, 0x0000000);
    for (int y = 0; y < WindowHeight / TileSize / TileResize; y++)
    {
        for (int x = 0; x < WindowWidth / TileSize / TileResize; x++)
        {
            string tileName = (*TileMap)[y][x];
            SetTile(x, y, tileName);
        }
    }
}
void SDLWindows::RenderFrame()
{
    uint8_t *pix;
    int pitch;
    SDL_RenderClear(SDLRenderer);
    SDL_LockTexture(SDLTexture, NULL, (void **)&pix, &pitch);
    for (int i = 0, texture_offset = 0, buffer_offset = 0; i < WindowHeight; i++, texture_offset += pitch, buffer_offset += WindowWidth)
        memcpy(pix + texture_offset, FrameBuffer + buffer_offset, WindowWidth * 4);
    SDL_UnlockTexture(SDLTexture);
    SDL_RenderTexture(SDLRenderer, SDLTexture, NULL, NULL);
    SDL_RenderPresent(SDLRenderer);
}

SDLWindows::SDLWindows(string Title, int WindowWidth, int WindowHeight, int TileResize, TileLoader *TileSet, vector<vector<string>> *TileMap)
{
    this->WindowWidth = WindowWidth;
    this->WindowHeight = WindowHeight;
    this->TileResize = TileResize;
    FrameBuffer = new int[WindowWidth * WindowHeight];
    SDLWindow = SDL_CreateWindow(Title.c_str(), WindowWidth, WindowHeight, 0);
    SDLRenderer = SDL_CreateRenderer(SDLWindow, NULL);
    SDLTexture = SDL_CreateTexture(SDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);
    this->TileSet = TileSet;
    this->TileMap = TileMap;
    TileSize = TileSet->GetTileSize();
}
SDLWindows::~SDLWindows()
{
    SDL_DestroyTexture(SDLTexture);
    SDL_DestroyRenderer(SDLRenderer);
    SDL_DestroyWindow(SDLWindow);
    delete[] FrameBuffer;
}
// Checks if the initialization of SDL components was successful.
bool SDLWindows::initSuccessful()
{
    if (!FrameBuffer || !SDLWindow || !SDLRenderer || !SDLTexture)
    {
        return false;
    }
    else
    {
        return true;
    }
}
vector<vector<string>> *SDLWindows::GetTileMap()
{
    return this->TileMap;
}
void SDLWindows::Update()
{
    UpdateFrameBuffer();
    RenderFrame();
}