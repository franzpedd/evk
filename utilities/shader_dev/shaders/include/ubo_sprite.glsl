// @brief holds information about a sprite
layout(set = 0, binding = 1) uniform ubo_sprite
{
    float uv_rotation;
    vec2 uv_offset;
    vec2 uv_scale;
} sprite;