#include "draws.hpp"

Draws::Draws() {

}

SDL_Color Draws::g_Colors[19] = {
    {0, 0, 0, 0},      // 0 FG_BLACK
    {0, 0, 127, 0},    // 1
    {0, 127, 0, 0},    // 2
    {0, 127, 127, 0},  // 3
    {127, 0, 0, 0},    // 4
    {127, 0, 127, 0},  // 5
    {127, 127, 0, 0},  // 6
    {127, 127, 127, 0},// 7
    {0, 0, 0, 0},
    {0, 0, 255, 0},    // 9
    {0, 255, 0, 0},    // A
    {0, 255, 255, 0},  // B
    {255, 0, 0, 0},    // C
    {255, 0, 255, 0},  // D
    {255, 255, 0, 0},  // E
    {255, 255, 255, 0},// F
    {169, 169, 169, 1},// FG_DARK_GREY
    {128, 128, 128, 1},// BG_DARK_GREY
    {255, 255, 255, 1} // FG_WHITE
};

void Draws::Draw(int x, int y, wchar_t in, short col,  SDL_Renderer* renderer)
{ 
    SDL_Color &c = Draws::g_Colors[col - 1];
    SDL_Rect rect {x,y,4,4};
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, in);
    SDL_RenderDrawPoint(renderer, rect.x, rect.y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
}

void Draws::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, SDL_Renderer* renderer)
{   
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1); // neutro
}

void Draws::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col, SDL_Renderer* renderer)
{
    auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
    auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, c, col, renderer); };
    
    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;   // Starting points
    dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2) goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) t1xp = signx1;//t1x += signx1;
                else          goto next1;
            }
            if (changed1) break;
            else t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;//t2x += signx2;
                else          goto next2;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next2:
        if (minx>t1x)
             minx = t1x; 
        if (minx>t2x) 
            minx = t2x;
        if (maxx<t1x) 
            maxx = t1x;
         if (maxx<t2x) 
             maxx = t2x;

        drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                     // Now increase y
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2) break;

    }
next:
    // Second half
    dx1 = (int)(x3 - x2);
     if (dx1<0) {
         dx1 = -dx1; signx1 = -1; 
     }
    else 
        signx1 = 1;

    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else   	   	  t1x += signx1;
            if (i<dx1) i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next4:

        if (minx>t1x) 
            minx = t1x;
        if (minx>t2x) 
            minx = t2x;
        if (maxx<t1x) 
        maxx = t1x;
        if (maxx<t2x)
              maxx = t2x;
        drawline(minx, maxx, y);   										
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y>y3) 
            return;
    }
}


// numeros indical posisão no vetor de cores
CHAR_INFO Draws::GetColour(float lum)
{
    short bg_col, fg_col;
    wchar_t sym;
    int pixel_bw = (int)(13.0f*lum);
    switch (pixel_bw)
    {
    case 0: bg_col = 0; fg_col = 0; sym = PIXEL_SOLID; break;

    case 1: bg_col = 0; fg_col = 17; sym = PIXEL_QUARTER; break;
    case 2: bg_col = 0; fg_col = 17; sym = PIXEL_HALF; break;
    case 3: bg_col = 0; fg_col = 17; sym = PIXEL_THREEQUARTERS; break;
    case 4: bg_col = 0; fg_col = 17; sym = PIXEL_SOLID; break;

    case 5: bg_col = 17; fg_col = 18; sym = PIXEL_QUARTER; break;
    case 6: bg_col = 17; fg_col = 18; sym = PIXEL_HALF; break;
    case 7: bg_col = 17; fg_col = 18; sym = PIXEL_THREEQUARTERS; break;
    case 8: bg_col = 17; fg_col = 18; sym = PIXEL_SOLID; break;

    case 9:  bg_col = 18; fg_col = 19; sym = PIXEL_QUARTER; break;
    case 10: bg_col = 18; fg_col = 19; sym = PIXEL_HALF; break;
    case 11: bg_col = 18; fg_col = 19; sym = PIXEL_THREEQUARTERS; break;
    case 12: bg_col = 18; fg_col = 19; sym = PIXEL_SOLID; break;
    default:
        bg_col = 0; fg_col = 0; sym = PIXEL_SOLID;
    }

    CHAR_INFO c;
    c.Attributes = bg_col | fg_col;
    c.Char.UnicodeChar = sym;
    return c;
}

