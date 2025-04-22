#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>

namespace RGS
{
    class Window
    {
    protected:
        std::string m_title;
        unsigned short m_width, m_height;
        bool m_closed = true;

    public:
        Window(const std::string title, unsigned short width, unsigned short height);
        virtual ~Window();

        virtual void show() const = 0;
        bool closed() const { return m_closed; }

    public:
        static void init();
        static void terminate();
        static Window *create(const std::string title, unsigned short width, unsigned short height);
    };
}

#endif