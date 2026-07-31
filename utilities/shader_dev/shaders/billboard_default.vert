#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/billboard.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_billboard.glsl"

layout(location = 0) out vec2 out_uv;

void main()
{
    mat4 model = func_get_billboard_matrix(push_const.model, camera.view, billboard.lockmode);
    gl_Position = camera.proj * camera.view * model * vec4(Billboard_Vertex[gl_VertexIndex].xyz, 1.0);
    out_uv = Billboard_UV[gl_VertexIndex];
}