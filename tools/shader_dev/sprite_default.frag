#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_sprite.glsl"

layout(set = 0, binding = 2) uniform sampler2D albedo;

layout(location = 0) in vec2 in_uv;
layout(location = 0) out vec4 out_color;

void main()
{
    //// apply UV transformations
    //vec2 transformed_uv = func_transform_uv(in_uv, sprite.uv_offset, sprite.uv_scale, radians(sprite.uv_rotation));
    //
    //// assign albedo color
    //out_color = texture(albedo, transformed_uv);
    //
    //// discard nearly tranparency
    //if(out_color.a < 0.01) discard;

    vec4 tex = texture(albedo, in_uv);
    if (tex.a == 0.0) {
        discard;
    }
    out_color = tex;
}