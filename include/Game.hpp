#pragma once

#include "Input.hpp"
#include "SDL.h"
#include <vector>
#include <string>

class Game
{
private:
    std::pair<int, int> Human;
    std::vector<std::vector<std::string>> *TileMap;
    InputManager *input_manager;

public:
    Game(std::vector<std::vector<std::string>> *TileMap, InputManager *input_manager);
    void update();
};