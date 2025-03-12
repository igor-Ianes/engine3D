#ifndef MATH3D_HPP
#define MATH3D_HPP

#include <cmath>

// Created a 2D structure to hold texture coordinates
struct vec2d
{
	float u = 0;
	float v = 0;
	float w = 1;
};

struct Vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1; // Need a 4th term to perform sensible matrix vector multiplication
};

struct Triangle
{
	Vec3d p[3];
	vec2d t[3]; // added a texture coord per vertex
	wchar_t sym;
	short col;
};

struct Mat4x4
{
	float m[4][4] = { 0 };
};

Vec3d Matrix_MultiplyVector(Mat4x4 &m, Vec3d &i);
Mat4x4 Matrix_MakeIdentity();
Mat4x4 Matrix_MakeRotationX(float fAngleRad);
Mat4x4 Matrix_MakeRotationY(float fAngleRad);
Mat4x4 Matrix_MakeRotationZ(float fAngleRad);
Mat4x4 Matrix_MakeTranslation(float x, float y, float z);
Mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
Mat4x4 Matrix_MultiplyMatrix(Mat4x4 &m1, Mat4x4 &m2);
Mat4x4 Matrix_PointAt(Vec3d &pos, Vec3d &target, Vec3d &up);
Mat4x4 Matrix_QuickInverse(Mat4x4 &m);
Vec3d Vector_IntersectPlane(Vec3d &plane_p, Vec3d &plane_n, Vec3d &lineStart, Vec3d &lineEnd, float &t);

inline Vec3d Vector_Add(Vec3d &v1, Vec3d &v2)
{
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline Vec3d Vector_Sub(Vec3d &v1, Vec3d &v2)
{
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline Vec3d Vector_Mul(Vec3d &v1, float k)
{
    return { v1.x * k, v1.y * k, v1.z * k };
}

inline Vec3d Vector_Div(Vec3d &v1, float k)
{
    return { v1.x / k, v1.y / k, v1.z / k };
}

inline float Vector_DotProduct(Vec3d &v1, Vec3d &v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
}

inline float Vector_Length(Vec3d &v)
{
    return sqrtf(Vector_DotProduct(v, v));
}

inline Vec3d Vector_Normalise(Vec3d &v)
{
    float l = Vector_Length(v);
    return { v.x / l, v.y / l, v.z / l };
}

inline Vec3d Vector_CrossProduct(Vec3d &v1, Vec3d &v2)
{
    Vec3d v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

#endif