#ifndef MATH_H_
#define MATH_H_

#include<string>

namespace RGS
{
    struct Vec2
    {
        float x;
        float y;
        constexpr Vec2() : x(0.0f), y(0.0f) {}
        constexpr Vec2(float x, float y) : x(x), y(y) {}
    
        operator const std::string() const;

    };

    struct Vec3
    {
        float x;
        float y;
        float z;

        constexpr Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        constexpr Vec3(float x, float y, float z)
            : x(x), y(y), z(z) {}

        operator Vec2() { return {x, y}; }
        operator const std::string() const;
    };

    struct Vec4
    {
        float x, y, z, w;

        constexpr Vec4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f){}
        constexpr Vec4(float val):x(val),y(val),z(val),w(val){}
        constexpr Vec4(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w) {}

        operator Vec2() const{ return {x, y}; }
        operator Vec3() const{ return {x, y, z}; }
        operator const std::string() const;
    };

    //按行优先存储
    struct Mat4{
        float M[4][4];

        Mat4(){
            for(short i=0;i<4;i++){
                for(short j=0;j<4;j++)
                    M[i][j]=0;
            }
        }

        Mat4(const Vec4& v0,const Vec4& v2,
            const Vec4& v3,const Vec4& v4);

        operator const std::string() const;

    };

    Vec2 operator+(const Vec2& left, const Vec2& right);
    Vec2 operator-(const Vec2& left, const Vec2& right);
    Vec2 operator*(const float left, const Vec2& right);
    Vec2 operator*(const Vec2& left, const float right);

    float dot(const Vec3 &left, const Vec3 &right);
    Vec4 operator*(const Mat4 &mat4, const Vec4 &vec4);

    Vec3 operator+(const Vec3& left, const Vec3& right);
    Vec3 operator-(const Vec3& left, const Vec3& right);
    Vec3 operator*(float left, const Vec3& right);
    Vec3 operator*(const Vec3& left, float right);
    Vec3 operator*(const Vec3& left, const Vec3& right);
    Vec3 operator/(const Vec3& left, const float right);
    Vec3 cross(const Vec3& left, const Vec3& right);
    Vec3 normalize(const Vec3& v);

    Vec4 operator+(const Vec4& left, const Vec4& right);
    Vec4 operator-(const Vec4& left, const Vec4& right);
    Vec4 operator*(const float left, const Vec4& right);
    Vec4 operator*(const Vec4& left, const float right);
    Vec4 operator/(const Vec4& left, const float right);
    Vec4 normalize(const Vec4& v);

    Mat4 operator*(const Mat4& left, const Mat4& right);
    Mat4& operator*=(Mat4& left, const Mat4& right);
    Mat4 mat4_indentity();
    Mat4 mat4_scale(float sx, float sy, float sz);
    Mat4 mat4_translate(float tx, float ty, float tz);
    Mat4 mat4_rotate_x(float angle);
    Mat4 mat4_rotate_y(float angle);
    Mat4 mat4_rotate_z(float angle);
    Mat4 mat4_perspective(float fovy, float aspect, float near, float far);
    Mat4 mat4_lookat(const Vec3& xAxis, const Vec3& yAxis, const Vec3& zAxis, const Vec3& eye);
    Mat4 mat4_lookat(const Vec3& eye, const Vec3& target, const Vec3& up);

    float lerp(const float start, const float end, const float t);
    Vec3 lerp(const Vec3& start, const Vec3& end, const float t);
    unsigned char float2uchar(const float f);
    float uchar2float(const unsigned char c);

    float clamp(const float val, const float min, const float max);

    // 将RGB：0~255映射到0~1
    Vec3 color_map(const Vec3& v);

} // namespace RGS

#endif