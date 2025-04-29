#pragma once
#ifndef SHAB_H_
#define SHAB_H_

#include"..\LMath.h"

namespace RGS {
    struct VertextBase {
        Vec4 model_pos={0,0,0,1};
        operator const std::string()const { return "Model: " + std::string(model_pos); }
    };

    struct VaryingBase{
        Vec4 clip_pos = { 0,0,0,1 };
    };

    struct UniformsBase{
        Mat4 MVP;
        operator const std::string()const{return "MVP: "+std::string(MVP);}
    };
}

#endif