#include <vector>
#include <iostream>
#include <SDL.h>
#include "SDLWindows.cpp"
#include "Input.cpp"
#include "TileLoader.hpp"
using namespace std;

const int TileSize = 32;
const int Width = 960;
const int Height = 640;
const int FPS = 60;

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
void MainLoop(SDLWindows *windows,InputManager *input_manager, int FPS = 60)
{
    while (!Quit())
    {
        input_manager->Update();
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
    InputManager input_manager=InputManager();
    vector<string> files = {"test.json"};
    TileLoader TileSet(files, TileSize);
    vector<vector<string>> TileMap(Height / TileSize, vector<string>(Width / TileSize, "house1"));
    SDLWindows MainWindows((string) "Game", Width, Height, &TileSet, &TileMap);

    if (!CheckWindowsInit(&MainWindows))
    {
        SDL_Quit();
        return -1;
    }
    MainLoop(&MainWindows,&input_manager);

    SDL_Quit();
    return 0;
}