#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>

class Graphics {

public:
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGH = 600;

private:

    static Graphics* sInstance;
    static bool sInitialized;

    SDL_Window* mWindow;
    SDL_Surface* mBackBuffer;

    SDL_Renderer* mRenderer;

public:

    static Graphics* Instance();
    static void Release();
    static bool Initialized();

    SDL_Texture*  LoadTexture(std::string path);
    SDL_Texture* CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color);

    void ClearBackBuffer();

    void DrawTexture(SDL_Texture* tex, SDL_Rect* clip = NULL, SDL_Rect* rend = NULL, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void Render();

    SDL_Window* GetWindow();

    SDL_Renderer* GetRender();

private:

    Graphics();
    ~Graphics();

    bool Init();
};

#endif