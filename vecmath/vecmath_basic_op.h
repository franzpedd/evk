#ifndef VECMATH_BASIC_OP_INCLUDED
#define VECMATH_BASIC_OP_INCLUDED

#include "vecmath_defines.h"
#include "vecmath_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// zeroing
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_zero();
VECMATH_API float3 float3_zero();
VECMATH_API float4 float4_zero();
VECMATH_API fmat2 fmat2_zero();
VECMATH_API fmat3 fmat3_zero();
VECMATH_API fmat4 fmat4_zero();
VECMATH_API fquat fquat_zero();
VECMATH_API fray fray_zero();

VECMATH_API double2 double2_zero();
VECMATH_API double3 double3_zero();
VECMATH_API double4 double4_zero();
VECMATH_API dmat2 dmat2_zero();
VECMATH_API dmat3 dmat3_zero();
VECMATH_API dmat4 dmat4_zero();
VECMATH_API dquat dquat_zero();
VECMATH_API dray dray_zero();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// oneing
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_one();
VECMATH_API float3 float3_one();
VECMATH_API float4 float4_one();
VECMATH_API fmat2 fmat2_one();
VECMATH_API fmat3 fmat3_one();
VECMATH_API fmat4 fmat4_one();
VECMATH_API fquat fquat_one();
VECMATH_API fray fray_one();

VECMATH_API double2 double2_one();
VECMATH_API double3 double3_one();
VECMATH_API double4 double4_one();
VECMATH_API dmat2 dmat2_one();
VECMATH_API dmat3 dmat3_one();
VECMATH_API dmat4 dmat4_one();
VECMATH_API dquat dquat_one();
VECMATH_API dray dray_one();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a+b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_add(const float2* a, const float2* b);
VECMATH_API float3 float3_add(const float3* a, const float3* b);
VECMATH_API float4 float4_add(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_add(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_add(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_add(const fmat4* a, const fmat4* b);
VECMATH_API fquat fquat_add(const fquat* a, const fquat* b);
VECMATH_API fray fray_add(const fray* a, const fray* b);

VECMATH_API double2 double2_add(const double2* a, const double2* b);
VECMATH_API double3 double3_add(const double3* a, const double3* b);
VECMATH_API double4 double4_add(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_add(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_add(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_add(const dmat4* a, const dmat4* b);
VECMATH_API dquat dquat_add(const dquat* a, const dquat* b);
VECMATH_API dray dray_add(const dray* a, const dray* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a-b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_sub(const float2* a, const float2* b);
VECMATH_API float3 float3_sub(const float3* a, const float3* b);
VECMATH_API float4 float4_sub(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_sub(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_sub(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_sub(const fmat4* a, const fmat4* b);
VECMATH_API fquat fquat_sub(const fquat* a, const fquat* b);
VECMATH_API fray fray_sub(const fray* a, const fray* b);

VECMATH_API double2 double2_sub(const double2* a, const double2* b);
VECMATH_API double3 double3_sub(const double3* a, const double3* b);
VECMATH_API double4 double4_sub(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_sub(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_sub(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_sub(const dmat4* a, const dmat4* b);
VECMATH_API dquat dquat_sub(const dquat* a, const dquat* b);
VECMATH_API dray dray_sub(const dray* a, const dray* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a*b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_mul(const float2* a, const float2* b);
VECMATH_API float3 float3_mul(const float3* a, const float3* b);
VECMATH_API float4 float4_mul(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_mul(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_mul(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_mul(const fmat4* a, const fmat4* b);
VECMATH_API fquat fquat_mul(const fquat* a, const fquat* b);
VECMATH_API fray fray_mul(const fray* a, const fray* b);

VECMATH_API double2 double2_mul(const double2* a, const double2* b);
VECMATH_API double3 double3_mul(const double3* a, const double3* b);
VECMATH_API double4 double4_mul(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_mul(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_mul(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_mul(const dmat4* a, const dmat4* b);
VECMATH_API dquat dquat_mul(const dquat* a, const dquat* b);
VECMATH_API dray dray_mul(const dray* a, const dray* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a/b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_div(const float2* a, const float2* b);
VECMATH_API float3 float3_div(const float3* a, const float3* b);
VECMATH_API float4 float4_div(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_div(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_div(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_div(const fmat4* a, const fmat4* b);
VECMATH_API fquat fquat_div(const fquat* a, const fquat* b);
VECMATH_API fray fray_div(const fray* a, const fray* b);

VECMATH_API double2 double2_div(const double2* a, const double2* b);
VECMATH_API double3 double3_div(const double3* a, const double3* b);
VECMATH_API double4 double4_div(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_div(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_div(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_div(const dmat4* a, const dmat4* b);
VECMATH_API dquat dquat_div(const dquat* a, const dquat* b);
VECMATH_API dray dray_div(const dray* a, const dray* b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// check if equals by value (uses epsilon)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API bool float2_equals(const float2* a, const float2* b);
VECMATH_API bool float3_equals(const float3* a, const float3* b);
VECMATH_API bool float4_equals(const float4* a, const float4* b);
VECMATH_API bool fmat2_equals(const fmat2* a, const fmat2* b);
VECMATH_API bool fmat3_equals(const fmat3* a, const fmat3* b);
VECMATH_API bool fmat4_equals(const fmat4* a, const fmat4* b);
VECMATH_API bool fquat_equals(const fquat* a, const fquat* b);
VECMATH_API bool fray_equals(const fray* a, const fray* b);

VECMATH_API bool double2_equals(const double2* a, const double2* b);
VECMATH_API bool double3_equals(const double3* a, const double3* b);
VECMATH_API bool double4_equals(const double4* a, const double4* b);
VECMATH_API bool dmat2_equals(const dmat2* a, const dmat2* b);
VECMATH_API bool dmat3_equals(const dmat3* a, const dmat3* b);
VECMATH_API bool dmat4_equals(const dmat4* a, const dmat4* b);
VECMATH_API bool dquat_equals(const dquat* a, const dquat* b);
VECMATH_API bool dray_equals(const dray* a, const dray* b);

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // VECMATH_BASIC_OP_INCLUDED
