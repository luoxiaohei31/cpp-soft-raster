#include "Application.h"
#include "Window.h"

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
    }
}
