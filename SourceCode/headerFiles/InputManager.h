#ifndef _INPUTMANAGER
#define _INPUTMANAGER
#include <SDL2/SDL.h>
#include<string.h>
#include "MathHelper.h"

class InputManager {

public:

    enum MOUSE_BUTTON {left = 0, right, middle, back, forward};

private:

    static InputManager* sInstance;

    Uint8* mPrevKeyboardState;
    const Uint8* mKeyboardState;
    int mKeyLength;

    Uint32 mPrevMouseState;
    Uint32 mMouseState;

    int mMouseXPos;
    int mMouseYPos;

public:

    static InputManager* Instance();
    static void Release();

    bool KeyDown(SDL_Scancode scancode);
    bool KeyPressed(SDL_Scancode scancode);
    bool KeyReleased(SDL_Scancode scancode);

    bool MouseButtonDown(MOUSE_BUTTON button);
    bool MouseButtonPressed(MOUSE_BUTTON button);
    bool MouseButtonReleased(MOUSE_BUTTON button);

    Vector2 MousePos();

    void Update();
    void UpdatePrevInput();

private:

    InputManager();
    ~InputManager();


};

#endif