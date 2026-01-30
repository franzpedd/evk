#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/quad.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_quad.glsl"

void main()
{
    mat4 billboard = func_get_billboard_matrix(push_const.model, camera.view, quad.billboard, quad.lock_axis.x, quad.lock_axis.y);
    gl_Position = camera.proj * camera.view * billboard * vec4(Quad_Vertex[gl_VertexIndex].xyz, 1.0);
}