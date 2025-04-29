#include "RGS\Window.h"
#include "RGS\WindowsWindow.h"
#include "RGS\Base.hpp"

RGS::Window::Window(const std::string title, unsigned short width, unsigned short height)
    : m_title(title), m_width(width), m_height(height)
{
    memset(m_keys, RGS_RELEASE, RGS_KEY_MAX_COUNT);
}

RGS::Window::~Window()
{
    this->terminate();
}

void RGS::Window::init()
{
    WindowsWindow::init();
}

void RGS::Window::terminate()
{
    WindowsWindow::terminate();
}

RGS::Window *RGS::Window::create(const std::string title, unsigned short width, unsigned short height)
{
    return new WindowsWindow(title, width, height);
}

void RGS::Window::poll_input_events()
{
    WindowsWindow::poll_input_events();
}
