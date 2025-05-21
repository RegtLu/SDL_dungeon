#include "Input.hpp"
#include <SDL.h>
#include <map>

InputManager::InputManager()
{
    CurrentKeyboardState = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < this->KeyboardLength; i++)
    {
        KeyMap[static_cast<SDL_Scancode>(i)] = Up;
    }
}
void InputManager::Update()
{
    for (int code = 0; code < this->KeyboardLength; code++)
    {
        bool isDownNow = CurrentKeyboardState[code] != 0;
        int &state = KeyMap[static_cast<SDL_Scancode>(code)];
        switch (state)
        {
        case Up:
            state = isDownNow ? Press : Up;
            break;
        case Release:
            state = isDownNow ? Press : Up;
            break;
        case Down:
            state = isDownNow ? Down : Release;
            break;
        case Press:
            state = isDownNow ? Down : Release;
            break;
        }
    }
}
bool InputManager::IsKeyUp(SDL_Scancode code)
{
    return KeyMap[static_cast<SDL_Scancode>(code)] == Up;
}
bool InputManager::IsKeyDown(SDL_Scancode code)
{
    return KeyMap[static_cast<SDL_Scancode>(code)] == Down;
}
bool InputManager::IsKeyRelease(SDL_Scancode code)
{
    return KeyMap[static_cast<SDL_Scancode>(code)] == Release;
}
bool InputManager::IsKeyPress(SDL_Scancode code)
{
    return KeyMap[static_cast<SDL_Scancode>(code)] == Press;
}