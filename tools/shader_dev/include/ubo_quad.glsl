// @brief holds information about a quad
layout(set = 0, binding = 1) uniform ubo_quad
{
    float billboard;
    float uv_rotation;
    vec2 lock_axis;
    vec2 uv_offset;
    vec2 uv_scale;
} quad;