#include "Window.h"
#include "WindowsWindow.h"

RGS::Window::Window(const std::string title, unsigned short width, unsigned short height)
    : m_title(title), m_width(width), m_height(height)
{
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
