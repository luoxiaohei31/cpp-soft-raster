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

void RGS::Application::on_update(float time)
{
    on_camera_update(time);

    FrameBuffer framebuffer(m_width, m_height);

    Program program(blinn_vertex_shader, blinn_fragment_shader);
    BlinnUnforms uniforms;
    Triangle<BlinnVertex> tri, tri1;

    Mat4 view = mat4_lookat(m_camera.Pos, m_camera.Pos + m_camera.Dir, m_camera.Up);
    const float PI = 3.1415926f;
    Mat4 proj = mat4_perspective(PI / 2.0f, m_camera.Aspect, 0.1f, 10.0f);
    uniforms.MVP = proj * view;

    program.EnableDoubleSided = true;

    uniforms.is_another = true;
    program.EnableBlend = false;
    program.EnableWriteDepth = true;
    tri1[0].model_pos = { 10.0f, 10.0f, -10.0f, 1.0f };
    tri1[1].model_pos = { -1.0f, -1.0f, -1.0f, 1.0f };
    tri1[2].model_pos = { 10.0f, -10.0f, -10.0f, 1.0f };
    Renderer::Draw(framebuffer, program, tri1, uniforms);

    uniforms.is_another = false;
    program.EnableBlend = true;
    program.EnableWriteDepth = false;
    tri[0].model_pos = { -10.0f, 10.0f, -10.0f, 1.0f };
    tri[1].model_pos = { -10.0f, -10.0f, -10.0f, 1.0f };
    tri[2].model_pos = { 1.0f, -1.0f, -1.0f, 1.0f };
    Renderer::Draw(framebuffer, program, tri, uniforms);
    
    m_window->draw_frame_buffer(framebuffer);

}

void RGS::Application::on_camera_update(float time){
    constexpr float speed = 5.0f;
    if (m_window->get_key(RGS_KEY_SPACE) == RGS_PRESS)
        m_camera.Pos = m_camera.Pos + speed * time * m_camera.Up;
    if (m_window->get_key(RGS_KEY_LEFT_SHIFT) == RGS_PRESS)
        m_camera.Pos = m_camera.Pos - speed * time * m_camera.Up;
    if (m_window->get_key(RGS_KEY_D) == RGS_PRESS)
        m_camera.Pos = m_camera.Pos + speed * time * m_camera.Right;
    if (m_window->get_key(RGS_KEY_A) == RGS_PRESS)
        m_camera.Pos = m_camera.Pos - speed * time * m_camera.Right;
    if (m_window->get_key(RGS_KEY_W) == RGS_PRESS)
        m_camera.Pos = m_camera.Pos + speed * time * m_camera.Dir;
    if (m_window->get_key(RGS_KEY_S) == RGS_PRESS)
        m_camera.Pos = m_camera.Pos - speed * time * m_camera.Dir;

    constexpr float rotate_speed = 1.0f;
    Mat4 rotation = mat4_indentity();
    if (m_window->get_key(RGS_KEY_Q) == RGS_PRESS)
        rotation = mat4_rotate_y(time * rotate_speed);
    if (m_window->get_key(RGS_KEY_E) == RGS_PRESS)
        rotation = mat4_rotate_y(-time * rotate_speed);
    m_camera.Dir = rotation * m_camera.Dir;
    m_camera.Right = rotation * m_camera.Right;
    m_camera.Dir = { normalize(m_camera.Dir) };
    m_camera.Right = { normalize(m_camera.Right) };
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
        auto now_frame_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now_frame_time - m_last_frame_time);
        float delta_time = duration.count() * 0.001f * 0.001f;
        m_last_frame_time = now_frame_time;
        this->on_update(delta_time);

        m_window->poll_input_events();
    }
}
