#ifndef APP_H_
#define APP_H_

#include "Window.h"
#include"LMath.h"

#include<chrono>

namespace RGS
{
    struct Camera{
        Vec4 Pos = { 0.0f, 0.0f, 0.0f, 1.0f };
        Vec4 Right = { 1.0f, 0.0f, 0.0f, 0.0f };
        Vec4 Up = { 0.0f, 1.0f, 0.0f, 0.0f };
        Vec4 Dir = { 0.0f, 0.0f, -1.0f, 0.0f };
        float Aspect = 4.0f / 4.0f;
    };
    
    class Application
    {
    private:
        std::string m_name;
        unsigned short m_width, m_height;
        std::chrono::steady_clock::time_point m_last_frame_time;

        Window* m_window;
        Camera m_camera;

    private:
        void init();
        void terminate();
        void on_update(float time);
        void on_camera_update(float time);

    public:
        Application() = default;
        Application(const std::string name, unsigned short width, unsigned short height);
        ~Application();

        void run();
    };

}

#endif