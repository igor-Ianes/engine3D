#include "graphics3d.hpp"
#include <iostream>

Graphics3d* Graphics3d::sInstance = NULL;

Graphics3d* Graphics3d::Instance(SDL_Window* window, SDL_Renderer * renderer, InputManager *input, Timer *m_Timer) {

    if(sInstance == NULL){
        sInstance = new Graphics3d();
        sInstance->run(window, renderer, input, m_Timer);
    }
    return sInstance;
}

Graphics3d::Graphics3d(){
 
}

void Graphics3d::Release() {
    delete sInstance;
    sInstance = NULL;
}

void Graphics3d::events(InputManager *input){
  float fElapsedTime = m_Timer->TimeScale() / 100;

        if (input->KeyDown(SDL_SCANCODE_UP))
			vCamera.y += 8.0f * fElapsedTime;	// Travel Upwards

		if (input->KeyDown(SDL_SCANCODE_DOWN))
			vCamera.y -= 8.0f * fElapsedTime;	// Travel Downwards


		// Dont use these two in FPS mode, it is confusing :P
		if (input->KeyDown(SDL_SCANCODE_LEFT))
			vCamera.x -= 8.0f * fElapsedTime;	// Travel Along X-Axis

		if (input->KeyDown(SDL_SCANCODE_RIGHT))
			vCamera.x += 8.0f * fElapsedTime;	// Travel Along X-Axis
		///////


		Vec3d vForward = Vector_Mul(vLookDir, 8.0f * fElapsedTime);

		// Standard FPS Control scheme, but turn instead of strafe
		if (input->KeyDown(SDL_SCANCODE_W))
			vCamera = Vector_Add(vCamera, vForward);

		if (input->KeyDown(SDL_SCANCODE_S))
			vCamera = Vector_Sub(vCamera, vForward);

		if (input->KeyDown(SDL_SCANCODE_A))
			fYaw -= 2.0f * fElapsedTime;

		if (input->KeyDown(SDL_SCANCODE_D))
			fYaw += 2.0f * fElapsedTime;
}

void Graphics3d::draw_all(){

    if(Graphics3d::OnUserUpdate(m_Timer->TimeScale() / 100) == false){
        std::cout << "Erro ao criar objeto!" <<std::endl;
    }

    events(input);

}
               
void Graphics3d::run(SDL_Window* window, SDL_Renderer * renderer, InputManager *input,  Timer *m_Timer){
     this->window = window;
     this->renderer = renderer;
     this->input = input;
     this->m_Timer = m_Timer;

    if(Graphics3d::OnUserCreate() == false){
        std::cout << "Erro ao criar objeto!" <<std::endl;
    }

}

void Graphics3d::Render() {
   
   this->draw_all();
}

bool Mesh::LoadFromObjectFile(std::string sFilename)
{
    std::ifstream f(sFilename);
    if (!f.is_open())
        return false;

    // Local cache of verts
    std::vector<Vec3d> verts;

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        std::stringstream s;
        s << line;

        char junk;

        if (line[0] == 'v')
        {
            Vec3d v;
            s >> junk >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }

        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
        }
    }

    return true;
}

bool Graphics3d::OnUserCreate() {

    pDepthBuffer = new float[SCREENWIDTH * SCREENHEIGHT];

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Load from object file
    MeshCube.LoadFromObjectFile("Assets/cubo.obj");
    //MeshCube.LoadFromObjectFile("Assets/esfera.obj");
    //MeshCube.LoadFromObjectFile("Assets/quarto.obj");
    //MeshCube.LoadFromObjectFile("Assets/teapot.obj");
    //MeshCube.LoadFromObjectFile("Assets/axis.obj");
    //MeshCube.LoadFromObjectFile("Assets/mountains.obj");

    sprTex1 = new olcSprite(L"Assets/minijario.spr");

    // Projection Matrix
    matProj = Matrix_MakeProjection(90.0f, (float)SCREENHEIGHT / (float)SCREENWIDTH, 0.1f, 1000.0f);
    return true;
}

