#include "RGS\LMath.h"
#include"RGS\Base.hpp"
#include <string>
#include <math.h>

float RGS::lerp(const float start, const float end, const float t) {
    return end * t + start * (1.0f - t);
}

RGS::Vec3 RGS::lerp(const Vec3& start, const Vec3& end, const float t){
    return end * t + start * (1.0f - t);
}

unsigned char RGS::float2uchar(const float f)
{
    return (unsigned char)(f * 255.0f);
}

float RGS::uchar2float(const unsigned char c)
{
    return (float)c / 255.0f;
}

float RGS::clamp(const float val, const float min, const float max){
    if (val < min)
        return min;
    else if (val > max)
        return max;
    else
        return val;
}

RGS::Vec3 RGS::color_map(const Vec3& color)
{
    auto map=[](float c)->float{
        c = c >= 0 ? c : 0;
        c = c < 255 ? c : 255;
        return c > 1 ? (float)c / 255 : c;
    };
    const float r = map(color.x);
    const float g = map(color.y);
    const float b = map(color.z);
    return { r,g,b };
}

RGS::Vec2::operator const std::string() const
{
    std::string s;
    s += '(';
    s += std::to_string(x);
    s += ", ";
    s += std::to_string(y);
    s += ')';

    return s;
}

RGS::Vec3::operator const std::string() const
{
    std::string s;
    s += '(';
    s += std::to_string(x);
    s += ", ";
    s += std::to_string(y);
    s += ", ";
    s += std::to_string(z);
    s += ')';

    return s;
}

RGS::Vec4::operator const std::string() const
{
    std::string s;
    s += '(';
    s += std::to_string(x);
    s += ", ";
    s += std::to_string(y);
    s += ", ";
    s += std::to_string(z);
    s += ", ";
    s += std::to_string(w);
    s += ')';

    return s;
}

RGS::Mat4::Mat4(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2, const Vec4 &v3)
{
    M[0][0]=v0.x; M[1][0]=v0.y; M[2][0]=v0.z; M[3][0]=v0.w;
    M[0][1]=v1.x; M[1][1]=v1.y; M[2][1]=v1.z; M[3][1]=v1.w;
    M[0][2]=v2.x; M[1][2]=v2.y; M[2][2]=v2.z; M[3][2]=v2.w;
    M[0][3]=v3.x; M[1][3]=v3.y; M[2][3]=v3.z; M[3][3]=v3.w;
}

RGS::Mat4::operator const std::string() const {
    std::string res;
    for (int i = 0; i < 4; i++)
    {
        res += "(";
        for (int j = 0; j < 4; j++)
        {
            res += std::to_string(M[i][j]);
            res += (j != 3) ? ", " : "";
        }
        res += i != 3 ? ")\n" : ")";
    }
    return res;
}

RGS::Vec2 RGS::operator+(const Vec2& left, const Vec2& right){
    return { left.x + right.x, left.y + right.y };
}

RGS::Vec2 RGS::operator-(const Vec2& left, const Vec2& right){
    return left + (-1.0f * right);
}

RGS::Vec2 RGS::operator*(const float left, const Vec2& right){
    return { left * right.x, left * right.y };
}

RGS::Vec2 RGS::operator*(const Vec2& left, const float right){
    return right * left;
}

float RGS::dot(const Vec3 &left, const Vec3 &right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

RGS::Vec4 RGS::operator*(const Mat4 &mat4, const Vec4 &vec4)
{
    Vec4 res;
    res.x = mat4.M[0][0] * vec4.x + mat4.M[0][1] * vec4.y + mat4.M[0][2] * vec4.z + mat4.M[0][3] * vec4.w;
    res.y = mat4.M[1][0] * vec4.x + mat4.M[1][1] * vec4.y + mat4.M[1][2] * vec4.z + mat4.M[1][3] * vec4.w;
    res.z = mat4.M[2][0] * vec4.x + mat4.M[2][1] * vec4.y + mat4.M[2][2] * vec4.z + mat4.M[2][3] * vec4.w;
    res.w = mat4.M[3][0] * vec4.x + mat4.M[3][1] * vec4.y + mat4.M[3][2] * vec4.z + mat4.M[3][3] * vec4.w;
    return res;
}

RGS::Vec3 RGS::operator+(const Vec3& left, const Vec3& right) {
    return { left.x + right.x,left.y + right.y,left.z + right.z };
}

RGS::Vec3 RGS::operator-(const Vec3& left, const Vec3& right) {
    return left + (-1.0f * right);
}

RGS::Vec3 RGS::operator*(float left, const Vec3 &right)
{
    return {left * right.x, left * right.y, left * right.z};
}

RGS::Vec3 RGS::operator*(const Vec3 &left, float right)
{
    return right * left;
}

RGS::Vec3 RGS::operator*(const Vec3 &left, const Vec3 &right)
{
    return cross(left, right);
}

RGS::Vec3 RGS::operator/(const Vec3 &left, const float right)
{
    return left * (1.0f / right);
}

RGS::Vec3 RGS::cross(const Vec3 &left, const Vec3 &right)
{
    Vec3 m;
    m.x = left.y * right.z - left.z * right.y;
    m.y = left.z * right.x - left.x * right.z;
    m.z = left.x * right.y - left.y * right.x;
    return m;
}

RGS::Vec3 RGS::normalize(const Vec3 &v)
{
    float len = (float)std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    ASSERT(len != 0);
    return v / len;
}

RGS::Vec4 RGS::operator+(const Vec4& left, const Vec4& right){
    Vec4 v;
    v.x = left.x + right.x;
    v.y = left.y + right.y;
    v.z = left.z + right.z;
    v.w = left.w + right.w;
    return v;
}

RGS::Vec4 RGS::operator-(const Vec4& left, const Vec4& right){
    return left + (-1.0f * right);
}

RGS::Vec4 RGS::operator*(const float left, const Vec4& right){
    return Vec4{ left * right.x, left * right.y, left * right.z, left * right.w };
}

RGS::Vec4 RGS::operator*(const Vec4& left, const float right){
    return right * left;
}

RGS::Vec4 RGS::operator/(const Vec4& left, const float right){
    ASSERT(right != 0);
    return left * (1.0f / right);
}

RGS::Vec4 RGS::normalize(const Vec4& v){
    float len = (float)std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    ASSERT(len != 0);
    return v / len;
}

RGS::Mat4 RGS::operator*(const Mat4& left, const Mat4& right)
{
    Mat4 res;
    for (short i = 0; i < 4; i++)
    {
        for (short j = 0; j < 4; j++)
        {
            for (short k = 0; k < 4; k++)
            {
                res.M[i][j] += left.M[i][k] * right.M[k][j];
            }
        }
    }
    return res;
}

RGS::Mat4 &RGS::operator*=(Mat4 &left, const Mat4 &right)
{
    left = left * right;
    return left;
}

RGS::Mat4 RGS::mat4_indentity()
{
    return Mat4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 });
}

