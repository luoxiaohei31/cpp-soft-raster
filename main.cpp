#include "Application.h"

#include <iostream>

int main(int, char **)
{

    RGS::Application app("RGS", 400, 300);

    app.run();
    
    return 0;
}
