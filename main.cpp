#include "RGS\Application.h"
#include "RGS\Base.hpp"
#include "RGS\LMath.h"

#include <iostream>

int main(int, char**){

    RGS::Application app("RGS", 400, 300);

    app.run();

    {
#define TTT
#ifdef TTT
        using color = BenchMark::TextColor::color;
        BenchMark::TextColor tc(color::F_BLUE | color::F_RED
            | color::F_HIGHLITGHT);
        BenchMark::TTime time;
#endif
    }

        return 0;
}
