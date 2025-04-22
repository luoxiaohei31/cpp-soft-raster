#ifndef WWINDOW_H_
#define WWINDOW_H_

#include "Window.h"

#include <windows.h>

namespace RGS
{

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const std::string title, unsigned short width, unsigned short height);
        ~WindowsWindow();

        void show() const override;

    public:
        static void init();
        static void terminate();

    private:
        static void win_register();
        static void win_unregister();

        static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msgID,
                                        const WPARAM wParam, const LPARAM lParam);

    private:
        HWND m_handle;
        HDC m_memory_dc;
        unsigned char *m_buffer;
        static bool s_inited;
    };
}

#endif