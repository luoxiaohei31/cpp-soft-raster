#include "LMath.h"
#include <stdio.h>

unsigned char RGS::float2uchar(const float f)
{
    return (unsigned char)(f * 255.0f);
}

float RGS::uchar2float(const unsigned char c)
{
    return (float)c / 255.0f;
}

float RGS::color_map(int c)
{
    c = c >= 0 ? c : 0;
    c = c < 255 ? c : 255;
    return (float)c / 255;
}

RGS::Vec3::Vec3(int x, int y, int z)
{
    this->x = color_map(x);
    this->y = color_map(y);
    this->z = color_map(z);
}
