#include "vecmath.h"

#include <math.h>
#include <string.h>

// functions implementation

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// add 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_add(const float2* a, const float2* b)
{
    float2 result = { 0 };
    result.xy.x = a->xy.x + b->xy.x;
    result.xy.y = a->xy.y + b->xy.y;
    return result;
}

VECMATH_API float3 float3_add(const float3* a, const float3* b)
{
    float3 result = { 0 };
    result.xyz.x = a->xyz.x + b->xyz.x;
    result.xyz.y = a->xyz.y + b->xyz.y;
    result.xyz.z = a->xyz.z + b->xyz.z;
    return result;
}

VECMATH_API float4 float4_add(const float4* a, const float4* b)
{
    float4 result = { 0 };
    result.xyzw.x = a->xyzw.x + b->xyzw.x;
    result.xyzw.y = a->xyzw.y + b->xyzw.y;
    result.xyzw.z = a->xyzw.z + b->xyzw.z;
    result.xyzw.w = a->xyzw.w + b->xyzw.w;
    return result;
}

VECMATH_API fmat2 fmat2_add(const fmat2* a, const fmat2* b)
{
    fmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 + b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 + b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 + b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 + b->matrix.m11;
    return result;
}

VECMATH_API fmat3 fmat3_add(const fmat3* a, const fmat3* b)
{
    fmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 + b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 + b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 + b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 + b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 + b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 + b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 + b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 + b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 + b->matrix.m22;
    return result;
}

VECMATH_API fmat4 fmat4_add(const fmat4* a, const fmat4* b)
{
    fmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 + b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 + b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 + b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 + b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 + b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 + b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 + b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 + b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 + b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 + b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 + b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 + b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 + b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 + b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 + b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 + b->matrix.m33;
    return result;
}

VECMATH_API double2 double2_add(const double2* a, const double2* b)
{
    double2 result = { 0 };
    result.xy.x = a->xy.x + b->xy.x;
    result.xy.y = a->xy.y + b->xy.y;
    return result;
}

VECMATH_API double3 double3_add(const double3* a, const double3* b)
{
    double3 result = { 0 };
    result.xyz.x = a->xyz.x + b->xyz.x;
    result.xyz.y = a->xyz.y + b->xyz.y;
    result.xyz.z = a->xyz.z + b->xyz.z;
    return result;
}

VECMATH_API double4 double4_add(const double4* a, const double4* b)
{
    double4 result = { 0 };
    result.xyzw.x = a->xyzw.x + b->xyzw.x;
    result.xyzw.y = a->xyzw.y + b->xyzw.y;
    result.xyzw.z = a->xyzw.z + b->xyzw.z;
    result.xyzw.w = a->xyzw.w + b->xyzw.w;
    return result;
}

VECMATH_API dmat2 dmat2_add(const dmat2* a, const dmat2* b)
{
    dmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 + b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 + b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 + b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 + b->matrix.m11;
    return result;
}

VECMATH_API dmat3 dmat3_add(const dmat3* a, const dmat3* b)
{
    dmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 + b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 + b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 + b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 + b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 + b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 + b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 + b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 + b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 + b->matrix.m22;
    return result;
}

VECMATH_API dmat4 dmat4_add(const dmat4* a, const dmat4* b)
{
    dmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 + b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 + b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 + b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 + b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 + b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 + b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 + b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 + b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 + b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 + b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 + b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 + b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 + b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 + b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 + b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 + b->matrix.m33;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// sub
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_sub(const float2* a, const float2* b)
{
    float2 result = { 0 };
    result.xy.x = a->xy.x - b->xy.x;
    result.xy.y = a->xy.y - b->xy.y;
    return result;
}

VECMATH_API float3 float3_sub(const float3* a, const float3* b)
{
    float3 result = { 0 };
    result.xyz.x = a->xyz.x - b->xyz.x;
    result.xyz.y = a->xyz.y - b->xyz.y;
    result.xyz.z = a->xyz.z - b->xyz.z;
    return result;
}

VECMATH_API float4 float4_sub(const float4* a, const float4* b)
{
    float4 result = { 0 };
    result.xyzw.x = a->xyzw.x - b->xyzw.x;
    result.xyzw.y = a->xyzw.y - b->xyzw.y;
    result.xyzw.z = a->xyzw.z - b->xyzw.z;
    result.xyzw.w = a->xyzw.w - b->xyzw.w;
    return result;
}

VECMATH_API fmat2 fmat2_sub(const fmat2* a, const fmat2* b)
{
    fmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 - b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 - b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 - b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 - b->matrix.m11;
    return result;
}

VECMATH_API fmat3 fmat3_sub(const fmat3* a, const fmat3* b)
{
    fmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 - b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 - b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 - b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 - b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 - b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 - b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 - b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 - b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 - b->matrix.m22;
    return result;
}

VECMATH_API fmat4 fmat4_sub(const fmat4* a, const fmat4* b)
{
    fmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 - b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 - b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 - b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 - b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 - b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 - b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 - b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 - b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 - b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 - b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 - b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 - b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 - b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 - b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 - b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 - b->matrix.m33;
    return result;
}

VECMATH_API double2 double2_sub(const double2* a, const double2* b)
{
    double2 result = { 0 };
    result.xy.x = a->xy.x - b->xy.x;
    result.xy.y = a->xy.y - b->xy.y;
    return result;
}

VECMATH_API double3 double3_sub(const double3* a, const double3* b)
{
    double3 result = { 0 };
    result.xyz.x = a->xyz.x - b->xyz.x;
    result.xyz.y = a->xyz.y - b->xyz.y;
    result.xyz.z = a->xyz.z - b->xyz.z;
    return result;
}

VECMATH_API double4 double4_sub(const double4* a, const double4* b)
{
    double4 result = { 0 };
    result.xyzw.x = a->xyzw.x - b->xyzw.x;
    result.xyzw.y = a->xyzw.y - b->xyzw.y;
    result.xyzw.z = a->xyzw.z - b->xyzw.z;
    result.xyzw.w = a->xyzw.w - b->xyzw.w;
    return result;
}

VECMATH_API dmat2 dmat2_sub(const dmat2* a, const dmat2* b)
{
    dmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 - b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 - b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 - b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 - b->matrix.m11;
    return result;
}

VECMATH_API dmat3 dmat3_sub(const dmat3* a, const dmat3* b)
{
    dmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 - b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 - b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 - b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 - b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 - b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 - b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 - b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 - b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 - b->matrix.m22;
    return result;
}

VECMATH_API dmat4 dmat4_sub(const dmat4* a, const dmat4* b)
{
    dmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 - b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 - b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 - b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 - b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 - b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 - b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 - b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 - b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 - b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 - b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 - b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 - b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 - b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 - b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 - b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 - b->matrix.m33;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// mul
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_mul(const float2* a, const float2* b)
{
    float2 result = { 0 };
    result.xy.x = a->xy.x * b->xy.x;
    result.xy.y = a->xy.y * b->xy.y;
    return result;
}

VECMATH_API float3 float3_mul(const float3* a, const float3* b)
{
    float3 result = { 0 };
    result.xyz.x = a->xyz.x * b->xyz.x;
    result.xyz.y = a->xyz.y * b->xyz.y;
    result.xyz.z = a->xyz.z * b->xyz.z;
    return result;
}

VECMATH_API float4 float4_mul(const float4* a, const float4* b)
{
    float4 result = { 0 };
    result.xyzw.x = a->xyzw.x * b->xyzw.x;
    result.xyzw.y = a->xyzw.y * b->xyzw.y;
    result.xyzw.z = a->xyzw.z * b->xyzw.z;
    result.xyzw.w = a->xyzw.w * b->xyzw.w;
    return result;
}

VECMATH_API fmat2 fmat2_mul(const fmat2* a, const fmat2* b)
{
    fmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 * b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 * b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 * b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 * b->matrix.m11;
    return result;
}

VECMATH_API fmat3 fmat3_mul(const fmat3* a, const fmat3* b)
{
    fmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 * b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 * b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 * b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 * b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 * b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 * b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 * b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 * b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 * b->matrix.m22;
    return result;
}

VECMATH_API fmat4 fmat4_mul(const fmat4* a, const fmat4* b)
{
    fmat4 result = { 0 };

    // column 0 of result
    result.matrix.m00 = a->matrix.m00 * b->matrix.m00 + a->matrix.m01 * b->matrix.m10 + a->matrix.m02 * b->matrix.m20 + a->matrix.m03 * b->matrix.m30;
    result.matrix.m10 = a->matrix.m10 * b->matrix.m00 + a->matrix.m11 * b->matrix.m10 + a->matrix.m12 * b->matrix.m20 + a->matrix.m13 * b->matrix.m30;
    result.matrix.m20 = a->matrix.m20 * b->matrix.m00 + a->matrix.m21 * b->matrix.m10 + a->matrix.m22 * b->matrix.m20 + a->matrix.m23 * b->matrix.m30;
    result.matrix.m30 = a->matrix.m30 * b->matrix.m00 + a->matrix.m31 * b->matrix.m10 + a->matrix.m32 * b->matrix.m20 + a->matrix.m33 * b->matrix.m30;

    // column 1 of result
    result.matrix.m01 = a->matrix.m00 * b->matrix.m01 + a->matrix.m01 * b->matrix.m11 + a->matrix.m02 * b->matrix.m21 + a->matrix.m03 * b->matrix.m31;
    result.matrix.m11 = a->matrix.m10 * b->matrix.m01 + a->matrix.m11 * b->matrix.m11 + a->matrix.m12 * b->matrix.m21 + a->matrix.m13 * b->matrix.m31;
    result.matrix.m21 = a->matrix.m20 * b->matrix.m01 + a->matrix.m21 * b->matrix.m11 + a->matrix.m22 * b->matrix.m21 + a->matrix.m23 * b->matrix.m31;
    result.matrix.m31 = a->matrix.m30 * b->matrix.m01 + a->matrix.m31 * b->matrix.m11 + a->matrix.m32 * b->matrix.m21 + a->matrix.m33 * b->matrix.m31;

    // column 2 of result
    result.matrix.m02 = a->matrix.m00 * b->matrix.m02 + a->matrix.m01 * b->matrix.m12 + a->matrix.m02 * b->matrix.m22 + a->matrix.m03 * b->matrix.m32;
    result.matrix.m12 = a->matrix.m10 * b->matrix.m02 + a->matrix.m11 * b->matrix.m12 + a->matrix.m12 * b->matrix.m22 + a->matrix.m13 * b->matrix.m32;
    result.matrix.m22 = a->matrix.m20 * b->matrix.m02 + a->matrix.m21 * b->matrix.m12 + a->matrix.m22 * b->matrix.m22 + a->matrix.m23 * b->matrix.m32;
    result.matrix.m32 = a->matrix.m30 * b->matrix.m02 + a->matrix.m31 * b->matrix.m12 + a->matrix.m32 * b->matrix.m22 + a->matrix.m33 * b->matrix.m32;

    // column 3 of result
    result.matrix.m03 = a->matrix.m00 * b->matrix.m03 + a->matrix.m01 * b->matrix.m13 + a->matrix.m02 * b->matrix.m23 + a->matrix.m03 * b->matrix.m33;
    result.matrix.m13 = a->matrix.m10 * b->matrix.m03 + a->matrix.m11 * b->matrix.m13 + a->matrix.m12 * b->matrix.m23 + a->matrix.m13 * b->matrix.m33;
    result.matrix.m23 = a->matrix.m20 * b->matrix.m03 + a->matrix.m21 * b->matrix.m13 + a->matrix.m22 * b->matrix.m23 + a->matrix.m23 * b->matrix.m33;
    result.matrix.m33 = a->matrix.m30 * b->matrix.m03 + a->matrix.m31 * b->matrix.m13 + a->matrix.m32 * b->matrix.m23 + a->matrix.m33 * b->matrix.m33;

    return result;
}

VECMATH_API double2 double2_mul(const double2* a, const double2* b)
{
    double2 result = { 0 };
    result.xy.x = a->xy.x * b->xy.x;
    result.xy.y = a->xy.y * b->xy.y;
    return result;
}

double3 double3_mul(const double3* a, const double3* b)
{
    double3 result = { 0 };
    result.xyz.x = a->xyz.x * b->xyz.x;
    result.xyz.y = a->xyz.y * b->xyz.y;
    result.xyz.z = a->xyz.z * b->xyz.z;
    return result;
}

VECMATH_API double4 double4_mul(const double4* a, const double4* b)
{
    double4 result = { 0 };
    result.xyzw.x = a->xyzw.x * b->xyzw.x;
    result.xyzw.y = a->xyzw.y * b->xyzw.y;
    result.xyzw.z = a->xyzw.z * b->xyzw.z;
    result.xyzw.w = a->xyzw.w * b->xyzw.w;
    return result;
}

VECMATH_API dmat2 dmat2_mul(const dmat2* a, const dmat2* b)
{
    dmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 * b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 * b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 * b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 * b->matrix.m11;
    return result;
}

VECMATH_API dmat3 dmat3_mul(const dmat3* a, const dmat3* b)
{
    dmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 * b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 * b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 * b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 * b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 * b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 * b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 * b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 * b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 * b->matrix.m22;
    return result;
}

VECMATH_API dmat4 dmat4_mul(const dmat4* a, const dmat4* b)
{
    dmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 * b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 * b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 * b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 * b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 * b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 * b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 * b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 * b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 * b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 * b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 * b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 * b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 * b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 * b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 * b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 * b->matrix.m33;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// div
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_div(const float2* a, const float2* b)
{
    float2 result = { 0 };
    result.xy.x = a->xy.x / b->xy.x;
    result.xy.y = a->xy.y / b->xy.y;
    return result;
}

VECMATH_API float3 float3_div(const float3* a, const float3* b)
{
    float3 result = { 0 };
    result.xyz.x = a->xyz.x / b->xyz.x;
    result.xyz.y = a->xyz.y / b->xyz.y;
    result.xyz.z = a->xyz.z / b->xyz.z;
    return result;
}

VECMATH_API float4 float4_div(const float4* a, const float4* b)
{
    float4 result = { 0 };
    result.xyzw.x = a->xyzw.x * b->xyzw.x;
    result.xyzw.y = a->xyzw.y * b->xyzw.y;
    result.xyzw.z = a->xyzw.z * b->xyzw.z;
    result.xyzw.w = a->xyzw.w * b->xyzw.w;
    return result;
}

VECMATH_API fmat2 fmat2_div(const fmat2* a, const fmat2* b)
{
    fmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 / b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 / b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 / b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 / b->matrix.m11;
    return result;
}

VECMATH_API fmat3 fmat3_div(const fmat3* a, const fmat3* b)
{
    fmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 / b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 / b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 / b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 / b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 / b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 / b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 / b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 / b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 / b->matrix.m22;
    return result;
}

VECMATH_API fmat4 fmat4_div(const fmat4* a, const fmat4* b)
{
    fmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 / b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 / b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 / b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 / b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 / b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 / b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 / b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 / b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 / b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 / b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 / b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 / b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 / b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 / b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 / b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 / b->matrix.m33;
    return result;
}

VECMATH_API double2 double2_div(const double2* a, const double2* b)
{
    double2 result = { 0 };
    result.xy.x = a->xy.x / b->xy.x;
    result.xy.y = a->xy.y / b->xy.y;
    return result;
}

VECMATH_API double3 double3_div(const double3* a, const double3* b)
{
    double3 result = { 0 };
    result.xyz.x = a->xyz.x / b->xyz.x;
    result.xyz.y = a->xyz.y / b->xyz.y;
    result.xyz.z = a->xyz.z / b->xyz.z;
    return result;
}

VECMATH_API double4 double4_div(const double4* a, const double4* b)
{
    double4 result = { 0 };
    result.xyzw.x = a->xyzw.x / b->xyzw.x;
    result.xyzw.y = a->xyzw.y / b->xyzw.y;
    result.xyzw.z = a->xyzw.z / b->xyzw.z;
    result.xyzw.w = a->xyzw.w / b->xyzw.w;
    return result;
}

VECMATH_API dmat2 dmat2_div(const dmat2* a, const dmat2* b)
{
    dmat2 result = { 0 };
    result.matrix.m00 = a->matrix.m00 / b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 / b->matrix.m01;

    result.matrix.m10 = a->matrix.m10 / b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 / b->matrix.m11;
    return result;
}

VECMATH_API dmat3 dmat3_div(const dmat3* a, const dmat3* b)
{
    dmat3 result = { 0 };
    result.matrix.m00 = a->matrix.m00 / b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 / b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 / b->matrix.m02;

    result.matrix.m10 = a->matrix.m10 / b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 / b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 / b->matrix.m12;

    result.matrix.m20 = a->matrix.m20 / b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 / b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 / b->matrix.m22;
    return result;
}

VECMATH_API dmat4 dmat4_div(const dmat4* a, const dmat4* b)
{
    dmat4 result = { 0 };
    result.matrix.m00 = a->matrix.m00 / b->matrix.m00;
    result.matrix.m01 = a->matrix.m01 / b->matrix.m01;
    result.matrix.m02 = a->matrix.m02 / b->matrix.m02;
    result.matrix.m03 = a->matrix.m03 / b->matrix.m03;

    result.matrix.m10 = a->matrix.m10 / b->matrix.m10;
    result.matrix.m11 = a->matrix.m11 / b->matrix.m11;
    result.matrix.m12 = a->matrix.m12 / b->matrix.m12;
    result.matrix.m13 = a->matrix.m13 / b->matrix.m13;

    result.matrix.m20 = a->matrix.m20 / b->matrix.m20;
    result.matrix.m21 = a->matrix.m21 / b->matrix.m21;
    result.matrix.m22 = a->matrix.m22 / b->matrix.m22;
    result.matrix.m23 = a->matrix.m23 / b->matrix.m23;

    result.matrix.m30 = a->matrix.m30 / b->matrix.m30;
    result.matrix.m31 = a->matrix.m31 / b->matrix.m31;
    result.matrix.m32 = a->matrix.m32 / b->matrix.m32;
    result.matrix.m33 = a->matrix.m33 / b->matrix.m33;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// equals
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API vecbool float2_equals(const float2 *a, const float2 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool float3_equals(const float3 *a, const float3 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool float4_equals(const float4 *a, const float4 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool fmat2_equals(const fmat2 *a, const fmat2 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool fmat3_equals(const fmat3 *a, const fmat3 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool fmat4_equals(const fmat4 *a, const fmat4 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool double2_equals(const double2 *a, const double2 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool double3_equals(const double3 *a, const double3 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool double4_equals(const double4 *a, const double4 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool dmat2_equals(const dmat2 *a, const dmat2 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool dmat3_equals(const dmat3 *a, const dmat3 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

VECMATH_API vecbool dmat4_equals(const dmat4 *a, const dmat4 *b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// aprox_equals
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API vecbool float2_aprox_equals(const float2 *a, const float2 *b)
{
    return  (fabsf(a->xy.x - b->xy.x) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->xy.y - b->xy.y) <= VECMATH_EPSILON_FZERO);
}

VECMATH_API vecbool float3_aprox_equals(const float3 *a, const float3 *b)
{
    return  (fabsf(a->xyz.x - b->xyz.x) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->xyz.y - b->xyz.y) <= VECMATH_EPSILON_FZERO) && 
            (fabsf(a->xyz.z - b->xyz.z) <= VECMATH_EPSILON_FZERO);
}

VECMATH_API vecbool float4_aprox_equals(const float4 *a, const float4 *b)
{
    return  (fabsf(a->xyzw.x - b->xyzw.x) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->xyzw.y - b->xyzw.y) <= VECMATH_EPSILON_FZERO) && 
            (fabsf(a->xyzw.z - b->xyzw.z) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->xyzw.w - b->xyzw.w) <= VECMATH_EPSILON_FZERO);
}

VECMATH_API vecbool fmat2_aprox_equals(const fmat2 *a, const fmat2 *b)
{
    return  (fabsf(a->matrix.m00 - b->matrix.m00) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m01 - b->matrix.m01) <= VECMATH_EPSILON_FZERO) && 

            (fabsf(a->matrix.m10 - b->matrix.m10) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m11 - b->matrix.m11) <= VECMATH_EPSILON_FZERO);
}

VECMATH_API vecbool fmat3_aprox_equals(const fmat3 *a, const fmat3 *b)
{
    return  (fabsf(a->matrix.m00 - b->matrix.m00) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m01 - b->matrix.m01) <= VECMATH_EPSILON_FZERO) && 
            (fabsf(a->matrix.m02 - b->matrix.m02) <= VECMATH_EPSILON_FZERO) && 

            (fabsf(a->matrix.m10 - b->matrix.m10) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m11 - b->matrix.m11) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m12 - b->matrix.m12) <= VECMATH_EPSILON_FZERO) && 

            (fabsf(a->matrix.m20 - b->matrix.m20) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m21 - b->matrix.m21) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m22 - b->matrix.m22) <= VECMATH_EPSILON_FZERO);
}

VECMATH_API vecbool fmat4_aprox_equals(const fmat4 *a, const fmat4 *b)
{
    return  (fabsf(a->matrix.m00 - b->matrix.m00) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m01 - b->matrix.m01) <= VECMATH_EPSILON_FZERO) && 
            (fabsf(a->matrix.m02 - b->matrix.m02) <= VECMATH_EPSILON_FZERO) && 
            (fabsf(a->matrix.m03 - b->matrix.m03) <= VECMATH_EPSILON_FZERO) && 

            (fabsf(a->matrix.m10 - b->matrix.m10) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m11 - b->matrix.m11) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m12 - b->matrix.m12) <= VECMATH_EPSILON_FZERO) && 
            (fabsf(a->matrix.m13 - b->matrix.m13) <= VECMATH_EPSILON_FZERO) && 

            (fabsf(a->matrix.m20 - b->matrix.m20) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m21 - b->matrix.m21) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m22 - b->matrix.m22) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m23 - b->matrix.m23) <= VECMATH_EPSILON_FZERO) && 

            (fabsf(a->matrix.m30 - b->matrix.m30) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m31 - b->matrix.m31) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m32 - b->matrix.m32) <= VECMATH_EPSILON_FZERO) &&
            (fabsf(a->matrix.m33 - b->matrix.m33) <= VECMATH_EPSILON_FZERO);
}

