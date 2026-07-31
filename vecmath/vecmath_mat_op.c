#include "vecmath_mat_op.h"
#include "vecmath_basic_op.h"
#include "vecmath_vec_op.h"
#include <math.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// identity
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat2 fmat2_identity() {
    return (fmat2){ .data = {
        {1.0f, 0.0f},
        {0.0f, 1.0f}
    }};
}

VECMATH_FUNC fmat3 fmat3_identity() {
    return (fmat3){ .data = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    }};
}

VECMATH_FUNC fmat4 fmat4_identity() {
    return (fmat4){ .data = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
}

VECMATH_FUNC dmat2 dmat2_identity() {
    return (dmat2){ .data = {
        {1.0, 0.0},
        {0.0, 1.0}
    }};
}

VECMATH_FUNC dmat3 dmat3_identity() {
    return (dmat3){ .data = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};
}

VECMATH_FUNC dmat4 dmat4_identity() {
    return (dmat4){ .data = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    }};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat*vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 fmat2_mul_float2(const fmat2* m, const float2* v) {
    float2 result;
    result.x = m->data[0][0] * v->x + m->data[0][1] * v->y;
    result.y = m->data[1][0] * v->x + m->data[1][1] * v->y;
    return result;
}

VECMATH_FUNC float3 fmat3_mul_float3(const fmat3* m, const float3* v) {
    float3 result;
    result.x = m->data[0][0] * v->x + m->data[0][1] * v->y + m->data[0][2] * v->z;
    result.y = m->data[1][0] * v->x + m->data[1][1] * v->y + m->data[1][2] * v->z;
    result.z = m->data[2][0] * v->x + m->data[2][1] * v->y + m->data[2][2] * v->z;
    return result;
}

VECMATH_FUNC float4 fmat4_mul_float4(const fmat4* m, const float4* v) {
    float4 result;
    result.x = m->data[0][0] * v->x + m->data[0][1] * v->y + m->data[0][2] * v->z + m->data[0][3] * v->w;
    result.y = m->data[1][0] * v->x + m->data[1][1] * v->y + m->data[1][2] * v->z + m->data[1][3] * v->w;
    result.z = m->data[2][0] * v->x + m->data[2][1] * v->y + m->data[2][2] * v->z + m->data[2][3] * v->w;
    result.w = m->data[3][0] * v->x + m->data[3][1] * v->y + m->data[3][2] * v->z + m->data[3][3] * v->w;
    return result;
}

VECMATH_FUNC double2 dmat2_mul_double2(const dmat2* m, const double2* v) {
    double2 result;
    result.x = m->data[0][0] * v->x + m->data[0][1] * v->y;
    result.y = m->data[1][0] * v->x + m->data[1][1] * v->y;
    return result;
}

VECMATH_FUNC double3 dmat3_mul_double3(const dmat3* m, const double3* v) {
    double3 result;
    result.x = m->data[0][0] * v->x + m->data[0][1] * v->y + m->data[0][2] * v->z;
    result.y = m->data[1][0] * v->x + m->data[1][1] * v->y + m->data[1][2] * v->z;
    result.z = m->data[2][0] * v->x + m->data[2][1] * v->y + m->data[2][2] * v->z;
    return result;
}

VECMATH_FUNC double4 dmat4_mul_double4(const dmat4* m, const double4* v) {
    double4 result;
    result.x = m->data[0][0] * v->x + m->data[0][1] * v->y + m->data[0][2] * v->z + m->data[0][3] * v->w;
    result.y = m->data[1][0] * v->x + m->data[1][1] * v->y + m->data[1][2] * v->z + m->data[1][3] * v->w;
    result.z = m->data[2][0] * v->x + m->data[2][1] * v->y + m->data[2][2] * v->z + m->data[2][3] * v->w;
    result.w = m->data[3][0] * v->x + m->data[3][1] * v->y + m->data[3][2] * v->z + m->data[3][3] * v->w;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// changes colums with lines
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat2 fmat2_transpose(const fmat2* m) {
    fmat2 result;
    result.data[0][0] = m->data[0][0];
    result.data[0][1] = m->data[1][0];
    result.data[1][0] = m->data[0][1];
    result.data[1][1] = m->data[1][1];
    return result;
}

VECMATH_FUNC fmat3 fmat3_transpose(const fmat3* m) {
    fmat3 result;
    result.data[0][0] = m->data[0][0];
    result.data[0][1] = m->data[1][0];
    result.data[0][2] = m->data[2][0];
    result.data[1][0] = m->data[0][1];
    result.data[1][1] = m->data[1][1];
    result.data[1][2] = m->data[2][1];
    result.data[2][0] = m->data[0][2];
    result.data[2][1] = m->data[1][2];
    result.data[2][2] = m->data[2][2];
    return result;
}

VECMATH_FUNC fmat4 fmat4_transpose(const fmat4* m) {
    fmat4 result;
    result.data[0][0] = m->data[0][0];
    result.data[0][1] = m->data[1][0];
    result.data[0][2] = m->data[2][0];
    result.data[0][3] = m->data[3][0];
    result.data[1][0] = m->data[0][1];
    result.data[1][1] = m->data[1][1];
    result.data[1][2] = m->data[2][1];
    result.data[1][3] = m->data[3][1];
    result.data[2][0] = m->data[0][2];
    result.data[2][1] = m->data[1][2];
    result.data[2][2] = m->data[2][2];
    result.data[2][3] = m->data[3][2];
    result.data[3][0] = m->data[0][3];
    result.data[3][1] = m->data[1][3];
    result.data[3][2] = m->data[2][3];
    result.data[3][3] = m->data[3][3];
    return result;
}

VECMATH_FUNC dmat2 dmat2_transpose(const dmat2* m) {
    dmat2 result;
    result.data[0][0] = m->data[0][0];
    result.data[0][1] = m->data[1][0];
    result.data[1][0] = m->data[0][1];
    result.data[1][1] = m->data[1][1];
    return result;
}

VECMATH_FUNC dmat3 dmat3_transpose(const dmat3* m) {
    dmat3 result;
    result.data[0][0] = m->data[0][0];
    result.data[0][1] = m->data[1][0];
    result.data[0][2] = m->data[2][0];
    result.data[1][0] = m->data[0][1];
    result.data[1][1] = m->data[1][1];
    result.data[1][2] = m->data[2][1];
    result.data[2][0] = m->data[0][2];
    result.data[2][1] = m->data[1][2];
    result.data[2][2] = m->data[2][2];
    return result;
}

VECMATH_FUNC dmat4 dmat4_transpose(const dmat4* m) {
    dmat4 result;
    result.data[0][0] = m->data[0][0];
    result.data[0][1] = m->data[1][0];
    result.data[0][2] = m->data[2][0];
    result.data[0][3] = m->data[3][0];
    result.data[1][0] = m->data[0][1];
    result.data[1][1] = m->data[1][1];
    result.data[1][2] = m->data[2][1];
    result.data[1][3] = m->data[3][1];
    result.data[2][0] = m->data[0][2];
    result.data[2][1] = m->data[1][2];
    result.data[2][2] = m->data[2][2];
    result.data[2][3] = m->data[3][2];
    result.data[3][0] = m->data[0][3];
    result.data[3][1] = m->data[1][3];
    result.data[3][2] = m->data[2][3];
    result.data[3][3] = m->data[3][3];
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the determinant
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float fmat2_determinant(const fmat2* m)
{
    return (m->data[0][0] * m->data[1][1]) - (m->data[0][1] * m->data[1][0]);
}

VECMATH_FUNC float fmat3_determinant(const fmat3* m)
{
    return m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) - 
           m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) + 
           m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
}

VECMATH_FUNC float fmat4_determinant(const fmat4* m)
{
    if (!m) return 0.0f;

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
        fmat3 submat = {{{sub[0][0], sub[0][1], sub[0][2]}, {sub[1][0], sub[1][1], sub[1][2]}, {sub[2][0], sub[2][1], sub[2][2]}}};
        det += (x % 2 == 0 ? 1 : -1) * m->data[0][x] * fmat3_determinant(&submat);
    }
    return det;
}

