#ifndef FB_H_
#define FB_H_

namespace RGS
{
    class Vec3;
    class FrameBuffer
    {
    private:
        unsigned short m_width;
        unsigned short m_height;
        unsigned int m_pixel_size;
        float *m_depth_buffer;
        Vec3* m_color_buffer;

        unsigned int get_pixel_index(unsigned short x, unsigned short y) const
        {
            return y * m_width + x;
        }

    public:
        FrameBuffer(unsigned short width, unsigned short height);
        ~FrameBuffer();

        unsigned short get_width() const { return m_width; }
        unsigned short get_heigth() const { return m_height; }
        float get_depth(unsigned short x, unsigned short y) const;
        Vec3 get_color(unsigned short x,unsigned short y)const;

        void set_color(unsigned short x,unsigned short y,const Vec3& color);
        void set_depth(unsigned short x,unsigned short y, const float depth);
    
        void clear(const Vec3& color);
        void clear_depth(float depth);
    };

} // namespace RGS

#endif