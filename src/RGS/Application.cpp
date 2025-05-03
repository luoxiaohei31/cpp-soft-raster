#include "RGS\Application.h"
#include "RGS\Window.h"
#include"RGS\LMath.h"
#include"RGS\FrameBuffer.h"
#include"RGS\Renderer.hpp"
#include"RGS\Shaders\BlinnShader.h"

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
    framebuffer.clear({0.0f,0.0f,0.0f});

    Program program(blinn_vertex_shader);
    Triangle<BlinnVertex> tri;
    tri[0].model_pos = { 0.0f, 0.0f, -10.0f, 1.0f };
    tri[1].model_pos = { -10.0f, -10.0f, -10.0f, 1.0f };
    tri[2].model_pos = { 30.0f, -10.0f, -10.0f, 1.0f };
    BlinnUnforms uniforms;
    const float PI = 3.1415926f;
    uniforms.MVP = mat4_perspective(90.0f / 180.0f * PI, 1.0f, 1.0f, 10.0f);

    Renderer::Draw(framebuffer, program, tri, uniforms);
    
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