bool Graphics3d::OnUserUpdate(float fElapsedTime) {
    // Set up rotation matrices
    Mat4x4 matRotZ, matRotX;
    fTheta += 1.0f * fElapsedTime; // Uncomment to spin me right round baby right round

    matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
    matRotX = Matrix_MakeRotationX(fTheta);

    Mat4x4 matTrans;
    matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

    Mat4x4 matWorld;
    matWorld = Matrix_MakeIdentity();	// Form World Matrix
    matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

    // Create "Point At" Matrix for camera
    Vec3d vUp = { 0,1,0 };
    Vec3d vTarget = { 0,0,1 };
    Mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
    vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
    vTarget = Vector_Add(vCamera, vLookDir);
    Mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

    // Make view matrix from camera
    Mat4x4 matView = Matrix_QuickInverse(matCamera);

    // Store triagles for rastering later
    std::vector<Triangle> vecTrianglesToRaster;
    
    // Draw Triangles
    for (auto tri : MeshCube.tris)
    {
        Triangle triProjected, triTransformed, triViewed;

        // World Matrix Transform
        triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
        triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
        triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
        triTransformed.t[0] = tri.t[0];
        triTransformed.t[1] = tri.t[1];
        triTransformed.t[2] = tri.t[2];
        
        // Calculate triangle Normal
        Vec3d normal, line1, line2;

        // Get lines either side of triangle
        line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
        line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

        // Take cross product of lines to get normal to triangle surface
        normal = Vector_CrossProduct(line1, line2);

        // You normally need to normalise a normal!
        normal = Vector_Normalise(normal);

        // Get Ray from triangle to camera
        Vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);
        
        // If ray is aligned with normal, then triangle is visible
        if(Vector_DotProduct(normal, vCameraRay) < 0.0f)
        {
            // Illumination
            Vec3d light_direction = { 0.0f, 1.0f, -1.0f };
            light_direction = Vector_Normalise(light_direction);

            // How "aligned" are light direction and triangle surface normal?
            float dp = std::max(0.1f, Vector_DotProduct(light_direction, normal));

            // Choose console colours as required (much easier with RGB)
            CHAR_INFO c = Draws::GetColour(dp);
            triTransformed.col = c.Attributes;
            triTransformed.sym = c.Char.UnicodeChar;
        
        // Convert World Space --> View Space
        triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
        triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
        triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
        triViewed.sym = triTransformed.sym;
        triViewed.col = triTransformed.col;
        triViewed.t[0] = triTransformed.t[0]; // new
        triViewed.t[1] = triTransformed.t[1]; // new
        triViewed.t[2] = triTransformed.t[2]; // new

        // Clip Viewed Triangle against near plane, this could form two additional
        // additional triangles. 
        int nClippedTriangles = 0;
        Triangle clipped[2];
        nClippedTriangles =  Draws::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

        // We may end up with multiple triangles form the clip, so project as
        // required
        for (int n = 0; n < nClippedTriangles; n++)
        {

        // Project Triangles from 3D --> 2D
        triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
        triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
        triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);
        triProjected.col = clipped[n].col;
        triProjected.sym = clipped[n].sym;
        triProjected.t[0] = clipped[n].t[0];
        triProjected.t[1] = clipped[n].t[1];
        triProjected.t[2] = clipped[n].t[2];


        triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
        triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
        triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

        triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
        triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
        triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

        triProjected.t[0].w = 1.0f / triProjected.p[0].w;
        triProjected.t[1].w = 1.0f / triProjected.p[1].w;
        triProjected.t[2].w = 1.0f / triProjected.p[2].w;

        // Scale into view, we moved the normalising into cartesian space
        // out of the matrix.vector function from the previous videos, so
        // do this manually
        triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
        triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
        triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);
        
        // X/Y are inverted so put them back
        triProjected.p[0].x *= -1.0f;
        triProjected.p[1].x *= -1.0f;
        triProjected.p[2].x *= -1.0f;
        triProjected.p[0].y *= -1.0f; // new block
        triProjected.p[1].y *= -1.0f;
        triProjected.p[2].y *= -1.0f;

        // Offset verts into visible normalised space
        Vec3d vOffsetView = { 1,1,0 };
        triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
        triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
        triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);
        triProjected.p[0].x *= 0.5f * (float)SCREENWIDTH;
        triProjected.p[0].y *= 0.5f * (float)SCREENHEIGHT;
        triProjected.p[1].x *= 0.5f * (float)SCREENWIDTH;
        triProjected.p[1].y *= 0.5f * (float)SCREENHEIGHT;
        triProjected.p[2].x *= 0.5f * (float)SCREENWIDTH;
        triProjected.p[2].y *= 0.5f * (float)SCREENHEIGHT;

        // Store Triangle for sorting
        vecTrianglesToRaster.push_back(triProjected);
        }
      }
    }
        // Clear Screen
        SDL_RenderClear(renderer);

        // Clear Depth Buffer
        for (int i = 0; i < SCREENWIDTH*SCREENHEIGHT; i++)
            pDepthBuffer[i] = 0.0f;


        for (auto &triToRaster : vecTrianglesToRaster)
    {
        // Clip triangles against all four screen edges, this could yield
        // a bunch of triangles, so create a queue that we traverse to 
        //  ensure we only test new triangles generated against planes
        Triangle clipped[2];
        std::list<Triangle> listTriangles;

        // Add initial triangle
        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++)
        {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                // Take triangle from front of queue
                Triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                // Clip it against a plane. We only need to test each 
                // subsequent plane, against subsequent new triangles
                // as all triangles after a plane clip are guaranteed
                // to lie on the inside of the plane. I like how this
                // comment is almost completely and utterly justified
                switch (p)
                {
                case 0:	nTrisToAdd =  Draws::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 1:	nTrisToAdd =  Draws::Triangle_ClipAgainstPlane({ 0.0f, (float)SCREENHEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 2:	nTrisToAdd =  Draws::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 3:	nTrisToAdd =  Draws::Triangle_ClipAgainstPlane({ (float)SCREENWIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                // Clipping may yield a variable number of triangles, so
                // add these new ones to the back of the queue for subsequent
                // clipping against next planes
                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }


        // Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
        for (auto &t : listTriangles)
        {   Draws::TexturedTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
                t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
                t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w, sprTex1, renderer, pDepthBuffer);
         
            Draws::DrawTriangle(t.p[0].x, t.p[0].y,t.p[1].x, t.p[1].y,t.p[2].x, t.p[2].y, renderer);
            Draws::FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.sym, t.col, renderer);
        }
    }
    
    return true;
}