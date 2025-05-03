#include "RGS\Renderer.hpp"

#include<math.h>

bool RGS::Renderer::is_vertex_Visble(const Vec4& clip_pos) {
    const bool x = std::fabs(clip_pos.x) <= clip_pos.w;
    const bool y = std::fabs(clip_pos.y) <= clip_pos.w;
    const bool z = std::fabs(clip_pos.z) <= clip_pos.w;
    return x && y && z;
}
bool RGS::Renderer::is_inside_plane(const Vec4& clip_pos, const Plane plane){

    //算(axis - w)是否在-0.000001~0.000001之间
    auto is_legal = [](float axis, float w){
        const double f = axis - w;
        bool b = (f >= 0);
        return b = b ? (f < 1e-6) : (-1e-6 < f);
    };
    
    switch (plane){
    case Plane::PSOITIVE_W:
        return is_legal(0.0f, clip_pos.w) || clip_pos.w >= 0.0f;
    case Plane::PSOITIVE_X:
        return is_legal(clip_pos.x, clip_pos.w) || clip_pos.x <= +clip_pos.w;
    case Plane::NEGATIVE_X:
        return is_legal(clip_pos.x, clip_pos.w) || clip_pos.x >= -clip_pos.w;
    case Plane::PSOITIVE_Y:
        return is_legal(clip_pos.y, clip_pos.w) || clip_pos.y <= +clip_pos.w;
    case Plane::NEGATIVE_Y:
        return is_legal(clip_pos.y, clip_pos.w) || clip_pos.y >= -clip_pos.w;
    case Plane::PSOITIVE_Z:
        return is_legal(clip_pos.z, clip_pos.w) || clip_pos.z <= +clip_pos.w;
    case Plane::NEGATIVE_Z:
        return is_legal(clip_pos.z, clip_pos.w) || clip_pos.z >= -clip_pos.w;
    default:
        ASSERT(false);
        return false;
    }
}
float RGS::Renderer::get_intersect_ratio(const Vec4& prev, const Vec4& curr, const Plane plane) {
    switch(plane){
    case Plane::PSOITIVE_W:
        return (prev.w - 0.0f) / (prev.w - curr.w);
    case Plane::PSOITIVE_X:
        return (prev.w - prev.x) / ((prev.w - prev.x) - (curr.w - curr.x));
    case Plane::NEGATIVE_X:
        return (prev.w + prev.x) / ((prev.w + prev.x) - (curr.w + curr.x));
    case Plane::PSOITIVE_Y:
        return (prev.w - prev.y) / ((prev.w - prev.y) - (curr.w - curr.y));
    case Plane::NEGATIVE_Y:
        return (prev.w + prev.y) / ((prev.w + prev.y) - (curr.w + curr.y));
    case Plane::PSOITIVE_Z:
        return (prev.w - prev.z) / ((prev.w - prev.z) - (curr.w - curr.z));
    case Plane::NEGATIVE_Z:
        return (prev.w + prev.z) / ((prev.w + prev.z) - (curr.w + curr.z));
    default:
        ASSERT(false);
        return 0.0f;
    }
}