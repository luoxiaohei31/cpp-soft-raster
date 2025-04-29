#include "RGS\FrameBuffer.h"
#include "RGS\LMath.h"
#include "RGS\Base.hpp"

RGS::FrameBuffer::FrameBuffer(unsigned short width, unsigned short height)
    : m_width(width), m_height(height)
{
    ASSERT(width > 0 && height > 0);
    m_pixel_size = m_width * m_height;
    m_color_buffer = new Vec3[m_pixel_size];
    m_depth_buffer = new float[m_pixel_size];
    this->clear({0.0f, 0.0f, 0.0f});
    this->clear_depth(1.0f);
}

RGS::FrameBuffer::~FrameBuffer()
{
    delete[] m_color_buffer;
    delete[] m_depth_buffer;
    m_color_buffer = nullptr;
    m_depth_buffer = nullptr;
}

RGS::Vec3 RGS::FrameBuffer::get_color(unsigned short x, unsigned short y) const
{
    auto index = this->get_pixel_index(x, y);
    if (index < m_pixel_size && index >= 0)
    {
        return m_color_buffer[index];
    }
    else
    {
        ASSERT(false);
        return Vec3();
    }
}

void RGS::FrameBuffer::set_color(unsigned short x, unsigned short y, const Vec3 &color)
{
    auto index = this->get_pixel_index(x, y);
    
    if (index < m_pixel_size && index >= 0)
    {
        m_color_buffer[index] = RGS::color_map(color);
    }
    else
    {
        ASSERT(false);
    }
}

void RGS::FrameBuffer::set_depth(unsigned short x, unsigned short y, const float depth)
{
    auto index = this->get_pixel_index(x, y);
    if (index < m_pixel_size && index >= 0)
    {
        m_depth_buffer[index] = depth;
    }
    else
    {
        ASSERT(false);
    }
}

void RGS::FrameBuffer::clear(const Vec3 &color = {0.0f, 0.0f, 0.0f})
{
    for (unsigned int i = 0; i < m_pixel_size; i++)
    {
        m_color_buffer[i]=RGS::color_map(color);
    }
}

void RGS::FrameBuffer::clear_depth(float depth = 1.0f)
{
    for (unsigned int i = 0; i < m_pixel_size; i++)
        m_depth_buffer[i] = depth;
}
