#include <vector>
#include <iostream>
#include <SDL.h>
#include "SDLWindows.hpp"
#include "TileLoader.hpp"
#include "Game.hpp"
using namespace std;

const int TileSize = 32;
const int TileResize = 2;
const int Width = 960;
const int Height = 640;
const int FPS = 10;

// Checks if the initialization of all the windows were successful.
bool CheckWindowsInit(SDLWindows *windows)
{
    if (!windows->initSuccessful())
    {
        // !窗口初始化失败
        return false;
    }
    return true;
}

// Return true if the application should stop.
bool Quit()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return true;
        }
    }
    return false;
}

// Main loop of game
void MainLoop(SDLWindows *windows, InputManager *input_manager, int FPS = 60)
{
    auto TileMap = windows->GetTileMap();
    auto game = Game(TileMap, input_manager);
    while (!Quit())
    {
        input_manager->Update();
        game.update();
        windows->Update();
        SDL_Delay(1000 / FPS);
    }
}

int main(int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        return -1;
    }
    InputManager input_manager = InputManager();
    vector<string> files = {"test.json"};
    TileLoader TileSet(files, TileSize, TileResize);
    vector<vector<string>> TileMap(Height / TileSize / TileResize, vector<string>(Width / TileSize / TileResize, "NULL"));
    SDLWindows MainWindows((string) "Game", Width, Height, TileResize, &TileSet, &TileMap);

    if (!CheckWindowsInit(&MainWindows))
    {
        SDL_Quit();
        return -1;
    }
    MainLoop(&MainWindows, &input_manager);

    SDL_Quit();
    return 0;
}