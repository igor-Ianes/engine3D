#ifndef  _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include "InputManager.h"
#include "graphics3d.hpp"
#include  "Graphics.h"


class GameManager {

    private:

    static GameManager* sInstance;

    const int FRAME_RATE = 60;

    bool mQuit;
    Graphics* mGraphics;
    InputManager* mInputMgr;

    Timer* mTimer;

    SDL_Event mEvents;

    Graphics3d* graphics3d;


public:

    static GameManager* Instance();
    static void Release();

    void Run();

private: 

    GameManager();
    ~GameManager();

    void EarlyUpdate();
    void Update();
    void LateUpdate();

    void Render();
};

#endif