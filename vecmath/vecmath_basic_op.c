#include "vecmath_util.h"
#include "vecmath_basic_op.h"
#include <math.h>
#include <string.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialization
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_zero() {
    return (float2){0.0f, 0.0f};
}

VECMATH_FUNC float3 float3_zero() {
    return (float3){0.0f, 0.0f, 0.0f};
}

VECMATH_FUNC float4 float4_zero() {
    return (float4){0.0f, 0.0f, 0.0f, 0.0f};
}

VECMATH_FUNC fmat2 fmat2_zero() {
    fmat2 m;
    m.data[0][0] = 0.0f; m.data[0][1] = 0.0f;
    m.data[1][0] = 0.0f; m.data[1][1] = 0.0f;
    return m;
}

VECMATH_FUNC fmat3 fmat3_zero() {
    fmat3 m;
    m.data[0][0] = 0.0f; m.data[0][1] = 0.0f; m.data[0][2] = 0.0f;
    m.data[1][0] = 0.0f; m.data[1][1] = 0.0f; m.data[1][2] = 0.0f;
    m.data[2][0] = 0.0f; m.data[2][1] = 0.0f; m.data[2][2] = 0.0f;
    return m;
}

VECMATH_FUNC fmat4 fmat4_zero() {
    fmat4 m;
    m.data[0][0] = 0.0f; m.data[0][1] = 0.0f; m.data[0][2] = 0.0f; m.data[0][3] = 0.0f;
    m.data[1][0] = 0.0f; m.data[1][1] = 0.0f; m.data[1][2] = 0.0f; m.data[1][3] = 0.0f;
    m.data[2][0] = 0.0f; m.data[2][1] = 0.0f; m.data[2][2] = 0.0f; m.data[2][3] = 0.0f;
    m.data[3][0] = 0.0f; m.data[3][1] = 0.0f; m.data[3][2] = 0.0f; m.data[3][3] = 0.0f;
    return m;
}

VECMATH_FUNC fquat fquat_zero() {
    return (fquat){0.0f, 0.0f, 0.0f, 0.0f};
}

VECMATH_FUNC fray fray_zero() {
    return (fray){.origin = float3_zero(), .direction = float3_zero()};
}

VECMATH_FUNC double2 double2_zero() {
    return (double2){0.0, 0.0};
}

VECMATH_FUNC double3 double3_zero() {
    return (double3){0.0, 0.0, 0.0};
}

VECMATH_FUNC double4 double4_zero() {
    return (double4){0.0, 0.0, 0.0, 0.0};
}

VECMATH_FUNC dmat2 dmat2_zero() {
    dmat2 m;
    m.data[0][0] = 0.0; m.data[0][1] = 0.0;
    m.data[1][0] = 0.0; m.data[1][1] = 0.0;
    return m;
}

VECMATH_FUNC dmat3 dmat3_zero() {
    dmat3 m;
    m.data[0][0] = 0.0; m.data[0][1] = 0.0; m.data[0][2] = 0.0;
    m.data[1][0] = 0.0; m.data[1][1] = 0.0; m.data[1][2] = 0.0;
    m.data[2][0] = 0.0; m.data[2][1] = 0.0; m.data[2][2] = 0.0;
    return m;
}

VECMATH_FUNC dmat4 dmat4_zero() {
    dmat4 m;
    m.data[0][0] = 0.0; m.data[0][1] = 0.0; m.data[0][2] = 0.0; m.data[0][3] = 0.0;
    m.data[1][0] = 0.0; m.data[1][1] = 0.0; m.data[1][2] = 0.0; m.data[1][3] = 0.0;
    m.data[2][0] = 0.0; m.data[2][1] = 0.0; m.data[2][2] = 0.0; m.data[2][3] = 0.0;
    m.data[3][0] = 0.0; m.data[3][1] = 0.0; m.data[3][2] = 0.0; m.data[3][3] = 0.0;
    return m;
}

VECMATH_FUNC dquat dquat_zero() {
    return (dquat){0.0, 0.0, 0.0, 0.0};
}

