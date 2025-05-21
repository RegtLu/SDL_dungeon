#include "Game.hpp"
#include "TileLoader.hpp"
#include "Input.hpp"
#include <iostream>

Game::Game(std::vector<std::vector<std::string>> *TileMap, InputManager *input_manager)
{
    this->TileMap = TileMap;
    this->input_manager = input_manager;
    Human = std::make_pair(3, 3);
}
void Game::update()
{
    if (input_manager->IsKeyDown(SDL_SCANCODE_W))
    {
        (*TileMap)[Human.first][Human.second] = "NULL";
        Human.first = (Human.first - 1 >= 0) ? Human.first - 1 : TileMap->size() - 1;
    }
    if (input_manager->IsKeyDown(SDL_SCANCODE_S))
    {
        (*TileMap)[Human.first][Human.second] = "NULL";
        Human.first = (Human.first + 1 < TileMap->size()) ? Human.first + 1 : 0;
    }
    if (input_manager->IsKeyDown(SDL_SCANCODE_A))
    {
        (*TileMap)[Human.first][Human.second] = "NULL";
        Human.second = (Human.second - 1 >= 0) ? Human.second - 1 : (*TileMap)[0].size() - 1;
    }
    if (input_manager->IsKeyDown(SDL_SCANCODE_D))
    {
        (*TileMap)[Human.first][Human.second] = "NULL";
        Human.second = (Human.second + 1 < (*TileMap)[0].size()) ? Human.second + 1 : 0;
    }
    (*TileMap)[Human.first][Human.second] = "house1";
}