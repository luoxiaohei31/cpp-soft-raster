#ifndef WINDOW_H_
#define WINDOW_H_

#include "InputCodes.h"

#include <string>

namespace RGS
{
    class FrameBuffer;
    class Window
    {
    protected:
        std::string m_title;
        unsigned short m_width, m_height;
        bool m_closed = true;

        char m_keys[RGS_KEY_MAX_COUNT];

    public:
        Window(const std::string title, unsigned short width, unsigned short height);
        virtual ~Window();

        virtual void show() const = 0;
        virtual void draw_frame_buffer(const FrameBuffer& framebuffer)=0;

        bool closed() const { return m_closed; }
        char get_key(const uint32_t index) const { return m_keys[index]; }

    public:
        static void init();
        static void terminate();
        static Window *create(const std::string title, unsigned short width, unsigned short height);
        static void poll_input_events();
    };
}

#endif