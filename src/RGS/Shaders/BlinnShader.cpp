#include "RGS\Shaders\BlinnShader.h"

void RGS::blinn_vertex_shader(const BlinnVertex& vertex, const BlinnUnforms& uniforms, BlinnVaryings& varyings) 
{
    varyings.clip_pos = uniforms.MVP * vertex.model_pos;
}

RGS::Vec4 RGS::blinn_fragment_shader(bool& discard, const BlinnVaryings& varyings, const BlinnUnforms& uniforms){
    if (uniforms.is_another){
        Vec4 color = varyings.ndc_pos * 0.5f + 0.5f;
        color.w = 1.0f;
        return color;
    }
    else{
        int x = varyings.frag_pos.x;
        if ((x + 10) % 10 > 4){
            discard = true;
            return Vec4{ 0.0f };
        }
        return Vec4{ 1.0f, 1.0f, 1.0f, 0.5f };
    }
}