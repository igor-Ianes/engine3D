#ifndef _DRAWS_HPP
#define _DRAWS_HPP

#include <SDL2/SDL.h>
#include <Windows.h>
#include "systemGraphics.hpp"
#include "math3d.hpp"

#define SCREENHEIGHT 600
#define SCREENWIDTH 800

enum PIXEL_TYPE
{
 // indica nivel de transparencia dos pixel ja que a library não soporta meio pixel
	PIXEL_SOLID = 255,
	PIXEL_THREEQUARTERS = 191,
	PIXEL_HALF = 127,
	PIXEL_QUARTER = 63,
};

class Draws {

    public:
    static SDL_Color g_Colors[19];

    public:
    Draws();
    static void Draw(int x, int y, wchar_t in = 255, short col = 19,  SDL_Renderer* renderer = nullptr);
    static void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F, SDL_Renderer* renderer = nullptr);
    static CHAR_INFO GetColour(float lum);
    static void TexturedTriangle(	int x1, int y1, float u1, float v1, float w1,
                            int x2, int y2, float u2, float v2, float w2,
                            int x3, int y3, float u3, float v3, float w3,
                            olcSprite *tex, SDL_Renderer* renderer, float *pDepthBuffer);
    static int Triangle_ClipAgainstPlane(Vec3d plane_p, Vec3d plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2);
    static void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, SDL_Renderer* renderer = nullptr);
};


#endif