RGS::Mat4 RGS::mat4_scale(float sx, float sy, float sz)
{
    Mat4 m = mat4_indentity();
    ASSERT(sx != 0 && sy != 0 && sz != 0);
    m.M[0][0] = sx;
    m.M[1][1] = sy;
    m.M[2][2] = sz;
    return m;
}

RGS::Mat4 RGS::mat4_translate(float tx, float ty, float tz)
{
    Mat4 m = mat4_indentity();
    m.M[0][3] = tx;
    m.M[1][3] = ty;
    m.M[2][3] = tz;
    return m;
}

RGS::Mat4 RGS::mat4_rotate_x(float angle)
{
    float c = (float)std::cos(angle);
    float s = (float)std::sin(angle);
    Mat4 m = mat4_indentity();
    m.M[1][1] = c;
    m.M[1][2] = -s;
    m.M[2][1] = s;
    m.M[2][2] = c;

    return m;
}

RGS::Mat4 RGS::mat4_rotate_y(float angle)
{
    float c = (float)std::cos(angle);
    float s = (float)std::sin(angle);
    Mat4 m = mat4_indentity();
    m.M[0][0] = c;
    m.M[0][2] = s;
    m.M[2][0] = -s;
    m.M[2][2] = c;
    return m;
}

RGS::Mat4 RGS::mat4_rotate_z(float angle)
{
    float c = (float)std::cos(angle);
    float s = (float)std::sin(angle);
    Mat4 m = mat4_indentity();
    m.M[0][0] = c;
    m.M[0][1] = -s;
    m.M[1][0] = s;
    m.M[1][1] = c;
    return m;
}

RGS::Mat4 RGS:: mat4_perspective(float fovy, float aspect, float lnear, float lfar)
{
    Mat4 m = mat4_indentity();
    float z_range = lfar - lnear;
    ASSERT(fovy > 0 && aspect > 0);
    ASSERT(lnear > 0 && lfar > 0 && z_range > 0);
    m.M[1][1] = 1.0f / std::tan(fovy / 2);
    m.M[0][0] = m.M[1][1] / aspect;
    m.M[2][2] = -(lfar + lnear) / z_range;
    m.M[2][3] = -2 * lfar * lnear / z_range;
    m.M[3][2] = -1;
    m.M[3][3] = 0;
    return m;
}

RGS::Mat4 RGS::mat4_lookat(const Vec3 &xAxis, const Vec3 &yAxis, const Vec3 &zAxis, const Vec3 &eye)
{
    Mat4 m = mat4_indentity();

    m.M[0][0] = xAxis.x;    m.M[0][1] = xAxis.y;    m.M[0][2] = xAxis.z;
    m.M[1][0] = yAxis.x;    m.M[1][1] = yAxis.y;    m.M[1][2] = yAxis.z;
    m.M[2][0] = zAxis.x;    m.M[2][1] = zAxis.y;    m.M[2][2] = zAxis.z;

    m.M[0][3] = -dot(xAxis, eye);
    m.M[1][3] = -dot(yAxis, eye);
    m.M[2][3] = -dot(zAxis, eye);

    return m;
}

RGS::Mat4 RGS::mat4_lookat(const Vec3 &eye, const Vec3 &target, const Vec3 &up)
{
    Vec3 zAxis = normalize(eye - target);
    Vec3 xAxis = normalize(cross(up, zAxis));
    Vec3 yXxis = normalize(cross(zAxis, xAxis));
    return mat4_lookat(xAxis, yXxis, zAxis, eye);
}