void Draws::TexturedTriangle(	int x1, int y1, float u1, float v1, float w1,
                        int x2, int y2, float u2, float v2, float w2,
                        int x3, int y3, float u3, float v3, float w3,
                        olcSprite *tex, SDL_Renderer* renderer, float *pDepthBuffer)
{
    if (y2 < y1)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);
        std::swap(u1, u2);
        std::swap(v1, v2);
        std::swap(w1, w2);
    }

    if (y3 < y1)
    {
        std::swap(y1, y3);
        std::swap(x1, x3);
        std::swap(u1, u3);
        std::swap(v1, v3);
        std::swap(w1, w3);
    }

    if (y3 < y2)
    {
    std::swap(y2, y3);
    std::swap(x2, x3);
    std::swap(u2, u3);
    std::swap(v2, v3);
    std::swap(w2, w3);
    }

    int dy1 = y2 - y1;
    int dx1 = x2 - x1;
    float dv1 = v2 - v1;
    float du1 = u2 - u1;
    float dw1 = w2 - w1;

    int dy2 = y3 - y1;
    int dx2 = x3 - x1;
    float dv2 = v3 - v1;
    float du2 = u3 - u1;
    float dw2 = w3 - w1;

    float tex_u, tex_v, tex_w;

    float dax_step = 0, dbx_step = 0,
          du1_step = 0, dv1_step = 0,
          du2_step = 0, dv2_step = 0,
          dw1_step=0, dw2_step=0;

    if (dy1) dax_step = dx1 / (float)abs(dy1);
    if (dy2) dbx_step = dx2 / (float)abs(dy2);

    if (dy1) du1_step = du1 / (float)abs(dy1);
    if (dy1) dv1_step = dv1 / (float)abs(dy1);
    if (dy1) dw1_step = dw1 / (float)abs(dy1);

    if (dy2) du2_step = du2 / (float)abs(dy2);
    if (dy2) dv2_step = dv2 / (float)abs(dy2);
    if (dy2) dw2_step = dw2 / (float)abs(dy2);

    if (dy1)
    {
        for (int i = y1; i <= y2; i++)
        {
            int ax = x1 + (float)(i - y1) * dax_step;
            int bx = x1 + (float)(i - y1) * dbx_step;

            float tex_su = u1 + (float)(i - y1) * du1_step;
            float tex_sv = v1 + (float)(i - y1) * dv1_step;
            float tex_sw = w1 + (float)(i - y1) * dw1_step;

            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;
            float tex_ew = w1 + (float)(i - y1) * dw2_step;

            if (ax > bx)
            {
                std::swap(ax, bx);
                std::swap(tex_su, tex_eu);
                std::swap(tex_sv, tex_ev);
                std::swap(tex_sw, tex_ew);
            }

            tex_u = tex_su;
            tex_v = tex_sv;
            tex_w = tex_sw;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for (int j = ax; j < bx; j++)
            {
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                tex_w = (1.0f - t) * tex_sw + t * tex_ew;
            if (tex_w > pDepthBuffer[i*SCREENWIDTH + j])
            {
                Draws::Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w), renderer);
                pDepthBuffer[i*SCREENWIDTH + j] = tex_w;
            }
            t += tstep;
           }

        }
    }

            dy1 = y3 - y2;
            dx1 = x3 - x2;
            dv1 = v3 - v2;
            du1 = u3 - u2;
            dw1 = w3 - w2;

            if (dy1) dax_step = dx1 / (float)abs(dy1);
            if (dy2) dbx_step = dx2 / (float)abs(dy2);

            du1_step = 0, dv1_step = 0;
            if (dy1) du1_step = du1 / (float)abs(dy1);
            if (dy1) dv1_step = dv1 / (float)abs(dy1);
            if (dy1) dw1_step = dw1 / (float)abs(dy1);

            if (dy1)
            {
             for (int i = y2; i <= y3; i++)
                 {
                    int ax = x2 + (float)(i - y2) * dax_step;
                    int bx = x1 + (float)(i - y1) * dbx_step;

                    float tex_su = u2 + (float)(i - y2) * du1_step;
                    float tex_sv = v2 + (float)(i - y2) * dv1_step;
                    float tex_sw = w2 + (float)(i - y2) * dw1_step;

                    float tex_eu = u1 + (float)(i - y1) * du2_step;
                    float tex_ev = v1 + (float)(i - y1) * dv2_step;
                    float tex_ew = w1 + (float)(i - y1) * dw2_step;

                    if (ax > bx)
                    {
                        std::swap(ax, bx);
                        std::swap(tex_su, tex_eu);
                        std::swap(tex_sv, tex_ev);
                        std::swap(tex_sw, tex_ew);
                    }

                    tex_u = tex_su;
                    tex_v = tex_sv;
                    tex_w = tex_sw;

                    float tstep = 1.0f / ((float)(bx - ax));
                    float t = 0.0f;

                    for (int j = ax; j < bx; j++)
                    {
                        tex_u = (1.0f - t) * tex_su + t * tex_eu;
                        tex_v = (1.0f - t) * tex_sv + t * tex_ev;
                        tex_w = (1.0f - t) * tex_sw + t * tex_ew;

                    if (tex_w > pDepthBuffer[i*SCREENWIDTH + j])
                    {
                        Draws::Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w), renderer);
                        pDepthBuffer[i*SCREENWIDTH + j] = tex_w;
                    }
                t += tstep;
            }
        }	
    }		
}


