#ifndef APP_H_
#define APP_H_

#include "Window.h"

namespace RGS
{
    class Application
    {
    private:
        std::string m_name;
        unsigned short m_width, m_height;

        Window *m_window;

    private:
        void init();
        void terminate();
        void on_update();

    public:
        Application() = default;
        Application(const std::string name, unsigned short width, unsigned short height);
        ~Application();

        void run();
    };

}

#endif