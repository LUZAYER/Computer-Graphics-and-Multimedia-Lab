#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <cstdlib>

// -------------------------------------------------------
// Vec2
// -------------------------------------------------------
struct Vec2
{
    float x, y;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    Vec2& operator*=(float s) { x *= s; y *= s; return *this; }

    float length() const { return sqrtf(x * x + y * y); }
};

// -------------------------------------------------------
// Vec3
// -------------------------------------------------------
struct Vec3
{
    float x, y, z;
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

// -------------------------------------------------------
// Mat4 - Column-major 4x4 matrix for OpenGL
// -------------------------------------------------------
struct Mat4
{
    float m[16];

    Mat4()
    {
        for (int i = 0; i < 16; i++) m[i] = 0.0f;
    }

    static Mat4 identity()
    {
        Mat4 r;
        r.m[0] = 1.0f; r.m[5] = 1.0f; r.m[10] = 1.0f; r.m[15] = 1.0f;
        return r;
    }

    static Mat4 ortho(float l, float r, float b, float t, float n, float f)
    {
        Mat4 result = identity();
        result.m[0]  =  2.0f / (r - l);
        result.m[5]  =  2.0f / (t - b);
        result.m[10] = -2.0f / (f - n);
        result.m[12] = -(r + l) / (r - l);
        result.m[13] = -(t + b) / (t - b);
        result.m[14] = -(f + n) / (f - n);
        return result;
    }

    static Mat4 translate(const Mat4& mat, const Vec3& v)
    {
        Mat4 result = mat;
        result.m[12] = mat.m[0]*v.x + mat.m[4]*v.y + mat.m[8]*v.z  + mat.m[12];
        result.m[13] = mat.m[1]*v.x + mat.m[5]*v.y + mat.m[9]*v.z  + mat.m[13];
        result.m[14] = mat.m[2]*v.x + mat.m[6]*v.y + mat.m[10]*v.z + mat.m[14];
        result.m[15] = mat.m[3]*v.x + mat.m[7]*v.y + mat.m[11]*v.z + mat.m[15];
        return result;
    }

    static Mat4 scale(const Mat4& mat, const Vec3& v)
    {
        Mat4 result = mat;
        for (int i = 0; i < 4; i++) result.m[i]     = mat.m[i]     * v.x;
        for (int i = 4; i < 8; i++) result.m[i]     = mat.m[i]     * v.y;
        for (int i = 8; i < 12; i++) result.m[i]    = mat.m[i]     * v.z;
        return result;
    }

    static Mat4 rotate(const Mat4& mat, float angle, const Vec3& axis)
    {
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.0f - c;
        float ax = axis.x, ay = axis.y, az = axis.z;
        float len = sqrtf(ax*ax + ay*ay + az*az);
        if (len > 0.0f) { ax /= len; ay /= len; az /= len; }

        Mat4 rot = identity();
        rot.m[0] = t*ax*ax + c;       rot.m[4] = t*ax*ay - s*az;    rot.m[8]  = t*ax*az + s*ay;
        rot.m[1] = t*ax*ay + s*az;    rot.m[5] = t*ay*ay + c;       rot.m[9]  = t*ay*az - s*ax;
        rot.m[2] = t*ax*az - s*ay;    rot.m[6] = t*ay*az + s*ax;    rot.m[10] = t*az*az + c;

        return mat * rot;
    }

    Mat4 operator*(const Mat4& o) const
    {
        Mat4 r;
        for (int col = 0; col < 4; col++)
            for (int row = 0; row < 4; row++)
                r.m[col*4+row] = m[0*4+row]*o.m[col*4+0] + m[1*4+row]*o.m[col*4+1]
                               + m[2*4+row]*o.m[col*4+2] + m[3*4+row]*o.m[col*4+3];
        return r;
    }

    const float* data() const { return m; }
};

// -------------------------------------------------------
// Utility functions
// -------------------------------------------------------
inline float lerpf(float a, float b, float t)
{
    return a + (b - a) * t;
}

inline float clampf(float val, float lo, float hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

inline float randomFloat(float minV, float maxV)
{
    return minV + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxV - minV)));
}

#endif
