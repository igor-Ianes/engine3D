#ifndef _GRAPHICS3D_H
#define _GRAPHICS3D_H

#include "InputManager.h"

#include <vector>
#include <fstream>
#include <string>
#include <list>

#include "Timer.h"
#include "math3d.hpp"
#include "systemGraphics.hpp"
#include "draws.hpp"

#define SCREENHEIGHT 600
#define SCREENWIDTH 800

struct Mesh
{
	std::vector<Triangle> tris;
	bool LoadFromObjectFile(std::string sFilename);
};

class Graphics3d {

    static Graphics3d* sInstance;

private:
    float x;
    float y;

public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    InputManager *input;
    Timer *m_Timer;
 
public:
    Graphics3d();
    void events(InputManager *GameManager);
    void draw_all();
    static Graphics3d* Instance(SDL_Window* window, SDL_Renderer * renderer, InputManager *input, Timer* m_Timer);
    static void Release();
    void run(SDL_Window* window, SDL_Renderer * renderer, InputManager *input, Timer *m_Timer);
    void Render();


private:
	Mesh MeshCube;
	Mat4x4 matProj; 		 // Matrix that converts from view space to screen space
	Vec3d vCamera = {0,0,0}; // Location of camera in world space
	Vec3d vLookDir;			 // Direction vector along the direction camera points
	float fYaw;				 // FPS Camera rotation in XZ plane
	float fTheta;            // Spins World transform

	olcSprite *sprTex1;

	float *pDepthBuffer = nullptr;

public:
	bool OnUserCreate();

    bool OnUserUpdate(float fElapsedTime);
};

#endif