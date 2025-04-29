#include "RGS\LMath.h"
#include "RGS\WindowsWindow.h"
#include "RGS\FrameBuffer.h"
#include "RGS\Base.hpp"

#define RGS_WINDOW_ENTRY_NAME "Entry"
#define RGS_WINDOW_CLASS_NAME "Class"

bool RGS::WindowsWindow::s_inited = false;

RGS::WindowsWindow::WindowsWindow(const std::string title, unsigned short width, unsigned short height)
    : Window(title, width, height)
{
    ASSERT(s_inited, "未初始化");

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.bottom = height;
    rect.right = width;
    AdjustWindowRect(&rect, style, false);
    m_handle = CreateWindow(RGS_WINDOW_CLASS_NAME, m_title.c_str(), style,
                            CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top,
                            NULL, NULL, GetModuleHandle(NULL), NULL);

    ASSERT(m_handle != nullptr);
    m_closed = false;
    SetProp(m_handle, RGS_WINDOW_ENTRY_NAME, this);

    HDC windowDC = GetDC(m_handle);
    m_memory_dc = CreateCompatibleDC(windowDC);

    BITMAPINFOHEADER bi_header = {};
    HBITMAP new_bit_map;
    HBITMAP old_bit_map;

    bi_header.biSize = sizeof(BITMAPINFOHEADER);
    bi_header.biWidth = m_width;
    bi_header.biHeight = -m_height;
    bi_header.biPlanes = 1;
    bi_header.biBitCount = 24;
    bi_header.biCompression = BI_RGB;

    new_bit_map = CreateDIBSection(m_memory_dc, (BITMAPINFO *)&bi_header, DIB_RGB_COLORS,
                                   (void **)&m_buffer, nullptr, 0);
    ASSERT(new_bit_map != nullptr);
    constexpr short channel_count = 3;
    unsigned int size = m_width * m_height * channel_count * sizeof(unsigned char);
    memset(m_buffer, 0, size);
    old_bit_map = (HBITMAP)SelectObject(m_memory_dc, new_bit_map);

    DeleteObject(old_bit_map);
    ReleaseDC(m_handle, windowDC);

    this->show();
}

RGS::WindowsWindow::~WindowsWindow()
{
    ShowWindow(m_handle, SW_HIDE);
    RemoveProp(m_handle, RGS_WINDOW_ENTRY_NAME);
    DeleteDC(m_memory_dc);
    DestroyWindow(m_handle);
}

void RGS::WindowsWindow::show() const
{
    HDC windowDC = GetDC(m_handle);
    // 将缓存中的数据发送到显示设备上
    BitBlt(windowDC, 0, 0, m_width, m_height, m_memory_dc, 0, 0, SRCCOPY);
    ShowWindow(m_handle, SW_SHOW);
    ReleaseDC(m_handle, windowDC);
}

void RGS::WindowsWindow::draw_frame_buffer(const FrameBuffer &framebuffer)
{
    // 尽可能显示
    const unsigned short fwidth = framebuffer.get_width();
    const unsigned short fheigth = framebuffer.get_heigth();
    const unsigned short width = m_width < fwidth ? m_width : fwidth;
    const unsigned short height = m_height < fheigth ? m_height : fheigth;

    for (std::size_t i = 0; i < height; i++)
    {
        for (std::size_t j = 0; j < width; j++)
        {
            // 翻转RGB显示
            constexpr unsigned short channel_count = 3;
            constexpr unsigned short rchannel = 2;
            constexpr unsigned short gchannel = 1;
            constexpr unsigned short bchannel = 0;

            Vec3 color = framebuffer.get_color(j, fheigth - 1 - i);
            const int pix_start = (i * m_width + j) * channel_count;
            const int r_index = pix_start + rchannel;
            const int g_index = pix_start + gchannel;
            const int b_index = pix_start + bchannel;

            m_buffer[r_index] = float2uchar(color.x);
            m_buffer[g_index] = float2uchar(color.y);
            m_buffer[b_index] = float2uchar(color.z);
        }
    }

    /*for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            Vec3 color=framebuffer.get_color(i,j);
            const int pix_start = (i*height + j) * 3;
            const int r_index = pix_start + 2;
            const int g_index = pix_start + 1;
            const int b_index = pix_start + 0;

            m_buffer[r_index] = float2uchar(color.x);
            m_buffer[g_index] = float2uchar(color.y);
            m_buffer[b_index] = float2uchar(color.z);
        }
    }*/
    this->show();
}

void RGS::WindowsWindow::init()
{
    ASSERT(!s_inited);
    win_register();
    s_inited = true;
}

void RGS::WindowsWindow::terminate()
{
    ASSERT(!s_inited);
    win_unregister();
    s_inited = false;
}

void RGS::WindowsWindow::win_register()
{
    ATOM atom;
    WNDCLASS wc = {0};
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(WHITE_BRUSH); // 背景色
    wc.hCursor = NULL;                        // 默认光标
    wc.hIcon = NULL;                          // 默认图标
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = WindowsWindow::WndProc; // 窗口处理函数
    wc.lpszClassName = RGS_WINDOW_CLASS_NAME;
    wc.style = CS_HREDRAW | CS_VREDRAW; // 拉伸是要重绘
    wc.lpszMenuName = NULL;             // 不要菜单
    atom = RegisterClass(&wc);          // 注册窗口
}

void RGS::WindowsWindow::win_unregister()
{
    UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
}

LRESULT RGS::WindowsWindow::WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam)
{
    WindowsWindow *window = (WindowsWindow *)GetProp(hWnd, RGS_WINDOW_ENTRY_NAME);
    if (window == nullptr)
        return DefWindowProc(hWnd, msgID, wParam, lParam);

    switch (msgID)
    {
    case WM_DESTROY:
        window->m_closed = true;
        return 0;
    case WM_KEYDOWN:
        key_press_impl(window, wParam, RGS_PRESS);
        return 0;
    case WM_KEYUP:
        key_press_impl(window, wParam, RGS_RELEASE);
        return 0;
    }
    return DefWindowProc(hWnd, msgID, wParam, lParam);
}

void RGS::WindowsWindow::key_press_impl(WindowsWindow *window, const WPARAM wParam, const char state)
{
    // 获取小键盘按键
    auto get_pad = [wParam]() -> unsigned short
    { return wParam + 224; };

    if (wParam >= '0' && wParam <= '9')
    {
        window->m_keys[wParam] = state;
        return;
    }
    if (get_pad() >= 320 && get_pad() <= 335)
    {
        window->m_keys[get_pad()] = state;
        return;
    }

    if (wParam >= 'A' && wParam <= 'Z')
    {
        window->m_keys[wParam] = state;
        return;
    }

    switch (wParam)
    {
    case VK_SPACE:
        window->m_keys[RGS_KEY_SPACE] = state;
        break;
    case VK_SHIFT:
        window->m_keys[RGS_KEY_LEFT_SHIFT] = state;
        window->m_keys[RGS_KEY_RIGHT_SHIFT] = state;
        break;

    default:
        break;
    }
}

void RGS::WindowsWindow::poll_input_events()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