VECMATH_FUNC double dmat2_determinant(const dmat2* m)
{
    return (m->data[0][0] * m->data[1][1]) - (m->data[0][1] * m->data[1][0]);
}

VECMATH_FUNC double dmat3_determinant(const dmat3* m)
{
    return m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) - 
           m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) + 
           m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
}

VECMATH_FUNC double dmat4_determinant(const dmat4* m)
{
    if (!m) return 0.0;

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
        dmat3 submat = {{{sub[0][0], sub[0][1], sub[0][2]}, {sub[1][0], sub[1][1], sub[1][2]}, {sub[2][0], sub[2][1], sub[2][2]}}};
        det += (x % 2 == 0 ? 1 : -1) * m->data[0][x] * dmat3_determinant(&submat);
    }
    return det;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the inverse matrix
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat2 fmat2_inverse(const fmat2* m) {
    fmat2 result;
    float det = m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    
    if (fabsf(det) < VECMATH_FLT_EPSILON) return fmat2_identity();
    
    float inv_det = 1.0f / det;
    result.data[0][0] =  m->data[1][1] * inv_det;
    result.data[0][1] = -m->data[0][1] * inv_det;
    result.data[1][0] = -m->data[1][0] * inv_det;
    result.data[1][1] =  m->data[0][0] * inv_det;
    return result;
}