VECMATH_FUNC dray dray_zero() {
    return (dray){.origin = double3_zero(), .direction = double3_zero()};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// oneing
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_one() {
    return (float2){1.0f, 1.0f};
}

VECMATH_FUNC float3 float3_one() {
    return (float3){1.0f, 1.0f, 1.0f};
}

VECMATH_FUNC float4 float4_one() {
    return (float4){1.0f, 1.0f, 1.0f, 1.0f};
}

VECMATH_FUNC fmat2 fmat2_one() {
    fmat2 m;
    m.data[0][0] = 1.0f; m.data[0][1] = 1.0f;
    m.data[1][0] = 1.0f; m.data[1][1] = 1.0f;
    return m;
}

VECMATH_FUNC fmat3 fmat3_one() {
    fmat3 m;
    m.data[0][0] = 1.0f; m.data[0][1] = 1.0f; m.data[0][2] = 1.0f;
    m.data[1][0] = 1.0f; m.data[1][1] = 1.0f; m.data[1][2] = 1.0f;
    m.data[2][0] = 1.0f; m.data[2][1] = 1.0f; m.data[2][2] = 1.0f;
    return m;
}

VECMATH_FUNC fmat4 fmat4_one() {
    fmat4 m;
    m.data[0][0] = 1.0f; m.data[0][1] = 1.0f; m.data[0][2] = 1.0f; m.data[0][3] = 1.0f;
    m.data[1][0] = 1.0f; m.data[1][1] = 1.0f; m.data[1][2] = 1.0f; m.data[1][3] = 1.0f;
    m.data[2][0] = 1.0f; m.data[2][1] = 1.0f; m.data[2][2] = 1.0f; m.data[2][3] = 1.0f;
    m.data[3][0] = 1.0f; m.data[3][1] = 1.0f; m.data[3][2] = 1.0f; m.data[3][3] = 1.0f;
    return m;
}

VECMATH_FUNC fquat fquat_one() {
    return (fquat){0.0f, 0.0f, 0.0f, 0.0f};
}

VECMATH_FUNC fray fray_one() {
    return (fray){.origin = float3_one(), .direction = float3_one()};
}

VECMATH_FUNC double2 double2_one() {
    return (double2){1.0, 1.0};
}

VECMATH_FUNC double3 double3_one() {
    return (double3){1.0, 1.0, 1.0};
}

VECMATH_FUNC double4 double4_one() {
    return (double4){1.0, 1.0, 1.0, 1.0};
}

VECMATH_FUNC dmat2 dmat2_one() {
    dmat2 m;
    m.data[0][0] = 1.0; m.data[0][1] = 1.0;
    m.data[1][0] = 1.0; m.data[1][1] = 1.0;
    return m;
}

VECMATH_FUNC dmat3 dmat3_one() {
    dmat3 m;
    m.data[0][0] = 1.0; m.data[0][1] = 1.0; m.data[0][2] = 1.0;
    m.data[1][0] = 1.0; m.data[1][1] = 1.0; m.data[1][2] = 1.0;
    m.data[2][0] = 1.0; m.data[2][1] = 1.0; m.data[2][2] = 1.0;
    return m;
}

VECMATH_FUNC dmat4 dmat4_one() {
    dmat4 m;
    m.data[0][0] = 1.0; m.data[0][1] = 1.0; m.data[0][2] = 1.0; m.data[0][3] = 1.0;
    m.data[1][0] = 1.0; m.data[1][1] = 1.0; m.data[1][2] = 1.0; m.data[1][3] = 1.0;
    m.data[2][0] = 1.0; m.data[2][1] = 1.0; m.data[2][2] = 1.0; m.data[2][3] = 1.0;
    m.data[3][0] = 1.0; m.data[3][1] = 1.0; m.data[3][2] = 1.0; m.data[3][3] = 1.0;
    return m;
}

VECMATH_FUNC dquat dquat_one() {
    return (dquat){1.0, 1.0, 1.0, 1.0};
}

VECMATH_FUNC dray dray_one() {
    return (dray){.origin = double3_one(), .direction = double3_one()};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a+b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_add(const float2* a, const float2* b) {
    return (float2){a->x + b->x, a->y + b->y};
}

VECMATH_FUNC float3 float3_add(const float3* a, const float3* b) {
    return (float3){a->x + b->x, a->y + b->y, a->z + b->z};
}

VECMATH_FUNC float4 float4_add(const float4* a, const float4* b) {
    return (float4){a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w};
}

VECMATH_FUNC fmat2 fmat2_add(const fmat2* a, const fmat2* b) {
    fmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat3 fmat3_add(const fmat3* a, const fmat3* b) {
    fmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat4 fmat4_add(const fmat4* a, const fmat4* b) {
    fmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fquat fquat_add(const fquat* a, const fquat* b) {
    return (fquat){a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w};
}

VECMATH_FUNC fray fray_add(const fray* a, const fray* b) {
    return (fray){
        .origin = float3_add(&a->origin, &b->origin),
        .direction = float3_add(&a->direction, &b->direction)
    };
}

VECMATH_FUNC double2 double2_add(const double2* a, const double2* b) {
    return (double2){a->x + b->x, a->y + b->y};
}

VECMATH_FUNC double3 double3_add(const double3* a, const double3* b) {
    return (double3){a->x + b->x, a->y + b->y, a->z + b->z};
}

VECMATH_FUNC double4 double4_add(const double4* a, const double4* b) {
    return (double4){a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w};
}

VECMATH_FUNC dmat2 dmat2_add(const dmat2* a, const dmat2* b) {
    dmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat3 dmat3_add(const dmat3* a, const dmat3* b) {
    dmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat4 dmat4_add(const dmat4* a, const dmat4* b) {
    dmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dquat dquat_add(const dquat* a, const dquat* b) {
    return (dquat){a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w};
}

VECMATH_FUNC dray dray_add(const dray* a, const dray* b) {
    return (dray){
        .origin = double3_add(&a->origin, &b->origin),
        .direction = double3_add(&a->direction, &b->direction)
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a-b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_sub(const float2* a, const float2* b) {
    return (float2){a->x - b->x, a->y - b->y};
}

VECMATH_FUNC float3 float3_sub(const float3* a, const float3* b) {
    return (float3){a->x - b->x, a->y - b->y, a->z - b->z};
}

VECMATH_FUNC float4 float4_sub(const float4* a, const float4* b) {
    return (float4){a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w};
}

VECMATH_FUNC fmat2 fmat2_sub(const fmat2* a, const fmat2* b) {
    fmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat3 fmat3_sub(const fmat3* a, const fmat3* b) {
    fmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat4 fmat4_sub(const fmat4* a, const fmat4* b) {
    fmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fquat fquat_sub(const fquat* a, const fquat* b) {
    return (fquat){a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w};
}

VECMATH_FUNC fray fray_sub(const fray* a, const fray* b) {
    return (fray){
        .origin = float3_sub(&a->origin, &b->origin),
        .direction = float3_sub(&a->direction, &b->direction)
    };
}

VECMATH_FUNC double2 double2_sub(const double2* a, const double2* b) {
    return (double2){a->x - b->x, a->y - b->y};
}

VECMATH_FUNC double3 double3_sub(const double3* a, const double3* b) {
    return (double3){a->x - b->x, a->y - b->y, a->z - b->z};
}

VECMATH_FUNC double4 double4_sub(const double4* a, const double4* b) {
    return (double4){a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w};
}

VECMATH_FUNC dmat2 dmat2_sub(const dmat2* a, const dmat2* b) {
    dmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat3 dmat3_sub(const dmat3* a, const dmat3* b) {
    dmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat4 dmat4_sub(const dmat4* a, const dmat4* b) {
    dmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dquat dquat_sub(const dquat* a, const dquat* b) {
    return (dquat){a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w};
}

VECMATH_FUNC dray dray_sub(const dray* a, const dray* b) {
    return (dray){
        .origin = double3_sub(&a->origin, &b->origin),
        .direction = double3_sub(&a->direction, &b->direction)
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a*b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_mul(const float2* a, const float2* b) {
    return (float2){a->x * b->x, a->y * b->y};
}

VECMATH_FUNC float3 float3_mul(const float3* a, const float3* b) {
    return (float3){a->x * b->x, a->y * b->y, a->z * b->z};
}

VECMATH_FUNC float4 float4_mul(const float4* a, const float4* b) {
    return (float4){a->x * b->x, a->y * b->y, a->z * b->z, a->w * b->w};
}

VECMATH_FUNC fmat2 fmat2_mul(const fmat2* a, const fmat2* b) {
    fmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat3 fmat3_mul(const fmat3* a, const fmat3* b) {
    fmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat4 fmat4_mul(const fmat4* a, const fmat4* b) {
    fmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fquat fquat_mul(const fquat* a, const fquat* b) {
    return (fquat){a->x * b->x, a->y * b->y, a->z * b->z, a->w * b->w};
}

VECMATH_FUNC fray fray_mul(const fray* a, const fray* b) {
    return (fray){
        .origin = float3_mul(&a->origin, &b->origin),
        .direction = float3_mul(&a->direction, &b->direction)
    };
}

VECMATH_FUNC double2 double2_mul(const double2* a, const double2* b) {
    return (double2){a->x * b->x, a->y * b->y};
}

VECMATH_FUNC double3 double3_mul(const double3* a, const double3* b) {
    return (double3){a->x * b->x, a->y * b->y, a->z * b->z};
}

VECMATH_FUNC double4 double4_mul(const double4* a, const double4* b) {
    return (double4){a->x * b->x, a->y * b->y, a->z * b->z, a->w * b->w};
}

VECMATH_FUNC dmat2 dmat2_mul(const dmat2* a, const dmat2* b) {
    dmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat3 dmat3_mul(const dmat3* a, const dmat3* b) {
    dmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat4 dmat4_mul(const dmat4* a, const dmat4* b) {
    dmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dquat dquat_mul(const dquat* a, const dquat* b) {
    return (dquat){a->x * b->x, a->y * b->y, a->z * b->z, a->w * b->w};
}

VECMATH_FUNC dray dray_mul(const dray* a, const dray* b) {
    return (dray){
        .origin = double3_mul(&a->origin, &b->origin),
        .direction = double3_mul(&a->direction, &b->direction)
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a/b
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float2 float2_div(const float2* a, const float2* b) {
    return (float2){a->x / b->x, a->y / b->y};
}

VECMATH_FUNC float3 float3_div(const float3* a, const float3* b) {
    return (float3){a->x / b->x, a->y / b->y, a->z / b->z};
}

VECMATH_FUNC float4 float4_div(const float4* a, const float4* b) {
    return (float4){a->x / b->x, a->y / b->y, a->z / b->z, a->w / b->w};
}

VECMATH_FUNC fmat2 fmat2_div(const fmat2* a, const fmat2* b) {
    fmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] / b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat3 fmat3_div(const fmat3* a, const fmat3* b) {
    fmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] / b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fmat4 fmat4_div(const fmat4* a, const fmat4* b) {
    fmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] / b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC fquat fquat_div(const fquat* a, const fquat* b) {
    return (fquat){a->x / b->x, a->y / b->y, a->z / b->z, a->w / b->w};
}

VECMATH_FUNC fray fray_div(const fray* a, const fray* b) {
    return (fray){
        .origin = float3_div(&a->origin, &b->origin),
        .direction = float3_div(&a->direction, &b->direction)
    };
}

VECMATH_FUNC double2 double2_div(const double2* a, const double2* b) {
    return (double2){a->x / b->x, a->y / b->y};
}

VECMATH_FUNC double3 double3_div(const double3* a, const double3* b) {
    return (double3){a->x / b->x, a->y / b->y, a->z / b->z};
}

VECMATH_FUNC double4 double4_div(const double4* a, const double4* b) {
    return (double4){a->x / b->x, a->y / b->y, a->z / b->z, a->w / b->w};
}

VECMATH_FUNC dmat2 dmat2_div(const dmat2* a, const dmat2* b) {
    dmat2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a->data[i][j] / b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat3 dmat3_div(const dmat3* a, const dmat3* b) {
    dmat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a->data[i][j] / b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dmat4 dmat4_div(const dmat4* a, const dmat4* b) {
    dmat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a->data[i][j] / b->data[i][j];
        }
    }
    return result;
}

VECMATH_FUNC dquat dquat_div(const dquat* a, const dquat* b) {
    return (dquat){a->x / b->x, a->y / b->y, a->z / b->z, a->w / b->w};
}

VECMATH_FUNC dray dray_div(const dray* a, const dray* b) {
    return (dray){
        .origin = double3_div(&a->origin, &b->origin),
        .direction = double3_div(&a->direction, &b->direction)
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// check if equals
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC bool float2_equals(const float2* a, const float2* b) {
    return fabsf(a->x - b->x) < VECMATH_FLT_EPSILON && fabsf(a->y - b->y) < VECMATH_FLT_EPSILON;
}

VECMATH_FUNC bool float3_equals(const float3* a, const float3* b) {
    return fabsf(a->x - b->x) < VECMATH_FLT_EPSILON && fabsf(a->y - b->y) < VECMATH_FLT_EPSILON && fabsf(a->z - b->z) < VECMATH_FLT_EPSILON;
}

VECMATH_FUNC bool float4_equals(const float4* a, const float4* b) {
    return fabsf(a->x - b->x) < VECMATH_FLT_EPSILON && fabsf(a->y - b->y) < VECMATH_FLT_EPSILON && fabsf(a->z - b->z) < VECMATH_FLT_EPSILON && fabsf(a->w - b->w) < VECMATH_FLT_EPSILON;
}

VECMATH_FUNC bool fmat2_equals(const fmat2* a, const fmat2* b) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (fabsf(a->data[i][j] - b->data[i][j]) >= VECMATH_FLT_EPSILON)
                return false;
        }
    }
    return true;
}

VECMATH_FUNC bool fmat3_equals(const fmat3* a, const fmat3* b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (fabsf(a->data[i][j] - b->data[i][j]) >= VECMATH_FLT_EPSILON)
                return false;
        }
    }
    return true;
}

VECMATH_FUNC bool fmat4_equals(const fmat4* a, const fmat4* b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (fabsf(a->data[i][j] - b->data[i][j]) >= VECMATH_FLT_EPSILON)
                return false;
        }
    }
    return true;
}

VECMATH_FUNC bool fquat_equals(const fquat* a, const fquat* b) {
    return fabsf(a->x - b->x) < VECMATH_FLT_EPSILON && fabsf(a->y - b->y) < VECMATH_FLT_EPSILON && fabsf(a->z - b->z) < VECMATH_FLT_EPSILON && fabsf(a->w - b->w) < VECMATH_FLT_EPSILON;
}

VECMATH_FUNC bool fray_equals(const fray* a, const fray* b) {
    return float3_equals(&a->origin, &b->origin) && float3_equals(&a->direction, &b->direction);
}

VECMATH_FUNC bool double2_equals(const double2* a, const double2* b) {
    return fabs(a->x - b->x) < VECMATH_DBL_EPSILON && fabs(a->y - b->y) < VECMATH_DBL_EPSILON;
}

VECMATH_FUNC bool double3_equals(const double3* a, const double3* b) {
    return fabs(a->x - b->x) < VECMATH_DBL_EPSILON && fabs(a->y - b->y) < VECMATH_DBL_EPSILON && fabs(a->z - b->z) < VECMATH_DBL_EPSILON;
}

VECMATH_FUNC bool double4_equals(const double4* a, const double4* b) {
    return fabs(a->x - b->x) < VECMATH_DBL_EPSILON && fabs(a->y - b->y) < VECMATH_DBL_EPSILON && fabs(a->z - b->z) < VECMATH_DBL_EPSILON && fabs(a->w - b->w) < VECMATH_DBL_EPSILON;
}

VECMATH_FUNC bool dmat2_equals(const dmat2* a, const dmat2* b) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (fabs(a->data[i][j] - b->data[i][j]) >= VECMATH_DBL_EPSILON)
                return false;
        }
    }
    return true;
}

VECMATH_FUNC bool dmat3_equals(const dmat3* a, const dmat3* b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (fabs(a->data[i][j] - b->data[i][j]) >= VECMATH_DBL_EPSILON)
                return false;
        }
    }
    return true;
}

VECMATH_FUNC bool dmat4_equals(const dmat4* a, const dmat4* b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (fabs(a->data[i][j] - b->data[i][j]) >= VECMATH_DBL_EPSILON)
                return false;
        }
    }
    return true;
}

VECMATH_FUNC bool dquat_equals(const dquat* a, const dquat* b) {
    return fabs(a->x - b->x) < VECMATH_DBL_EPSILON && fabs(a->y - b->y) < VECMATH_DBL_EPSILON && fabs(a->z - b->z) < VECMATH_DBL_EPSILON && fabs(a->w - b->w) < VECMATH_DBL_EPSILON;
}

VECMATH_FUNC bool dray_equals(const dray* a, const dray* b) {
    return double3_equals(&a->origin, &b->origin) && double3_equals(&a->direction, &b->direction);
}

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */