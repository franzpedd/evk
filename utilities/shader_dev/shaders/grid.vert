#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/ubo_camera.glsl"

layout(location = 1) out vec3 out_near_point;
layout(location = 2) out vec3 out_far_point;

vec3 Grid_Vertex[6] = vec3[]
(
    vec3(1, 1, 0),
    vec3(-1, -1, 0),
    vec3(-1, 1, 0),

    vec3(-1, -1, 0),
    vec3(1, 1, 0),
    vec3(1, -1, 0)
);

/// @brief unprojects points to infinity
vec3 unproject_point(float x, float y, float z, mat4 view, mat4 projection)
{
    vec4 point =  inverse(view) * inverse(projection) * vec4(x, y, z, 1.0);
    return point.xyz / point.w;
}

// entrypoint
void main()
{
    vec3 point = Grid_Vertex[gl_VertexIndex].xyz;

    out_near_point = unproject_point(point.x, point.y, 0.0, camera.view, camera.proj).xyz;
    out_far_point = unproject_point(point.x, point.y, 1.0, camera.view, camera.proj).xyz;

    gl_Position = vec4(point, 1.0);
}