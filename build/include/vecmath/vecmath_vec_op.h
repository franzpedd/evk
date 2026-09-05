#ifndef VECMATH_VECMATH_OP_INCLUDED
#define VECMATH_VECMATH_OP_INCLUDED

#include "vecmath_defines.h"
#include "vecmath_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float * value
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_scalar(const float2* v, const float value);
VECMATH_API float3 float3_scalar(const float3* v, const float value);
VECMATH_API float4 float4_scalar(const float4* v, const float value);
VECMATH_API double2 double2_scalar(const double2* v, const float value);
VECMATH_API double3 double3_scalar(const double3* v, const float value);
VECMATH_API double4 double4_scalar(const double4* v, const float value);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float * mat
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_mul_fmat2(const float2* v, const fmat2* m);
VECMATH_API float3 float3_mul_fmat3(const float3* v, const fmat3* m);
VECMATH_API float4 float4_mul_fmat4(const float4* v, const fmat4* m);
VECMATH_API double2 double3_mul_dmat2(const double2* v, const dmat2* m);
VECMATH_API double3 double3_mul_dmat3(const double3* v, const dmat3* m);
VECMATH_API double4 double4_mul_dmat4(const double4* v, const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pythagorean length
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_length(const float2* v);
VECMATH_API float float3_length(const float3* v);
VECMATH_API float float4_length(const float4* v);
VECMATH_API double double2_length(const double2* v);
VECMATH_API double double3_length(const double3* v);
VECMATH_API double double4_length(const double4* v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the distance between vectors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_distance(const float2* a, const float2* b);
VECMATH_API float float3_distance(const float3* a, const float3* b);
VECMATH_API float float4_distance(const float4* a, const float4* b);
VECMATH_API double double2_distance(const double2* a, const double2* b);
VECMATH_API double double3_distance(const double3* a, const double3* b);
VECMATH_API double double4_distance(const double4* a, const double4* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns normalized/unit vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_normalize(const float2* v);
VECMATH_API float3 float3_normalize(const float3* v);
VECMATH_API float4 float4_normalize(const float4* v);
VECMATH_API double2 double2_normalize(const double2* v);
VECMATH_API double3 double3_normalize(const double3* v);
VECMATH_API double4 double4_normalize(const double4* v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the calcualted dot product between vectors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_dot(const float2* a, const float2* b);
VECMATH_API float float3_dot(const float3* a, const float3* b);
VECMATH_API float float4_dot(const float4* a, const float4* b);
VECMATH_API double double2_dot(const double2* a, const double2* b);
VECMATH_API double double3_dot(const double3* a, const double3* b);
VECMATH_API double double4_dot(const double4* a, const double4* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the calcualted cross product between vectors (not mathmatically defined for float4, perpendicular product for float2)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_cross(const float2* a, const float2* b);
VECMATH_API float3 float3_cross(const float3* a, const float3* b);
VECMATH_API double double2_cross(const double2* a, const double2* b);
VECMATH_API double3 double3_cross(const double3* a, const double3* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// apply linear interpolation between a and b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_lerp(const float2* a, const float2* b, float t);
VECMATH_API float3 float3_lerp(const float3* a, const float3* b, float t);
VECMATH_API float4 float4_lerp(const float4* a, const float4* b, float t);
VECMATH_API double2 double2_lerp(const double2* a, const double2* b, double t);
VECMATH_API double3 double3_lerp(const double3* a, const double3* b, double t);
VECMATH_API double4 double4_lerp(const double4* a, const double4* b, double t);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the reflection of a vector given it's normal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_reflect(const float2* v, const float2* normal);
VECMATH_API float3 float3_reflect(const float3* v, const float3* normal);
VECMATH_API float4 float4_reflect(const float4* v, const float4* normal);
VECMATH_API double2 double2_reflect(const double2* v, const double2* normal);
VECMATH_API double3 double3_reflect(const double3* v, const double3* normal);
VECMATH_API double4 double4_reflect(const double4* v, const double4* normal);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the projection of two vectors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_project(const float2* a, const float2* b);
VECMATH_API float3 float3_project(const float3* a, const float3* b);
VECMATH_API float4 float4_project(const float4* a, const float4* b);
VECMATH_API double2 double2_project(const double2* a, const double2* b);
VECMATH_API double3 double3_project(const double3* a, const double3* b);
VECMATH_API double4 double4_project(const double4* a, const double4* b);

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // VECMATH_VECMATH_OP_INCLUDED
