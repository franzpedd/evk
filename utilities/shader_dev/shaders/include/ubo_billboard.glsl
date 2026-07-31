// @brief holds information about a billboard
layout(set = 0, binding = 1) uniform ubo_billboard
{
    uint lockmode;
    float uv_rotation;
    vec2 uv_offset;
    vec2 uv_scale;
} billboard;