VECMATH_FUNC fmat3 fmat3_inverse(const fmat3* m) {
    fmat3 result;
    float det;
    
    det = m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
          m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
          m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    
    if (fabsf(det) < VECMATH_FLT_EPSILON) return fmat3_identity();
    
    float inv_det = 1.0f / det;
    
    // calculate inverse using adjugate matrix
    result.data[0][0] = (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) * inv_det;
    result.data[0][1] = (m->data[0][2] * m->data[2][1] - m->data[0][1] * m->data[2][2]) * inv_det;
    result.data[0][2] = (m->data[0][1] * m->data[1][2] - m->data[0][2] * m->data[1][1]) * inv_det;
    result.data[1][0] = (m->data[1][2] * m->data[2][0] - m->data[1][0] * m->data[2][2]) * inv_det;
    result.data[1][1] = (m->data[0][0] * m->data[2][2] - m->data[0][2] * m->data[2][0]) * inv_det;
    result.data[1][2] = (m->data[0][2] * m->data[1][0] - m->data[0][0] * m->data[1][2]) * inv_det;
    result.data[2][0] = (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]) * inv_det;
    result.data[2][1] = (m->data[0][1] * m->data[2][0] - m->data[0][0] * m->data[2][1]) * inv_det;
    result.data[2][2] = (m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0]) * inv_det;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_inverse(const fmat4* m) {
    fmat4 result;
    float det;
    
    // calculate determinant using cofactor expansion along first row
    det = m->data[0][0] * (
              m->data[1][1] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
              m->data[1][2] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) +
              m->data[1][3] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1])
          ) -
          m->data[0][1] * (
              m->data[1][0] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
              m->data[1][2] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
              m->data[1][3] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0])
          ) +
          m->data[0][2] * (
              m->data[1][0] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) -
              m->data[1][1] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
              m->data[1][3] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
          ) -
          m->data[0][3] * (
              m->data[1][0] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1]) -
              m->data[1][1] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0]) +
              m->data[1][2] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
          );
    
    if (fabsf(det) < VECMATH_FLT_EPSILON) return fmat4_identity();
    
    float inv_det = 1.0f / det;
    
    result.data[0][0] = (
        m->data[1][1] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[1][2] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) +
        m->data[1][3] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1])
    ) * inv_det;
    
    result.data[0][1] = -(
        m->data[0][1] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) +
        m->data[0][3] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1])
    ) * inv_det;
    
    result.data[0][2] = (
        m->data[0][1] * (m->data[1][2] * m->data[3][3] - m->data[1][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[1][1] * m->data[3][3] - m->data[1][3] * m->data[3][1]) +
        m->data[0][3] * (m->data[1][1] * m->data[3][2] - m->data[1][2] * m->data[3][1])
    ) * inv_det;
    
    result.data[0][3] = -(
        m->data[0][1] * (m->data[1][2] * m->data[2][3] - m->data[1][3] * m->data[2][2]) -
        m->data[0][2] * (m->data[1][1] * m->data[2][3] - m->data[1][3] * m->data[2][1]) +
        m->data[0][3] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1])
    ) * inv_det;
    
    result.data[1][0] = -(
        m->data[1][0] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[1][2] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[1][3] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0])
    ) * inv_det;
    
    result.data[1][1] = (
        m->data[0][0] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0])
    ) * inv_det;
    
    result.data[1][2] = -(
        m->data[0][0] * (m->data[1][2] * m->data[3][3] - m->data[1][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[1][0] * m->data[3][3] - m->data[1][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[3][2] - m->data[1][2] * m->data[3][0])
    ) * inv_det;
    
    result.data[1][3] = (
        m->data[0][0] * (m->data[1][2] * m->data[2][3] - m->data[1][3] * m->data[2][2]) -
        m->data[0][2] * (m->data[1][0] * m->data[2][3] - m->data[1][3] * m->data[2][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0])
    ) * inv_det;
    
    result.data[2][0] = (
        m->data[1][0] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) -
        m->data[1][1] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[1][3] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[2][1] = -(
        m->data[0][0] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) -
        m->data[0][1] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[2][2] = (
        m->data[0][0] * (m->data[1][1] * m->data[3][3] - m->data[1][3] * m->data[3][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[3][3] - m->data[1][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[3][1] - m->data[1][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[2][3] = -(
        m->data[0][0] * (m->data[1][1] * m->data[2][3] - m->data[1][3] * m->data[2][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[2][3] - m->data[1][3] * m->data[2][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0])
    ) * inv_det;
    
    result.data[3][0] = -(
        m->data[1][0] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1]) -
        m->data[1][1] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0]) +
        m->data[1][2] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[3][1] = (
        m->data[0][0] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1]) -
        m->data[0][1] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0]) +
        m->data[0][2] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[3][2] = -(
        m->data[0][0] * (m->data[1][1] * m->data[3][2] - m->data[1][2] * m->data[3][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[3][2] - m->data[1][2] * m->data[3][0]) +
        m->data[0][2] * (m->data[1][0] * m->data[3][1] - m->data[1][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[3][3] = (
        m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
        m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0])
    ) * inv_det;
    
    return result;
}

VECMATH_FUNC dmat2 dmat2_inverse(const dmat2* m) {
    dmat2 result;
    double det = m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    
    if (fabs(det) < VECMATH_DBL_EPSILON) return dmat2_identity();
    
    double inv_det = 1.0 / det;
    result.data[0][0] =  m->data[1][1] * inv_det;
    result.data[0][1] = -m->data[0][1] * inv_det;
    result.data[1][0] = -m->data[1][0] * inv_det;
    result.data[1][1] =  m->data[0][0] * inv_det;
    return result;
}

VECMATH_FUNC dmat3 dmat3_inverse(const dmat3* m) {
    dmat3 result;
    double det;
    
    det = m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
          m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
          m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    
    if (fabs(det) < VECMATH_DBL_EPSILON) return dmat3_identity();
    
    double inv_det = 1.0 / det;
    
    result.data[0][0] = (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) * inv_det;
    result.data[0][1] = (m->data[0][2] * m->data[2][1] - m->data[0][1] * m->data[2][2]) * inv_det;
    result.data[0][2] = (m->data[0][1] * m->data[1][2] - m->data[0][2] * m->data[1][1]) * inv_det;
    result.data[1][0] = (m->data[1][2] * m->data[2][0] - m->data[1][0] * m->data[2][2]) * inv_det;
    result.data[1][1] = (m->data[0][0] * m->data[2][2] - m->data[0][2] * m->data[2][0]) * inv_det;
    result.data[1][2] = (m->data[0][2] * m->data[1][0] - m->data[0][0] * m->data[1][2]) * inv_det;
    result.data[2][0] = (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]) * inv_det;
    result.data[2][1] = (m->data[0][1] * m->data[2][0] - m->data[0][0] * m->data[2][1]) * inv_det;
    result.data[2][2] = (m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0]) * inv_det;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_inverse(const dmat4* m) {
    dmat4 result;
    double det;
    
    det = m->data[0][0] * (
              m->data[1][1] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
              m->data[1][2] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) +
              m->data[1][3] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1])
          ) -
          m->data[0][1] * (
              m->data[1][0] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
              m->data[1][2] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
              m->data[1][3] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0])
          ) +
          m->data[0][2] * (
              m->data[1][0] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) -
              m->data[1][1] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
              m->data[1][3] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
          ) -
          m->data[0][3] * (
              m->data[1][0] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1]) -
              m->data[1][1] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0]) +
              m->data[1][2] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
          );
    
    if (fabs(det) < VECMATH_DBL_EPSILON) return dmat4_identity();
    
    double inv_det = 1.0 / det;
    
    result.data[0][0] = (
        m->data[1][1] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[1][2] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) +
        m->data[1][3] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1])
    ) * inv_det;
    
    result.data[0][1] = -(
        m->data[0][1] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) +
        m->data[0][3] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1])
    ) * inv_det;
    
    result.data[0][2] = (
        m->data[0][1] * (m->data[1][2] * m->data[3][3] - m->data[1][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[1][1] * m->data[3][3] - m->data[1][3] * m->data[3][1]) +
        m->data[0][3] * (m->data[1][1] * m->data[3][2] - m->data[1][2] * m->data[3][1])
    ) * inv_det;
    
    result.data[0][3] = -(
        m->data[0][1] * (m->data[1][2] * m->data[2][3] - m->data[1][3] * m->data[2][2]) -
        m->data[0][2] * (m->data[1][1] * m->data[2][3] - m->data[1][3] * m->data[2][1]) +
        m->data[0][3] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1])
    ) * inv_det;
    
    result.data[1][0] = -(
        m->data[1][0] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[1][2] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[1][3] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0])
    ) * inv_det;
    
    result.data[1][1] = (
        m->data[0][0] * (m->data[2][2] * m->data[3][3] - m->data[2][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0])
    ) * inv_det;
    
    result.data[1][2] = -(
        m->data[0][0] * (m->data[1][2] * m->data[3][3] - m->data[1][3] * m->data[3][2]) -
        m->data[0][2] * (m->data[1][0] * m->data[3][3] - m->data[1][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[3][2] - m->data[1][2] * m->data[3][0])
    ) * inv_det;
    
    result.data[1][3] = (
        m->data[0][0] * (m->data[1][2] * m->data[2][3] - m->data[1][3] * m->data[2][2]) -
        m->data[0][2] * (m->data[1][0] * m->data[2][3] - m->data[1][3] * m->data[2][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0])
    ) * inv_det;
    
    result.data[2][0] = (
        m->data[1][0] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) -
        m->data[1][1] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[1][3] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[2][1] = -(
        m->data[0][0] * (m->data[2][1] * m->data[3][3] - m->data[2][3] * m->data[3][1]) -
        m->data[0][1] * (m->data[2][0] * m->data[3][3] - m->data[2][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[2][2] = (
        m->data[0][0] * (m->data[1][1] * m->data[3][3] - m->data[1][3] * m->data[3][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[3][3] - m->data[1][3] * m->data[3][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[3][1] - m->data[1][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[2][3] = -(
        m->data[0][0] * (m->data[1][1] * m->data[2][3] - m->data[1][3] * m->data[2][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[2][3] - m->data[1][3] * m->data[2][0]) +
        m->data[0][3] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0])
    ) * inv_det;
    
    result.data[3][0] = -(
        m->data[1][0] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1]) -
        m->data[1][1] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0]) +
        m->data[1][2] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[3][1] = (
        m->data[0][0] * (m->data[2][1] * m->data[3][2] - m->data[2][2] * m->data[3][1]) -
        m->data[0][1] * (m->data[2][0] * m->data[3][2] - m->data[2][2] * m->data[3][0]) +
        m->data[0][2] * (m->data[2][0] * m->data[3][1] - m->data[2][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[3][2] = -(
        m->data[0][0] * (m->data[1][1] * m->data[3][2] - m->data[1][2] * m->data[3][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[3][2] - m->data[1][2] * m->data[3][0]) +
        m->data[0][2] * (m->data[1][0] * m->data[3][1] - m->data[1][1] * m->data[3][0])
    ) * inv_det;
    
    result.data[3][3] = (
        m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
        m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
        m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0])
    ) * inv_det;
    
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// translation decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get translation from matrix
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float3 fmat4_get_translation_rowmajor(const fmat4* m) {
    float3 result = float3_zero();
    if (!m) return result;
    
    result.x = m->data[0][3];
    result.y = m->data[1][3];
    result.z = m->data[2][3];
    return result;
}

VECMATH_FUNC float3 fmat4_get_translation_colmajor(const fmat4* m) {
    float3 result = float3_zero();
    if (!m) return result;
    
    result.x = m->data[3][0];
    result.y = m->data[3][1];
    result.z = m->data[3][2];
    return result;
}

VECMATH_FUNC double3 dmat4_get_translation_rowmajor(const dmat4* m) {
    double3 result = double3_zero();
    if (!m) return result;
    
    result.x = m->data[0][3];
    result.y = m->data[1][3];
    result.z = m->data[2][3];
    return result;
}

VECMATH_FUNC double3 dmat4_get_translation_colmajor(const dmat4* m) {
    double3 result = double3_zero();
    if (!m) return result;
    
    result.x = m->data[3][0];
    result.y = m->data[3][1];
    result.z = m->data[3][2];
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat scale decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float3 fmat4_get_scale_rowmajor(const fmat4* m) {
    float3 scale = float3_zero();
    if (!m) return scale;

    scale.x = sqrtf(m->data[0][0] * m->data[0][0] + m->data[0][1] * m->data[0][1] + m->data[0][2] * m->data[0][2]);
    scale.y = sqrtf(m->data[1][0] * m->data[1][0] + m->data[1][1] * m->data[1][1] + m->data[1][2] * m->data[1][2]);
    scale.z = sqrtf(m->data[2][0] * m->data[2][0] + m->data[2][1] * m->data[2][1] + m->data[2][2] * m->data[2][2]);
    
    float det = m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
                m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
                m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    
    if (det < 0) scale.x = -scale.x;
    
    return scale;
}

VECMATH_FUNC float3 fmat4_get_scale_colmajor(const fmat4* m) {
    float3 scale = float3_zero();
    if (!m) return scale;

    scale.x = sqrtf(m->data[0][0] * m->data[0][0] + m->data[1][0] * m->data[1][0] + m->data[2][0] * m->data[2][0]);
    scale.y = sqrtf(m->data[0][1] * m->data[0][1] + m->data[1][1] * m->data[1][1] + m->data[2][1] * m->data[2][1]);
    scale.z = sqrtf(m->data[0][2] * m->data[0][2] + m->data[1][2] * m->data[1][2] + m->data[2][2] * m->data[2][2]);
    
    float det = m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
                m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
                m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    
    if (det < 0) scale.x = -scale.x;
    
    return scale;
}

VECMATH_FUNC double3 dmat4_get_scale_rowmajor(const dmat4* m) {
    double3 scale = double3_zero();
    if (!m) return scale;

    scale.x = sqrt(m->data[0][0] * m->data[0][0] + m->data[0][1] * m->data[0][1] + m->data[0][2] * m->data[0][2]);
    scale.y = sqrt(m->data[1][0] * m->data[1][0] + m->data[1][1] * m->data[1][1] + m->data[1][2] * m->data[1][2]);
    scale.z = sqrt(m->data[2][0] * m->data[2][0] + m->data[2][1] * m->data[2][1] + m->data[2][2] * m->data[2][2]);
    
    double det = m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
                 m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
                 m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    
    if (det < 0) scale.x = -scale.x;
    
    return scale;
}

VECMATH_FUNC double3 dmat4_get_scale_colmajor(const dmat4* m) {
    double3 scale = double3_zero();
    if (!m) return scale;

    scale.x = sqrt(m->data[0][0] * m->data[0][0] + m->data[1][0] * m->data[1][0] + m->data[2][0] * m->data[2][0]);
    scale.y = sqrt(m->data[0][1] * m->data[0][1] + m->data[1][1] * m->data[1][1] + m->data[2][1] * m->data[2][1]);
    scale.z = sqrt(m->data[0][2] * m->data[0][2] + m->data[1][2] * m->data[1][2] + m->data[2][2] * m->data[2][2]);
    
    double det = m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
                 m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
                 m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
    
    if (det < 0) scale.x = -scale.x;
    
    return scale;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat rotation decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float3 fmat4_get_rotation_rowmajor(const fmat4* m) {
    float3 rotation = float3_zero();
    if (!m) return rotation;

    float3 scale = fmat4_get_scale_rowmajor(m);
    
    if (scale.x < VECMATH_FLT_EPSILON || scale.y < VECMATH_FLT_EPSILON || scale.z < VECMATH_FLT_EPSILON) return rotation;
    
    // remove scale from rows (row-major)
    float m00 = m->data[0][0] / scale.x;
    float m01 = m->data[0][1] / scale.x;
    float m02 = m->data[0][2] / scale.x;
    
    float m10 = m->data[1][0] / scale.y;
    float m11 = m->data[1][1] / scale.y;
    float m12 = m->data[1][2] / scale.y;
    
    float m20 = m->data[2][0] / scale.z;
    float m21 = m->data[2][1] / scale.z;
    float m22 = m->data[2][2] / scale.z;
    
    // extract Euler angles from row-major rotation matrix
    rotation.y = atan2f(m02, sqrtf(m00 * m00 + m01 * m01));
    
    if (fabsf(rotation.y - VECMATH_FLT_HALF_PI) < VECMATH_FLT_EPSILON) {
        rotation.x = atan2f(m10, m11);
        rotation.z = 0.0f;
    }

    else if (fabsf(rotation.y + VECMATH_FLT_HALF_PI) < VECMATH_FLT_EPSILON) {
        rotation.x = -atan2f(m10, m11);
        rotation.z = 0.0f;
    }

    else {
        rotation.x = atan2f(m12, m22);
        rotation.z = atan2f(m01, m00);
    }
    
    return rotation;
}

VECMATH_FUNC float3 fmat4_get_rotation_colmajor(const fmat4* m) {
    float3 rotation = float3_zero();
    if (!m) return rotation;

    float3 scale = fmat4_get_scale_colmajor(m);
    
    if (scale.x < VECMATH_FLT_EPSILON || scale.y < VECMATH_FLT_EPSILON || scale.z < VECMATH_FLT_EPSILON) return rotation;
    
    // remove scale from columns (column-major)
    float m00 = m->data[0][0] / scale.x;
    float m10 = m->data[1][0] / scale.x;
    float m20 = m->data[2][0] / scale.x;
    
    float m01 = m->data[0][1] / scale.y;
    float m11 = m->data[1][1] / scale.y;
    float m21 = m->data[2][1] / scale.y;
    
    float m02 = m->data[0][2] / scale.z;
    float m12 = m->data[1][2] / scale.z;
    float m22 = m->data[2][2] / scale.z;
    
    // extract Euler angles from column-major rotation matrix
    rotation.y = atan2f(-m20, sqrtf(m00 * m00 + m10 * m10));
    
    if (fabsf(rotation.y - VECMATH_FLT_HALF_PI) < VECMATH_FLT_EPSILON) {
        rotation.x = atan2f(m01, m11);
        rotation.z = 0.0f;
    } 

    else if (fabsf(rotation.y + VECMATH_FLT_HALF_PI) < VECMATH_FLT_EPSILON) {
        rotation.x = -atan2f(m01, m11);
        rotation.z = 0.0f;
    } 

    else {
        rotation.x = atan2f(m21, m22);
        rotation.z = atan2f(m10, m00);
    }
    
    return rotation;
}

VECMATH_FUNC double3 dmat4_get_rotation_rowmajor(const dmat4* m) {
    double3 rotation = double3_zero();
    if (!m) return rotation;

    double3 scale = dmat4_get_scale_rowmajor(m);
    
    if (scale.x < VECMATH_DBL_EPSILON || scale.y < VECMATH_DBL_EPSILON || scale.z < VECMATH_DBL_EPSILON) return rotation;
    
    double m00 = m->data[0][0] / scale.x;
    double m01 = m->data[0][1] / scale.x;
    double m02 = m->data[0][2] / scale.x;
    
    double m10 = m->data[1][0] / scale.y;
    double m11 = m->data[1][1] / scale.y;
    double m12 = m->data[1][2] / scale.y;
    
    double m20 = m->data[2][0] / scale.z;
    double m21 = m->data[2][1] / scale.z;
    double m22 = m->data[2][2] / scale.z;
    
    rotation.y = atan2(m02, sqrt(m00 * m00 + m01 * m01));
    
    if (fabs(rotation.y - VECMATH_DBL_HALF_PI) < VECMATH_DBL_EPSILON) {
        rotation.x = atan2(m10, m11);
        rotation.z = 0.0;
    }

    else if (fabs(rotation.y + VECMATH_DBL_HALF_PI) < VECMATH_DBL_EPSILON) {
        rotation.x = -atan2(m10, m11);
        rotation.z = 0.0;
    }

    else {
        rotation.x = atan2(m12, m22);
        rotation.z = atan2(m01, m00);
    }
    
    return rotation;
}

VECMATH_FUNC double3 dmat4_get_rotation_colmajor(const dmat4* m) {
    double3 rotation = double3_zero();
    if (!m) return rotation;

    double3 scale = dmat4_get_scale_colmajor(m);
    
    if (scale.x < VECMATH_DBL_EPSILON || scale.y < VECMATH_DBL_EPSILON || scale.z < VECMATH_DBL_EPSILON) {
        return rotation;
    }
    
    double m00 = m->data[0][0] / scale.x;
    double m10 = m->data[1][0] / scale.x;
    double m20 = m->data[2][0] / scale.x;
    
    double m01 = m->data[0][1] / scale.y;
    double m11 = m->data[1][1] / scale.y;
    double m21 = m->data[2][1] / scale.y;
    
    double m02 = m->data[0][2] / scale.z;
    double m12 = m->data[1][2] / scale.z;
    double m22 = m->data[2][2] / scale.z;
    
    rotation.y = atan2(-m20, sqrt(m00 * m00 + m10 * m10));
    
    if (fabs(rotation.y - VECMATH_DBL_HALF_PI) < VECMATH_DBL_EPSILON) {
        rotation.x = atan2(m01, m11);
        rotation.z = 0.0;
    }

    else if (fabs(rotation.y + VECMATH_DBL_HALF_PI) < VECMATH_DBL_EPSILON) {
        rotation.x = -atan2(m01, m11);
        rotation.z = 0.0;
    }

    else {
        rotation.x = atan2(m21, m22);
        rotation.z = atan2(m10, m00);
    }
    
    return rotation;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat translation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fmat4_translate_rowmajor(const fmat4* m, const float3* dir) {
    if (!m) return fmat4_identity();
    fmat4 result = *m;

    result.data[0][3] += dir->x;
    result.data[1][3] += dir->y;
    result.data[2][3] += dir->z;
    return result;
}

VECMATH_FUNC fmat4 fmat4_translate_colmajor(const fmat4* m, const float3* dir) {
    if (!m) return fmat4_identity();
    fmat4 result = *m;

    result.data[0][3] = m->data[0][0] * dir->x + m->data[0][1] * dir->y + m->data[0][2] * dir->z + m->data[0][3];
    result.data[1][3] = m->data[1][0] * dir->x + m->data[1][1] * dir->y + m->data[1][2] * dir->z + m->data[1][3];
    result.data[2][3] = m->data[2][0] * dir->x + m->data[2][1] * dir->y + m->data[2][2] * dir->z + m->data[2][3];
    result.data[3][3] = m->data[3][0] * dir->x + m->data[3][1] * dir->y + m->data[3][2] * dir->z + m->data[3][3];

    return result;
}

VECMATH_FUNC dmat4 dmat4_translate_rowmajor(const dmat4* m, const double3* dir) {
    if (!m) return dmat4_identity();
    dmat4 result = *m;

    result.data[0][3] += dir->x;
    result.data[1][3] += dir->y;
    result.data[2][3] += dir->z;
    return result;
}

VECMATH_FUNC dmat4 dmat4_translate_colmajor(const dmat4* m, const double3* dir) {
    if (!m) return dmat4_identity();
    dmat4 result = *m;

    result.data[0][3] = m->data[0][0] * dir->x + m->data[0][1] * dir->y + m->data[0][2] * dir->z + m->data[0][3];
    result.data[1][3] = m->data[1][0] * dir->x + m->data[1][1] * dir->y + m->data[1][2] * dir->z + m->data[1][3];
    result.data[2][3] = m->data[2][0] * dir->x + m->data[2][1] * dir->y + m->data[2][2] * dir->z + m->data[2][3];
    result.data[3][3] = m->data[3][0] * dir->x + m->data[3][1] * dir->y + m->data[3][2] * dir->z + m->data[3][3];

    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat scale
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fmat4_scale_rowmajor(const fmat4* m, const float3* dim) {
    if (!m) return fmat4_identity();
    fmat4 result = *m;

    result.data[0][0] *= dim->x;
    result.data[0][1] *= dim->x;
    result.data[0][2] *= dim->x;
    result.data[0][3] *= dim->x;

    result.data[1][0] *= dim->y;
    result.data[1][1] *= dim->y;
    result.data[1][2] *= dim->y;
    result.data[1][3] *= dim->y;

    result.data[2][0] *= dim->z;
    result.data[2][1] *= dim->z;
    result.data[2][2] *= dim->z;
    result.data[2][3] *= dim->z;

    return result;
}

VECMATH_FUNC fmat4 fmat4_scale_colmajor(const fmat4* m, const float3* dim) {
    if (!m) return fmat4_identity();
    fmat4 result = *m;

    result.data[0][0] *= dim->x;
    result.data[1][0] *= dim->x;
    result.data[2][0] *= dim->x;
    result.data[3][0] *= dim->x;

    result.data[0][1] *= dim->y;
    result.data[1][1] *= dim->y;
    result.data[2][1] *= dim->y;
    result.data[3][1] *= dim->y;

    result.data[0][2] *= dim->z;
    result.data[1][2] *= dim->z;
    result.data[2][2] *= dim->z;
    result.data[3][2] *= dim->z;

    return result;
}

VECMATH_FUNC dmat4 dmat4_scale_rowmajor(const dmat4* m, const double3* dim) {
    if (!m) return dmat4_identity();
    dmat4 result = *m;

    result.data[0][0] *= dim->x;
    result.data[0][1] *= dim->x;
    result.data[0][2] *= dim->x;
    result.data[0][3] *= dim->x;

    result.data[1][0] *= dim->y;
    result.data[1][1] *= dim->y;
    result.data[1][2] *= dim->y;
    result.data[1][3] *= dim->y;

    result.data[2][0] *= dim->z;
    result.data[2][1] *= dim->z;
    result.data[2][2] *= dim->z;
    result.data[2][3] *= dim->z;

    return result;
}

VECMATH_FUNC dmat4 dmat4_scale_colmajor(const dmat4* m, const double3* dim) {
    if (!m) return dmat4_identity();
    dmat4 result = *m;

    result.data[0][0] *= dim->x;
    result.data[1][0] *= dim->x;
    result.data[2][0] *= dim->x;
    result.data[3][0] *= dim->x;

    result.data[0][1] *= dim->y;
    result.data[1][1] *= dim->y;
    result.data[2][1] *= dim->y;
    result.data[3][1] *= dim->y;

    result.data[0][2] *= dim->z;
    result.data[1][2] *= dim->z;
    result.data[2][2] *= dim->z;
    result.data[3][2] *= dim->z;

    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat rotation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fmat4_rotate_colmajor(const fmat4* m, float angle, const float3* axis) {
    if (!m || !axis) return fmat4_identity();

    float3 axis_n = float3_normalize(axis);
    float c = cosf(angle);
    float s = sinf(angle);
    float one_minus_c = 1.0f - c;
    fmat4 rotate = fmat4_zero();
    
    rotate.data[0][0] = c + axis_n.x * axis_n.x * one_minus_c;
    rotate.data[1][0] = axis_n.y * axis_n.x * one_minus_c + axis_n.z * s;
    rotate.data[2][0] = axis_n.z * axis_n.x * one_minus_c - axis_n.y * s;

    rotate.data[0][1] = axis_n.x * axis_n.y * one_minus_c - axis_n.z * s;
    rotate.data[1][1] = c + axis_n.y * axis_n.y * one_minus_c;
    rotate.data[2][1] = axis_n.z * axis_n.y * one_minus_c + axis_n.x * s;

    rotate.data[0][2] = axis_n.x * axis_n.z * one_minus_c + axis_n.y * s;
    rotate.data[1][2] = axis_n.y * axis_n.z * one_minus_c - axis_n.x * s;
    rotate.data[2][2] = c + axis_n.z * axis_n.z * one_minus_c;
    
    rotate.data[3][3] = 1.0f;
    
    return fmat4_mul(m, &rotate);
}

VECMATH_FUNC fmat4 fmat4_rotate_rowmajor(const fmat4* m, float angle, const float3* axis) {
    if (!m || !axis) return fmat4_identity();

    float3 axis_n = float3_normalize(axis);
    float c = cosf(angle);
    float s = sinf(angle);
    float one_minus_c = 1.0f - c;
    
    fmat4 rotate = fmat4_zero();
    
    rotate.data[0][0] = c + axis_n.x * axis_n.x * one_minus_c;
    rotate.data[0][1] = axis_n.x * axis_n.y * one_minus_c - axis_n.z * s;
    rotate.data[0][2] = axis_n.x * axis_n.z * one_minus_c + axis_n.y * s;
    rotate.data[0][3] = 0.0f;

    rotate.data[1][0] = axis_n.y * axis_n.x * one_minus_c + axis_n.z * s;
    rotate.data[1][1] = c + axis_n.y * axis_n.y * one_minus_c;
    rotate.data[1][2] = axis_n.y * axis_n.z * one_minus_c - axis_n.x * s;
    rotate.data[1][3] = 0.0f;

    rotate.data[2][0] = axis_n.z * axis_n.x * one_minus_c - axis_n.y * s;
    rotate.data[2][1] = axis_n.z * axis_n.y * one_minus_c + axis_n.x * s;
    rotate.data[2][2] = c + axis_n.z * axis_n.z * one_minus_c;
    rotate.data[2][3] = 0.0f;

    rotate.data[3][0] = 0.0f;
    rotate.data[3][1] = 0.0f;
    rotate.data[3][2] = 0.0f;
    rotate.data[3][3] = 1.0f;
    
    return fmat4_mul(&rotate, m);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// double matrix rotation (axis-angle) - fixed versions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC dmat4 dmat4_rotate_colmajor(const dmat4* m, double angle, const double3* axis) {
    if (!m || !axis) return dmat4_identity();

    double3 axis_n = double3_normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double one_minus_c = 1.0 - c;
    dmat4 rotate = dmat4_zero();
    
    rotate.data[0][0] = c + axis_n.x * axis_n.x * one_minus_c;
    rotate.data[1][0] = axis_n.y * axis_n.x * one_minus_c + axis_n.z * s;
    rotate.data[2][0] = axis_n.z * axis_n.x * one_minus_c - axis_n.y * s;

    rotate.data[0][1] = axis_n.x * axis_n.y * one_minus_c - axis_n.z * s;
    rotate.data[1][1] = c + axis_n.y * axis_n.y * one_minus_c;
    rotate.data[2][1] = axis_n.z * axis_n.y * one_minus_c + axis_n.x * s;

    rotate.data[0][2] = axis_n.x * axis_n.z * one_minus_c + axis_n.y * s;
    rotate.data[1][2] = axis_n.y * axis_n.z * one_minus_c - axis_n.x * s;
    rotate.data[2][2] = c + axis_n.z * axis_n.z * one_minus_c;
    
    rotate.data[3][3] = 1.0;
    
    return dmat4_mul(m, &rotate);
}

VECMATH_FUNC dmat4 dmat4_rotate_rowmajor(const dmat4* m, double angle, const double3* axis) {
    if (!m || !axis) return dmat4_identity();

    double3 axis_n = double3_normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double one_minus_c = 1.0 - c;
    
    dmat4 rotate = dmat4_zero();
    
    rotate.data[0][0] = c + axis_n.x * axis_n.x * one_minus_c;
    rotate.data[0][1] = axis_n.x * axis_n.y * one_minus_c - axis_n.z * s;
    rotate.data[0][2] = axis_n.x * axis_n.z * one_minus_c + axis_n.y * s;
    rotate.data[0][3] = 0.0;

    rotate.data[1][0] = axis_n.y * axis_n.x * one_minus_c + axis_n.z * s;
    rotate.data[1][1] = c + axis_n.y * axis_n.y * one_minus_c;
    rotate.data[1][2] = axis_n.y * axis_n.z * one_minus_c - axis_n.x * s;
    rotate.data[1][3] = 0.0;

    rotate.data[2][0] = axis_n.z * axis_n.x * one_minus_c - axis_n.y * s;
    rotate.data[2][1] = axis_n.z * axis_n.y * one_minus_c + axis_n.x * s;
    rotate.data[2][2] = c + axis_n.z * axis_n.z * one_minus_c;
    rotate.data[2][3] = 0.0;

    rotate.data[3][0] = 0.0;
    rotate.data[3][1] = 0.0;
    rotate.data[3][2] = 0.0;
    rotate.data[3][3] = 1.0;
    
    return dmat4_mul(&rotate, m);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC void fmat4_decompose_rowmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale) {
    if (translation)    *translation = fmat4_get_translation_rowmajor(m);
    if (scale)          *scale = fmat4_get_scale_rowmajor(m);
    if (rotation)       *rotation = fmat4_get_rotation_rowmajor(m);
}

VECMATH_FUNC void fmat4_decompose_colmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale) {
    if (translation)    *translation = fmat4_get_translation_colmajor(m);
    if (scale)          *scale = fmat4_get_scale_colmajor(m);
    if (rotation)       *rotation = fmat4_get_rotation_colmajor(m);
}

VECMATH_FUNC void dmat4_decompose_rowmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale) {
    if (translation)    *translation = dmat4_get_translation_rowmajor(m);
    if (scale)          *scale = dmat4_get_scale_rowmajor(m);
    if (rotation)       *rotation = dmat4_get_rotation_rowmajor(m);
}

VECMATH_FUNC void dmat4_decompose_colmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale) {
    if (translation)    *translation = dmat4_get_translation_colmajor(m);
    if (scale)          *scale = dmat4_get_scale_colmajor(m);
    if (rotation)       *rotation = dmat4_get_rotation_colmajor(m);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// look at projection
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fmat4_lookat_vulkan(const float3* eye, const float3* target, const float3* up) {
    if (!eye || !target || !up) return fmat4_identity();

    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross = float3_cross(up, &f);
    float3 r = float3_normalize(&cross);
    float3 u = float3_cross(&f, &r);
    
    fmat4 result = fmat4_zero();
    
    result.data[0][0] = r.x;
    result.data[1][0] = r.y;
    result.data[2][0] = r.z;
    result.data[3][0] = -float3_dot(&r, eye);
    
    result.data[0][1] = -u.x;  // Y flip for vulkan
    result.data[1][1] = -u.y;  // Y flip for vulkan
    result.data[2][1] = -u.z;  // Y flip for vulkan
    result.data[3][1] = float3_dot(&u, eye);
    
    result.data[0][2] = -f.x;  // right-handed: negative z forward
    result.data[1][2] = -f.y;  // right-handed: negative z forward
    result.data[2][2] = -f.z;  // right-handed: negative z forward
    result.data[3][2] = float3_dot(&f, eye);
    
    result.data[0][3] = 0.0f;
    result.data[1][3] = 0.0f;
    result.data[2][3] = 0.0f;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_lookat_directx(const float3* eye, const float3* target, const float3* up) {
    if (!eye || !target || !up) return fmat4_identity();

    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross0 = float3_cross(up, &f);
    float3 s = float3_normalize(&cross0);
    float3 cross1 = float3_cross(&f, &s);
    float3 u = float3_normalize(&cross1);
    
    fmat4 result = fmat4_zero();
    
    result.data[0][0] = s.x;
    result.data[0][1] = u.x;
    result.data[0][2] = f.x;
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.y;
    result.data[1][1] = u.y;
    result.data[1][2] = f.y;
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.z;
    result.data[2][1] = u.z;
    result.data[2][2] = f.z;
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -float3_dot(&s, eye);
    result.data[3][1] = -float3_dot(&u, eye);
    result.data[3][2] = -float3_dot(&f, eye);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_lookat_opengl(const float3* eye, const float3* target, const float3* up) {
    if (!eye || !target || !up) return fmat4_identity();

    float3 sub = float3_sub(target, eye);
    float3 f = float3_normalize(&sub);
    float3 cross0 = float3_cross(&f, up);
    float3 s = float3_normalize(&cross0);
    float3 cross1 = float3_cross(&s, &f);
    float3 u = float3_normalize(&cross1);
    
    fmat4 result = fmat4_zero();
    
    result.data[0][0] = s.x;
    result.data[0][1] = u.x;
    result.data[0][2] = -f.x;
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.y;
    result.data[1][1] = u.y;
    result.data[1][2] = -f.y;
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.z;
    result.data[2][1] = u.z;
    result.data[2][2] = -f.z;
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -float3_dot(&s, eye);
    result.data[3][1] = -float3_dot(&u, eye);
    result.data[3][2] = float3_dot(&f, eye);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_lookat_vulkan(const double3* eye, const double3* target, const double3* up) {
    if (!eye || !target || !up) return dmat4_identity();

    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross = double3_cross(up, &f);
    double3 r = double3_normalize(&cross);
    double3 u = double3_cross(&f, &r);
    
    dmat4 result = dmat4_zero();
    
    result.data[0][0] = r.x;
    result.data[1][0] = r.y;
    result.data[2][0] = r.z;
    result.data[3][0] = -double3_dot(&r, eye);
    
    result.data[0][1] = -u.x;
    result.data[1][1] = -u.y;
    result.data[2][1] = -u.z;
    result.data[3][1] = double3_dot(&u, eye);
    
    result.data[0][2] = -f.x; 
    result.data[1][2] = -f.y; 
    result.data[2][2] = -f.z; 
    result.data[3][2] = double3_dot(&f, eye);
    
    result.data[0][3] = 0.0f;
    result.data[1][3] = 0.0f;
    result.data[2][3] = 0.0f;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_lookat_directx(const double3* eye, const double3* target, const double3* up) {
    if (!eye || !target || !up) return dmat4_identity();

    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross0 = double3_cross(up, &f);
    double3 s = double3_normalize(&cross0);
    double3 cross1 = double3_cross(&f, &s);
    double3 u = double3_normalize(&cross1);
    
    dmat4 result = dmat4_zero();
    
    result.data[0][0] = s.x;
    result.data[0][1] = u.x;
    result.data[0][2] = f.x;
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.y;
    result.data[1][1] = u.y;
    result.data[1][2] = f.y;
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.z;
    result.data[2][1] = u.z;
    result.data[2][2] = f.z;
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -double3_dot(&s, eye);
    result.data[3][1] = -double3_dot(&u, eye);
    result.data[3][2] = -double3_dot(&f, eye);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_lookat_opengl(const double3* eye, const double3* target, const double3* up) {
    if (!eye || !target || !up) return dmat4_identity();

    double3 sub = double3_sub(target, eye);
    double3 f = double3_normalize(&sub);
    double3 cross0 = double3_cross(&f, up);
    double3 s = double3_normalize(&cross0);
    double3 cross1 = double3_cross(&s, &f);
    double3 u = double3_normalize(&cross1);
    
    dmat4 result = dmat4_zero();
    
    result.data[0][0] = s.x;
    result.data[0][1] = u.x;
    result.data[0][2] = -f.x;
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = s.y;
    result.data[1][1] = u.y;
    result.data[1][2] = -f.y;
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = s.z;
    result.data[2][1] = u.z;
    result.data[2][2] = -f.z;
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = -double3_dot(&s, eye);
    result.data[3][1] = -double3_dot(&u, eye);
    result.data[3][2] = double3_dot(&f, eye);
    result.data[3][3] = 1.0f;
    
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// perspective projection 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fmat4_perspective_vulkan(float fov_rad, float aspect, float nearVal, float farVal) {
    float tan_half_fov = tanf(fov_rad * 0.5f);
    float f = 1.0f / tan_half_fov;
    float range_inv = 1.0f / (nearVal - farVal);
    
    fmat4 result = fmat4_zero();
    
    // column-major
    result.data[0][0] = f / aspect;
    result.data[1][1] = -f;
    result.data[2][2] = farVal * range_inv;
    result.data[2][3] = -1.0f;
    result.data[3][2] = farVal * nearVal * range_inv;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_perspective_directx(float fov_rad, float aspect, float nearVal, float farVal) {
    float tan_half_fov = tanf(fov_rad * 0.5f);
    float f = 1.0f / tan_half_fov;
    
    fmat4 result = fmat4_zero();
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = farVal / (farVal - nearVal);
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(farVal * nearVal) / (farVal - nearVal);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_perspective_opengl(float fov_rad, float aspect, float nearVal, float farVal) {
    float tan_half_fov = tanf(fov_rad * 0.5f);
    float f = 1.0f / tan_half_fov;
    
    fmat4 result = fmat4_zero();
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = (farVal + nearVal) / (farVal - nearVal);
    result.data[2][3] = 1.0f;
    result.data[3][2] = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_perspective_vulkan(double fov_rad, double aspect, double nearVal, double farVal) {
    double tan_half_fov = tan(fov_rad * 0.5);
    double f = 1.0 / tan_half_fov;
    double range_inv = 1.0 / (nearVal - farVal);
    
    dmat4 result = dmat4_zero();
    result.data[0][0] = f / aspect;
    result.data[1][1] = -f;
    result.data[2][2] = farVal * range_inv;
    result.data[2][3] = -1.0;
    result.data[3][2] = farVal * nearVal * range_inv;
    result.data[3][3] = 1.0;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_perspective_directx(double fov_rad, double aspect, double nearVal, double farVal) {
    double tan_half_fov = tan(fov_rad * 0.5);
    double f = 1.0 / tan_half_fov;
    
    dmat4 result = dmat4_zero();
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = farVal / (farVal - nearVal);
    result.data[2][3] = 1.0;
    result.data[3][2] = -(farVal * nearVal) / (farVal - nearVal);
    result.data[3][3] = 1.0;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_perspective_opengl(double fov_rad, double aspect, double nearVal, double farVal) {
    double tan_half_fov = tan(fov_rad * 0.5);
    double f = 1.0 / tan_half_fov;
    
    dmat4 result = dmat4_zero();
    result.data[0][0] = f / aspect;
    result.data[1][1] = f;
    result.data[2][2] = (farVal + nearVal) / (farVal - nearVal);
    result.data[2][3] = 1.0;
    result.data[3][2] = -(2.0 * farVal * nearVal) / (farVal - nearVal);
    result.data[3][3] = 1.0;
    
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// orthographic projection 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fmat4_orthographic_vulkan(float left, float right, float bottom, float top, float nearVal, float farVal) {
    float rl = right - left;
    float tb = top - bottom;
    float fn = farVal - nearVal;
    
    fmat4 result = fmat4_zero();
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = -2.0f / tb; 
    result.data[2][2] = 1.0f / fn;
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -nearVal / fn;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_orthographic_directx(float left, float right, float bottom, float top, float nearVal, float farVal) {
    float rl = right - left;
    float tb = top - bottom;
    float fn = farVal - nearVal;
    
    fmat4 result = fmat4_zero();
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = 2.0f / tb;
    result.data[2][2] = 1.0f / fn;
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -nearVal / fn;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fmat4_orthographic_opengl(float left, float right, float bottom, float top, float nearVal, float farVal) {
    float rl = right - left;
    float tb = top - bottom;
    float fn = farVal - nearVal;
    
    fmat4 result = fmat4_zero();
    result.data[0][0] = 2.0f / rl;
    result.data[1][1] = 2.0f / tb;
    result.data[2][2] = -2.0f / fn;
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -(farVal + nearVal) / fn; 
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_orthographic_vulkan(double left, double right, double bottom, double top, double nearVal, double farVal) {
    double rl = right - left;
    double tb = top - bottom;
    double fn = farVal - nearVal;
    
    dmat4 result = dmat4_zero();
    result.data[0][0] = 2.0 / rl;
    result.data[1][1] = -2.0 / tb;
    result.data[2][2] = 1.0 / fn;
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -nearVal / fn; 
    result.data[3][3] = 1.0;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_orthographic_directx(double left, double right, double bottom, double top, double nearVal, double farVal) {
    double rl = right - left;
    double tb = top - bottom;
    double fn = farVal - nearVal;
    
    dmat4 result = dmat4_zero();
    result.data[0][0] = 2.0 / rl;
    result.data[1][1] = 2.0 / tb;
    result.data[2][2] = 1.0 / fn;
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -nearVal / fn;
    result.data[3][3] = 1.0;
    
    return result;
}

VECMATH_FUNC dmat4 dmat4_orthographic_opengl(double left, double right, double bottom, double top, double nearVal, double farVal) {
    double rl = right - left;
    double tb = top - bottom;
    double fn = farVal - nearVal;
    
    dmat4 result = dmat4_zero();
    result.data[0][0] = 2.0 / rl;
    result.data[1][1] = 2.0 / tb;
    result.data[2][2] = -2.0 / fn;
    result.data[3][0] = -(right + left) / rl;
    result.data[3][1] = -(top + bottom) / tb;
    result.data[3][2] = -(farVal + nearVal) / fn;
    result.data[3][3] = 1.0;
    
    return result;
}

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */