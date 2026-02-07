#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/sprite.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_sprite.glsl"

void main()
{
    gl_Position = camera.proj * camera.view * vec4(Sprite_Vertex[gl_VertexIndex].xyz, 1.0);
}