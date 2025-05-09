#include "SDL.h"
#include "TileLoader.hpp"
#include <string>
using namespace std;
// A windows with it's own SDLWindow,SDLRenderer and SDLTexture
class SDLWindows
{
private:
    SDL_Window *SDLWindow;
    SDL_Renderer *SDLRenderer;
    SDL_Texture *SDLTexture;
    int *FrameBuffer;
    int WindowWidth;
    int WindowHeight;
    TileLoader *TileSet;
    vector<vector<string>> *TileMap;
    int TileSize;
    void SetTile(int x, int y, string name)
    {
        vector<uint32_t> tileData = TileSet->GetTile(name);
        for (int i = 0; i < TileSize; i++)
        {
            for (int j = 0; j < TileSize; j++)
            {
                int dst_x = x * TileSize + i;
                int dst_y = y * TileSize + j;
                if (dst_x >= WindowWidth || dst_y >= WindowHeight)
                    continue;
                int srcIndex = j * TileSize + i;
                uint32_t pixel = tileData[srcIndex];
                FrameBuffer[dst_y * WindowWidth + dst_x] = pixel;
            }
        }
    }

    void UpdateFrameBuffer()
    {
        fill(FrameBuffer, FrameBuffer + WindowWidth * WindowHeight, 0xff00ff00);
        for (int y = 0; y < WindowHeight / TileSize; y++)
        {
            for (int x = 0; x < WindowWidth / TileSize; x++)
            {
                string tileName = (*TileMap)[y][x];
                SetTile(x, y, tileName);
            }
        }
    }
    void RenderFrame()
    {
        uint8_t *pix;
        int pitch;
        SDL_LockTexture(SDLTexture, NULL, (void **)&pix, &pitch);
        for (int i = 0, texture_offset = 0, buffer_offset = 0; i < WindowHeight; i++, texture_offset += pitch, buffer_offset += WindowWidth)
            memcpy(pix + texture_offset, FrameBuffer + buffer_offset, WindowWidth * 4);
        SDL_UnlockTexture(SDLTexture);
        SDL_RenderTexture(SDLRenderer, SDLTexture, NULL, NULL);
        SDL_RenderPresent(SDLRenderer);
    }

public:
    SDLWindows(string Title = "Window", int WindowWidth = 960, int WindowHeight = 540, TileLoader *TileSet = nullptr, vector<vector<string>> *TileMap = nullptr)
    {
        this->WindowWidth = WindowWidth;
        this->WindowHeight = WindowHeight;
        FrameBuffer = new int[WindowWidth * WindowHeight];
        SDLWindow = SDL_CreateWindow(Title.c_str(), WindowWidth, WindowHeight, 0);
        SDLRenderer = SDL_CreateRenderer(SDLWindow, NULL);
        SDLTexture = SDL_CreateTexture(SDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);
        this->TileSet = TileSet;
        this->TileMap = TileMap;
        TileSize = TileSet->GetTileSize();
    }
    ~SDLWindows()
    {
        SDL_DestroyTexture(SDLTexture);
        SDL_DestroyRenderer(SDLRenderer);
        SDL_DestroyWindow(SDLWindow);
    }
    // Checks if the initialization of SDL components was successful.
    bool initSuccessful()
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
    void Update()
    {
        UpdateFrameBuffer();
        RenderFrame();
    }
};