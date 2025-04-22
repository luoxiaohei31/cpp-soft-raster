#include "Base.hpp"
#include "WindowsWindow.h"

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
    }
    return DefWindowProc(hWnd, msgID, wParam, lParam);
}
