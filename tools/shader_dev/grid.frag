#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/function.glsl"
#include "include/ubo_camera.glsl"

layout(location = 1) in vec3 in_near_point;
layout(location = 2) in vec3 in_far_point;

layout(location = 0) out vec4 out_color;

const float const_near = 0.1;
const float const_far = 256.0;

/// @brief creates a scaled grid visualization with X and Z axis highlighting
vec4 func_create_grid(vec3 pos, float scale)
{
    vec2 coord = pos.xz * scale;
    vec2 coord_dxdy = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / coord_dxdy;
    float line = min(grid.x, grid.y);
    float minimumz = min(coord_dxdy.y, 1);
    float minimumx = min(coord_dxdy.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    if(pos.x > -0.1 * minimumx && pos.x < 0.1 * minimumx) color.z = 1.0; // z axis
    if(pos.z > -0.1 * minimumz && pos.z < 0.1 * minimumz) color.x = 1.0; // x axis

    return color;
}

float func_compute_depth(vec3 pos)
{
    vec4 clip_space = camera.proj * camera.view * vec4(pos.xyz, 1.0);
    return (clip_space.z / clip_space.w);
}

/// @brief calculates linear depth (eye-space Z) normalized to [0, 1]
float func_compute_linear_depth(vec3 pos)
{
    vec4 clip_space = camera.proj * camera.view * vec4(pos.xyz, 1.0);
    float clip_depth = clip_space.z / clip_space.w;
    
    // convert to linear depth in [0, 1] range for perspective projection
    float linear_depth = (2.0 * const_near * const_far) / (const_far + const_near - clip_depth * (const_far - const_near));
    return linear_depth / const_far;
}

void main()
{
    // line parametric equation for ground intersection
    float t = -in_near_point.y / (in_far_point.y - in_near_point.y);
    
    // only render if intersection is in front of camera (t > 0)
    if(t <= 0.0) discard;
    
    // fragment 3D position on ground plane
    vec3 pos = in_near_point + t * (in_far_point - in_near_point);
    
    // set fragment depth
    gl_FragDepth = func_compute_depth(pos);
    
    // calculate fading based on linear depth
    float linear_depth = func_compute_linear_depth(pos);
    float fading = max(0.0, (0.5 - linear_depth));
    
    // create multi-resolution grid (10m and 1m scales)
    vec4 grid_color = (func_create_grid(pos, 10.0) + func_create_grid(pos, 1.0));
    grid_color.a *= fading;
    
    out_color = grid_color;
    
    // discard nearly transparent fragments
    if(out_color.a <= 0.01) discard;
}