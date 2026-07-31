#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/sprite.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_sprite.glsl"

void main()
{
    vec3 vertex_pos = Sprite_Vertex[gl_VertexIndex].xyz;
    vec4 world_pos = push_const.model * vec4(vertex_pos, 1.0);
    gl_Position = camera.proj * camera.view * world_pos;
}