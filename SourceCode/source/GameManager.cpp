#include "GameManager.h"
#include "Timer.h"

GameManager * GameManager::sInstance = NULL;

GameManager * GameManager::Instance() {

    if(sInstance == NULL)
        sInstance = new GameManager();

    return sInstance;
}

void GameManager::Release() {

    delete sInstance;
    sInstance = NULL;
}

GameManager::GameManager() {

    mQuit = false;

    mGraphics = Graphics::Instance();

    if(!Graphics::Initialized())
        mQuit = true;

    mInputMgr = InputManager::Instance();

    mTimer = Timer::Instance();

    graphics3d = Graphics3d::Instance(mGraphics->GetWindow(), mGraphics->GetRender(), mInputMgr, mTimer);

}

GameManager::~GameManager() {

    Graphics::Release();
    mGraphics = NULL;

    InputManager::Release();
    mInputMgr = NULL;

    Timer::Release();
    mTimer = NULL;

    delete graphics3d;
    graphics3d = NULL;

}

void GameManager::EarlyUpdate() {
      mTimer->Reset();

}

void GameManager::Update() {
    
            mInputMgr->Update();
}

void GameManager::Render(){
        mGraphics->ClearBackBuffer();    //SDL_RenderClear
        graphics3d->Render();
        mGraphics->Render();             //SDL_RenderPresent
}

void GameManager::LateUpdate(){ 
        mInputMgr->UpdatePrevInput();
        
}

void GameManager::Run() {

    while(!mQuit) {

        mTimer->Update();

        while(SDL_PollEvent(&mEvents) != 0){
            if(mEvents.type == SDL_QUIT) {

                mQuit = true;
            }
        }

        if(mTimer->DeltaTime() >= (1.0f / FRAME_RATE)){

            EarlyUpdate();
            Update();
            LateUpdate(); // colision and phisics
            Render();
        }
    }
}