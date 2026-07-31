#include "vecmath_vec_op.h"
#include "vecmath_basic_op.h"
#include <math.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float * value
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_scalar(const float2* v, const float value) {
    float2 result = { 0 };
    result.x = v->x * value;
    result.y = v->y * value;
    return result;
}

VECMATH_FUNC float3 float3_scalar(const float3* v, const float value) {
    float3 result = { 0 };
    result.x = v->x * value;
    result.y = v->y * value;
    result.z = v->z * value;
    return result;
}

VECMATH_FUNC float4 float4_scalar(const float4* v, const float value) {
    float4 result = { 0 };
    result.x = v->x * value;
    result.y = v->y * value;
    result.z = v->z * value;
    result.w = v->w * value;
    return result;
}

VECMATH_FUNC double2 double2_scalar(const double2 *v, const float value) {
    double2 result = { 0 };
    result.x = v->x * value;
    result.y = v->y * value;
    return result;
}

VECMATH_FUNC double3 double3_scalar(const double3 *v, const float value) {
    double3 result = { 0 };
    result.x = v->x * value;
    result.y = v->y * value;
    result.z = v->z * value;
    return result;
}

VECMATH_FUNC double4 double4_scalar(const double4 *v, const float value) {
    double4 result = { 0 };
    result.x = v->x * value;
    result.y = v->y * value;
    result.z = v->z * value;
    result.w = v->w * value;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float * mat
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_mul_fmat2(const float2* v, const fmat2* m) {
    float2 result;
    result.x = v->x * m->data[0][0] + v->y * m->data[1][0];
    result.y = v->x * m->data[0][1] + v->y * m->data[1][1];
    return result;
}

VECMATH_FUNC float3 float3_mul_fmat3(const float3* v, const fmat3* m) {
    float3 result;
    result.x = v->x * m->data[0][0] + v->y * m->data[1][0] + v->z * m->data[2][0];
    result.y = v->x * m->data[0][1] + v->y * m->data[1][1] + v->z * m->data[2][1];
    result.z = v->x * m->data[0][2] + v->y * m->data[1][2] + v->z * m->data[2][2];
    return result;
}

VECMATH_FUNC float4 float4_mul_fmat4(const float4* v, const fmat4* m) {
    float4 result;
    result.x = v->x * m->data[0][0] + v->y * m->data[1][0] + v->z * m->data[2][0] + v->w * m->data[3][0];
    result.y = v->x * m->data[0][1] + v->y * m->data[1][1] + v->z * m->data[2][1] + v->w * m->data[3][1];
    result.z = v->x * m->data[0][2] + v->y * m->data[1][2] + v->z * m->data[2][2] + v->w * m->data[3][2];
    result.w = v->x * m->data[0][3] + v->y * m->data[1][3] + v->z * m->data[2][3] + v->w * m->data[3][3];
    return result;
}

VECMATH_FUNC double2 double3_mul_dmat2(const double2* v, const dmat2* m) {
    double2 result;
    result.x = v->x * m->data[0][0] + v->y * m->data[1][0];
    result.y = v->x * m->data[0][1] + v->y * m->data[1][1];
    return result;
}

VECMATH_FUNC double3 double3_mul_dmat3(const double3* v, const dmat3* m) {
    double3 result;
    result.x = v->x * m->data[0][0] + v->y * m->data[1][0] + v->z * m->data[2][0];
    result.y = v->x * m->data[0][1] + v->y * m->data[1][1] + v->z * m->data[2][1];
    result.z = v->x * m->data[0][2] + v->y * m->data[1][2] + v->z * m->data[2][2];
    return result;
}

VECMATH_FUNC double4 double4_mul_dmat4(const double4* v, const dmat4* m) {
    double4 result;
    result.x = v->x * m->data[0][0] + v->y * m->data[1][0] + v->z * m->data[2][0] + v->w * m->data[3][0];
    result.y = v->x * m->data[0][1] + v->y * m->data[1][1] + v->z * m->data[2][1] + v->w * m->data[3][1];
    result.z = v->x * m->data[0][2] + v->y * m->data[1][2] + v->z * m->data[2][2] + v->w * m->data[3][2];
    result.w = v->x * m->data[0][3] + v->y * m->data[1][3] + v->z * m->data[2][3] + v->w * m->data[3][3];
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pythagorean length
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float float2_length(const float2 *v) {
    return sqrtf((v->x * v->x) + (v->y * v->y));
}

VECMATH_FUNC float float3_length(const float3* v) {
    return sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

VECMATH_FUNC float float4_length(const float4* v) {
    return sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z) + (v->w * v->w));
}

VECMATH_FUNC double double2_length(const double2* v) {
    return sqrt((v->x * v->x) + (v->y * v->y));
}

VECMATH_FUNC double double3_length(const double3* v) {
    return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

VECMATH_FUNC double double4_length(const double4* v) {
    return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z) + (v->w * v->w));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the distance between vectors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float float2_distance(const float2* a, const float2* b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    return sqrtf(dx * dx + dy * dy);
}

VECMATH_FUNC float float3_distance(const float3* a, const float3* b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dz = a->z - b->z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

VECMATH_FUNC float float4_distance(const float4* a, const float4* b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dz = a->z - b->z;
    float dw = a->w - b->w;
    return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
}

VECMATH_FUNC double double2_distance(const double2* a, const double2* b) {
    double dx = a->x - b->x;
    double dy = a->y - b->y;
    return sqrt(dx * dx + dy * dy);
}

VECMATH_FUNC double double3_distance(const double3* a, const double3* b) {
    double dx = a->x - b->x;
    double dy = a->y - b->y;
    double dz = a->z - b->z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

VECMATH_FUNC double double4_distance(const double4* a, const double4* b) {
    double dx = a->x - b->x;
    double dy = a->y - b->y;
    double dz = a->z - b->z;
    double dw = a->w - b->w;
    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns normalized/unit vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_normalize(const float2* v) {
    float len = float2_length(v);
    if (len < VECMATH_FLT_EPSILON) return (float2){0.0f, 0.0f};

    float inv_len = 1.0f / len;
    return (float2){v->x * inv_len, v->y * inv_len};
}

VECMATH_FUNC float3 float3_normalize(const float3* v) {
    float len = float3_length(v);
    if (len < VECMATH_FLT_EPSILON) return (float3){0.0f, 0.0f, 0.0f};

    float inv_len = 1.0f / len;
    return (float3){v->x * inv_len, v->y * inv_len, v->z * inv_len};
}

VECMATH_FUNC float4 float4_normalize(const float4* v) {
    float len = float4_length(v);
    if (len < VECMATH_FLT_EPSILON) return (float4){0.0f, 0.0f, 0.0f, 0.0f};

    float inv_len = 1.0f / len;
    return (float4){v->x * inv_len, v->y * inv_len, v->z * inv_len, v->w * inv_len};
}

// Double versions
VECMATH_FUNC double2 double2_normalize(const double2* v) {
    double len = double2_length(v);
    if (len < VECMATH_DBL_EPSILON) return (double2){0.0, 0.0};

    double inv_len = 1.0 / len;
    return (double2){v->x * inv_len, v->y * inv_len};
}

VECMATH_FUNC double3 double3_normalize(const double3* v) {
    double len = double3_length(v);
    if (len < VECMATH_DBL_EPSILON) return (double3){0.0, 0.0, 0.0};

    double inv_len = 1.0 / len;
    return (double3){v->x * inv_len, v->y * inv_len, v->z * inv_len};
}

VECMATH_FUNC double4 double4_normalize(const double4* v) {
    double len = double4_length(v);
    if (len < VECMATH_DBL_EPSILON) return (double4){0.0, 0.0, 0.0, 0.0};

    double inv_len = 1.0 / len;
    return (double4){v->x * inv_len, v->y * inv_len, v->z * inv_len, v->w * inv_len};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the calcualted dot product between vectors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float float2_dot(const float2* a, const float2* b) {
    return (a->x * b->x) + (a->y * b->y);
}

VECMATH_FUNC float float3_dot(const float3* a, const float3* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

VECMATH_FUNC float float4_dot(const float4* a, const float4* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z) + (a->w * b->w);
}

VECMATH_FUNC double double2_dot(const double2* a, const double2* b) {
    return (a->x * b->x) + (a->y * b->y);
}

VECMATH_FUNC double double3_dot(const double3* a, const double3* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

VECMATH_FUNC double double4_dot(const double4* a, const double4* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z) + (a->w * b->w);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the calcualted cross product between vectors (not mathmatically defined for float4, perpendicular product for float2)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float float2_cross(const float2* a, const float2* b) {
    return (a->x * b->y) - (a->y * b->x);
}

VECMATH_FUNC float3 float3_cross(const float3* a, const float3* b) {
    float3 result = { 0 };
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    return result; 
}

VECMATH_FUNC double double2_cross(const double2* a, const double2* b) {
    return (a->x * b->y) - (a->y * b->x);
}

VECMATH_FUNC double3 double3_cross(const double3* a, const double3* b) {
    double3 result = { 0 };
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// apply linear interpolation between a and b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_lerp(const float2* a, const float2* b, float t) {
    float2 result = { 0 };
    result.x = a->x + (b->x - a->x) * t;
    result.y = a->y + (b->y - a->y) * t;
    return result;
}

VECMATH_FUNC float3 float3_lerp(const float3* a, const float3* b, float t) {
    float3 result = { 0 };
    result.x = a->x + (b->x - a->x) * t;
    result.y = a->y + (b->y - a->y) * t;
    result.z = a->z + (b->z - a->z) * t;
    return result;
}

VECMATH_FUNC float4 float4_lerp(const float4* a, const float4* b, float t) {
    float4 result = { 0 };
    result.x = a->x + (b->x - a->x) * t;
    result.y = a->y + (b->y - a->y) * t;
    result.z = a->z + (b->z - a->z) * t;
    result.w = a->w + (b->w - a->w) * t;
    return result;
}

VECMATH_FUNC double2 double2_lerp(const double2* a, const double2* b, double t) {
    double2 result = { 0 };
    result.x = a->x + (b->x - a->x) * t;
    result.y = a->y + (b->y - a->y) * t;
    return result;
}

VECMATH_FUNC double3 double3_lerp(const double3* a, const double3* b, double t) {
    double3 result = { 0 };
    result.x = a->x + (b->x - a->x) * t;
    result.y = a->y + (b->y - a->y) * t;
    result.z = a->z + (b->z - a->z) * t;
    return result;
}

VECMATH_FUNC double4 double4_lerp(const double4* a, const double4* b, double t) {
    double4 result = { 0 };
    result.x = a->x + (b->x - a->x) * t;
    result.y = a->y + (b->y - a->y) * t;
    result.z = a->z + (b->z - a->z) * t;
    result.w = a->w + (b->w - a->w) * t;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the reflection of a vector given it's normal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_reflect(const float2* v, const float2* normal) {
    float2 result = { 0 };
    float dot = float2_dot(v, normal);
    float normal_length_sq = float2_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_FLT_EPSILON) {
        float factor = 2.0f * dot / normal_length_sq;
        result.x = v->x - factor * normal->x;
        result.y = v->y - factor * normal->y;
    }
    return result;
}

VECMATH_FUNC float3 float3_reflect(const float3* v, const float3* normal)
{
    float3 result = { 0 };
    float dot = float3_dot(v, normal);
    float normal_length_sq = float3_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_FLT_EPSILON) {
        float factor = 2.0f * dot / normal_length_sq;
        result.x = v->x - factor * normal->x;
        result.y = v->y - factor * normal->y;
        result.z = v->z - factor * normal->z;
    }
    return result;
}

VECMATH_FUNC float4 float4_reflect(const float4* v, const float4* normal)
{
    float4 result = { 0 };
    float dot = float4_dot(v, normal);
    float normal_length_sq = float4_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_FLT_EPSILON) {
        float factor = 2.0f * dot / normal_length_sq;
        result.x = v->x - factor * normal->x;
        result.y = v->y - factor * normal->y;
        result.z = v->z - factor * normal->z;
        result.w = v->w - factor * normal->w;
    }
    return result;
}

VECMATH_FUNC double2 double2_reflect(const double2* v, const double2* normal)
{
    double2 result = { 0 };
    double dot = double2_dot(v, normal);
    double normal_length_sq = double2_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_DBL_EPSILON) {
        double factor = 2.0f * dot / normal_length_sq;
        result.x = v->x - factor * normal->x;
        result.y = v->y - factor * normal->y;
    }
    return result;
}

VECMATH_FUNC double3 double3_reflect(const double3* v, const double3* normal)
{
    double3 result = { 0 };
    double dot = double3_dot(v, normal);
    double normal_length_sq = double3_dot(normal, normal);
    
    if (normal_length_sq >VECMATH_DBL_EPSILON) {
        double factor = 2.0f * dot / normal_length_sq;
        result.x = v->x - factor * normal->x;
        result.y = v->y - factor * normal->y;
        result.z = v->z - factor * normal->z;
    }
    return result;
}

VECMATH_FUNC double4 double4_reflect(const double4* v, const double4* normal)
{
    double4 result = { 0 };
    double dot = double4_dot(v, normal);
    double normal_length_sq = double4_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_DBL_EPSILON) {
        double factor = 2.0f * dot / normal_length_sq;
        result.x = v->x - factor * normal->x;
        result.y = v->y - factor * normal->y;
        result.z = v->z - factor * normal->z;
        result.w = v->w - factor * normal->w;
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the projection of two vectors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_project(const float2* a, const float2* b) {
    float2 result = { 0 };
    result.x = 0.0f;
    result.y = 0.0f;

    float dot_ab = float2_dot(a, b);
    float dot_bb = float2_dot(b, b);
    
    if (dot_bb > VECMATH_FLT_EPSILON) {
        float scale = dot_ab / dot_bb;
        result.x = b->x * scale;
        result.y = b->y * scale;
    }
    return result;
}

VECMATH_FUNC float3 float3_project(const float3* a, const float3* b) {
    float3 result = { 0 };
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;

    float dot_ab = float3_dot(a, b);
    float dot_bb = float3_dot(b, b);
    
    if (dot_bb > VECMATH_FLT_EPSILON) {
        float scale = dot_ab / dot_bb;
        result.x = b->x * scale;
        result.y = b->y * scale;
        result.z = b->z * scale;
    }
    return result;
}

VECMATH_FUNC float4 float4_project(const float4 *a, const float4 *b) {
    float4 result = { 0 };
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;
    result.w = 0.0f;

    float dot_ab = float4_dot(a, b);
    float dot_bb = float4_dot(b, b);
    
    if (dot_bb > VECMATH_FLT_EPSILON) {
        float scale = dot_ab / dot_bb;
        result.x = b->x * scale;
        result.y = b->y * scale;
        result.z = b->z * scale;
        result.w = b->w * scale;
    }
    return result;
}

VECMATH_FUNC double2 double2_project(const double2* a, const double2* b) {
    double2 result = { 0 };
    result.x = 0.0f;
    result.y = 0.0f;

    double dot_ab = double2_dot(a, b);
    double dot_bb = double2_dot(b, b);
    
    if (dot_bb > VECMATH_DBL_EPSILON) {
        double scale = dot_ab / dot_bb;
        result.x = b->x * scale;
        result.y = b->y * scale;
    }
    return result;
}

VECMATH_FUNC double3 double3_project(const double3 *a, const double3 *b) {
    double3 result = { 0 };
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;

    double dot_ab = double3_dot(a, b);
    double dot_bb = double3_dot(b, b);
    
    if (dot_bb > VECMATH_DBL_EPSILON) {
        double scale = dot_ab / dot_bb;
        result.x = b->x * scale;
        result.y = b->y * scale;
        result.z = b->z * scale;
   }
    return result;
}

VECMATH_FUNC double4 double4_project(const double4 *a, const double4 *b) {
    double4 result = { 0 };
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;
    result.w = 0.0f;

    double dot_ab = double4_dot(a, b);
    double dot_bb = double4_dot(b, b);
    
    if (dot_bb > VECMATH_DBL_EPSILON) {
        double scale = dot_ab / dot_bb;
        result.x = b->x * scale;
        result.y = b->y * scale;
        result.z = b->z * scale;
        result.w = b->w * scale;
    }
    return result;
}

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */