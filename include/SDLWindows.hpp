#pragma once
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
    int TileResize;
    vector<vector<string>> *TileMap;
    int TileSize;
    void SetTile(int x, int y, string name);
    void UpdateFrameBuffer();
    void RenderFrame();

public:
    SDLWindows(string Title = "Window", int WindowWidth = 960, int WindowHeight = 540, int TileResize = 1, TileLoader *TileSet = nullptr, vector<vector<string>> *TileMap = nullptr);
    ~SDLWindows();
    // Checks if the initialization of SDL components was successful.
    bool initSuccessful();
    vector<vector<string>> *GetTileMap();
    void Update();
};