#include "RGS\Application.h"
#include "RGS\Base.hpp"
#include "RGS\LMath.h"

#include <iostream>

int main(int, char**){

    RGS::Application app("RGS", 400, 300);

    app.run();
    std::printf("GoodBye RGS!");

#define TTT
#ifndef TTT
#include"test.hpp"
    try
    {
        using color = BenchMark::TextColor::color;
        BenchMark::TextColor tc(color::F_BLUE | color::F_RED
            | color::F_HIGHLITGHT);
        BenchMark::TTime time;
        Base* b = new Derived(10);
        b->print();
        std::printf("%d\n", b);
        delete b;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
        
#endif
    return 0;
}
