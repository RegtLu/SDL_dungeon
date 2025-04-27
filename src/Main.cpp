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

// Processes input events from the SDL event queue.
// Return false if the application should stop.
bool ProcessInput()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
        if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE)
        {
            return false;
        }
        if (event.type == SDL_EVENT_KEY_UP)
        {
            // ! process_input(event.key.key);
        }
    }
    return true;
}

// Main loop of game
void MainLoop(SDLWindows *windows, int FPS = 60)
{
    while (ProcessInput())
    {
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
    vector<string> files = {"test.json"};
    TileLoader TileSet(files, TileSize);
    vector<vector<string>> TileMap(Height / TileSize, vector<string>(Width / TileSize, "house1"));
    SDLWindows MainWindows((string) "Game", Width, Height, &TileSet, &TileMap);

    if (!CheckWindowsInit(&MainWindows))
    {
        SDL_Quit();
        return -1;
    }
    MainLoop(&MainWindows);

    SDL_Quit();
    return 0;
}