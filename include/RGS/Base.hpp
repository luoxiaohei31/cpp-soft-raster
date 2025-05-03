#pragma once
#ifndef BASE_H_
#define BASE_H_

#include<windows.h>
#include<stdio.h>
#include<chrono>


#define LOG(...)
#define ASSERT(x, ...)        \
    {                         \
        if (!(x))             \
        {                     \
            LOG(__VA_ARGS__); \
            __debugbreak();   \
        }                     \
    }

namespace BenchMark
    {
        struct TextColor{
            enum color{
                F_RED=0x4,
                F_GREEN=0x2,
                F_BLUE=0x1,
                B_RED=0x40,
                B_GREEN=0x20,
                B_BLUE=0x10,
                F_HIGHLITGHT=0x8,       //高亮
                B_HIGHLITGHT=0x80,      //背景高亮
                F_UNDERSCORE=0x8000     //下划线
            };
            using style= unsigned short;
            style m_color;
            TextColor(style color):m_color(color){
                HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole,m_color);
            }
            void operator |(color c1){
                m_color = m_color | c1;
            }
            void operator ()(style s){
                HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole,s);
            }
            ~TextColor(){
                HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole,7);
            }
        };

        struct TTime
        {
            using Clock = ::std::chrono::steady_clock;
            using Duration = ::std::chrono::duration<float, ::std::milli>;
            using TimePoint = ::std::chrono::time_point<Clock, Duration>;
            TimePoint end;
            TTime() : end(Clock::now()) {}
            ~TTime()
            {
                TextColor tc(TextColor::F_GREEN | TextColor::F_HIGHLITGHT);
                Duration d = Clock::now() - end;
                printf("\n\t    time: %f ms\n", d.count());
            }
        };
    } // namespace BenchMark
    

#endif