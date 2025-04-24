#include "Application.h"
#include "Window.h"
#include"LMath.h"
#include"FrameBuffer.h"

void RGS::Application::init()
{
    Window::init();
    m_window = Window::create(m_name, m_width, m_height);
}

void RGS::Application::terminate()
{
    Window::terminate();
}

void RGS::Application::on_update()
{
    if (m_window->get_key(RGS_KEY_KP_0) == RGS_PRESS)
        printf("0被按下\n");

    FrameBuffer framebuffer(m_width,m_height);
    framebuffer.clear(Vec3{194,193,225});
    //framebuffer.clear({1.0f,0.0f,1.0f});
    m_window->draw_frame_buffer(framebuffer);

}

RGS::Application::Application(const std::string name, unsigned short width, unsigned short height)
    : m_name(name), m_width(width), m_height(height)
{
    this->init();
}

RGS::Application::~Application()
{
    delete m_window;
    this->terminate();
}

void RGS::Application::run()
{
    while (!m_window->closed())
    {
        this->on_update();

        m_window->poll_input_events();
    }
}
