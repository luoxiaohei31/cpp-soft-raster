#include "RGS\Renderer.hpp"

#include<math.h>
#include<algorithm>

bool RGS::Renderer::is_vertex_Visble(const Vec4& clip_pos) {
    const bool x = std::fabs(clip_pos.x) <= clip_pos.w;
    const bool y = std::fabs(clip_pos.y) <= clip_pos.w;
    const bool z = std::fabs(clip_pos.z) <= clip_pos.w;
    return x && y && z;
}

bool RGS::Renderer::is_inside_plane(const Vec4& clip_pos, const Plane plane){

    //算(axis - w)是否在-0.00001~0.00001之间
    auto is_legal = [](float axis, float w){
        axis = std::fabs(axis);
        const double f = axis - w;
        bool b = (f >= 0);
        return b = b ? (f < 1e-5) : (-1e-5 < f);
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

float RGS::Renderer::get_intersect_ratio(const Vec4& prev, const Vec4& curr, const Plane plane){
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

void RGS::Renderer::calculate_weights(float(&screen_weights)[3], float(&weights)[3], const Vec4(&frag_coord)[3], const Vec2& screen_point){
    Vec2 ab = frag_coord[1] - frag_coord[0];
    Vec2 ac = frag_coord[2] - frag_coord[0];
    Vec2 ap = screen_point - frag_coord[0];
    float factor = 1.0f / (ab.x * ac.y - ab.y * ac.x);
    float s = (ac.y * ap.x - ac.x * ap.y) * factor;
    float t = (ab.x * ap.y - ab.y * ap.x) * factor;
    screen_weights[0] = 1 - s - t;
    screen_weights[1] = s;
    screen_weights[2] = t;

    float w0 = frag_coord[0].w * screen_weights[0];
    float w1 = frag_coord[1].w * screen_weights[1];
    float w2 = frag_coord[2].w * screen_weights[2];
    float normalizer = 1.0f / (w0 + w1 + w2);
    weights[0] = w0 * normalizer;
    weights[1] = w1 * normalizer;
    weights[2] = w2 * normalizer;
}

bool RGS::Renderer::is_inside_triangle(float(&weights)[3], const float epsilon){
    return weights[0] >= -epsilon && weights[1] >= -epsilon && weights[2] >= -epsilon;
}

RGS::Renderer::BoundingBox RGS::Renderer::get_bounding_box(const Vec4(&frag_coord)[3], const int width, const int height){
    auto xlist = { frag_coord[0].x, frag_coord[1].x, frag_coord[2].x };
    auto ylist = { frag_coord[0].y, frag_coord[1].y, frag_coord[2].y };

    float x_min = std::min<float>(xlist);
    float x_max = std::max<float>(xlist);
    float y_min = std::min<float>(ylist);
    float y_max = std::max<float>(ylist);

    x_min = RGS::clamp(x_min, 0.0f, (float)(width - 1));
    x_max = RGS::clamp(x_max, 0.0f, (float)(width - 1));
    y_min = RGS::clamp(y_min, 0.0f, (float)(height - 1));
    y_max = RGS::clamp(y_max, 0.0f, (float)(height - 1));

    BoundingBox bBox;
    bBox.min_x = std::floor(x_min);
    bBox.max_x = std::ceil(x_max);
    bBox.min_y = std::floor(y_min);
    bBox.max_y = std::ceil(y_max);
    
    return bBox;
}

bool RGS::Renderer::is_back_facing(const Vec4& a, const Vec4& b, const Vec4& c){
    //逆时针为正面（可见）
    //(b.x-a.x)*(c.y-b.y)-(b.y-a.y)*(c.x-b.x)
    float signed_area = a.x * b.y - a.y * b.x +
                        b.x * c.y - b.y * c.x +
                        c.x * a.y - c.y * a.x;
    return signed_area <= 0;
}

bool RGS::Renderer::pass_depth_test(float write_depth, float f_depth, DepthFuncType depth_func){

    const float EPSILON = 1e-5f;
    switch (depth_func)
    {
    case DepthFuncType::LESS:
        return f_depth - write_depth > EPSILON;
    case DepthFuncType::LEQUAL:
        return f_depth - write_depth >= -EPSILON;
    case DepthFuncType::ALWAYS:
        return true;
    default:
        return false;
    }
}