int Draws::Triangle_ClipAgainstPlane(Vec3d plane_p, Vec3d plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2)
{
    // Make sure plane normal is indeed normal
    plane_n = Vector_Normalise(plane_n);

    // Return signed shortest distance from point to plane, plane normal must be normalised
    auto dist = [&](Vec3d &p)
    {
        //Vec3d n = Vector_Normalise(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
    };

    // Create two temporary storage arrays to classify points either side of plane
    // If distance sign is positive, point lies on "inside" of plane
    Vec3d* inside_points[3];  int nInsidePointCount = 0;
    Vec3d* outside_points[3]; int nOutsidePointCount = 0;
    vec2d* inside_tex[3]; int nInsideTexCount = 0;
    vec2d* outside_tex[3]; int nOutsideTexCount = 0;


    // Get signed distance of each point in triangle to plane
    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; inside_tex[nInsideTexCount++] = &in_tri.t[0]; }
    else {
        outside_points[nOutsidePointCount++] = &in_tri.p[0]; outside_tex[nOutsideTexCount++] = &in_tri.t[0];
    }
    if (d1 >= 0) {
        inside_points[nInsidePointCount++] = &in_tri.p[1]; inside_tex[nInsideTexCount++] = &in_tri.t[1];
    }
    else {
        outside_points[nOutsidePointCount++] = &in_tri.p[1];  outside_tex[nOutsideTexCount++] = &in_tri.t[1];
    }
    if (d2 >= 0) {
        inside_points[nInsidePointCount++] = &in_tri.p[2]; inside_tex[nInsideTexCount++] = &in_tri.t[2];
    }
    else {
        outside_points[nOutsidePointCount++] = &in_tri.p[2];  outside_tex[nOutsideTexCount++] = &in_tri.t[2];
    }

    // Now classify triangle points, and break the input triangle into 
    // smaller output triangles if required. There are four possible
    // outcomes...

    if (nInsidePointCount == 0)
    {
        // All points lie on the outside of plane, so clip whole triangle
        // It ceases to exist

        return 0; // No returned triangles are valid
    }

    if (nInsidePointCount == 3)
    {
        // All points lie on the inside of plane, so do nothing
        // and allow the triangle to simply pass through
        out_tri1 = in_tri;

        return 1; // Just the one returned original triangle is valid
    }

    if (nInsidePointCount == 1 && nOutsidePointCount == 2)
    {
        // Triangle should be clipped. As two points lie outside
        // the plane, the triangle simply becomes a smaller triangle

        // Copy appearance info to new triangle
        out_tri1.col =  in_tri.col;
        out_tri1.sym = in_tri.sym;

        // The inside point is valid, so keep that...
        out_tri1.p[0] = *inside_points[0];
        out_tri1.t[0] = *inside_tex[0];

        // but the two new points are at the locations where the 
        // original sides of the triangle (lines) intersect with the plane
        float t;
        out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
        out_tri1.t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
        out_tri1.t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
        out_tri1.t[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

        out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
        out_tri1.t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
        out_tri1.t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
        out_tri1.t[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

        return 1; // Return the newly formed single triangle
    }

    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
        // Triangle should be clipped. As two points lie inside the plane,
        // the clipped triangle becomes a "quad". Fortunately, we can
        // represent a quad with two new triangles

        // Copy appearance info to new triangles
        out_tri1.col =  in_tri.col;
        out_tri1.sym = in_tri.sym;

        out_tri2.col =  in_tri.col;
        out_tri2.sym = in_tri.sym;

        // The first triangle consists of the two inside points and a new
        // point determined by the location where one side of the triangle
        // intersects with the plane
        out_tri1.p[0] = *inside_points[0];
        out_tri1.p[1] = *inside_points[1];
        out_tri1.t[0] = *inside_tex[0];
        out_tri1.t[1] = *inside_tex[1];

        float t;
        out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
        out_tri1.t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
        out_tri1.t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
        out_tri1.t[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

        // The second triangle is composed of one of he inside points, a
        // new point determined by the intersection of the other side of the 
        // triangle and the plane, and the newly created point above
        out_tri2.p[0] = *inside_points[1];
        out_tri2.t[0] = *inside_tex[1];
        out_tri2.p[1] = out_tri1.p[2];
        out_tri2.t[1] = out_tri1.t[2];
        out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
        out_tri2.t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
        out_tri2.t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
        out_tri2.t[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
        return 2; // Return two newly formed triangles which form a quad
    }

    return 0;
}