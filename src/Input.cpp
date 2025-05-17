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
    InputManager()
    {
        CurrentKeyboardState = SDL_GetKeyboardState(NULL);
        for (int i = 0; i < this->KeyboardLength; i++)
        {
            KeyMap[static_cast<SDL_Scancode>(i)] = Up;
        }
    }
    void Update()
    {
        SDL_PumpEvents();
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
    bool IsKeyUp(SDL_Scancode code){
        return KeyMap[static_cast<SDL_Scancode>(code)]==Up;
    }
    bool IsKeyDown(SDL_Scancode code){
        return KeyMap[static_cast<SDL_Scancode>(code)]==Down;
    }
    bool IsKeyRelease(SDL_Scancode code){
        return KeyMap[static_cast<SDL_Scancode>(code)]==Release;
    }
    bool IsKeyPress(SDL_Scancode code){
        return KeyMap[static_cast<SDL_Scancode>(code)]==Press;
    }
};