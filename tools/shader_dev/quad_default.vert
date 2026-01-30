#version 460
#extension GL_GOOGLE_include_directive : enable

#include "include/constants.glsl"
#include "include/function.glsl"
#include "include/quad.glsl"
#include "include/ubo_camera.glsl"
#include "include/ubo_quad.glsl"

layout(location = 0) out vec2 out_uv;

/// @brief returns the correct uv orientation if billboard is active
vec2 func_quad_get_correct_uv()
{
    vec2 frag = Quad_UV[gl_VertexIndex];
    frag = (frag * quad.uv_scale) + quad.uv_offset;

    if(quad.billboard == 1.0) 
    {
        if(quad.lock_axis.x == 1.0 && quad.lock_axis.y == 1.0)
        {
            frag = func_rotate_uv(frag, radians(quad.uv_rotation));
            return frag;
        }

        else if(quad.lock_axis.x == 1.0)
        {
            frag = func_rotate_uv(frag, radians(90.0 + quad.uv_rotation));
            frag.y = 1.0 - frag.y;
        }

        else if(quad.lock_axis.y == 1.0)
        {
            frag.x = 1.0 - frag.x;
            frag = func_rotate_uv(frag, radians(quad.uv_rotation));
        }

        else
        {
            frag.x = 1.0 - frag.x;
            frag = func_rotate_uv(frag, radians(quad.uv_rotation));
        }
    }

    else 
    {
        frag = func_rotate_uv(frag, radians(quad.uv_rotation));
    }

    return frag;
}

void main()
{
    mat4 billboard = func_get_billboard_matrix(push_const.model, camera.view, quad.billboard, quad.lock_axis.x, quad.lock_axis.y);
    gl_Position = camera.proj * camera.view * billboard * vec4(Quad_Vertex[gl_VertexIndex].xyz, 1.0);
    out_uv = func_quad_get_correct_uv();
}