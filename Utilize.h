
#ifndef UTILIZE_H
#define UTILIZE_H
#include <math.h>

namespace MeshLib
{
constexpr auto PAI = 3.1415926;

struct Vec3
{
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {};
    Vec3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
    Vec3(const Vec3& a, const Vec3& b)
        : x(b.x - a.x), y(b.y - a.y), z(b.z - a.z) {}
    Vec3(const Vec3&) = default;
    Vec3(Vec3&&) = default;
    Vec3& operator=(const Vec3&) = default;
    Vec3& operator=(Vec3&&) = default;

    void init(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }
    void Normalized()
    {
        float sum = 0.000001f;//防止除零
        sum += static_cast<float>(pow(x, 2) + pow(y, 2) + pow(z, 2));
        sum = sqrt(sum);
        x /= sum;
        y /= sum;
        z /= sum;
    }
    float dist(const Vec3& v) const
    {
        return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
    }

    bool operator ==(const Vec3& v) const
    {
        float dx = (x - v.x), dy = (y - v.y), dz = (z - v.z);
        return fabs((double)dx) <= 1e-6 && fabs((double)dy) <= 1e-6 && fabs((double)dz) <= 1e-6;
    }
    Vec3 operator +(const Vec3& rhs) { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vec3 operator -(const Vec3& rhs) { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vec3 operator *(const float& rhs) { return Vec3(x * rhs, y * rhs, z * rhs); }
    Vec3 operator /(const float& rhs) { return Vec3(x / rhs, y / rhs, z / rhs); }
    Vec3& operator +=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3& operator -=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vec3& operator *=(const float& muti)
    {
        x *= muti;
        y *= muti;
        z *= muti;
        return *this;
    }
    Vec3& operator /=(const float& div)
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }
};

class Utilize
{

public:
    Utilize() {}
    // 向量叉乘
    static Vec3 CrossProduct(Vec3 a, Vec3 b)
    {
        float x = a.y * b.z - a.z * b.y;
        float y = a.z * b.x - a.x * b.z;
        float z = a.x * b.y - a.y * b.x;
        return Vec3(x, y, z);
    }

    // 向量的模
    static float norm(Vec3 v)
    {
        float sum = 0;
        sum += static_cast<float>(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
        return sqrt(sum);
    }

    // 向量的角
    static float angle3D(Vec3 a, Vec3 b, Vec3 c) {
        Vec3 v1(a - b);
        Vec3 v2(c - b);
        float x1 = v1.x;
        float y1 = v1.y;
        float z1 = v1.z;
        float x2 = v2.x;
        float y2 = v2.y;
        float z2 = v2.z;
        float n = (x1 * x2 + y1 * y2 + z1 * z2);
        float dn1 = (x1 * x1 + y1 * y1 + z1 * z1);
        float dn2 = (x2 * x2 + y2 * y2 + z2 * z2);
        float m = static_cast<float>(sqrt((double)dn1) * sqrt((double)dn2));
        return static_cast<float>(acos(n / m) * 180 / PAI);
    }

    static float Cross2D(Vec3 a, Vec3 b)
    {
        return a.x * b.y - b.x * a.y;
    }
};
}
#endif // UTILIZE_H
