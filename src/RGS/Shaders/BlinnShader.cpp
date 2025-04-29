#include "RGS\Shaders\BlinnShader.h"

void RGS::blinn_vertex_shader(const BlinnVertex& vertex, 
                            const BlinnUnforms& uniforms, 
                            BlinnVaryings& varyings) 
{
    varyings.clip_pos = uniforms.MVP * vertex.model_pos;
}