VECMATH_API vecbool double2_aprox_equals(const double2 *a, const double2 *b)
{
    return  (fabs(a->xy.x - b->xy.x) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->xy.y - b->xy.y) <= VECMATH_EPSILON_DZERO);
}

VECMATH_API vecbool double3_aprox_equals(const double3 *a, const double3 *b)
{
    return  (fabs(a->xyz.x - b->xyz.x) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->xyz.y - b->xyz.y) <= VECMATH_EPSILON_DZERO) && 
            (fabs(a->xyz.z - b->xyz.z) <= VECMATH_EPSILON_DZERO);
}

VECMATH_API vecbool double4_aprox_equals(const double4 *a, const double4 *b)
{
    return  (fabs(a->xyzw.x - b->xyzw.x) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->xyzw.y - b->xyzw.y) <= VECMATH_EPSILON_DZERO) && 
            (fabs(a->xyzw.z - b->xyzw.z) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->xyzw.w - b->xyzw.w) <= VECMATH_EPSILON_DZERO);
}

VECMATH_API vecbool dmat2_aprox_equals(const dmat2 *a, const dmat2 *b)
{
    return  (fabs(a->matrix.m00 - b->matrix.m00) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m01 - b->matrix.m01) <= VECMATH_EPSILON_DZERO) && 

            (fabs(a->matrix.m10 - b->matrix.m10) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m11 - b->matrix.m11) <= VECMATH_EPSILON_DZERO);
}

VECMATH_API vecbool dmat3_aprox_equals(const dmat3 *a, const dmat3 *b)
{
    return  (fabs(a->matrix.m00 - b->matrix.m00) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m01 - b->matrix.m01) <= VECMATH_EPSILON_DZERO) && 
            (fabs(a->matrix.m02 - b->matrix.m02) <= VECMATH_EPSILON_DZERO) && 

            (fabs(a->matrix.m10 - b->matrix.m10) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m11 - b->matrix.m11) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m12 - b->matrix.m12) <= VECMATH_EPSILON_DZERO) && 

            (fabs(a->matrix.m20 - b->matrix.m20) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m21 - b->matrix.m21) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m22 - b->matrix.m22) <= VECMATH_EPSILON_DZERO);
}

VECMATH_API vecbool dmat4_aprox_equals(const dmat4 *a, const dmat4 *b)
{
    return  (fabs(a->matrix.m00 - b->matrix.m00) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m01 - b->matrix.m01) <= VECMATH_EPSILON_DZERO) && 
            (fabs(a->matrix.m02 - b->matrix.m02) <= VECMATH_EPSILON_DZERO) && 
            (fabs(a->matrix.m03 - b->matrix.m03) <= VECMATH_EPSILON_DZERO) && 

            (fabs(a->matrix.m10 - b->matrix.m10) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m11 - b->matrix.m11) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m12 - b->matrix.m12) <= VECMATH_EPSILON_DZERO) && 
            (fabs(a->matrix.m13 - b->matrix.m13) <= VECMATH_EPSILON_DZERO) && 

            (fabs(a->matrix.m20 - b->matrix.m20) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m21 - b->matrix.m21) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m22 - b->matrix.m22) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m23 - b->matrix.m23) <= VECMATH_EPSILON_DZERO) && 

            (fabs(a->matrix.m30 - b->matrix.m30) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m31 - b->matrix.m31) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m32 - b->matrix.m32) <= VECMATH_EPSILON_DZERO) &&
            (fabs(a->matrix.m33 - b->matrix.m33) <= VECMATH_EPSILON_DZERO);
}
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// float_mul
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float4 float4_mul_fmat4(const float4* v, const fmat4* m)
{
    float4 result = { 0 };
    result.xyzw.x = v->xyzw.x * m->matrix.m00 + v->xyzw.y * m->matrix.m10 + v->xyzw.z * m->matrix.m20 + v->xyzw.w * m->matrix.m30;
    result.xyzw.y = v->xyzw.x * m->matrix.m01 + v->xyzw.y * m->matrix.m11 + v->xyzw.z * m->matrix.m21 + v->xyzw.w * m->matrix.m31;
    result.xyzw.z = v->xyzw.x * m->matrix.m02 + v->xyzw.y * m->matrix.m12 + v->xyzw.z * m->matrix.m22 + v->xyzw.w * m->matrix.m32;
    result.xyzw.w = v->xyzw.x * m->matrix.m03 + v->xyzw.y * m->matrix.m13 + v->xyzw.z * m->matrix.m23 + v->xyzw.w * m->matrix.m33;
    
    return result;
}

