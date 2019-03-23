#include "ImprovedPerlinNoise.h"
#include <cmath>

double ImprovedPerlinNoise::fade(double t) const
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double ImprovedPerlinNoise::lerp(double t, double a, double b) const
{
    return a + t * (b - a);
}

double ImprovedPerlinNoise::grad(int hash, double x, double y, double z) const
{
    int h = hash & 15;
    double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double ImprovedPerlinNoise::noise(double x, double y, double z) const
{
    int X = (int)std::floor(x) & 255, Y = (int)std::floor(y) & 255,
        Z = (int)std::floor(z) & 255;
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);
    double u = fade(x), v = fade(y), w = fade(z);
    int A = int(p[X] + Y), AA = int(p[A] + Z), AB = int(p[A + 1] + Z),
        B = int(p[X + 1] + Y), BA = int(p[B] + Z), BB = int(p[B + 1] + Z);

    return lerp(
        w,
        lerp(v,
             lerp(u, grad((int)p[AA], x, y, z), grad((int)p[BA], x - 1, y, z)),
             lerp(u, grad((int)p[AB], x, y - 1, z),
                  grad((int)p[BB], x - 1, y - 1, z))),
        lerp(v,
             lerp(u, grad((int)p[AA + 1], x, y, z - 1),
                  grad((int)p[BA + 1], x - 1, y, z - 1)),
             lerp(u, grad((int)p[AB + 1], x, y - 1, z - 1),
                  grad((int)p[BB + 1], x - 1, y - 1, z - 1))));
}
