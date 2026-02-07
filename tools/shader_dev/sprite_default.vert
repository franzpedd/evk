#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/sprite.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_sprite.glsl"

layout(location = 0) out vec2 out_uv;

void main()
{
    // get vertex position
    vec3 vertex_pos = Sprite_Vertex[gl_VertexIndex].xyz;
    
    // apply model matrix
    vec4 world_pos = push_const.model * vec4(vertex_pos, 1.0);
    
    // transform to clip space
    gl_Position = camera.proj * camera.view * world_pos;
    
    // pass UV coordinates unchanged to fragment shader
    out_uv = Sprite_UV[gl_VertexIndex];
}