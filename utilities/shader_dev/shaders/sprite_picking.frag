#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/ubo_camera.glsl"

layout(set = 0, binding = 2) uniform sampler2D albedo; // this is here because it uses the same descriptors as the default quad shader, but it's a waste of resources
layout(location = 0) out uint out_color;

void main()
{
    out_color = push_const.id;
}