#pragma once

#include <SDL.h>
#include <map>

class InputManager
{
private:
    std::map<SDL_Keycode, int> KeyMap;
    // 基于SDL_Scancode.h
    int KeyboardLength = 513;
    const bool *CurrentKeyboardState;

public:
    enum KeyState
    {
        Up = 0,      // 0: 持续未按下
        Down = 1,    // 1: 持续按下
        Release = 2, // 2: 刚刚释放
        Press = 3    // 3: 刚刚按下
    };
    InputManager();
    void Update();
    bool IsKeyUp(SDL_Scancode code);
    bool IsKeyDown(SDL_Scancode code);
    bool IsKeyRelease(SDL_Scancode code);
    bool IsKeyPress(SDL_Scancode code);
};