VECMATH_API double4 double4_mul_fmat4(const double4* v, const dmat4* m)
{
    double4 result = { 0 };
    result.xyzw.x = v->xyzw.x * m->matrix.m00 + v->xyzw.y * m->matrix.m10 + v->xyzw.z * m->matrix.m20 + v->xyzw.w * m->matrix.m30;
    result.xyzw.y = v->xyzw.x * m->matrix.m01 + v->xyzw.y * m->matrix.m11 + v->xyzw.z * m->matrix.m21 + v->xyzw.w * m->matrix.m31;
    result.xyzw.z = v->xyzw.x * m->matrix.m02 + v->xyzw.y * m->matrix.m12 + v->xyzw.z * m->matrix.m22 + v->xyzw.w * m->matrix.m32;
    result.xyzw.w = v->xyzw.x * m->matrix.m03 + v->xyzw.y * m->matrix.m13 + v->xyzw.z * m->matrix.m23 + v->xyzw.w * m->matrix.m33;
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// scalar_const
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_scalar(const float2* v, const float value)
{
    float2 result = { 0 };
    result.xy.x = v->xy.x * value;
    result.xy.y = v->xy.y * value;
    return result;
}

VECMATH_API float3 float3_scalar(const float3* v, const float value)
{
    float3 result = { 0 };
    result.xyz.x = v->xyz.x * value;
    result.xyz.y = v->xyz.y * value;
    result.xyz.z = v->xyz.z * value;
    return result;
}

VECMATH_API float4 float4_scalar(const float4* v, const float value)
{
    float4 result = { 0 };
    result.xyzw.x = v->xyzw.x * value;
    result.xyzw.y = v->xyzw.y * value;
    result.xyzw.z = v->xyzw.z * value;
    result.xyzw.w = v->xyzw.w * value;
    return result;
}

VECMATH_API double2 double2_scalar(const double2 *v, const float value)
{
    double2 result = { 0 };
    result.xy.x = v->xy.x * value;
    result.xy.y = v->xy.y * value;
    return result;
}

VECMATH_API double3 double3_scalar(const double3 *v, const float value)
{
    double3 result = { 0 };
    result.xyz.x = v->xyz.x * value;
    result.xyz.y = v->xyz.y * value;
    result.xyz.z = v->xyz.z * value;
    return result;
}

VECMATH_API double4 double4_scalar(const double4 *v, const float value)
{
    double4 result = { 0 };
    result.xyzw.x = v->xyzw.x * value;
    result.xyzw.y = v->xyzw.y * value;
    result.xyzw.z = v->xyzw.z * value;
    result.xyzw.w = v->xyzw.w * value;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// length
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_length(const float2 *v)
{
    return sqrtf((v->xy.x * v->xy.x) + (v->xy.y * v->xy.y));
}

VECMATH_API float float3_length(const float3* v)
{
    return sqrtf((v->xyz.x * v->xyz.x) + (v->xyz.y * v->xyz.y) + (v->xyz.z * v->xyz.z));
}

VECMATH_API float float4_length(const float4* v)
{
    return sqrtf((v->xyzw.x * v->xyzw.x) + (v->xyzw.y * v->xyzw.y) + (v->xyzw.z * v->xyzw.z) + (v->xyzw.w * v->xyzw.w));
}

VECMATH_API double double2_length(const double2* v)
{
    return sqrt((v->xy.x * v->xy.x) + (v->xy.y * v->xy.y));
}

VECMATH_API double double3_length(const double3* v)
{
    return sqrt((v->xyz.x * v->xyz.x) + (v->xyz.y * v->xyz.y) + (v->xyz.z * v->xyz.z));
}

VECMATH_API double double4_length(const double4* v)
{
    return sqrt((v->xyzw.x * v->xyzw.x) + (v->xyzw.y * v->xyzw.y) + (v->xyzw.z * v->xyzw.z) + (v->xyzw.w * v->xyzw.w));
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// length_sqrt 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_length_sqrt(const float2* v)
{
    return (v->xy.x * v->xy.x) + (v->xy.y * v->xy.y);
}

VECMATH_API float float3_length_sqrt(const float3* v)
{
    return (v->xyz.x * v->xyz.x) + (v->xyz.y * v->xyz.y) + (v->xyz.z * v->xyz.z);
}

VECMATH_API float float4_length_sqrt(const float4* v)
{
    return (v->xyzw.x * v->xyzw.x) + (v->xyzw.y * v->xyzw.y) + (v->xyzw.z * v->xyzw.z) + (v->xyzw.w * v->xyzw.w);
}

VECMATH_API double double2_length_sqrt(const double2 *v)
{
    return (v->xy.x * v->xy.x) + (v->xy.y * v->xy.y);
}

VECMATH_API double double3_length_sqrt(const double3 *v)
{
    return (v->xyz.x * v->xyz.x) + (v->xyz.y * v->xyz.y) + (v->xyz.z * v->xyz.z);
}

VECMATH_API double double4_length_sqrt(const double4* v)
{
    return (v->xyzw.x * v->xyzw.x) + (v->xyzw.y * v->xyzw.y) + (v->xyzw.z * v->xyzw.z) + (v->xyzw.w * v->xyzw.w);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// normalize 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_normalize(const float2* v)
{
    float len = float2_length(v);
    float2 result = { 0 };
    result.xy.x = 0.0f;
    result.xy.y = 0.0f;

    if (len > 0.0f) {
        result.xy.x = v->xy.x / len;
        result.xy.y = v->xy.y / len;
    }
    return result;
}

VECMATH_API float3 float3_normalize(const float3* v)
{
    float len = float3_length(v);
    float3 result = { 0 };
    result.xyz.x = 0.0f;
    result.xyz.y = 0.0f;
    result.xyz.z = 0.0f;

    if (len > 0.0f) {
        result.xyz.x = v->xyz.x / len;
        result.xyz.y = v->xyz.y / len;
        result.xyz.z = v->xyz.z / len;
    }
    return result;
}

VECMATH_API float4 float4_normalize(const float4* v)
{
    float len = float4_length(v);
    float4 result = { 0 };
    result.xyzw.x = 0.0f;
    result.xyzw.y = 0.0f;
    result.xyzw.z = 0.0f;
    result.xyzw.w = 0.0f;
    
    if (len > 0.0f) {
        result.xyzw.x = v->xyzw.x / len;
        result.xyzw.y = v->xyzw.y / len;
        result.xyzw.z = v->xyzw.z / len;
        result.xyzw.w = v->xyzw.w / len;
    }
    
    return result;
}

VECMATH_API double2 double2_normalize(const double2* v)
{
    double len = double2_length(v);
    double2 result = { 0 };
    result.xy.x = 0.0f;
    result.xy.y = 0.0f;
    
    if (len > 0.0f) {
        result.xy.x = v->xy.x / len;
        result.xy.y = v->xy.y / len;
    }
    return result;
}

VECMATH_API double3 double3_normalize(const double3 *v)
{
    double len = double3_length(v);
    double3 result = { 0 };
    result.xyz.x = 0.0f;
    result.xyz.y = 0.0f;
    result.xyz.z = 0.0f;

    if (len > 0.0f) {
        result.xyz.x = v->xyz.x / len;
        result.xyz.y = v->xyz.y / len;
        result.xyz.z = v->xyz.z / len;
    }
    return result;
}

VECMATH_API double4 double4_normalize(const double4* v)
{
    double len = double4_length(v);
    double4 result = { 0 };
    result.xyzw.x = 0.0f;
    result.xyzw.y = 0.0f;
    result.xyzw.z = 0.0f;
    result.xyzw.w = 0.0f;

    if (len > 0.0f) {
        result.xyzw.x = v->xyzw.x / len;
        result.xyzw.y = v->xyzw.y / len;
        result.xyzw.z = v->xyzw.z / len;
        result.xyzw.w = v->xyzw.w / len;
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// dot 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_dot(const float2 *a, const float2 *b)
{
    return (a->xy.x * b->xy.x) + (a->xy.y * b->xy.y);
}

VECMATH_API float float3_dot(const float3* a, const float3* b)
{
    return (a->xyz.x * b->xyz.x) + (a->xyz.y * b->xyz.y) + (a->xyz.z * b->xyz.z);
}

VECMATH_API float float4_dot(const float4* a, const float4* b)
{
    return (a->xyzw.x * b->xyzw.x) + (a->xyzw.y * b->xyzw.y) + (a->xyzw.z * b->xyzw.z) + (a->xyzw.w * b->xyzw.w);
}

VECMATH_API double double2_dot(const double2* a, const double2* b)
{
    return (a->xy.x * b->xy.x) + (a->xy.y * b->xy.y);
}

VECMATH_API double double3_dot(const double3* a, const double3* b)
{
    return (a->xyz.x * b->xyz.x) + (a->xyz.y * b->xyz.y) + (a->xyz.z * b->xyz.z);
}

VECMATH_API double double4_dot(const double4* a, const double4* b)
{
    return (a->xyzw.x * b->xyzw.x) + (a->xyzw.y * b->xyzw.y) + (a->xyzw.z * b->xyzw.z) + (a->xyzw.w * b->xyzw.w);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// cross 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_cross(const float2* a, const float2* b)
{
    return (a->xy.x * b->xy.y) - (a->xy.y * b->xy.x);
}

VECMATH_API float3 float3_cross(const float3* a, const float3* b)
{
    float3 result = { 0 };
    result.xyz.x = a->xyz.y * b->xyz.z - a->xyz.z * b->xyz.y;
    result.xyz.y = a->xyz.z * b->xyz.x - a->xyz.x * b->xyz.z;
    result.xyz.z = a->xyz.x * b->xyz.y - a->xyz.y * b->xyz.x;
    return result; 
}

VECMATH_API double double2_cross(const double2* a, const double2* b)
{
    return (a->xy.x * b->xy.y) - (a->xy.y * b->xy.x);
}

VECMATH_API double3 double3_cross(const double3* a, const double3* b)
{
    double3 result = { 0 };
    result.xyz.x = a->xyz.y * b->xyz.z - a->xyz.z * b->xyz.y;
    result.xyz.y = a->xyz.z * b->xyz.x - a->xyz.x * b->xyz.z;
    result.xyz.z = a->xyz.x * b->xyz.y - a->xyz.y * b->xyz.x;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// scale 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_scale(const float2* v, float scalar)
{
    float2 result = { 0 };
    result.xy.x = v->xy.x * scalar;
    result.xy.y = v->xy.y * scalar;
    return result;
}

VECMATH_API float3 float3_scale(const float3* v, float scalar)
{
    float3 result = { 0 };
    result.xyz.x = v->xyz.x * scalar;
    result.xyz.y = v->xyz.y * scalar;
    result.xyz.z = v->xyz.z * scalar;
    return result;
}

VECMATH_API float4 float4_scale(const float4* v, float scalar)
{
    float4 result = { 0 };
    result.xyzw.x = v->xyzw.x * scalar;
    result.xyzw.y = v->xyzw.y * scalar;
    result.xyzw.z = v->xyzw.z * scalar;
    result.xyzw.w = v->xyzw.w * scalar;
    return result;
}

VECMATH_API double2 double2_scale(const double2* v, double scalar)
{
    double2 result = { 0 };
    result.xy.x = v->xy.x * scalar;
    result.xy.y = v->xy.y * scalar;
    return result;
}

VECMATH_API double3 double3_scale(const double3* v, double scalar)
{
    double3 result = { 0 };
    result.xyz.x = v->xyz.x * scalar;
    result.xyz.y = v->xyz.y * scalar;
    result.xyz.z = v->xyz.z * scalar;
    return result;
}

VECMATH_API double4 double4_scale(const double4* v, double scalar)
{
    double4 result = { 0 };
    result.xyzw.x = v->xyzw.x * scalar;
    result.xyzw.y = v->xyzw.y * scalar;
    result.xyzw.z = v->xyzw.z * scalar;
    result.xyzw.w = v->xyzw.w * scalar;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// lerp 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_lerp(const float2* a, const float2* b, float t)
{
    float2 result = { 0 };
    result.xy.x = a->xy.x + (b->xy.x - a->xy.x) * t;
    result.xy.y = a->xy.y + (b->xy.y - a->xy.y) * t;
    return result;
}

VECMATH_API float3 float3_lerp(const float3* a, const float3* b, float t)
{
    float3 result = { 0 };
    result.xyz.x = a->xyz.x + (b->xyz.x - a->xyz.x) * t;
    result.xyz.y = a->xyz.y + (b->xyz.y - a->xyz.y) * t;
    result.xyz.z = a->xyz.z + (b->xyz.z - a->xyz.z) * t;
    return result;
}

VECMATH_API float4 float4_lerp(const float4* a, const float4* b, float t)
{
    float4 result = { 0 };
    result.xyzw.x = a->xyzw.x + (b->xyzw.x - a->xyzw.x) * t;
    result.xyzw.y = a->xyzw.y + (b->xyzw.y - a->xyzw.y) * t;
    result.xyzw.z = a->xyzw.z + (b->xyzw.z - a->xyzw.z) * t;
    result.xyzw.w = a->xyzw.w + (b->xyzw.w - a->xyzw.w) * t;
    return result;
}

VECMATH_API double2 double2_lerp(const double2* a, const double2* b, double t)
{
    double2 result = { 0 };
    result.xy.x = a->xy.x + (b->xy.x - a->xy.x) * t;
    result.xy.y = a->xy.y + (b->xy.y - a->xy.y) * t;
    return result;
}

VECMATH_API double3 double3_lerp(const double3* a, const double3* b, double t)
{
    double3 result = { 0 };
    result.xyz.x = a->xyz.x + (b->xyz.x - a->xyz.x) * t;
    result.xyz.y = a->xyz.y + (b->xyz.y - a->xyz.y) * t;
    result.xyz.z = a->xyz.z + (b->xyz.z - a->xyz.z) * t;
    return result;
}

VECMATH_API double4 double4_lerp(const double4* a, const double4* b, double t)
{
    double4 result = { 0 };
    result.xyzw.x = a->xyzw.x + (b->xyzw.x - a->xyzw.x) * t;
    result.xyzw.y = a->xyzw.y + (b->xyzw.y - a->xyzw.y) * t;
    result.xyzw.z = a->xyzw.z + (b->xyzw.z - a->xyzw.z) * t;
    result.xyzw.w = a->xyzw.w + (b->xyzw.w - a->xyzw.w) * t;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// distance 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_distance(const float2* a, const float2* b)
{
    float dx = a->xy.x - b->xy.x;
    float dy = a->xy.y - b->xy.y;
    return sqrtf(dx * dx + dy * dy);
}

VECMATH_API float float3_distance(const float3* a, const float3* b)
{
    float dx = a->xyz.x - b->xyz.x;
    float dy = a->xyz.y - b->xyz.y;
    float dz = a->xyz.z - b->xyz.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

VECMATH_API float float4_distance(const float4* a, const float4* b)
{
    float dx = a->xyzw.x - b->xyzw.x;
    float dy = a->xyzw.y - b->xyzw.y;
    float dz = a->xyzw.z - b->xyzw.z;
    float dw = a->xyzw.w - b->xyzw.w;
    return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
}

VECMATH_API double double2_distance(const double2* a, const double2* b)
{
    double dx = a->xy.x - b->xy.x;
    double dy = a->xy.y - b->xy.y;
    return sqrt(dx * dx + dy * dy);
}

VECMATH_API double double3_distance(const double3* a, const double3* b)
{
    double dx = a->xyz.x - b->xyz.x;
    double dy = a->xyz.y - b->xyz.y;
    double dz = a->xyz.z - b->xyz.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

VECMATH_API double double4_distance(const double4* a, const double4* b)
{
    double dx = a->xyzw.x - b->xyzw.x;
    double dy = a->xyzw.y - b->xyzw.y;
    double dz = a->xyzw.z - b->xyzw.z;
    double dw = a->xyzw.w - b->xyzw.w;
    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// distance_sqrt 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float float2_distance_sqrt(const float2* a, const float2* b)
{
    float dx = a->xy.x - b->xy.x;
    float dy = a->xy.y - b->xy.y;
    return dx * dx + dy * dy;
}

VECMATH_API float float3_distance_sqrt(const float3* a, const float3* b)
{
    float dx = a->xyz.x - b->xyz.x;
    float dy = a->xyz.y - b->xyz.y;
    float dz = a->xyz.z - b->xyz.z;
    return dx * dx + dy * dy + dz * dz;
}

VECMATH_API float float4_distance_sqrt(const float4* a, const float4* b)
{
    float dx = a->xyzw.x - b->xyzw.x;
    float dy = a->xyzw.y - b->xyzw.y;
    float dz = a->xyzw.z - b->xyzw.z;
    float dw = a->xyzw.w - b->xyzw.w;
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

VECMATH_API double double2_distance_sqrt(const double2* a, const double2*b )
{
    double dx = a->xy.x - b->xy.x;
    double dy = a->xy.y - b->xy.y;
    return dx * dx + dy * dy;
}

VECMATH_API double double3_distance_sqrt(const double3* a, const double3* b)
{
    double dx = a->xyz.x - b->xyz.x;
    double dy = a->xyz.y - b->xyz.y;
    double dz = a->xyz.z - b->xyz.z;
    return dx * dx + dy * dy + dz * dz;
}

VECMATH_API double double4_distance_sqrt(const double4* a, const double4* b)
{
    double dx = a->xyzw.x - b->xyzw.x;
    double dy = a->xyzw.y - b->xyzw.y;
    double dz = a->xyzw.z - b->xyzw.z;
    double dw = a->xyzw.w - b->xyzw.w;
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// reflect
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_reflect(const float2* v, const float2* normal)
{
    float2 result = { 0 };
    float dot = float2_dot(v, normal);
    float normal_length_sq = float2_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_EPSILON_FZERO) {
        float factor = 2.0f * dot / normal_length_sq;
        result.xy.x = v->xy.x - factor * normal->xy.x;
        result.xy.y = v->xy.y - factor * normal->xy.y;
    }
    return result;
}

VECMATH_API float3 float3_reflect(const float3* v, const float3* normal)
{
    float3 result = { 0 };
    float dot = float3_dot(v, normal);
    float normal_length_sq = float3_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_EPSILON_FZERO) {
        float factor = 2.0f * dot / normal_length_sq;
        result.xyz.x = v->xyz.x - factor * normal->xyz.x;
        result.xyz.y = v->xyz.y - factor * normal->xyz.y;
        result.xyz.z = v->xyz.z - factor * normal->xyz.z;
    }
    return result;
}

VECMATH_API float4 float4_reflect(const float4* v, const float4* normal)
{
    float4 result = { 0 };
    float dot = float4_dot(v, normal);
    float normal_length_sq = float4_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_EPSILON_FZERO) {
        float factor = 2.0f * dot / normal_length_sq;
        result.xyzw.x = v->xyzw.x - factor * normal->xyzw.x;
        result.xyzw.y = v->xyzw.y - factor * normal->xyzw.y;
        result.xyzw.z = v->xyzw.z - factor * normal->xyzw.z;
        result.xyzw.w = v->xyzw.w - factor * normal->xyzw.w;
    }
    return result;
}

VECMATH_API double2 double2_reflect(const double2* v, const double2* normal)
{
    double2 result = { 0 };
    double dot = double2_dot(v, normal);
    double normal_length_sq = double2_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_EPSILON_DZERO) {
        double factor = 2.0f * dot / normal_length_sq;
        result.xy.x = v->xy.x - factor * normal->xy.x;
        result.xy.y = v->xy.y - factor * normal->xy.y;
    }
    return result;
}

VECMATH_API double3 double3_reflect(const double3* v, const double3* normal)
{
    double3 result = { 0 };
    double dot = double3_dot(v, normal);
    double normal_length_sq = double3_dot(normal, normal);
    
    if (normal_length_sq >VECMATH_EPSILON_DZERO) {
        double factor = 2.0f * dot / normal_length_sq;
        result.xyz.x = v->xyz.x - factor * normal->xyz.x;
        result.xyz.y = v->xyz.y - factor * normal->xyz.y;
        result.xyz.z = v->xyz.z - factor * normal->xyz.z;
    }
    return result;
}

VECMATH_API double4 double4_reflect(const double4* v, const double4* normal)
{
    double4 result = { 0 };
    double dot = double4_dot(v, normal);
    double normal_length_sq = double4_dot(normal, normal);
    
    if (normal_length_sq > VECMATH_EPSILON_DZERO) {
        double factor = 2.0f * dot / normal_length_sq;
        result.xyzw.x = v->xyzw.x - factor * normal->xyzw.x;
        result.xyzw.y = v->xyzw.y - factor * normal->xyzw.y;
        result.xyzw.z = v->xyzw.z - factor * normal->xyzw.z;
        result.xyzw.w = v->xyzw.w - factor * normal->xyzw.w;
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// project
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 float2_project(const float2* a, const float2* b)
{
    float2 result = { 0 };
    result.xy.x = 0.0f;
    result.xy.y = 0.0f;

    float dot_ab = float2_dot(a, b);
    float dot_bb = float2_dot(b, b);
    
    if (dot_bb > VECMATH_EPSILON_FZERO) {
        float scale = dot_ab / dot_bb;
        result.xy.x = b->xy.x * scale;
        result.xy.y = b->xy.y * scale;
    }
    return result;
}

VECMATH_API float3 float3_project(const float3* a, const float3* b)
{
    float3 result = { 0 };
    result.xyz.x = 0.0f;
    result.xyz.y = 0.0f;
    result.xyz.z = 0.0f;

    float dot_ab = float3_dot(a, b);
    float dot_bb = float3_dot(b, b);
    
    if (dot_bb > VECMATH_EPSILON_FZERO) {
        float scale = dot_ab / dot_bb;
        result.xyz.x = b->xyz.x * scale;
        result.xyz.y = b->xyz.y * scale;
        result.xyz.z = b->xyz.z * scale;
    }
    return result;
}

VECMATH_API float4 float4_project(const float4 *a, const float4 *b)
{
    float4 result = { 0 };
    result.xyzw.x = 0.0f;
    result.xyzw.y = 0.0f;
    result.xyzw.z = 0.0f;
    result.xyzw.w = 0.0f;

    float dot_ab = float4_dot(a, b);
    float dot_bb = float4_dot(b, b);
    
    if (dot_bb > VECMATH_EPSILON_FZERO) {
        float scale = dot_ab / dot_bb;
        result.xyzw.x = b->xyzw.x * scale;
        result.xyzw.y = b->xyzw.y * scale;
        result.xyzw.z = b->xyzw.z * scale;
        result.xyzw.w = b->xyzw.w * scale;
    }
    return result;
}

VECMATH_API double2 double2_project(const double2* a, const double2* b)
{
    double2 result = { 0 };
    result.xy.x = 0.0f;
    result.xy.y = 0.0f;

    double dot_ab = double2_dot(a, b);
    double dot_bb = double2_dot(b, b);
    
    if (dot_bb > VECMATH_EPSILON_DZERO) {
        double scale = dot_ab / dot_bb;
        result.xy.x = b->xy.x * scale;
        result.xy.y = b->xy.y * scale;
    }
    return result;
}

VECMATH_API double3 double3_project(const double3 *a, const double3 *b)
{
    double3 result = { 0 };
    result.xyz.x = 0.0f;
    result.xyz.y = 0.0f;
    result.xyz.z = 0.0f;

    double dot_ab = double3_dot(a, b);
    double dot_bb = double3_dot(b, b);
    
    if (dot_bb > VECMATH_EPSILON_DZERO) {
        double scale = dot_ab / dot_bb;
        result.xyz.x = b->xyz.x * scale;
        result.xyz.y = b->xyz.y * scale;
        result.xyz.z = b->xyz.z * scale;
   }
    return result;
}

VECMATH_API double4 double4_project(const double4 *a, const double4 *b)
{
    double4 result = { 0 };
    result.xyzw.x = 0.0f;
    result.xyzw.y = 0.0f;
    result.xyzw.z = 0.0f;
    result.xyzw.w = 0.0f;

    double dot_ab = double4_dot(a, b);
    double dot_bb = double4_dot(b, b);
    
    if (dot_bb > VECMATH_EPSILON_DZERO) {
        double scale = dot_ab / dot_bb;
        result.xyzw.x = b->xyzw.x * scale;
        result.xyzw.y = b->xyzw.y * scale;
        result.xyzw.z = b->xyzw.z * scale;
        result.xyzw.w = b->xyzw.w * scale;
    }
    return result;
}
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// identity 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_identity()
{
    fmat2 mat = { 0 };
    mat.matrix.m00 = 1.0f; mat.matrix.m01 = 0.0f;
    mat.matrix.m10 = 0.0f; mat.matrix.m11 = 1.0f;
    return mat;
}

VECMATH_API fmat3 fmat3_identity()
{
    fmat3 mat = { 0 };
    mat.matrix.m00 = 1.0f; mat.matrix.m01 = 0.0f; mat.matrix.m02 = 0.0f;
    mat.matrix.m10 = 0.0f; mat.matrix.m11 = 1.0f; mat.matrix.m12 = 0.0f;
    mat.matrix.m20 = 0.0f; mat.matrix.m21 = 0.0f; mat.matrix.m22 = 1.0f;
    return mat;
}

VECMATH_API fmat4 fmat4_identity()
{
    fmat4 mat = { 0 };
    mat.matrix.m00 = 1.0f; mat.matrix.m01 = 0.0f; mat.matrix.m02 = 0.0f; mat.matrix.m03 = 0.0f;
    mat.matrix.m10 = 0.0f; mat.matrix.m11 = 1.0f; mat.matrix.m12 = 0.0f; mat.matrix.m13 = 0.0f;
    mat.matrix.m20 = 0.0f; mat.matrix.m21 = 0.0f; mat.matrix.m22 = 1.0f; mat.matrix.m23 = 0.0f;
    mat.matrix.m30 = 0.0f; mat.matrix.m31 = 0.0f; mat.matrix.m32 = 0.0f; mat.matrix.m33 = 1.0f;
    return mat;
}

VECMATH_API dmat2 dmat2_identity()
{
    dmat2 mat = { 0 };
    mat.matrix.m00 = 1.0f; mat.matrix.m01 = 0.0f;
    mat.matrix.m10 = 0.0f; mat.matrix.m11 = 1.0f;
    return mat;
}

VECMATH_API dmat3 dmat3_identity()
{
    dmat3 mat = { 0 };
    mat.matrix.m00 = 1.0f; mat.matrix.m01 = 0.0f; mat.matrix.m02 = 0.0f;
    mat.matrix.m10 = 0.0f; mat.matrix.m11 = 1.0f; mat.matrix.m12 = 0.0f;
    mat.matrix.m20 = 0.0f; mat.matrix.m21 = 0.0f; mat.matrix.m22 = 1.0f;
    return mat;
}

VECMATH_API dmat4 dmat4_identity()
{
    dmat4 mat = { 0 };
    mat.matrix.m00 = 1.0f; mat.matrix.m01 = 0.0f; mat.matrix.m02 = 0.0f; mat.matrix.m03 = 0.0f;
    mat.matrix.m10 = 0.0f; mat.matrix.m11 = 1.0f; mat.matrix.m12 = 0.0f; mat.matrix.m13 = 0.0f;
    mat.matrix.m20 = 0.0f; mat.matrix.m21 = 0.0f; mat.matrix.m22 = 1.0f; mat.matrix.m23 = 0.0f;
    mat.matrix.m30 = 0.0f; mat.matrix.m31 = 0.0f; mat.matrix.m32 = 0.0f; mat.matrix.m33 = 1.0f;
    return mat;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// mul_vec 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_mul_float2(const fmat2* m, const float2* v)
{
    fmat2 result = { 0 };
    result.matrix.m00 = m->matrix.m00 * v->xy.x;
    result.matrix.m10 = m->matrix.m10 * v->xy.x;
    
    result.matrix.m01 = m->matrix.m01 * v->xy.y;
    result.matrix.m11 = m->matrix.m11 * v->xy.y;
    return result;
}

VECMATH_API fmat3 fmat3_mul_float3(const fmat3* m, const float3* v)
{
    fmat3 result = { 0 };
    result.matrix.m00 = m->matrix.m00 * v->xyz.x;
    result.matrix.m10 = m->matrix.m10 * v->xyz.x;
    result.matrix.m20 = m->matrix.m20 * v->xyz.x;
    
    result.matrix.m01 = m->matrix.m01 * v->xyz.y;
    result.matrix.m11 = m->matrix.m11 * v->xyz.y;
    result.matrix.m21 = m->matrix.m21 * v->xyz.y;
    
    result.matrix.m02 = m->matrix.m02 * v->xyz.z;
    result.matrix.m12 = m->matrix.m12 * v->xyz.z;
    result.matrix.m22 = m->matrix.m22 * v->xyz.z;
    return result;
}

VECMATH_API fmat4 fmat4_mul_float4(const fmat4* m, const float4* v)
{
    fmat4 result = { 0 };
    result.matrix.m00 = m->matrix.m00 * v->xyzw.x;
    result.matrix.m10 = m->matrix.m10 * v->xyzw.x;
    result.matrix.m20 = m->matrix.m20 * v->xyzw.x;
    result.matrix.m30 = m->matrix.m30 * v->xyzw.x;
    
    result.matrix.m01 = m->matrix.m01 * v->xyzw.y;
    result.matrix.m11 = m->matrix.m11 * v->xyzw.y;
    result.matrix.m21 = m->matrix.m21 * v->xyzw.y;
    result.matrix.m31 = m->matrix.m31 * v->xyzw.y;
    
    result.matrix.m02 = m->matrix.m02 * v->xyzw.z;
    result.matrix.m12 = m->matrix.m12 * v->xyzw.z;
    result.matrix.m22 = m->matrix.m22 * v->xyzw.z;
    result.matrix.m32 = m->matrix.m32 * v->xyzw.z;
    
    result.matrix.m03 = m->matrix.m03 * v->xyzw.w;
    result.matrix.m13 = m->matrix.m13 * v->xyzw.w;
    result.matrix.m23 = m->matrix.m23 * v->xyzw.w;
    result.matrix.m33 = m->matrix.m33 * v->xyzw.w;
    return result;
}

VECMATH_API dmat2 dmat2_mul_double2(const dmat2* m, const double2* v)
{
    dmat2 result = { 0 };
    result.matrix.m00 = m->matrix.m00 * v->xy.x;
    result.matrix.m10 = m->matrix.m10 * v->xy.x;
    
    result.matrix.m01 = m->matrix.m01 * v->xy.y;
    result.matrix.m11 = m->matrix.m11 * v->xy.y;
    return result;
}

VECMATH_API dmat3 dmat3_mul_double3(const dmat3* m, const double3* v)
{
    dmat3 result = { 0 };
    result.matrix.m00 = m->matrix.m00 * v->xyz.x;
    result.matrix.m10 = m->matrix.m10 * v->xyz.x;
    result.matrix.m20 = m->matrix.m20 * v->xyz.x;
    
    result.matrix.m01 = m->matrix.m01 * v->xyz.y;
    result.matrix.m11 = m->matrix.m11 * v->xyz.y;
    result.matrix.m21 = m->matrix.m21 * v->xyz.y;
    
    result.matrix.m02 = m->matrix.m02 * v->xyz.z;
    result.matrix.m12 = m->matrix.m12 * v->xyz.z;
    result.matrix.m22 = m->matrix.m22 * v->xyz.z;
    return result;
}

VECMATH_API dmat4 dmat4_mul_double4(const dmat4* m, const double4* v)
{
    dmat4 result = { 0 };
    result.matrix.m00 = m->matrix.m00 * v->xyzw.x;
    result.matrix.m10 = m->matrix.m10 * v->xyzw.x;
    result.matrix.m20 = m->matrix.m20 * v->xyzw.x;
    result.matrix.m30 = m->matrix.m30 * v->xyzw.x;
    
    result.matrix.m01 = m->matrix.m01 * v->xyzw.y;
    result.matrix.m11 = m->matrix.m11 * v->xyzw.y;
    result.matrix.m21 = m->matrix.m21 * v->xyzw.y;
    result.matrix.m31 = m->matrix.m31 * v->xyzw.y;
    
    result.matrix.m02 = m->matrix.m02 * v->xyzw.z;
    result.matrix.m12 = m->matrix.m12 * v->xyzw.z;
    result.matrix.m22 = m->matrix.m22 * v->xyzw.z;
    result.matrix.m32 = m->matrix.m32 * v->xyzw.z;
    
    result.matrix.m03 = m->matrix.m03 * v->xyzw.w;
    result.matrix.m13 = m->matrix.m13 * v->xyzw.w;
    result.matrix.m23 = m->matrix.m23 * v->xyzw.w;
    result.matrix.m33 = m->matrix.m33 * v->xyzw.w;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// transpose 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_transpose(const fmat2* m)
{
    fmat2 result = { 0 };
    result.matrix.m00 = m->matrix.m00; 
    result.matrix.m01 = m->matrix.m10;

    result.matrix.m10 = m->matrix.m01;
    result.matrix.m11 = m->matrix.m11;
    return result;
}

VECMATH_API fmat3 fmat3_transpose(const fmat3* m)
{
    fmat3 result = { 0 };
    result.matrix.m00 = m->matrix.m00;
    result.matrix.m01 = m->matrix.m10;
    result.matrix.m02 = m->matrix.m20;
    
    result.matrix.m10 = m->matrix.m01;
    result.matrix.m11 = m->matrix.m11;
    result.matrix.m12 = m->matrix.m21;
    
    result.matrix.m20 = m->matrix.m02;
    result.matrix.m21 = m->matrix.m12;
    result.matrix.m22 = m->matrix.m22;
    return result;
}

VECMATH_API fmat4 fmat4_transpose(const fmat4* m)
{
    fmat4 result = { 0 };
    result.matrix.m00 = m->matrix.m00;
    result.matrix.m01 = m->matrix.m10;
    result.matrix.m02 = m->matrix.m20;
    result.matrix.m03 = m->matrix.m30;
    
    result.matrix.m10 = m->matrix.m01;
    result.matrix.m11 = m->matrix.m11;
    result.matrix.m12 = m->matrix.m21;
    result.matrix.m13 = m->matrix.m31;
    
    result.matrix.m20 = m->matrix.m02;
    result.matrix.m21 = m->matrix.m12;
    result.matrix.m22 = m->matrix.m22;
    result.matrix.m23 = m->matrix.m32;
    
    result.matrix.m30 = m->matrix.m03;
    result.matrix.m31 = m->matrix.m13;
    result.matrix.m32 = m->matrix.m23;
    result.matrix.m33 = m->matrix.m33;
    return result;
}

VECMATH_API dmat2 dmat2_transpose(const dmat2* m)
{
    dmat2 result = { 0 };
    result.matrix.m00 = m->matrix.m00; 
    result.matrix.m01 = m->matrix.m10;

    result.matrix.m10 = m->matrix.m01;
    result.matrix.m11 = m->matrix.m11;
    return result;
}

VECMATH_API dmat3 dmat3_transpose(const dmat3* m)
{
    dmat3 result = { 0 };
    result.matrix.m00 = m->matrix.m00;
    result.matrix.m01 = m->matrix.m10;
    result.matrix.m02 = m->matrix.m20;
    
    result.matrix.m10 = m->matrix.m01;
    result.matrix.m11 = m->matrix.m11;
    result.matrix.m12 = m->matrix.m21;
    
    result.matrix.m20 = m->matrix.m02;
    result.matrix.m21 = m->matrix.m12;
    result.matrix.m22 = m->matrix.m22;
    return result;
}

VECMATH_API dmat4 dmat4_transpose(const dmat4* m)
{
    dmat4 result = { 0 };
    result.matrix.m00 = m->matrix.m00;
    result.matrix.m01 = m->matrix.m10;
    result.matrix.m02 = m->matrix.m20;
    result.matrix.m03 = m->matrix.m30;
    
    result.matrix.m10 = m->matrix.m01;
    result.matrix.m11 = m->matrix.m11;
    result.matrix.m12 = m->matrix.m21;
    result.matrix.m13 = m->matrix.m31;
    
    result.matrix.m20 = m->matrix.m02;
    result.matrix.m21 = m->matrix.m12;
    result.matrix.m22 = m->matrix.m22;
    result.matrix.m23 = m->matrix.m32;
    
    result.matrix.m30 = m->matrix.m03;
    result.matrix.m31 = m->matrix.m13;
    result.matrix.m32 = m->matrix.m23;
    result.matrix.m33 = m->matrix.m33;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// determinant 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float fmat2_determinant(const fmat2* m)
{
    return (m->matrix.m00 * m->matrix.m11) - (m->matrix.m01 * m->matrix.m10);
}

VECMATH_API float fmat3_determinant(const fmat3* m)
{
    return m->matrix.m00 * (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) 
        - m->matrix.m01 * (m->matrix.m10 * m->matrix.m22 - m->matrix.m12 * m->matrix.m20) 
        + m->matrix.m02 * (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20);
}

VECMATH_API float fmat4_determinant(const fmat4* m)
{
    float det = 0;
    float sub[3][3] = { 0 };
    
    for (int x = 0; x < 4; x++) {
        int subi = 0;
        for (int i = 1; i < 4; i++) {
            int subj = 0;
            for (int j = 0; j < 4; j++) {
                if (j == x) continue;
                sub[subi][subj] = m->data[i][j];
                subj++;
            }
            subi++;
        }
        fmat3 submat = {{{sub[0][0], sub[0][1], sub[0][2]},
            {sub[1][0], sub[1][1], sub[1][2]},
            {sub[2][0], sub[2][1], sub[2][2]}}};
        det += (x % 2 == 0 ? 1 : -1) * m->data[0][x] * fmat3_determinant(&submat);
    }
    return det;
}

VECMATH_API double dmat2_determinant(const dmat2* m)
{
    return (m->matrix.m00 * m->matrix.m11) - (m->matrix.m01 * m->matrix.m10);
}

VECMATH_API double dmat3_determinant(const dmat3* m)
{
    return m->matrix.m00 * (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) 
        - m->matrix.m01 * (m->matrix.m10 * m->matrix.m22 - m->matrix.m12 * m->matrix.m20) 
        + m->matrix.m02 * (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20);
}

VECMATH_API double dmat4_determinant(const dmat4* m)
{
    double det = 0;
    double sub[3][3] = { 0 };
    
    for (int x = 0; x < 4; x++) {
        int subi = 0;
        for (int i = 1; i < 4; i++) {
            int subj = 0;
            for (int j = 0; j < 4; j++) {
                if (j == x) continue;
                sub[subi][subj] = m->data[i][j];
                subj++;
            }
            subi++;
        }
        dmat3 submat = {{{sub[0][0], sub[0][1], sub[0][2]},
            {sub[1][0], sub[1][1], sub[1][2]},
            {sub[2][0], sub[2][1], sub[2][2]}}};
        det += (x % 2 == 0 ? 1 : -1) * m->data[0][x] * dmat3_determinant(&submat);
    }
    return det;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// inverse 
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_inverse(const fmat2* m)
{
    fmat2 result = { 0 };
    float det = fmat2_determinant(m);

    if (fabsf(det) < VECMATH_EPSILON_FZERO) {
        return fmat2_identity();
    }
    
    float inv_det = 1.0f / det;
    result.matrix.m00 =  m->matrix.m11 * inv_det;
    result.matrix.m01 = -m->matrix.m01 * inv_det;
    result.matrix.m10 = -m->matrix.m10 * inv_det;
    result.matrix.m11 =  m->matrix.m00 * inv_det;
    return result;
}

VECMATH_API fmat3 fmat3_inverse(const fmat3* m)
{
    fmat3 result = { 0 };
    float det = fmat3_determinant(m);
    if (fabsf(det) < VECMATH_EPSILON_FZERO) {
        return fmat3_identity();
    }
    
    float inv_det = 1.0f / det;
    result.matrix.m00 = (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) * inv_det;
    result.matrix.m01 = (m->matrix.m02 * m->matrix.m21 - m->matrix.m01 * m->matrix.m22) * inv_det;
    result.matrix.m02 = (m->matrix.m01 * m->matrix.m12 - m->matrix.m02 * m->matrix.m11) * inv_det;
    
    result.matrix.m10 = (m->matrix.m12 * m->matrix.m20 - m->matrix.m10 * m->matrix.m22) * inv_det;
    result.matrix.m11 = (m->matrix.m00 * m->matrix.m22 - m->matrix.m02 * m->matrix.m20) * inv_det;
    result.matrix.m12 = (m->matrix.m02 * m->matrix.m10 - m->matrix.m00 * m->matrix.m12) * inv_det;
    
    result.matrix.m20 = (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20) * inv_det;
    result.matrix.m21 = (m->matrix.m01 * m->matrix.m20 - m->matrix.m00 * m->matrix.m21) * inv_det;
    result.matrix.m22 = (m->matrix.m00 * m->matrix.m11 - m->matrix.m01 * m->matrix.m10) * inv_det;
    return result;
}

VECMATH_API fmat4 fmat4_inverse(const fmat4* m)
{
    // Using your matrix struct for clarity
    const float* mm = &m->matrix.m00;
    
    float inv[16] = { 0 };
    
    inv[0] = mm[5]  * mm[10] * mm[15] - 
             mm[5]  * mm[11] * mm[14] - 
             mm[9]  * mm[6]  * mm[15] + 
             mm[9]  * mm[7]  * mm[14] +
             mm[13] * mm[6]  * mm[11] - 
             mm[13] * mm[7]  * mm[10];

    inv[4] = -mm[4]  * mm[10] * mm[15] + 
              mm[4]  * mm[11] * mm[14] + 
              mm[8]  * mm[6]  * mm[15] - 
              mm[8]  * mm[7]  * mm[14] - 
              mm[12] * mm[6]  * mm[11] + 
              mm[12] * mm[7]  * mm[10];

    inv[8] = mm[4]  * mm[9] * mm[15] - 
             mm[4]  * mm[11] * mm[13] - 
             mm[8]  * mm[5] * mm[15] + 
             mm[8]  * mm[7] * mm[13] + 
             mm[12] * mm[5] * mm[11] - 
             mm[12] * mm[7] * mm[9];

    inv[12] = -mm[4]  * mm[9] * mm[14] + 
               mm[4]  * mm[10] * mm[13] +
               mm[8]  * mm[5] * mm[14] - 
               mm[8]  * mm[6] * mm[13] - 
               mm[12] * mm[5] * mm[10] + 
               mm[12] * mm[6] * mm[9];

    inv[1] = -mm[1]  * mm[10] * mm[15] + 
              mm[1]  * mm[11] * mm[14] + 
              mm[9]  * mm[2] * mm[15] - 
              mm[9]  * mm[3] * mm[14] - 
              mm[13] * mm[2] * mm[11] + 
              mm[13] * mm[3] * mm[10];

    inv[5] = mm[0]  * mm[10] * mm[15] - 
             mm[0]  * mm[11] * mm[14] - 
             mm[8]  * mm[2] * mm[15] + 
             mm[8]  * mm[3] * mm[14] + 
             mm[12] * mm[2] * mm[11] - 
             mm[12] * mm[3] * mm[10];

    inv[9] = -mm[0]  * mm[9] * mm[15] + 
              mm[0]  * mm[11] * mm[13] + 
              mm[8]  * mm[1] * mm[15] - 
              mm[8]  * mm[3] * mm[13] - 
              mm[12] * mm[1] * mm[11] + 
              mm[12] * mm[3] * mm[9];

    inv[13] = mm[0]  * mm[9] * mm[14] - 
              mm[0]  * mm[10] * mm[13] - 
              mm[8]  * mm[1] * mm[14] + 
              mm[8]  * mm[2] * mm[13] + 
              mm[12] * mm[1] * mm[10] - 
              mm[12] * mm[2] * mm[9];

    inv[2] = mm[1]  * mm[6] * mm[15] - 
             mm[1]  * mm[7] * mm[14] - 
             mm[5]  * mm[2] * mm[15] + 
             mm[5]  * mm[3] * mm[14] + 
             mm[13] * mm[2] * mm[7] - 
             mm[13] * mm[3] * mm[6];

    inv[6] = -mm[0]  * mm[6] * mm[15] + 
              mm[0]  * mm[7] * mm[14] + 
              mm[4]  * mm[2] * mm[15] - 
              mm[4]  * mm[3] * mm[14] - 
              mm[12] * mm[2] * mm[7] + 
              mm[12] * mm[3] * mm[6];

    inv[10] = mm[0]  * mm[5] * mm[15] - 
              mm[0]  * mm[7] * mm[13] - 
              mm[4]  * mm[1] * mm[15] + 
              mm[4]  * mm[3] * mm[13] + 
              mm[12] * mm[1] * mm[7] - 
              mm[12] * mm[3] * mm[5];

    inv[14] = -mm[0]  * mm[5] * mm[14] + 
               mm[0]  * mm[6] * mm[13] + 
               mm[4]  * mm[1] * mm[14] - 
               mm[4]  * mm[2] * mm[13] - 
               mm[12] * mm[1] * mm[6] + 
               mm[12] * mm[2] * mm[5];

    inv[3] = -mm[1] * mm[6] * mm[11] + 
              mm[1] * mm[7] * mm[10] + 
              mm[5] * mm[2] * mm[11] - 
              mm[5] * mm[3] * mm[10] - 
              mm[9] * mm[2] * mm[7] + 
              mm[9] * mm[3] * mm[6];

    inv[7] = mm[0] * mm[6] * mm[11] - 
             mm[0] * mm[7] * mm[10] - 
             mm[4] * mm[2] * mm[11] + 
             mm[4] * mm[3] * mm[10] + 
             mm[8] * mm[2] * mm[7] - 
             mm[8] * mm[3] * mm[6];

    inv[11] = -mm[0] * mm[5] * mm[11] + 
               mm[0] * mm[7] * mm[9] + 
               mm[4] * mm[1] * mm[11] - 
               mm[4] * mm[3] * mm[9] - 
               mm[8] * mm[1] * mm[7] + 
               mm[8] * mm[3] * mm[5];

    inv[15] = mm[0] * mm[5] * mm[10] - 
              mm[0] * mm[6] * mm[9] - 
              mm[4] * mm[1] * mm[10] + 
              mm[4] * mm[2] * mm[9] + 
              mm[8] * mm[1] * mm[6] - 
              mm[8] * mm[2] * mm[5];

    float det = mm[0] * inv[0] + mm[1] * inv[4] + mm[2] * inv[8] + mm[3] * inv[12];
    
    if (det == 0.0f) {
        return fmat4_identity(); // Fallback
    }
    
    det = 1.0f / det;
    
    fmat4 result = { 0 };
    for (int i = 0; i < 16; i++) {
        (&result.matrix.m00)[i] = inv[i] * det;
    }
    
    return result;
}

VECMATH_API dmat2 dmat2_inverse(const dmat2* m)
{
    dmat2 result = { 0 };
    double det = dmat2_determinant(m);

    if (fabs(det) < VECMATH_EPSILON_DZERO) {
        return dmat2_identity();
    }
    
    double inv_det = 1.0f / det;
    result.matrix.m00 =  m->matrix.m11 * inv_det;
    result.matrix.m01 = -m->matrix.m01 * inv_det;
    result.matrix.m10 = -m->matrix.m10 * inv_det;
    result.matrix.m11 =  m->matrix.m00 * inv_det;
    return result;
}

VECMATH_API dmat3 dmat3_inverse(const dmat3* m)
{
    dmat3 result = { 0 };
    double det = dmat3_determinant(m);
    if (fabs(det) < VECMATH_EPSILON_DZERO) {
        return dmat3_identity();
    }
    
    double inv_det = 1.0f / det;
    result.matrix.m00 = (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) * inv_det;
    result.matrix.m01 = (m->matrix.m02 * m->matrix.m21 - m->matrix.m01 * m->matrix.m22) * inv_det;
    result.matrix.m02 = (m->matrix.m01 * m->matrix.m12 - m->matrix.m02 * m->matrix.m11) * inv_det;
    
    result.matrix.m10 = (m->matrix.m12 * m->matrix.m20 - m->matrix.m10 * m->matrix.m22) * inv_det;
    result.matrix.m11 = (m->matrix.m00 * m->matrix.m22 - m->matrix.m02 * m->matrix.m20) * inv_det;
    result.matrix.m12 = (m->matrix.m02 * m->matrix.m10 - m->matrix.m00 * m->matrix.m12) * inv_det;
    
    result.matrix.m20 = (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20) * inv_det;
    result.matrix.m21 = (m->matrix.m01 * m->matrix.m20 - m->matrix.m00 * m->matrix.m21) * inv_det;
    result.matrix.m22 = (m->matrix.m00 * m->matrix.m11 - m->matrix.m01 * m->matrix.m10) * inv_det;
    return result;
}

VECMATH_API dmat4 dmat4_inverse(const dmat4* m)
{
    dmat4 result = { 0 };
    double inv[4][4] = { 0 };
    
    // calculate cofactors and determinant
    inv[0][0] = m->matrix.m11 * m->matrix.m22 * m->matrix.m33 - 
        m->matrix.m11 * m->matrix.m23 * m->matrix.m32 - 
        m->matrix.m21 * m->matrix.m12 * m->matrix.m33 + 
        m->matrix.m21 * m->matrix.m13 * m->matrix.m32 + 
        m->matrix.m31 * m->matrix.m12 * m->matrix.m23 - 
        m->matrix.m31 * m->matrix.m13 * m->matrix.m22;

    inv[1][0] = -m->matrix.m10 * m->matrix.m22 * m->matrix.m33 + 
        m->matrix.m10 * m->matrix.m23 * m->matrix.m32 + 
        m->matrix.m20 * m->matrix.m12 * m->matrix.m33 - 
        m->matrix.m20 * m->matrix.m13 * m->matrix.m32 - 
        m->matrix.m30 * m->matrix.m12 * m->matrix.m23 + 
        m->matrix.m30 * m->matrix.m13 * m->matrix.m22;

    inv[2][0] = m->matrix.m10 * m->matrix.m21 * m->matrix.m33 - 
        m->matrix.m10 * m->matrix.m23 * m->matrix.m31 - 
        m->matrix.m20 * m->matrix.m11 * m->matrix.m33 + 
        m->matrix.m20 * m->matrix.m13 * m->matrix.m31 + 
        m->matrix.m30 * m->matrix.m11 * m->matrix.m23 - 
        m->matrix.m30 * m->matrix.m13 * m->matrix.m21;

    inv[3][0] = -m->matrix.m10 * m->matrix.m21 * m->matrix.m32 + 
        m->matrix.m10 * m->matrix.m22 * m->matrix.m31 + 
        m->matrix.m20 * m->matrix.m11 * m->matrix.m32 - 
        m->matrix.m20 * m->matrix.m12 * m->matrix.m31 - 
        m->matrix.m30 * m->matrix.m11 * m->matrix.m22 + 
        m->matrix.m30 * m->matrix.m12 * m->matrix.m21;

    inv[0][1] = -m->matrix.m01 * m->matrix.m22 * m->matrix.m33 + 
        m->matrix.m01 * m->matrix.m23 * m->matrix.m32 + 
        m->matrix.m21 * m->matrix.m02 * m->matrix.m33 - 
        m->matrix.m21 * m->matrix.m03 * m->matrix.m32 - 
        m->matrix.m31 * m->matrix.m02 * m->matrix.m23 + 
        m->matrix.m31 * m->matrix.m03 * m->matrix.m22;

    inv[1][1] = m->matrix.m00 * m->matrix.m22 * m->matrix.m33 - 
        m->matrix.m00 * m->matrix.m23 * m->matrix.m32 - 
        m->matrix.m20 * m->matrix.m02 * m->matrix.m33 + 
        m->matrix.m20 * m->matrix.m03 * m->matrix.m32 + 
        m->matrix.m30 * m->matrix.m02 * m->matrix.m23 - 
        m->matrix.m30 * m->matrix.m03 * m->matrix.m22;

    inv[2][1] = -m->matrix.m00 * m->matrix.m21 * m->matrix.m33 + 
        m->matrix.m00 * m->matrix.m23 * m->matrix.m31 + 
        m->matrix.m20 * m->matrix.m01 * m->matrix.m33 - 
        m->matrix.m20 * m->matrix.m03 * m->matrix.m31 - 
        m->matrix.m30 * m->matrix.m01 * m->matrix.m23 + 
        m->matrix.m30 * m->matrix.m03 * m->matrix.m21;

    inv[3][1] = m->matrix.m00 * m->matrix.m21 * m->matrix.m32 - 
        m->matrix.m00 * m->matrix.m22 * m->matrix.m31 - 
        m->matrix.m20 * m->matrix.m01 * m->matrix.m32 + 
        m->matrix.m20 * m->matrix.m02 * m->matrix.m31 + 
        m->matrix.m30 * m->matrix.m01 * m->matrix.m22 - 
        m->matrix.m30 * m->matrix.m02 * m->matrix.m21;

    inv[0][2] = m->matrix.m01 * m->matrix.m12 * m->matrix.m33 - 
        m->matrix.m01 * m->matrix.m13 * m->matrix.m32 - 
        m->matrix.m11 * m->matrix.m02 * m->matrix.m33 + 
        m->matrix.m11 * m->matrix.m03 * m->matrix.m32 + 
        m->matrix.m31 * m->matrix.m02 * m->matrix.m13 - 
        m->matrix.m31 * m->matrix.m03 * m->matrix.m12;

    inv[1][2] = -m->matrix.m00 * m->matrix.m12 * m->matrix.m33 + 
        m->matrix.m00 * m->matrix.m13 * m->matrix.m32 + 
        m->matrix.m10 * m->matrix.m02 * m->matrix.m33 - 
        m->matrix.m10 * m->matrix.m03 * m->matrix.m32 - 
        m->matrix.m30 * m->matrix.m02 * m->matrix.m13 + 
        m->matrix.m30 * m->matrix.m03 * m->matrix.m12;

    inv[2][2] = m->matrix.m00 * m->matrix.m11 * m->matrix.m33 - 
        m->matrix.m00 * m->matrix.m13 * m->matrix.m31 - 
        m->matrix.m10 * m->matrix.m01 * m->matrix.m33 + 
        m->matrix.m10 * m->matrix.m03 * m->matrix.m31 + 
        m->matrix.m30 * m->matrix.m01 * m->matrix.m13 - 
        m->matrix.m30 * m->matrix.m03 * m->matrix.m11;

    inv[3][2] = -m->matrix.m00 * m->matrix.m11 * m->matrix.m32 + 
        m->matrix.m00 * m->matrix.m12 * m->matrix.m31 + 
        m->matrix.m10 * m->matrix.m01 * m->matrix.m32 - 
        m->matrix.m10 * m->matrix.m02 * m->matrix.m31 - 
        m->matrix.m30 * m->matrix.m01 * m->matrix.m12 + 
        m->matrix.m30 * m->matrix.m02 * m->matrix.m11;

    inv[0][3] = -m->matrix.m01 * m->matrix.m12 * m->matrix.m23 + 
        m->matrix.m01 * m->matrix.m13 * m->matrix.m22 + 
        m->matrix.m11 * m->matrix.m02 * m->matrix.m23 - 
        m->matrix.m11 * m->matrix.m03 * m->matrix.m22 - 
        m->matrix.m21 * m->matrix.m02 * m->matrix.m13 + 
        m->matrix.m21 * m->matrix.m03 * m->matrix.m12;

    inv[1][3] = m->matrix.m00 * m->matrix.m12 * m->matrix.m23 - 
        m->matrix.m00 * m->matrix.m13 * m->matrix.m22 - 
        m->matrix.m10 * m->matrix.m02 * m->matrix.m23 + 
        m->matrix.m10 * m->matrix.m03 * m->matrix.m22 + 
        m->matrix.m20 * m->matrix.m02 * m->matrix.m13 - 
        m->matrix.m20 * m->matrix.m03 * m->matrix.m12;

    inv[2][3] = -m->matrix.m00 * m->matrix.m11 * m->matrix.m23 + 
        m->matrix.m00 * m->matrix.m13 * m->matrix.m21 + 
        m->matrix.m10 * m->matrix.m01 * m->matrix.m23 - 
        m->matrix.m10 * m->matrix.m03 * m->matrix.m21 - 
        m->matrix.m20 * m->matrix.m01 * m->matrix.m13 + 
        m->matrix.m20 * m->matrix.m03 * m->matrix.m11;

    inv[3][3] = m->matrix.m00 * m->matrix.m11 * m->matrix.m22 - 
        m->matrix.m00 * m->matrix.m12 * m->matrix.m21 - 
        m->matrix.m10 * m->matrix.m01 * m->matrix.m22 + 
        m->matrix.m10 * m->matrix.m02 * m->matrix.m21 + 
        m->matrix.m20 * m->matrix.m01 * m->matrix.m12 - 
        m->matrix.m20 * m->matrix.m02 * m->matrix.m11;

    // calculate determinant
    double det = m->matrix.m00 * inv[0][0] + m->matrix.m01 * inv[1][0] + m->matrix.m02 * inv[2][0] + m->matrix.m03 * inv[3][0];

    if (fabs(det) < VECMATH_EPSILON_DZERO) {
        return dmat4_identity();
    }

    // scale by 1/determinant
    double inv_det = 1.0f / det;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = inv[i][j] * inv_det;
        }
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// get_translation
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fmat4_get_translation_rowmajor(const fmat4* m)
{
    float3 result = { 0 };
    result.xyz.x = m->matrix.m03;
    result.xyz.y = m->matrix.m13;
    result.xyz.z = m->matrix.m23;
    return result;
}

VECMATH_API float3 fmat4_get_translation_colmajor(const fmat4* m)
{
    float3 result = { 0 };
    result.xyz.x = m->matrix.m30;
    result.xyz.y = m->matrix.m31;
    result.xyz.z = m->matrix.m32;
    return result;
}

VECMATH_API double3 dmat4_get_translation_rowmajor(const dmat4* m)
{
    double3 result = { 0 };
    result.xyz.x = m->matrix.m03;
    result.xyz.y = m->matrix.m13;
    result.xyz.z = m->matrix.m23;
    return result;
}

VECMATH_API double3 dmat4_get_translation_colmajor(const dmat4* m)
{
    double3 result = { 0 };
    result.xyz.x = m->matrix.m30;
    result.xyz.y = m->matrix.m31;
    result.xyz.z = m->matrix.m32;
    return result;
}


/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// get_scale
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fmat4_get_scale_rowmajor(const fmat4* m)
{
    // scale is length of each row (basis vectors)
    float3 scale = { 0 };
    scale.xyz.x = sqrtf(m->matrix.m00 * m->matrix.m00 + m->matrix.m01 * m->matrix.m01 + m->matrix.m02*m->matrix.m02);
    scale.xyz.y = sqrtf(m->matrix.m10 * m->matrix.m10 + m->matrix.m11 * m->matrix.m11 + m->matrix.m12*m->matrix.m12);
    scale.xyz.z = sqrtf(m->matrix.m20 * m->matrix.m20 + m->matrix.m21 * m->matrix.m21 + m->matrix.m22*m->matrix.m22);
    
    // check for negative scale
    float det = 
        m->matrix.m00 * (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) -
        m->matrix.m01 * (m->matrix.m10 * m->matrix.m22 - m->matrix.m12 * m->matrix.m20) +
        m->matrix.m02 * (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20);
    
    if (det < 0) {
        scale.xyz.x = -scale.xyz.x;
    }
    
    return scale;
}

VECMATH_API float3 fmat4_get_scale_colmajor(const fmat4* m)
{
    // scale is length of each column (basis vectors)
    float3 scale = { 0 };
    scale.xyz.x = sqrtf(m->matrix.m00 * m->matrix.m00 + m->matrix.m10 * m->matrix.m10 + m->matrix.m20 * m->matrix.m20);
    scale.xyz.y = sqrtf(m->matrix.m01 * m->matrix.m01 + m->matrix.m11 * m->matrix.m11 + m->matrix.m21 * m->matrix.m21);
    scale.xyz.z = sqrtf(m->matrix.m02 * m->matrix.m02 + m->matrix.m12 * m->matrix.m12 + m->matrix.m22 * m->matrix.m22);
    
    // check for negative scale
    float det =
        m->matrix.m00 * (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) -
        m->matrix.m01 * (m->matrix.m10 * m->matrix.m22 - m->matrix.m12 * m->matrix.m20) +
        m->matrix.m02 * (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20);
    
    if (det < 0) {
        scale.xyz.x = -scale.xyz.x;
    }
    
    return scale;
}

VECMATH_API double3 dmat4_get_scale_rowmajor(const dmat4* m)
{
    // scale is length of each row (basis vectors)
    double3 scale = { 0 };
    scale.xyz.x = sqrt(m->matrix.m00 * m->matrix.m00 + m->matrix.m01 * m->matrix.m01 + m->matrix.m02*m->matrix.m02);
    scale.xyz.y = sqrt(m->matrix.m10 * m->matrix.m10 + m->matrix.m11 * m->matrix.m11 + m->matrix.m12*m->matrix.m12);
    scale.xyz.z = sqrt(m->matrix.m20 * m->matrix.m20 + m->matrix.m21 * m->matrix.m21 + m->matrix.m22*m->matrix.m22);
    
    // check for negative scale
    double det = 
        m->matrix.m00 * (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) -
        m->matrix.m01 * (m->matrix.m10 * m->matrix.m22 - m->matrix.m12 * m->matrix.m20) +
        m->matrix.m02 * (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20);
    
    if (det < 0) {
        scale.xyz.x = -scale.xyz.x;
    }
    
    return scale;
}

VECMATH_API double3 dmat4_get_scale_colmajor(const dmat4* m)
{
    // scale is length of each column (basis vectors)
    double3 scale = { 0 };
    scale.xyz.x = sqrt(m->matrix.m00 * m->matrix.m00 + m->matrix.m10 * m->matrix.m10 + m->matrix.m20 * m->matrix.m20);
    scale.xyz.y = sqrt(m->matrix.m01 * m->matrix.m01 + m->matrix.m11 * m->matrix.m11 + m->matrix.m21 * m->matrix.m21);
    scale.xyz.z = sqrt(m->matrix.m02 * m->matrix.m02 + m->matrix.m12 * m->matrix.m12 + m->matrix.m22 * m->matrix.m22);
    
    // check for negative scale
    double det =
        m->matrix.m00 * (m->matrix.m11 * m->matrix.m22 - m->matrix.m12 * m->matrix.m21) -
        m->matrix.m01 * (m->matrix.m10 * m->matrix.m22 - m->matrix.m12 * m->matrix.m20) +
        m->matrix.m02 * (m->matrix.m10 * m->matrix.m21 - m->matrix.m11 * m->matrix.m20);
    
    if (det < 0) {
        scale.xyz.x = -scale.xyz.x;
    }
    
    return scale;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// get_rotation
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fmat4_get_rotation_rowmajor(const fmat4* m)
{
    float3 scale = fmat4_get_scale_rowmajor(m);
    float3 rotation = { 0 };
    
    if (scale.xyz.x < VECMATH_EPSILON_FZERO || scale.xyz.y < VECMATH_EPSILON_FZERO || scale.xyz.z < VECMATH_EPSILON_FZERO) {
        return rotation;
    }
    
    // remove scale from rows
    float m00 = m->matrix.m00 / scale.xyz.x;
    float m01 = m->matrix.m01 / scale.xyz.x;
    float m02 = m->matrix.m02 / scale.xyz.x;
    
    float m10 = m->matrix.m10 / scale.xyz.y;
    float m11 = m->matrix.m11 / scale.xyz.y;
    float m12 = m->matrix.m12 / scale.xyz.y;
    
    float m20 = m->matrix.m20 / scale.xyz.z;
    float m21 = m->matrix.m21 / scale.xyz.z;
    float m22 = m->matrix.m22 / scale.xyz.z;
    
    // extract Euler angles from row-major rotation matrix
    rotation.xyz.y = atan2f(m02, sqrtf(m00*m00 + m01*m01));
    
    if (fabsf(rotation.xyz.y - (float)VECMATH_EPSILON_PI / 2.0f) < VECMATH_EPSILON_FZERO) {
        rotation.xyz.x = atan2f(m10, m11);
        rotation.xyz.z = 0.0f;
    }

    else if (fabsf(rotation.xyz.y + (float)VECMATH_EPSILON_PI / 2.0f) < VECMATH_EPSILON_FZERO) {
        rotation.xyz.x = -atan2f(m10, m11);
        rotation.xyz.z = 0.0f;
    }

    else {
        rotation.xyz.x = atan2f(m12, m22);
        rotation.xyz.z = atan2f(m01, m00);
    }
    
    return rotation;
}

VECMATH_API float3 fmat4_get_rotation_colmajor(const fmat4* m)
{
    float3 scale = fmat4_get_scale_colmajor(m);
    float3 rotation = { 0 };
    
    if (scale.xyz.x < VECMATH_EPSILON_FZERO || scale.xyz.y < VECMATH_EPSILON_FZERO || scale.xyz.z < VECMATH_EPSILON_FZERO) {
        return rotation;
    }
    
    // remove scale from columns
    float m00 = m->matrix.m00 / scale.xyz.x;
    float m10 = m->matrix.m10 / scale.xyz.x;
    float m20 = m->matrix.m20 / scale.xyz.x;
    
    float m01 = m->matrix.m01 / scale.xyz.y;
    float m11 = m->matrix.m11 / scale.xyz.y;
    float m21 = m->matrix.m21 / scale.xyz.y;
    
    float m02 = m->matrix.m02 / scale.xyz.z;
    float m12 = m->matrix.m12 / scale.xyz.z;
    float m22 = m->matrix.m22 / scale.xyz.z;
    
    // extract euler angles from column-major rotation matrix
    rotation.xyz.y = atan2f(-m20, sqrtf(m00*m00 + m10*m10));
    
    if (fabsf(rotation.xyz.y - (float)VECMATH_EPSILON_PI / 2.0f) < VECMATH_EPSILON_FZERO) {
        rotation.xyz.x = atan2f(m01, m11);
        rotation.xyz.z = 0.0f;
    }

    else if (fabsf(rotation.xyz.y + (float)VECMATH_EPSILON_PI / 2.0f) < VECMATH_EPSILON_FZERO) {
        rotation.xyz.x = -atan2f(m01, m11);
        rotation.xyz.z = 0.0f;
    }

    else {
        rotation.xyz.x = atan2f(m21, m22);
        rotation.xyz.z = atan2f(m10, m00);
    }
    
    return rotation;
}

VECMATH_API double3 dmat4_get_rotation_rowmajor(const dmat4* m)
{
    double3 scale = dmat4_get_scale_rowmajor(m);
    double3 rotation = { 0 };
    
    if (scale.xyz.x < VECMATH_EPSILON_DZERO || scale.xyz.y < VECMATH_EPSILON_DZERO || scale.xyz.z < VECMATH_EPSILON_DZERO) {
        return rotation;
    }
    
    // remove scale from rows
    double m00 = m->matrix.m00 / scale.xyz.x;
    double m01 = m->matrix.m01 / scale.xyz.x;
    double m02 = m->matrix.m02 / scale.xyz.x;
    
    double m10 = m->matrix.m10 / scale.xyz.y;
    double m11 = m->matrix.m11 / scale.xyz.y;
    double m12 = m->matrix.m12 / scale.xyz.y;
    
    double m20 = m->matrix.m20 / scale.xyz.z;
    double m21 = m->matrix.m21 / scale.xyz.z;
    double m22 = m->matrix.m22 / scale.xyz.z;
    
    // extract Euler angles from row-major rotation matrix
    rotation.xyz.y = atan2(m02, sqrt(m00*m00 + m01*m01));
    
    if (fabs(rotation.xyz.y - VECMATH_EPSILON_PI / 2.0) < VECMATH_EPSILON_DZERO) {
        rotation.xyz.x = atan2(m10, m11);
        rotation.xyz.z = 0.0;
    }

    else if (fabs(rotation.xyz.y + VECMATH_EPSILON_PI / 2.0) < VECMATH_EPSILON_DZERO) {
        rotation.xyz.x = -atan2(m10, m11);
        rotation.xyz.z = 0.0;
    }

    else {
        rotation.xyz.x = atan2(m12, m22);
        rotation.xyz.z = atan2(m01, m00);
    }
    
    return rotation;
}

VECMATH_API double3 dmat4_get_rotation_colmajor(const dmat4* m)
{
    double3 scale = dmat4_get_scale_colmajor(m);
    double3 rotation = { 0 };
    
    if (scale.xyz.x < VECMATH_EPSILON_DZERO || scale.xyz.y < VECMATH_EPSILON_DZERO || scale.xyz.z < VECMATH_EPSILON_DZERO) {
        return rotation;
    }
    
    // remove scale from columns
    double m00 = m->matrix.m00 / scale.xyz.x;
    double m10 = m->matrix.m10 / scale.xyz.x;
    double m20 = m->matrix.m20 / scale.xyz.x;
    
    double m01 = m->matrix.m01 / scale.xyz.y;
    double m11 = m->matrix.m11 / scale.xyz.y;
    double m21 = m->matrix.m21 / scale.xyz.y;
    
    double m02 = m->matrix.m02 / scale.xyz.z;
    double m12 = m->matrix.m12 / scale.xyz.z;
    double m22 = m->matrix.m22 / scale.xyz.z;
    
    // extract euler angles from column-major rotation matrix
    rotation.xyz.y = atan2(-m20, sqrt(m00*m00 + m10*m10));
    
    if (fabs(rotation.xyz.y - VECMATH_EPSILON_PI / 2.0) < VECMATH_EPSILON_DZERO) {
        rotation.xyz.x = atan2(m01, m11);
        rotation.xyz.z = 0.0;
    }

    else if (fabs(rotation.xyz.y + VECMATH_EPSILON_PI / 2.0) < VECMATH_EPSILON_DZERO) {
        rotation.xyz.x = -atan2(m01, m11);
        rotation.xyz.z = 0.0;
    }

    else {
        rotation.xyz.x = atan2(m21, m22);
        rotation.xyz.z = atan2(m10, m00);
    }
    
    return rotation;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// decompose
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API void fmat4_decompose_rowmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale)
{
    if (translation)    *translation = fmat4_get_translation_rowmajor(m);
    if (scale)          *scale = fmat4_get_scale_rowmajor(m);
    if (rotation)       *rotation = fmat4_get_rotation_rowmajor(m);
}

VECMATH_API void fmat4_decompose_colmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale)
{
    if (translation)    *translation = fmat4_get_translation_colmajor(m);
    if (scale)          *scale = fmat4_get_scale_colmajor(m);
    if (rotation)       *rotation = fmat4_get_rotation_colmajor(m);
}

VECMATH_API void dmat4_decompose_rowmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale)
{
    if (translation)    *translation = dmat4_get_translation_rowmajor(m);
    if (scale)          *scale = dmat4_get_scale_rowmajor(m);
    if (rotation)       *rotation = dmat4_get_rotation_rowmajor(m);
}

VECMATH_API void dmat4_decompose_colmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale)
{
    if (translation)    *translation = dmat4_get_translation_colmajor(m);
    if (scale)          *scale = dmat4_get_scale_colmajor(m);
    if (rotation)       *rotation = dmat4_get_rotation_colmajor(m);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// translate
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_translate_rowmajor(const fmat4* m, const float3* dir)
{
    fmat4 result = *m;

    result.data[3][0] += dir->xyz.x;
    result.data[3][1] += dir->xyz.y;
    result.data[3][2] += dir->xyz.z;
    return result;
}

VECMATH_API fmat4 fmat4_translate_colmajor(const fmat4* m, const float3 *dir)
{
    fmat4 result = *m;

    // column-major: translation goes in m03, m13, m23
    result.matrix.m03 = m->matrix.m00 * dir->xyz.x + m->matrix.m01 * dir->xyz.y + m->matrix.m02 * dir->xyz.z + m->matrix.m03;
    result.matrix.m13 = m->matrix.m10 * dir->xyz.x + m->matrix.m11 * dir->xyz.y + m->matrix.m12 * dir->xyz.z + m->matrix.m13;
    result.matrix.m23 = m->matrix.m20 * dir->xyz.x + m->matrix.m21 * dir->xyz.y + m->matrix.m22 * dir->xyz.z + m->matrix.m23;
    result.matrix.m33 = m->matrix.m30 * dir->xyz.x + m->matrix.m31 * dir->xyz.y + m->matrix.m32 * dir->xyz.z + m->matrix.m33;

    return result;
}

VECMATH_API dmat4 dmat4_translate_rowmajor(const dmat4* m, const double3* dir)
{
    dmat4 result = *m;

    result.data[3][0] += dir->xyz.x;
    result.data[3][1] += dir->xyz.y;
    result.data[3][2] += dir->xyz.z;
    return result;
}

VECMATH_API dmat4 dmat4_translate_colmajor(const dmat4* m, const double3* dir)
{
    dmat4 result = *m;

    result.data[0][3] += dir->xyz.x;
    result.data[1][3] += dir->xyz.y;
    result.data[2][3] += dir->xyz.z;
    return result;
}


/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// rotate
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_rotate_colmajor(const fmat4* m, float angle, const float3* axis)
{
    // normalize axis and compute temp values
    float3 axis_n = float3_normalize(axis);
    float c = cosf(angle);
    float s = sinf(angle);
    float one_minus_c = 1.0f - c;
    fmat4 rotate = { 0 };
    
    // column 0
    rotate.data[0][0] = c + axis_n.xyz.x * axis_n.xyz.x * one_minus_c;
    rotate.data[1][0] = axis_n.xyz.y * axis_n.xyz.x * one_minus_c + axis_n.xyz.z * s;
    rotate.data[2][0] = axis_n.xyz.z * axis_n.xyz.x * one_minus_c - axis_n.xyz.y * s;
    // column 1
    rotate.data[0][1] = axis_n.xyz.x * axis_n.xyz.y * one_minus_c - axis_n.xyz.z * s;
    rotate.data[1][1] = c + axis_n.xyz.y * axis_n.xyz.y * one_minus_c;
    rotate.data[2][1] = axis_n.xyz.z * axis_n.xyz.y * one_minus_c + axis_n.xyz.x * s;
    // column 2
    rotate.data[0][2] = axis_n.xyz.x * axis_n.xyz.z * one_minus_c + axis_n.xyz.y * s;
    rotate.data[1][2] = axis_n.xyz.y * axis_n.xyz.z * one_minus_c - axis_n.xyz.x * s;
    rotate.data[2][2] = c + axis_n.xyz.z * axis_n.xyz.z * one_minus_c;
    
    // column 3 (identity)
    rotate.data[3][3] = 1.0f;
    
    // multiply: result = m * rotate (column-major)
    fmat4 result = { 0 };
    
    // column 0
    result.data[0][0] = m->data[0][0]*rotate.data[0][0] + m->data[0][1]*rotate.data[1][0] + m->data[0][2]*rotate.data[2][0] + m->data[0][3]*rotate.data[3][0];
    result.data[1][0] = m->data[1][0]*rotate.data[0][0] + m->data[1][1]*rotate.data[1][0] + m->data[1][2]*rotate.data[2][0] + m->data[1][3]*rotate.data[3][0];
    result.data[2][0] = m->data[2][0]*rotate.data[0][0] + m->data[2][1]*rotate.data[1][0] + m->data[2][2]*rotate.data[2][0] + m->data[2][3]*rotate.data[3][0];
    result.data[3][0] = m->data[3][0]*rotate.data[0][0] + m->data[3][1]*rotate.data[1][0] + m->data[3][2]*rotate.data[2][0] + m->data[3][3]*rotate.data[3][0];
    // column 1
    result.data[0][1] = m->data[0][0]*rotate.data[0][1] + m->data[0][1]*rotate.data[1][1] + m->data[0][2]*rotate.data[2][1] + m->data[0][3]*rotate.data[3][1];
    result.data[1][1] = m->data[1][0]*rotate.data[0][1] + m->data[1][1]*rotate.data[1][1] + m->data[1][2]*rotate.data[2][1] + m->data[1][3]*rotate.data[3][1];
    result.data[2][1] = m->data[2][0]*rotate.data[0][1] + m->data[2][1]*rotate.data[1][1] + m->data[2][2]*rotate.data[2][1] + m->data[2][3]*rotate.data[3][1];
    result.data[3][1] = m->data[3][0]*rotate.data[0][1] + m->data[3][1]*rotate.data[1][1] + m->data[3][2]*rotate.data[2][1] + m->data[3][3]*rotate.data[3][1];
    // column 2
    result.data[0][2] = m->data[0][0]*rotate.data[0][2] + m->data[0][1]*rotate.data[1][2] + m->data[0][2]*rotate.data[2][2] + m->data[0][3]*rotate.data[3][2];
    result.data[1][2] = m->data[1][0]*rotate.data[0][2] + m->data[1][1]*rotate.data[1][2] + m->data[1][2]*rotate.data[2][2] + m->data[1][3]*rotate.data[3][2];
    result.data[2][2] = m->data[2][0]*rotate.data[0][2] + m->data[2][1]*rotate.data[1][2] + m->data[2][2]*rotate.data[2][2] + m->data[2][3]*rotate.data[3][2];
    result.data[3][2] = m->data[3][0]*rotate.data[0][2] + m->data[3][1]*rotate.data[1][2] + m->data[3][2]*rotate.data[2][2] + m->data[3][3]*rotate.data[3][2];
    // column 3
    result.data[0][3] = m->data[0][3];
    result.data[1][3] = m->data[1][3];
    result.data[2][3] = m->data[2][3];
    result.data[3][3] = m->data[3][3];
    
    return result;
}

VECMATH_API fmat4 fmat4_rotate_rowmajor(const fmat4* m, float angle, const float3* axis)
{
    // normalize axis and compute temp values
    float3 axis_n = float3_normalize(axis);
    float c = cosf(angle);
    float s = sinf(angle);
    float one_minus_c = 1.0f - c;
    
    // construct rotation matrix (row-major)
    fmat4 rotate = { 0 };
    
    // row 0
    rotate.data[0][0] = c + axis_n.xyz.x * axis_n.xyz.x * one_minus_c;
    rotate.data[0][1] = axis_n.xyz.x * axis_n.xyz.y * one_minus_c - axis_n.xyz.z * s;
    rotate.data[0][2] = axis_n.xyz.x * axis_n.xyz.z * one_minus_c + axis_n.xyz.y * s;
    rotate.data[0][3] = 0.0f;
    // row 1
    rotate.data[1][0] = axis_n.xyz.y * axis_n.xyz.x * one_minus_c + axis_n.xyz.z * s;
    rotate.data[1][1] = c + axis_n.xyz.y * axis_n.xyz.y * one_minus_c;
    rotate.data[1][2] = axis_n.xyz.y * axis_n.xyz.z * one_minus_c - axis_n.xyz.x * s;
    rotate.data[1][3] = 0.0f;
    // row 2
    rotate.data[2][0] = axis_n.xyz.z * axis_n.xyz.x * one_minus_c - axis_n.xyz.y * s;
    rotate.data[2][1] = axis_n.xyz.z * axis_n.xyz.y * one_minus_c + axis_n.xyz.x * s;
    rotate.data[2][2] = c + axis_n.xyz.z * axis_n.xyz.z * one_minus_c;
    rotate.data[2][3] = 0.0f;
    // row 3
    rotate.data[3][0] = 0.0f;
    rotate.data[3][1] = 0.0f;
    rotate.data[3][2] = 0.0f;
    rotate.data[3][3] = 1.0f;
    
    // multiply: result = rotate * m (row-major)
    fmat4 result = { 0 };
    
    // row 0
    result.data[0][0] = rotate.data[0][0]*m->data[0][0] + rotate.data[0][1]*m->data[1][0] + rotate.data[0][2]*m->data[2][0] + rotate.data[0][3]*m->data[3][0];
    result.data[0][1] = rotate.data[0][0]*m->data[0][1] + rotate.data[0][1]*m->data[1][1] + rotate.data[0][2]*m->data[2][1] + rotate.data[0][3]*m->data[3][1];
    result.data[0][2] = rotate.data[0][0]*m->data[0][2] + rotate.data[0][1]*m->data[1][2] + rotate.data[0][2]*m->data[2][2] + rotate.data[0][3]*m->data[3][2];
    result.data[0][3] = rotate.data[0][0]*m->data[0][3] + rotate.data[0][1]*m->data[1][3] + rotate.data[0][2]*m->data[2][3] + rotate.data[0][3]*m->data[3][3];
    // row 1
    result.data[1][0] = rotate.data[1][0]*m->data[0][0] + rotate.data[1][1]*m->data[1][0] + rotate.data[1][2]*m->data[2][0] + rotate.data[1][3]*m->data[3][0];
    result.data[1][1] = rotate.data[1][0]*m->data[0][1] + rotate.data[1][1]*m->data[1][1] + rotate.data[1][2]*m->data[2][1] + rotate.data[1][3]*m->data[3][1];
    result.data[1][2] = rotate.data[1][0]*m->data[0][2] + rotate.data[1][1]*m->data[1][2] + rotate.data[1][2]*m->data[2][2] + rotate.data[1][3]*m->data[3][2];
    result.data[1][3] = rotate.data[1][0]*m->data[0][3] + rotate.data[1][1]*m->data[1][3] + rotate.data[1][2]*m->data[2][3] + rotate.data[1][3]*m->data[3][3];
    // row 2
    result.data[2][0] = rotate.data[2][0]*m->data[0][0] + rotate.data[2][1]*m->data[1][0] + rotate.data[2][2]*m->data[2][0] + rotate.data[2][3]*m->data[3][0];
    result.data[2][1] = rotate.data[2][0]*m->data[0][1] + rotate.data[2][1]*m->data[1][1] + rotate.data[2][2]*m->data[2][1] + rotate.data[2][3]*m->data[3][1];
    result.data[2][2] = rotate.data[2][0]*m->data[0][2] + rotate.data[2][1]*m->data[1][2] + rotate.data[2][2]*m->data[2][2] + rotate.data[2][3]*m->data[3][2];
    result.data[2][3] = rotate.data[2][0]*m->data[0][3] + rotate.data[2][1]*m->data[1][3] + rotate.data[2][2]*m->data[2][3] + rotate.data[2][3]*m->data[3][3];
    // row 3 
    result.data[3][0] = m->data[3][0];
    result.data[3][1] = m->data[3][1];
    result.data[3][2] = m->data[3][2];
    result.data[3][3] = m->data[3][3];
    
    return result;
}

VECMATH_API dmat4 dmat4_rotate_colmajor(const dmat4* m, double angle, const double3* axis)
{
    // normalize axis and compute temp values
    double3 axis_n = double3_normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double one_minus_c = 1.0 - c;
    dmat4 rotate = { 0 };
    
    // column 0
    rotate.data[0][0] = c + axis_n.xyz.x * axis_n.xyz.x * one_minus_c;
    rotate.data[1][0] = axis_n.xyz.y * axis_n.xyz.x * one_minus_c + axis_n.xyz.z * s;
    rotate.data[2][0] = axis_n.xyz.z * axis_n.xyz.x * one_minus_c - axis_n.xyz.y * s;
    // column 1
    rotate.data[0][1] = axis_n.xyz.x * axis_n.xyz.y * one_minus_c - axis_n.xyz.z * s;
    rotate.data[1][1] = c + axis_n.xyz.y * axis_n.xyz.y * one_minus_c;
    rotate.data[2][1] = axis_n.xyz.z * axis_n.xyz.y * one_minus_c + axis_n.xyz.x * s;
    // column 2
    rotate.data[0][2] = axis_n.xyz.x * axis_n.xyz.z * one_minus_c + axis_n.xyz.y * s;
    rotate.data[1][2] = axis_n.xyz.y * axis_n.xyz.z * one_minus_c - axis_n.xyz.x * s;
    rotate.data[2][2] = c + axis_n.xyz.z * axis_n.xyz.z * one_minus_c;
    
    // column 3 (identity)
    rotate.data[3][3] = 1.0f;
    
    // multiply: result = m * rotate (column-major)
    dmat4 result = { 0 };
    
    // column 0
    result.data[0][0] = m->data[0][0]*rotate.data[0][0] + m->data[0][1]*rotate.data[1][0] + m->data[0][2]*rotate.data[2][0] + m->data[0][3]*rotate.data[3][0];
    result.data[1][0] = m->data[1][0]*rotate.data[0][0] + m->data[1][1]*rotate.data[1][0] + m->data[1][2]*rotate.data[2][0] + m->data[1][3]*rotate.data[3][0];
    result.data[2][0] = m->data[2][0]*rotate.data[0][0] + m->data[2][1]*rotate.data[1][0] + m->data[2][2]*rotate.data[2][0] + m->data[2][3]*rotate.data[3][0];
    result.data[3][0] = m->data[3][0]*rotate.data[0][0] + m->data[3][1]*rotate.data[1][0] + m->data[3][2]*rotate.data[2][0] + m->data[3][3]*rotate.data[3][0];
    // column 1
    result.data[0][1] = m->data[0][0]*rotate.data[0][1] + m->data[0][1]*rotate.data[1][1] + m->data[0][2]*rotate.data[2][1] + m->data[0][3]*rotate.data[3][1];
    result.data[1][1] = m->data[1][0]*rotate.data[0][1] + m->data[1][1]*rotate.data[1][1] + m->data[1][2]*rotate.data[2][1] + m->data[1][3]*rotate.data[3][1];
    result.data[2][1] = m->data[2][0]*rotate.data[0][1] + m->data[2][1]*rotate.data[1][1] + m->data[2][2]*rotate.data[2][1] + m->data[2][3]*rotate.data[3][1];
    result.data[3][1] = m->data[3][0]*rotate.data[0][1] + m->data[3][1]*rotate.data[1][1] + m->data[3][2]*rotate.data[2][1] + m->data[3][3]*rotate.data[3][1];
    // column 2
    result.data[0][2] = m->data[0][0]*rotate.data[0][2] + m->data[0][1]*rotate.data[1][2] + m->data[0][2]*rotate.data[2][2] + m->data[0][3]*rotate.data[3][2];
    result.data[1][2] = m->data[1][0]*rotate.data[0][2] + m->data[1][1]*rotate.data[1][2] + m->data[1][2]*rotate.data[2][2] + m->data[1][3]*rotate.data[3][2];
    result.data[2][2] = m->data[2][0]*rotate.data[0][2] + m->data[2][1]*rotate.data[1][2] + m->data[2][2]*rotate.data[2][2] + m->data[2][3]*rotate.data[3][2];
    result.data[3][2] = m->data[3][0]*rotate.data[0][2] + m->data[3][1]*rotate.data[1][2] + m->data[3][2]*rotate.data[2][2] + m->data[3][3]*rotate.data[3][2];
    // column 3
    result.data[0][3] = m->data[0][3];
    result.data[1][3] = m->data[1][3];
    result.data[2][3] = m->data[2][3];
    result.data[3][3] = m->data[3][3];
    
    return result;
}

VECMATH_API dmat4 dmat4_rotate_rowmajor(const dmat4* m, double angle, const double3* axis)
{
    // normalize axis and compute temp values
    double3 axis_n = double3_normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double one_minus_c = 1.0f - c;
    
    // construct rotation matrix (row-major)
    dmat4 rotate = { 0 };
    
    // row 0
    rotate.data[0][0] = c + axis_n.xyz.x * axis_n.xyz.x * one_minus_c;
    rotate.data[0][1] = axis_n.xyz.x * axis_n.xyz.y * one_minus_c - axis_n.xyz.z * s;
    rotate.data[0][2] = axis_n.xyz.x * axis_n.xyz.z * one_minus_c + axis_n.xyz.y * s;
    rotate.data[0][3] = 0.0f;
    // row 1
    rotate.data[1][0] = axis_n.xyz.y * axis_n.xyz.x * one_minus_c + axis_n.xyz.z * s;
    rotate.data[1][1] = c + axis_n.xyz.y * axis_n.xyz.y * one_minus_c;
    rotate.data[1][2] = axis_n.xyz.y * axis_n.xyz.z * one_minus_c - axis_n.xyz.x * s;
    rotate.data[1][3] = 0.0f;
    // row 2
    rotate.data[2][0] = axis_n.xyz.z * axis_n.xyz.x * one_minus_c - axis_n.xyz.y * s;
    rotate.data[2][1] = axis_n.xyz.z * axis_n.xyz.y * one_minus_c + axis_n.xyz.x * s;
    rotate.data[2][2] = c + axis_n.xyz.z * axis_n.xyz.z * one_minus_c;
    rotate.data[2][3] = 0.0f;
    // row 3
    rotate.data[3][0] = 0.0f;
    rotate.data[3][1] = 0.0f;
    rotate.data[3][2] = 0.0f;
    rotate.data[3][3] = 1.0f;
    
    // multiply: result = rotate * m (row-major)
    dmat4 result = { 0 };
    
    // row 0
    result.data[0][0] = rotate.data[0][0]*m->data[0][0] + rotate.data[0][1]*m->data[1][0] + rotate.data[0][2]*m->data[2][0] + rotate.data[0][3]*m->data[3][0];
    result.data[0][1] = rotate.data[0][0]*m->data[0][1] + rotate.data[0][1]*m->data[1][1] + rotate.data[0][2]*m->data[2][1] + rotate.data[0][3]*m->data[3][1];
    result.data[0][2] = rotate.data[0][0]*m->data[0][2] + rotate.data[0][1]*m->data[1][2] + rotate.data[0][2]*m->data[2][2] + rotate.data[0][3]*m->data[3][2];
    result.data[0][3] = rotate.data[0][0]*m->data[0][3] + rotate.data[0][1]*m->data[1][3] + rotate.data[0][2]*m->data[2][3] + rotate.data[0][3]*m->data[3][3];
    // row 1
    result.data[1][0] = rotate.data[1][0]*m->data[0][0] + rotate.data[1][1]*m->data[1][0] + rotate.data[1][2]*m->data[2][0] + rotate.data[1][3]*m->data[3][0];
    result.data[1][1] = rotate.data[1][0]*m->data[0][1] + rotate.data[1][1]*m->data[1][1] + rotate.data[1][2]*m->data[2][1] + rotate.data[1][3]*m->data[3][1];
    result.data[1][2] = rotate.data[1][0]*m->data[0][2] + rotate.data[1][1]*m->data[1][2] + rotate.data[1][2]*m->data[2][2] + rotate.data[1][3]*m->data[3][2];
    result.data[1][3] = rotate.data[1][0]*m->data[0][3] + rotate.data[1][1]*m->data[1][3] + rotate.data[1][2]*m->data[2][3] + rotate.data[1][3]*m->data[3][3];
    // row 2
    result.data[2][0] = rotate.data[2][0]*m->data[0][0] + rotate.data[2][1]*m->data[1][0] + rotate.data[2][2]*m->data[2][0] + rotate.data[2][3]*m->data[3][0];
    result.data[2][1] = rotate.data[2][0]*m->data[0][1] + rotate.data[2][1]*m->data[1][1] + rotate.data[2][2]*m->data[2][1] + rotate.data[2][3]*m->data[3][1];
    result.data[2][2] = rotate.data[2][0]*m->data[0][2] + rotate.data[2][1]*m->data[1][2] + rotate.data[2][2]*m->data[2][2] + rotate.data[2][3]*m->data[3][2];
    result.data[2][3] = rotate.data[2][0]*m->data[0][3] + rotate.data[2][1]*m->data[1][3] + rotate.data[2][2]*m->data[2][3] + rotate.data[2][3]*m->data[3][3];
    // row 3 
    result.data[3][0] = m->data[3][0];
    result.data[3][1] = m->data[3][1];
    result.data[3][2] = m->data[3][2];
    result.data[3][3] = m->data[3][3];
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// scale
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_scale_rowmajor(const fmat4* m, const float3* dim)
{
    fmat4 result = { 0 };

    result.data[0][0] = m->data[0][0] * dim->xyz.x;
    result.data[0][1] = m->data[0][1] * dim->xyz.x;
    result.data[0][2] = m->data[0][2] * dim->xyz.x;
    result.data[0][3] = m->data[0][3] * dim->xyz.x;

    result.data[1][0] = m->data[1][0] * dim->xyz.y;
    result.data[1][1] = m->data[1][1] * dim->xyz.y;
    result.data[1][2] = m->data[1][2] * dim->xyz.y;
    result.data[1][3] = m->data[1][3] * dim->xyz.y;

    result.data[2][0] = m->data[2][0] * dim->xyz.z;
    result.data[2][1] = m->data[2][1] * dim->xyz.z;
    result.data[2][2] = m->data[2][2] * dim->xyz.z;
    result.data[2][3] = m->data[2][3] * dim->xyz.z;

    result.data[3][0] = m->data[3][0];
    result.data[3][1] = m->data[3][1];
    result.data[3][2] = m->data[3][2];
    result.data[3][3] = m->data[3][3];
    return result;
}

VECMATH_API fmat4 fmat4_scale_colmajor(const fmat4* m, const float3* dim)
{
    fmat4 result = *m;

    // column 0 (X axis)
    result.matrix.m00 *= dim->xyz.x;
    result.matrix.m10 *= dim->xyz.x;
    result.matrix.m20 *= dim->xyz.x;
    result.matrix.m30 *= dim->xyz.x;

    // column 1 (Y axis)
    result.matrix.m01 *= dim->xyz.y;
    result.matrix.m11 *= dim->xyz.y;
    result.matrix.m21 *= dim->xyz.y;
    result.matrix.m31 *= dim->xyz.y;

    // column 2 (Z axis)
    result.matrix.m02 *= dim->xyz.z;
    result.matrix.m12 *= dim->xyz.z;
    result.matrix.m22 *= dim->xyz.z;
    result.matrix.m32 *= dim->xyz.z;

    return result;
}

VECMATH_API dmat4 dmat4_scale_rowmajor(const dmat4* m, const double3* dim)
{
    dmat4 result = { 0 };

    result.data[0][0] = m->data[0][0] * dim->xyz.x;
    result.data[0][1] = m->data[0][1] * dim->xyz.x;
    result.data[0][2] = m->data[0][2] * dim->xyz.x;
    result.data[0][3] = m->data[0][3] * dim->xyz.x;

    result.data[1][0] = m->data[1][0] * dim->xyz.y;
    result.data[1][1] = m->data[1][1] * dim->xyz.y;
    result.data[1][2] = m->data[1][2] * dim->xyz.y;
    result.data[1][3] = m->data[1][3] * dim->xyz.y;

    result.data[2][0] = m->data[2][0] * dim->xyz.z;
    result.data[2][1] = m->data[2][1] * dim->xyz.z;
    result.data[2][2] = m->data[2][2] * dim->xyz.z;
    result.data[2][3] = m->data[2][3] * dim->xyz.z;

    result.data[3][0] = m->data[3][0];
    result.data[3][1] = m->data[3][1];
    result.data[3][2] = m->data[3][2];
    result.data[3][3] = m->data[3][3];
    return result;
}

VECMATH_API dmat4 dmat4_scale_colmajor(const dmat4* m, const double3* dim)
{
    dmat4 result = { 0 };

    result.data[0][0] = m->data[0][0] * dim->xyz.x;
    result.data[1][0] = m->data[1][0] * dim->xyz.x;
    result.data[2][0] = m->data[2][0] * dim->xyz.x;
    result.data[3][0] = m->data[3][0] * dim->xyz.x;

    result.data[0][1] = m->data[0][1] * dim->xyz.y;
    result.data[1][1] = m->data[1][1] * dim->xyz.y;
    result.data[2][1] = m->data[2][1] * dim->xyz.y;
    result.data[3][1] = m->data[3][1] * dim->xyz.y;

    result.data[0][2] = m->data[0][2] * dim->xyz.z;
    result.data[1][2] = m->data[1][2] * dim->xyz.z;
    result.data[2][2] = m->data[2][2] * dim->xyz.z;
    result.data[3][2] = m->data[3][2] * dim->xyz.z;

    result.data[0][3] = m->data[0][3];
    result.data[1][3] = m->data[1][3];
    result.data[2][3] = m->data[2][3];
    result.data[3][3] = m->data[3][3];

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// lookat
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_lookat_agnostic(const float3 *eye, const float3 *target, const float3 *up)
{
    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross = float3_cross(&f, up);
    float3 s = float3_normalize(&cross);
    float3 u = float3_cross(&s, &f);
    fmat4 result = fmat4_identity();
    
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = u.xyz.x;
    result.data[0][2] = -f.xyz.x;
    
    result.data[1][0] = s.xyz.y;
    result.data[1][1] = u.xyz.y;
    result.data[1][2] = -f.xyz.y;
    
    result.data[2][0] = s.xyz.z;
    result.data[2][1] = u.xyz.z;
    result.data[2][2] = -f.xyz.z;
    
    result.data[0][3] = -float3_dot(&s, eye);
    result.data[1][3] = -float3_dot(&u, eye);
    result.data[2][3] = float3_dot(&f, eye);
    
    return result;
}

VECMATH_API fmat4 fmat4_lookat_vulkan(const float3* eye, const float3* target, const float3* up)
{
    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross = float3_cross(up, &f);
    float3 r = float3_normalize(&cross);
    float3 u = float3_cross(&f, &r);
    
    fmat4 result = { 0 };
    
    result.matrix.m00 = r.xyz.x;
    result.matrix.m10 = r.xyz.y;
    result.matrix.m20 = r.xyz.z;
    result.matrix.m30 = -float3_dot(&r, eye);
    
    result.matrix.m01 = -u.xyz.x;  // Y flip for Vulkan
    result.matrix.m11 = -u.xyz.y;  // Y flip for Vulkan
    result.matrix.m21 = -u.xyz.z;  // Y flip for Vulkan
    result.matrix.m31 = float3_dot(&u, eye);
    
    result.matrix.m02 = -f.xyz.x;  // Right-handed: negative Z forward
    result.matrix.m12 = -f.xyz.y;  // Right-handed: negative Z forward
    result.matrix.m22 = -f.xyz.z;  // Right-handed: negative Z forward
    result.matrix.m32 = float3_dot(&f, eye);
    
    result.matrix.m03 = 0.0f;
    result.matrix.m13 = 0.0f;
    result.matrix.m23 = 0.0f;
    result.matrix.m33 = 1.0f;
    
    return result;
}

VECMATH_API fmat4 fmat4_lookat_directx(const float3* eye, const float3* target, const float3* up)
{
    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross0 = float3_cross(up, &f);
    float3 s = float3_normalize(&cross0);  // reverse cross for left-handed
    float3 cross1 = float3_cross(&f, &s);
    float3 u = float3_normalize(&cross1);  // reverse cross for left-handed
    
    fmat4 result = { 0 };
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = u.xyz.x;
    result.data[0][2] = f.xyz.x;
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.xyz.y;
    result.data[1][1] = u.xyz.y;
    result.data[1][2] = f.xyz.y;
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.xyz.z;
    result.data[2][1] = u.xyz.z;
    result.data[2][2] = f.xyz.z;
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -float3_dot(&s, eye);
    result.data[3][1] = -float3_dot(&u, eye);
    result.data[3][2] = -float3_dot(&f, eye);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API fmat4 fmat4_lookat_opengl(const float3 *eye, const float3 *target, const float3 *up)
{
    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross0 = float3_cross(&f, up);
    float3 s = float3_normalize(&cross0);
    float3 cross1 = float3_cross(&s, &f);
    float3 u = float3_normalize(&cross1);
    
    fmat4 result = { 0 };
    
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = u.xyz.x;
    result.data[0][2] = -f.xyz.x;  // negative for right-handed view space
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.xyz.y;
    result.data[1][1] = u.xyz.y;
    result.data[1][2] = -f.xyz.y;  // negative for right-handed view space
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.xyz.z;
    result.data[2][1] = u.xyz.z;
    result.data[2][2] = -f.xyz.z;  // negative for right-handed view space
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -float3_dot(&s, eye);
    result.data[3][1] = -float3_dot(&u, eye);
    result.data[3][2] = float3_dot(&f, eye);  // positive for right-handed
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API dmat4 dmat4_lookat_agnostic(const double3 *eye, const double3 *target, const double3 *up)
{
    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross = double3_cross(&f, up);
    double3 s = double3_normalize(&cross);
    double3 u = double3_cross(&s, &f);
    dmat4 result = dmat4_identity();
    
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = u.xyz.x;
    result.data[0][2] = -f.xyz.x;
    
    result.data[1][0] = s.xyz.y;
    result.data[1][1] = u.xyz.y;
    result.data[1][2] = -f.xyz.y;
    
    result.data[2][0] = s.xyz.z;
    result.data[2][1] = u.xyz.z;
    result.data[2][2] = -f.xyz.z;
    
    result.data[0][3] = -double3_dot(&s, eye);
    result.data[1][3] = -double3_dot(&u, eye);
    result.data[2][3] = double3_dot(&f, eye);
    
    return result;
}

VECMATH_API dmat4 dmat4_lookat_vulkan(const double3* eye, const double3* target, const double3* up)
{
    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross0 = double3_cross(&f, up);
    double3 s = double3_normalize(&cross0);
    double3 cross1 = double3_cross(&s, &f);
    double3 u = double3_normalize(&cross1);
    
    dmat4 result = { 0 };
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = -s.xyz.y;  // flip Y for Vulkan
    result.data[0][2] = s.xyz.z;
    result.data[0][3] = -double3_dot(&s, eye);
    
    result.data[1][0] = u.xyz.x;
    result.data[1][1] = -u.xyz.y;  // flip Y for Vulkan
    result.data[1][2] = u.xyz.z;
    result.data[1][3] = -double3_dot(&u, eye);
    
    result.data[2][0] = -f.xyz.x;  // negative Z for right-handed view space
    result.data[2][1] = f.xyz.y;   // Y already flipped above
    result.data[2][2] = -f.xyz.z;  // negative Z for right-handed view space
    result.data[2][3] = double3_dot(&f, eye);  // positive for right-handed
    
    result.data[3][0] = 0.0f;
    result.data[3][1] = 0.0f;
    result.data[3][2] = 0.0f;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API dmat4 dmat4_lookat_directx(const double3* eye, const double3* target, const double3* up)
{
    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross0 = double3_cross(up, &f);
    double3 s = double3_normalize(&cross0);  // reverse cross for left-handed
    double3 cross1 = double3_cross(&f, &s);
    double3 u = double3_normalize(&cross1);  // reverse cross for left-handed
    
    dmat4 result = { 0 };
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = u.xyz.x;
    result.data[0][2] = f.xyz.x;
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.xyz.y;
    result.data[1][1] = u.xyz.y;
    result.data[1][2] = f.xyz.y;
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.xyz.z;
    result.data[2][1] = u.xyz.z;
    result.data[2][2] = f.xyz.z;
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -double3_dot(&s, eye);
    result.data[3][1] = -double3_dot(&u, eye);
    result.data[3][2] = -double3_dot(&f, eye);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API dmat4 dmat4_lookat_opengl(const double3* eye, const double3* target, const double3* up)
{
    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross0 = double3_cross(&f, up);
    double3 s = double3_normalize(&cross0);
    double3 cross1 = double3_cross(&s, &f);
    double3 u = double3_normalize(&cross1);
    
    dmat4 result = { 0 };
    result.data[0][0] = s.xyz.x;
    result.data[0][1] = u.xyz.x;
    result.data[0][2] = -f.xyz.x;  // negative for right-handed view space
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.xyz.y;
    result.data[1][1] = u.xyz.y;
    result.data[1][2] = -f.xyz.y;  // negative for right-handed view space
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.xyz.z;
    result.data[2][1] = u.xyz.z;
    result.data[2][2] = -f.xyz.z;  // negative for right-handed view space
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -double3_dot(&s, eye);
    result.data[3][1] = -double3_dot(&u, eye);
    result.data[3][2] = double3_dot(&f, eye);  // positive for right-handed
    result.data[3][3] = 1.0f;
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// perspective
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_perspective_vulkan(float fov_rad, float aspect, float nearVal, float farVal)
{
    float tan_half_fov = tanf(fov_rad * 0.5f);
    float f = 1.0f / tan_half_fov;
    float range_inv = 1.0f / (nearVal - farVal);
    
    fmat4 result = { 0 };
    
    // column-major order for Vulkan
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = farVal * range_inv;
    result.data[2][3] = -1.0f;
    result.data[3][2] = farVal * nearVal * range_inv;
    
    return result;
}

VECMATH_API fmat4 fmat4_perspective_directx(float fov_rad, float aspect, float nearVal, float farVal)
{
    float tan_half_fov = tanf(fov_rad * 0.5f);
    float f = 1.0f / tan_half_fov;
    
    fmat4 result = { 0 };
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = farVal / (farVal - nearVal);  // Z [0, 1] mapping
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(farVal * nearVal) / (farVal - nearVal);
    
    return result;
}

VECMATH_API fmat4 fmat4_perspective_opengl(float fov_rad, float aspect, float nearVal, float farVal)
{
    float tan_half_fov = tanf(fov_rad * 0.5f);
    float f = 1.0f / tan_half_fov;
    
    fmat4 result = { 0 };
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = (farVal + nearVal) / (farVal - nearVal);  // Z [-1, 1] mapping
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    
    return result;
}

VECMATH_API dmat4 dmat4_perspective_vulkan(double fov_rad, double aspect, double nearVal, double farVal)
{
    double tan_half_fov = tan(fov_rad * 0.5f);
    double f = 1.0f / tan_half_fov;
    
    dmat4 result = { 0 };
    result.data[0][0] = f / aspect;
    result.data[1][1] = -f;  // flip Y for Vulkan's Y-down
    result.data[2][2] = farVal / (farVal - nearVal);  // Z [0, 1] mapping
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(farVal * nearVal) / (farVal - nearVal);
    
    return result;
}

VECMATH_API dmat4 dmat4_perspective_directx(double fov_rad, double aspect, double nearVal, double farVal)
{
    double tan_half_fov = tan(fov_rad * 0.5f);
    double f = 1.0f / tan_half_fov;
    
    dmat4 result = { 0 };
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = farVal / (farVal - nearVal);  // Z [0, 1] mapping
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(farVal * nearVal) / (farVal - nearVal);
    
    return result;
}

VECMATH_API dmat4 dmat4_perspective_opengl(double fov_rad, double aspect, double nearVal, double farVal)
{
    double tan_half_fov = tan(fov_rad * 0.5f);
    double f = 1.0f / tan_half_fov;
    
    dmat4 result = { 0 };
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = (farVal + nearVal) / (farVal - nearVal);  // Z [-1, 1] mapping
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// orthographic
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_orthographic_vulkan(float left, float right, float bottom, float top, float nearVal, float farVal)
{
    float rl = right - left;
    float tb = top - bottom;
    float fn = farVal - nearVal;
    
    fmat4 result = { 0 };
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = -2.0f / tb;  // flip Y for Vulkan
    result.data[2][2] = 1.0f / fn;   // Z [0, 1]
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = (top + bottom) / tb;  // positive for Y-down
    result.data[3][2] = -nearVal / fn;  // map near to 0, far to 1
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API fmat4 fmat4_orthographic_directx(float left, float right, float bottom, float top, float nearVal, float farVal)
{
    float rl = right - left;
    float tb = top - bottom;
    float fn = farVal - nearVal;
    
    fmat4 result = { 0 };
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = 2.0f / tb;
    result.data[2][2] = 1.0f / fn;  // Z [0, 1]
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -nearVal / fn;  // map near to 0, far to 1
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API fmat4 fmat4_orthographic_opengl(float left, float right, float bottom, float top, float nearVal, float farVal)
{
    float rl = right - left;
    float tb = top - bottom;
    float fn = farVal - nearVal;
    
    fmat4 result = { 0 };
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = 2.0f / tb;
    result.data[2][2] = -2.0f / fn;  // Z [-1, 1] (negative for right-handed)
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -(farVal + nearVal) / fn;  // map near to -1, far to 1
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API dmat4 dmat4_orthographic_vulkan(double left, double right, double bottom, double top, double nearVal, double farVal)
{
    double rl = right - left;
    double tb = top - bottom;
    double fn = farVal - nearVal;
    
    dmat4 result = { 0 };
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = -2.0f / tb;  // flip Y for Vulkan
    result.data[2][2] = 1.0f / fn;   // Z [0, 1]
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = (top + bottom) / tb;  // positive for Y-down
    result.data[3][2] = -nearVal / fn;  // map near to 0, far to 1
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API dmat4 dmat4_orthographic_directx(double left, double right, double bottom, double top, double nearVal, double farVal)
{
    double rl = right - left;
    double tb = top - bottom;
    double fn = farVal - nearVal;
    
    dmat4 result = { 0 };
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = 2.0f / tb;
    result.data[2][2] = 1.0f / fn;  // Z [0, 1]
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -nearVal / fn;  // map near to 0, far to 1
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_API dmat4 dmat4_orthographic_opengl(double left, double right, double bottom, double top, double nearVal, double farVal)
{
    double rl = right - left;
    double tb = top - bottom;
    double fn = farVal - nearVal;
    
    dmat4 result = { 0 };
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = 2.0f / tb;
    result.data[2][2] = -2.0f / fn;  // Z [-1, 1] (negative for right-handed)
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -(farVal + nearVal) / fn;  // map near to -1, far to 1
    result.data[3][3] = 1.0f;
    
    return result;
}
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// identity
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_identity()
{
    fquat result = { 0 };
    result.vector.x = 0;
    result.vector.y = 0;
    result.vector.z = 0;
    result.vector.w = 1;
    return result;
}

VECMATH_API dquat dquat_identity()
{
    dquat result = { 0 };
    result.vector.x = 0;
    result.vector.y = 0;
    result.vector.z = 0;
    result.vector.w = 1;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// mul
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_mul(const fquat* q1, const fquat* q2)
{
    fquat result = { 0 };
    result.vector.x = q1->vector.w * q2->vector.x + q1->vector.x * q2->vector.w + q1->vector.y * q2->vector.z - q1->vector.z * q2->vector.y;
    result.vector.y = q1->vector.w * q2->vector.y - q1->vector.x * q2->vector.z + q1->vector.y * q2->vector.w + q1->vector.z * q2->vector.x;
    result.vector.z = q1->vector.w * q2->vector.z + q1->vector.x * q2->vector.y - q1->vector.y * q2->vector.x + q1->vector.z * q2->vector.w;
    result.vector.w = q1->vector.w * q2->vector.w - q1->vector.x * q2->vector.x - q1->vector.y * q2->vector.y - q1->vector.z * q2->vector.z;
    return result;
}

VECMATH_API dquat dquat_mul(const dquat* q1, const dquat* q2)
{
    dquat result = { 0 };
    result.vector.x = q1->vector.w * q2->vector.x + q1->vector.x * q2->vector.w + q1->vector.y * q2->vector.z - q1->vector.z * q2->vector.y;
    result.vector.y = q1->vector.w * q2->vector.y - q1->vector.x * q2->vector.z + q1->vector.y * q2->vector.w + q1->vector.z * q2->vector.x;
    result.vector.z = q1->vector.w * q2->vector.z + q1->vector.x * q2->vector.y - q1->vector.y * q2->vector.x + q1->vector.z * q2->vector.w;
    result.vector.w = q1->vector.w * q2->vector.w - q1->vector.x * q2->vector.x - q1->vector.y * q2->vector.y - q1->vector.z * q2->vector.z;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// length
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float fquat_length(const fquat* q)
{
    return sqrtf(q->vector.x * q->vector.x + q->vector.y * q->vector.y + q->vector.z * q->vector.z + q->vector.w * q->vector.w);
}

VECMATH_API double dquat_length(const dquat* q)
{
    return sqrt(q->vector.x * q->vector.x + q->vector.y * q->vector.y + q->vector.z * q->vector.z + q->vector.w * q->vector.w);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// conjugate
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_conjugate(const fquat* q)
{
    fquat result = { 0 };
    result.vector.x = -q->vector.x;
    result.vector.y = -q->vector.y;
    result.vector.z = -q->vector.z;
    result.vector.w = -q->vector.w;
    return result;
}

VECMATH_API dquat dquat_conjugate(const dquat* q)
{
    dquat result = { 0 };
    result.vector.x = -q->vector.x;
    result.vector.y = -q->vector.y;
    result.vector.z = -q->vector.z;
    result.vector.w = -q->vector.w;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// normalize
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_normalize(const fquat *q)
{
    float len = sqrtf(q->vector.x * q->vector.x + q->vector.y * q->vector.y + q->vector.z * q->vector.z + q->vector.w * q->vector.w);
    if (len > VECMATH_EPSILON_FZERO) {
        fquat result = { 0 };
        result.vector.x = q->vector.x / len;
        result.vector.y = q->vector.y / len;
        result.vector.z = q->vector.z / len;
        result.vector.w = q->vector.w / len;
        return result;
    }
    return fquat_identity();
}

VECMATH_API dquat dquat_normalize(const dquat* q)
{
    double len = sqrt(q->vector.x * q->vector.x + q->vector.y * q->vector.y + q->vector.z * q->vector.z + q->vector.w * q->vector.w);
    if (len > VECMATH_EPSILON_DZERO) {
        dquat result = { 0 };
        result.vector.x = q->vector.x / len;
        result.vector.y = q->vector.y / len;
        result.vector.z = q->vector.z / len;
        result.vector.w = q->vector.w / len;
        return result;
    }
    return dquat_identity();
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// dot
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float fquat_dot(const fquat* q1, const fquat* q2)
{
    return q1->vector.x * q2->vector.x + q1->vector.y * q2->vector.y + q1->vector.z * q2->vector.z + q1->vector.w * q2->vector.w;
}

VECMATH_API double dquat_dot(const dquat* q1, const dquat* q2)
{
    return q1->vector.x * q2->vector.x + q1->vector.y * q2->vector.y + q1->vector.z * q2->vector.z + q1->vector.w * q2->vector.w;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// lerp
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_lerp(const fquat *q1, const fquat *q2, float t)
{
    fquat result = { 0 };
    result.vector.x = q1->vector.x + t * (q2->vector.x - q1->vector.x);
    result.vector.y = q1->vector.y + t * (q2->vector.y - q1->vector.y);
    result.vector.z = q1->vector.z + t * (q2->vector.z - q1->vector.z);
    result.vector.w = q1->vector.w + t * (q2->vector.w - q1->vector.w);
    return result;
}

VECMATH_API dquat dquat_lerp(const dquat *q1, const dquat *q2, double t)
{
    dquat result = { 0 };
    result.vector.x = q1->vector.x + t * (q2->vector.x - q1->vector.x);
    result.vector.y = q1->vector.y + t * (q2->vector.y - q1->vector.y);
    result.vector.z = q1->vector.z + t * (q2->vector.z - q1->vector.z);
    result.vector.w = q1->vector.w + t * (q2->vector.w - q1->vector.w);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// slerp
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_slerp(const fquat* q1, const fquat* q2, float t)
{
    float cos_theta = fquat_dot(q1, q2);
    
    if (fabsf(cos_theta) > 0.9999f) {
        return *q1;
    }
    
    fquat q2_adj = *q2;
    if (cos_theta < 0.0f) {
        q2_adj.vector.x = -q2->vector.x;
        q2_adj.vector.y = -q2->vector.y;
        q2_adj.vector.z = -q2->vector.z;
        q2_adj.vector.w = -q2->vector.w;
        cos_theta = -cos_theta;
    }
    
    float theta = acosf(cos_theta);
    float sin_theta = sqrtf(1.0f - cos_theta*cos_theta);
    
    if (fabsf(sin_theta) < VECMATH_EPSILON_FZERO) {
        return *q1;
    }
    
    float wa = sinf((1.0f - t) * theta) / sin_theta;
    float wb = sinf(t * theta) / sin_theta;
    
    fquat result = { 0 };
    result.vector.x = wa * q1->vector.x + wb * q2_adj.vector.x;
    result.vector.y = wa * q1->vector.y + wb * q2_adj.vector.y;
    result.vector.z = wa * q1->vector.z + wb * q2_adj.vector.z;
    result.vector.w = wa * q1->vector.w + wb * q2_adj.vector.w;
    return result;
}

VECMATH_API dquat dquat_slerp(const dquat* q1, const dquat* q2, double t)
{
    double cos_theta = dquat_dot(q1, q2);
    
    if (fabs(cos_theta) > 0.9999) {
        return *q1;
    }
    
    dquat q2_adj = *q2;
    if (cos_theta < 0.0f) {
        q2_adj.vector.x = -q2->vector.x;
        q2_adj.vector.y = -q2->vector.y;
        q2_adj.vector.z = -q2->vector.z;
        q2_adj.vector.w = -q2->vector.w;
        cos_theta = -cos_theta;
    }
    
    double theta = acos(cos_theta);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    
    if (fabs(sin_theta) < VECMATH_EPSILON_DZERO) {
        return *q1;
    }
    
    double wa = sin((1.0 - t) * theta) / sin_theta;
    double wb = sin(t * theta) / sin_theta;
    
    dquat result = { 0 };
    result.vector.x = wa * q1->vector.x + wb * q2_adj.vector.x;
    result.vector.y = wa * q1->vector.y + wb * q2_adj.vector.y;
    result.vector.z = wa * q1->vector.z + wb * q2_adj.vector.z;
    result.vector.w = wa * q1->vector.w + wb * q2_adj.vector.w;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// from_euler
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_from_euler(const float3* rad)
{
    float cy = cosf(rad->xyz.y * 0.5f);
    float sy = sinf(rad->xyz.y * 0.5f);
    float cp = cosf(rad->xyz.x * 0.5f);
    float sp = sinf(rad->xyz.x * 0.5f);
    float cr = cosf(rad->xyz.z * 0.5f);
    float sr = sinf(rad->xyz.z * 0.5f);

    fquat q = { 0 };
    q.vector.w = cr * cp * cy + sr * sp * sy;
    q.vector.x = sr * cp * cy - cr * sp * sy;
    q.vector.y = cr * sp * cy + sr * cp * sy;
    q.vector.z = cr * cp * sy - sr * sp * cy;
    return q;
}

VECMATH_API dquat dquat_from_euler(const double3* d)
{
    double cy = cos(d->xyz.y * 0.5);
    double sy = sin(d->xyz.y * 0.5);
    double cp = cos(d->xyz.x * 0.5);
    double sp = sin(d->xyz.x * 0.5);
    double cr = cos(d->xyz.z * 0.5);
    double sr = sin(d->xyz.z * 0.5);

    dquat q = { 0 };
    q.vector.w = cr * cp * cy + sr * sp * sy;
    q.vector.x = sr * cp * cy - cr * sp * sy;
    q.vector.y = cr * sp * cy + sr * cp * sy;
    q.vector.z = cr * cp * sy - sr * sp * cy;
    return q;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// to_euler
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fquat_to_euler(const fquat* q)
{
    float3 angles = { 0 };
    fquat q_norm = fquat_normalize(q);
    float x = q_norm.vector.x, y = q_norm.vector.y, z = q_norm.vector.z, w = q_norm.vector.w;
    
    // roll (x-axis rotation)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    angles.xyz.x = atan2f(sinr_cosp, cosr_cosp);
    
    // pitch (y-axis rotation)
    float sinp = 2.0f * (w * y - z * x);
    if (fabsf(sinp) >= 1.0f) {
        angles.xyz.y = copysignf((float)VECMATH_EPSILON_PI / 2.0f, sinp); // use 90 degrees if out of range
    }

    else {
        angles.xyz.y = asinf(sinp);
    }
    
    // yaw (z-axis rotation)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    angles.xyz.z = atan2f(siny_cosp, cosy_cosp);
    return angles;
}

VECMATH_API double3 dquat_to_euler(const dquat *q)
{
    double3 angles = { 0 };
    dquat q_norm = dquat_normalize(q);
    double x = q_norm.vector.x, y = q_norm.vector.y, z = q_norm.vector.z, w = q_norm.vector.w;
    
    // roll (x-axis rotation)
    double sinr_cosp = 2.0 * (w * x + y * z);
    double cosr_cosp = 1.0 - 2.0 * (x * x + y * y);
    angles.xyz.x = atan2(sinr_cosp, cosr_cosp);
    
    // pitch (y-axis rotation)
    double sinp = 2.0 * (w * y - z * x);
    if (fabs(sinp) >= 1.0) {
        angles.xyz.y = copysign(VECMATH_EPSILON_PI / 2.0, sinp); // use 90 degrees if out of range
    }

    else {
        angles.xyz.y = asin(sinp);
    }
    
    // yaw (z-axis rotation)
    double siny_cosp = 2.0 * (w * z + x * y);
    double cosy_cosp = 1.0 - 2.0 * (y * y + z * z);
    angles.xyz.z = atan2(siny_cosp, cosy_cosp);
    return angles;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// to_matrix
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fquat_to_fmat4_rowmajor(const fquat *q)
{
    fquat q_norm = fquat_normalize(q);
    float x = q_norm.vector.x, y = q_norm.vector.y, z = q_norm.vector.z, w = q_norm.vector.w;
    
    fmat4 result = { 0 };
    result.matrix.m00 = 1.0f - 2.0f * y * y - 2.0f * z * z;
    result.matrix.m01 = 2.0f * x * y - 2.0f * w * z;
    result.matrix.m02 = 2.0f * x * z + 2.0f * w * y;
    
    result.matrix.m10 = 2.0f * x * y + 2.0f * w * z;
    result.matrix.m11 = 1.0f - 2.0f*x*x - 2.0f * z * z;
    result.matrix.m12 = 2.0f * y * z - 2.0f * w * x;
    
    result.matrix.m20 = 2.0f * x * z - 2.0f * w * y;
    result.matrix.m21 = 2.0f * y * z + 2.0f * w * x;
    result.matrix.m22 = 1.0f - 2.0f * x * x - 2.0f * y * y;
    
    result.matrix.m33 = 1.0f;
    return result;
}

VECMATH_API fmat4 fquat_to_fmat4_colmajor(const fquat *q)
{
    fquat q_norm = fquat_normalize(q);
    float x = q_norm.vector.x, y = q_norm.vector.y, z = q_norm.vector.z, w = q_norm.vector.w;

    fmat4 result = { 0 };
    result.matrix.m00 = 1.0f - 2.0f * y * y - 2.0f * z * z;
    result.matrix.m10 = 2.0f * x * y + 2.0f * w * z;
    result.matrix.m20 = 2.0f * x * z - 2.0f * w * y;

    result.matrix.m01 = 2.0f * x * y - 2.0f * w * z;
    result.matrix.m11 = 1.0f - 2.0f * x * x - 2.0f * z * z;
    result.matrix.m21 = 2.0f * y * z + 2.0f * w * x;

    result.matrix.m02 = 2.0f * x * z + 2.0f * w * y;
    result.matrix.m12 = 2.0f * y * z - 2.0f * w * x;
    result.matrix.m22 = 1.0f - 2.0f * x * x - 2.0f * y * y;

    result.matrix.m33 = 1.0f;
    return result;
}

VECMATH_API dmat4 dquat_to_dmat4_rowmajor(const dquat *q)
{
    dquat q_norm = dquat_normalize(q);
    double x = q_norm.vector.x, y = q_norm.vector.y, z = q_norm.vector.z, w = q_norm.vector.w;
    
    dmat4 result = { 0 };
    result.matrix.m00 = 1.0 - 2.0 * y * y - 2.0 * z * z;
    result.matrix.m01 = 2.0 * x * y - 2.0 * w * z;
    result.matrix.m02 = 2.0 * x * z + 2.0 * w * y;

    result.matrix.m10 = 2.0 * x * y + 2.0 * w * z;
    result.matrix.m11 = 1.0 - 2.0 * x * x - 2.0 * z * z;
    result.matrix.m12 = 2.0 * y * z - 2.0 * w * x;

    result.matrix.m20 = 2.0 * x * z - 2.0 * w * y;
    result.matrix.m21 = 2.0 * y * z + 2.0 * w * x;
    result.matrix.m22 = 1.0 - 2.0 * x * x - 2.0 * y * y;

    result.matrix.m33 = 1.0;
    return result;
}

VECMATH_API dmat4 dquat_to_dmat4_colmajor(const dquat *q)
{
    dquat q_norm = dquat_normalize(q);
    double x = q_norm.vector.x, y = q_norm.vector.y, z = q_norm.vector.z, w = q_norm.vector.w;
    
    dmat4 result = { 0 };
    result.matrix.m00 = 1.0 - 2.0 * y * y - 2.0 * z * z;
    result.matrix.m10 = 2.0 * x * y + 2.0 * w * z;
    result.matrix.m20 = 2.0 * x * z - 2.0 * w * y;

    result.matrix.m01 = 2.0 * x * y - 2.0 * w * z;
    result.matrix.m11 = 1.0 - 2.0 * x * x - 2.0 * z * z;
    result.matrix.m21 = 2.0 * y * z + 2.0 * w * x;

    result.matrix.m02 = 2.0 * x * z + 2.0 * w * y;
    result.matrix.m12 = 2.0 * y * z - 2.0 * w * x;
    result.matrix.m22 = 1.0 - 2.0 * x * x - 2.0 * y * y;
    
    result.matrix.m33 = 1.0;
    return result;
}
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// angle utilities
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float to_fradians(float degrees)
{
    return (float)(degrees * (VECMATH_EPSILON_PI / 180.0f));
}

VECMATH_API float to_fdegrees(float radians)
{
    return radians * (180.0f / (float)VECMATH_EPSILON_PI);
}

VECMATH_API float f_cos(float degree)
{
    return cosf(degree);
}

VECMATH_API float f_sin(float degree)
{
    return sinf(degree);
}

VECMATH_API float f_tan(float degree)
{
    return tanf(degree);
}

VECMATH_API double to_dradians(double degrees)
{
    return (double)(degrees * (VECMATH_EPSILON_PI / 180.0));
}

VECMATH_API double to_ddegrees(double radians)
{
    return radians * (180.0f / (float)VECMATH_EPSILON_PI);
}

VECMATH_API double d_cos(double degree)
{
    return cos(degree);
}

VECMATH_API double d_sin(double degree)
{
    return sin(degree);
}

VECMATH_API double d_tan(double degree)
{
    return tan(degree);
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// clamp utilities
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float f_max(const float x, const float y)
{
    return (x < y) ? y : x;
}

VECMATH_API float f_min(const float x, const float y)
{
    return (y < x) ? y : x;
}

VECMATH_API float f_clamp(const float x, const float upper, const float lower)
{
    return f_min(upper, f_max(x, lower));
}

VECMATH_API double d_max(const double x, const double y)
{
    return (x < y) ? y : x;
}

VECMATH_API double d_min(const double x, const double y)
{
    return (y < x) ? y : x;
}

VECMATH_API double d_clamp(const double x, const double upper, const double lower)
{
    return d_min(upper, d_max(x, lower));
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// power
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float f_power(float b, int e)
{
    // base case: anything^0 = 1
    if (e == 0) return 1.0f;
    
    // handle negative exponents
    if (e < 0) {
        if (e == VECMATH_EPSILON_INT_MIN) { // avoid overflow when negating INT_MIN
            return 1.0f / (b * f_power(b, -(e + 1)));
        }
        return 1.0f / f_power(b, -e);
    }
    
    // recursive case: use exponentiation by squaring
    float temp = f_power(b, e / 2);
    return (e % 2 == 0) ? temp * temp : b * temp * temp;
}

VECMATH_API double d_power(double b, int e)
{
    // base case: anything^0 = 1
    if (e == 0) return 1;
    
    // handle negative exponents
    if (e < 0) {
        if (e == VECMATH_EPSILON_INT_MIN) { // avoid overflow
            return 1 / (b * d_power(b, -(e + 1)));
        }
        return 1 / d_power(b, -e);
    }
    
    // recursive case: use exponentiation by squaring
    double temp = d_power(b, e / 2);
    return (e % 2 == 0) ? temp * temp : b * temp * temp;
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// other utility
/////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float f_log2(const float x)
{
    return log2f(x);
}

VECMATH_API double d_log2(const double x)
{
    return log2(x);
}

VECMATH_API int i_floor(const double x)
{
    if (x >= 0) return (int)x;
    
    int truncated = (int)x;
    return (x == truncated) ? truncated : truncated - 1;
}
VECMATH_API fray fray_from_screen_point_vulkan(const float2* screenPos, const float2* windowSize, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp)
{
    fray result;

    float ndcX = (2.0f * screenPos->xy.x) / windowSize->xy.x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos->xy.y) / windowSize->xy.y;

    float3 cameraRight = float3_cross(cameraUp, cameraFront);
    cameraRight = float3_normalize(&cameraRight);

    float3 up = float3_cross(cameraFront, &cameraRight);
    float3 normalizedUp = float3_normalize(&up);

    float halfHeight = tanf(fov * 0.5f);
    float halfWidth = halfHeight * aspectRatio;

    float3 direction = {
        cameraFront->xyz.x + cameraRight.xyz.x * (ndcX * halfWidth) + normalizedUp.xyz.x * (ndcY * halfHeight),
        cameraFront->xyz.y + cameraRight.xyz.y * (ndcX * halfWidth) + normalizedUp.xyz.y * (ndcY * halfHeight),
        cameraFront->xyz.z + cameraRight.xyz.z * (ndcX * halfWidth) + normalizedUp.xyz.z * (ndcY * halfHeight)
    };

    result.origin = *cameraPos;
    result.direction = float3_normalize(&direction);

    return result;
}

VECMATH_API float3 fray_screen_to_world_point_vulkan(const float2* screenPos, const float2* windowSize, float distance, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp)
{
    float ndcX = (2.0f * screenPos->xy.x) / windowSize->xy.x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos->xy.y) / windowSize->xy.y;

    float3 aux = {
        cameraUp->xyz.y * cameraFront->xyz.z - cameraUp->xyz.z * cameraFront->xyz.y,
        cameraUp->xyz.z * cameraFront->xyz.x - cameraUp->xyz.x * cameraFront->xyz.z,
        cameraUp->xyz.x * cameraFront->xyz.y - cameraUp->xyz.y * cameraFront->xyz.x
    };

    float3 cameraRight = float3_normalize(&aux);

    float3 up = {
        cameraFront->xyz.y * cameraRight.xyz.z - cameraFront->xyz.z * cameraRight.xyz.y,
        cameraFront->xyz.z * cameraRight.xyz.x - cameraFront->xyz.x * cameraRight.xyz.z,
        cameraFront->xyz.x * cameraRight.xyz.y - cameraFront->xyz.y * cameraRight.xyz.x
    };
    float3 normalizedUp = float3_normalize(&up);

    float halfHeight = distance * tanf(fov * 0.5f);
    float halfWidth = halfHeight * aspectRatio;

    float3 worldPoint = {
        cameraPos->xyz.x + cameraFront->xyz.x * distance + cameraRight.xyz.x * (ndcX * halfWidth) + normalizedUp.xyz.x * (ndcY * halfHeight),
        cameraPos->xyz.y + cameraFront->xyz.y * distance + cameraRight.xyz.y * (ndcX * halfWidth) + normalizedUp.xyz.y * (ndcY * halfHeight),
        cameraPos->xyz.z + cameraFront->xyz.z * distance + cameraRight.xyz.z * (ndcX * halfWidth) + normalizedUp.xyz.z * (ndcY * halfHeight)
    };

    return worldPoint;
}
