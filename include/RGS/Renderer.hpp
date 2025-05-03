#pragma once
#ifndef REND_H_
#define REND_H_

#include"Shaders\ShaderBase.h"
#include"FrameBuffer.h"
#include"Base.hpp"

#include<type_traits>

namespace RGS {

    template<typename vertex_t>
    struct Triangle{
        static_assert(std::is_base_of_v<VertextBase,vertex_t>,"vertex_t 必须继承自 RGS::VertextBase");

        vertex_t vertex[3];
        Triangle()=default;

        const vertex_t& operator[](short index) const {
            index = index < 0 ? 0 : index;
            index = index >= 3 ? 2 : index;
            return vertex[index];
        }
        vertex_t& operator[](unsigned short index){
            return const_cast<vertex_t&>(static_cast<const Triangle&>(*this)[index]);
        }
    };

    template<typename vertex_t,typename uniforms_t,typename varyings_t>
    struct Program {
        using vertext_shader_t = void (*)(const vertex_t&,
                                        const uniforms_t&,
                                        varyings_t&);
        vertext_shader_t vertex_shader;

        Program(const vertext_shader_t vertexShader) :vertex_shader(vertexShader) {}
    };

    class Renderer {
        private:
            static constexpr unsigned short RGS_MAX_VARYINGS = 9;

        private:
            enum class Plane{
                PSOITIVE_W, PSOITIVE_X, PSOITIVE_Y, PSOITIVE_Z,
                            NEGATIVE_X, NEGATIVE_Y, NEGATIVE_Z
            };

            static bool is_vertex_Visble(const Vec4& clip_pos);
            static bool is_inside_plane(const Vec4& clip_pos, const Plane plane);
            static float get_intersect_ratio(const Vec4& prev, const Vec4& curr, const Plane plane);

            template<typename varyings_t>
            static void lerp_varyings(varyings_t& out, const float ratio,
                                    const varyings_t& start, const varyings_t& end)
            {
                constexpr uint32_t float_num = sizeof(varyings_t) / sizeof(float);
                float* start_float = (float*)&start;
                float* end_float = (float*)&end;
                float* out_float = (float*)&out;

                for (int i = 0; i < (int)float_num; i++) {
                    out_float[i] = lerp(start_float[i], end_float[i], ratio);
                }
            }


            template<typename varyings_t>
            static int clip_against_plane(varyings_t(&out_varyings)[RGS_MAX_VARYINGS],
                                        const varyings_t(&in_varyings)[RGS_MAX_VARYINGS],
                                        const Plane plane,
                                        const int in_vertex_num)
            {
                ASSERT(in_vertex_num >= 3);
                int out_vertext_num = 0;
                for(int i = 0;i < in_vertex_num; i++) {
                    int prev_index = (in_vertex_num - 1 + i) % in_vertex_num;
                    int curr_index = i;

                    const varyings_t& prev_varyings = in_varyings[prev_index];
                    const varyings_t& curr_varyings = in_varyings[curr_index];
                    
                    const bool prev_inside = is_inside_plane(prev_varyings.clip_pos, plane);
                    const bool curr_inside = is_inside_plane(curr_varyings.clip_pos, plane);

                    if(curr_inside!=prev_inside){
                        float ratio = get_intersect_ratio(prev_varyings.clip_pos, curr_varyings.clip_pos, plane);
                        lerp_varyings(out_varyings[out_vertext_num], ratio, prev_varyings, curr_varyings);
                        out_vertext_num++;
                    }

                    if(curr_inside){
                        out_varyings[out_vertext_num] = in_varyings[curr_index];
                        out_vertext_num++;
                    }
                }

                ASSERT(out_vertext_num <= RGS_MAX_VARYINGS);
                return out_vertext_num;
            }

