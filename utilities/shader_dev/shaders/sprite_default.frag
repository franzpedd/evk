#version 460
#extension GL_GOOGLE_include_directive : enable

layout(set = 0, binding = 2) uniform sampler2D albedo;

layout(location = 0) in vec2 in_uv;
layout(location = 0) out vec4 out_color;

void main()
{
    vec4 tex = texture(albedo, in_uv);
    if (tex.a == 0.0) discard;
    out_color = tex;
}