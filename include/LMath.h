#ifndef MATH_H_
#define MATH_H_

namespace RGS
{
    struct Vec3{
        float x;
        float y;
        float z;

        constexpr Vec3():x(0.0f),y(0.0f),z(0.0f){}
        constexpr Vec3(float x,float y,float z)
            :x(x),y(y),z(z){}
        Vec3(int x,int y,int z);
    };

    unsigned char float2uchar(const float f);
    float uchar2float(const unsigned char c);

    //将RGB：0~255映射到0~1
    float color_map(int c);

} // namespace RGS


#endif