            template<typename varyings_t>
            static int clip(varyings_t(&varyings)[RGS_MAX_VARYINGS]){
                const bool v0_Visble = is_vertex_Visble(varyings[0].clip_pos);
                const bool v1_Visble = is_vertex_Visble(varyings[1].clip_pos);
                const bool v2_Visble = is_vertex_Visble(varyings[2].clip_pos);
                if(v0_Visble && v1_Visble && v2_Visble)
                    return 3;

                short vertex_num = 3;
                varyings_t varyings_[RGS_MAX_VARYINGS];
                vertex_num = clip_against_plane(varyings_, varyings, Plane::PSOITIVE_W, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings, varyings_, Plane::PSOITIVE_X, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings_, varyings, Plane::NEGATIVE_X, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings, varyings_, Plane::PSOITIVE_Y, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings_, varyings, Plane::NEGATIVE_Y, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings, varyings_, Plane::PSOITIVE_Z, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings_, varyings, Plane::NEGATIVE_Z, vertex_num);
                if (vertex_num == 0) return 0;
                memcpy(varyings, varyings_, sizeof(varyings_));

                return vertex_num;
            }

            //归一化设备坐标
            template<typename varyings_t>
            static void caculate_ndc_pos(varyings_t(&varyings)[RGS_MAX_VARYINGS], const int vertex_num){
                for (int i = 0; i < vertex_num; i++){
                    float w = varyings[i].clip_pos.w;
                    varyings[i].ndc_pos = varyings[i].clip_pos / w;
                    varyings[i].ndc_pos.w = 1.0f / w;
                }
            }

            template<typename varyings_t>
            static void caculate_frag_pos(varyings_t(&varyings)[RGS_MAX_VARYINGS],
                                        const int vertex_num,
                                        const float width,
                                        const float height){
                for (int i = 0; i < vertex_num; i++){
                    float x = (varyings[i].ndc_pos.x + 1.0f) * 0.5f * width;
                    float y = (varyings[i].ndc_pos.y + 1.0f) * 0.5f * height;
                    float z = (varyings[i].ndc_pos.z + 1.0f) * 0.5f;
                    float w = varyings[i].ndc_pos.w;

                    varyings[i].frag_pos.x = x;
                    varyings[i].frag_pos.y = y;
                    varyings[i].frag_pos.z = z;
                    varyings[i].frag_pos.w = w;
                }
            }

            template<typename vertex_t, typename uniforms_t, typename varyings_t>
            static void rasterize_triangle(FrameBuffer& framebuffer,
                                        const Program<vertex_t, uniforms_t, varyings_t>& program,
                                        const varyings_t(&varyings)[3],
                                        const uniforms_t& uniforms){
                for (int i = 0; i < 3; i++){
                    int x = varyings[i].frag_pos.x;
                    int y = varyings[i].frag_pos.y;
                    for (int j = -5; j < 6; j++){
                        for (int k = -5; k < 6; k++)
                            framebuffer.set_color(x + j, y + k, { 1.0f, 1.0f, 1.0f });
                    }
                }
            }

        public:
            template<typename vertex_t, typename uniforms_t, typename varyings_t>
            static void Draw(FrameBuffer& framebuffer,
                            const Program<vertex_t, uniforms_t, varyings_t>& program,
                            const Triangle<vertex_t>& triangle,
                            const uniforms_t& uniforms) {
                            static_assert(std::is_base_of_v<VertextBase, vertex_t>, "vertex_t 必须继承自VertextBase");
                static_assert(std::is_base_of_v<VaryingBase, varyings_t>, "varyings_t必须继承自VaryingBase");

                //Vertex Shading & Projection
                varyings_t varyings[RGS_MAX_VARYINGS];
                for (int i = 0; i < 3; i++) {
                    program.vertex_shader(triangle[i], uniforms, varyings[i]);
                }

                //Clipping
                int vertexNum = clip(varyings);

                //screen mapping
                caculate_ndc_pos(varyings, vertexNum);
                float f_width = framebuffer.get_width();
                float f_height = framebuffer.get_heigth();
                caculate_frag_pos(varyings, vertexNum, f_width, f_height);

                //triangle assemble & rasterization
                for (int i = 0; i < vertexNum - 2; i++){
                    varyings_t tri_varyings[3];
                    tri_varyings[0] = varyings[0];
                    tri_varyings[1] = varyings[i + 1];
                    tri_varyings[2] = varyings[i + 2];

                    rasterize_triangle(framebuffer, program, tri_varyings, uniforms);
                }

            }
    };
}

#endif