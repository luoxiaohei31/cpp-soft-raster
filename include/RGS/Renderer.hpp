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
                PSOITIVE_W, PSOITIVE_X, PSOITIVE_Y,
                PSOITIVE_Z, NEGATIVE_X, NEGATIVE_Y, NEGATIVE_Z
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
                vertex_num = clip_against_plane(varyings_, varyings, Plane::PSOITIVE_X, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings, varyings_, Plane::NEGATIVE_X, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings_, varyings, Plane::PSOITIVE_Y, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings, varyings_, Plane::NEGATIVE_Y, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings_, varyings, Plane::PSOITIVE_Z, vertex_num);
                if (vertex_num == 0) return 0;
                vertex_num = clip_against_plane(varyings, varyings_, Plane::NEGATIVE_Z, vertex_num);
                if (vertex_num == 0) return 0;
                memcpy(varyings, varyings_, sizeof(varyings_));

                return vertex_num;
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
                int vertexNum=clip(varyings);
            }
    };
}

#endif