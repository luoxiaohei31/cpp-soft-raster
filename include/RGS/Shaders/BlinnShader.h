#pragma once
#ifndef BLISHA_H_
#define BLISHA_H_

#include"ShaderBase.h"

namespace RGS{
    struct BlinnVertex :public VertextBase {};

    struct BlinnUnforms :public UniformsBase {};

    struct BlinnVaryings :public VaryingBase {};

    void blinn_vertex_shader(const BlinnVertex& vertex,
                            const BlinnUnforms& uniforms,
                            BlinnVaryings& varyings);

    }

#endif