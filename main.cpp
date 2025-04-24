#include "Application.h"
#include "Base.hpp"

#include <iostream>

int main(int, char **)
{

    RGS::Application app("RGS", 400, 300);

    app.run();
    std::cout<<"goodbye!\n";

    return 0;
}
