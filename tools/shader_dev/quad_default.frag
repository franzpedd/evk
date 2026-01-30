#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_quad.glsl"

layout(set = 0, binding = 2) uniform sampler2D albedo;

layout(location = 0) in vec2 in_uv;
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = texture(albedo, func_transform_uv(in_uv, quad.uv_offset, quad.uv_scale, radians(quad.uv_rotation)));
    if(out_color.a == 0.0) discard;
}