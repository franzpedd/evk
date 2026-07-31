#include "vecmath_mat_op.h"
#include "vecmath_basic_op.h"
#include "vecmath_vec_op.h"
#include <math.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// quaternion operations
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fquat fquat_identity(void) {
    return (fquat){0.0f, 0.0f, 0.0f, 1.0f};
}

VECMATH_FUNC dquat dquat_identity(void) {
    return (dquat){0.0, 0.0, 0.0, 1.0};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// length
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float fquat_length(const fquat* q) {
    if (!q) return 0.0f;
    return sqrtf(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
}

VECMATH_FUNC double dquat_length(const dquat* q) {
    if (!q) return 0.0;
    return sqrt(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// quaternion conjugate (inverse for unit quaternions)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fquat fquat_conjugate(const fquat* q) {
    if (!q) return fquat_identity();
    return (fquat){-q->x, -q->y, -q->z, q->w};
}

VECMATH_FUNC dquat dquat_conjugate(const dquat* q) {
    if (!q) return dquat_identity();
    return (dquat){-q->x, -q->y, -q->z, q->w};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// normalizes the quaternion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fquat fquat_normalize(const fquat* q) {
    if (!q) return fquat_identity();
    
    float len = fquat_length(q);
    if (len < VECMATH_FLT_EPSILON) return fquat_identity();
    
    float inv_len = 1.0f / len;
    return (fquat){q->x * inv_len, q->y * inv_len, q->z * inv_len, q->w * inv_len};
}

VECMATH_FUNC dquat dquat_normalize(const dquat* q) {
    if (!q) return dquat_identity();
    
    double len = dquat_length(q);
    if (len < VECMATH_DBL_EPSILON) return dquat_identity();
    
    double inv_len = 1.0 / len;
    return (dquat){q->x * inv_len, q->y * inv_len, q->z * inv_len, q->w * inv_len};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the dot product of two quaternions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float fquat_dot(const fquat* q1, const fquat* q2) {
    if (!q1 || !q2) return 0.0f;
    return q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w;
}

VECMATH_FUNC double dquat_dot(const dquat* q1, const dquat* q2) {
    if (!q1 || !q2) return 0.0;
    return q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// performs linear interpolation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fquat fquat_lerp(const fquat* q1, const fquat* q2, float t) {
    if (!q1 || !q2) return fquat_identity();
    
    // Clamp t to [0, 1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    
    return (fquat){
        q1->x + t * (q2->x - q1->x),
        q1->y + t * (q2->y - q1->y),
        q1->z + t * (q2->z - q1->z),
        q1->w + t * (q2->w - q1->w)
    };
}

VECMATH_FUNC dquat dquat_lerp(const dquat* q1, const dquat* q2, double t) {
    if (!q1 || !q2) return dquat_identity();
    
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    
    return (dquat){
        q1->x + t * (q2->x - q1->x),
        q1->y + t * (q2->y - q1->y),
        q1->z + t * (q2->z - q1->z),
        q1->w + t * (q2->w - q1->w)
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// performs spherical interpolation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fquat fquat_slerp(const fquat* q1, const fquat* q2, float t) {
    if (!q1 || !q2) return fquat_identity();
    
    // clamp t to [0, 1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    
    // compute dot product
    float dot = fquat_dot(q1, q2);
    
    // if dot is negative, flip one quaternion to take the shortest path
    fquat q2_flipped = *q2;
    if (dot < 0.0f) {
        dot = -dot;
        q2_flipped.x = -q2->x;
        q2_flipped.y = -q2->y;
        q2_flipped.z = -q2->z;
        q2_flipped.w = -q2->w;
    }
    
    // if quaternions are very close, use linear interpolation
    if (dot > 0.9995f) return fquat_lerp(q1, &q2_flipped, t);
    
    // compute angle and interpolation factors
    float theta = acosf(dot);
    float sin_theta = sinf(theta);
    float inv_sin_theta = 1.0f / sin_theta;
    float t1 = sinf((1.0f - t) * theta) * inv_sin_theta;
    float t2 = sinf(t * theta) * inv_sin_theta;
    
    // interpolate
    return (fquat){
        q1->x * t1 + q2_flipped.x * t2,
        q1->y * t1 + q2_flipped.y * t2,
        q1->z * t1 + q2_flipped.z * t2,
        q1->w * t1 + q2_flipped.w * t2
    };
}

VECMATH_FUNC dquat dquat_slerp(const dquat* q1, const dquat* q2, double t) {
    if (!q1 || !q2) return dquat_identity();
    
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    
    double dot = dquat_dot(q1, q2);
    
    dquat q2_flipped = *q2;
    if (dot < 0.0) {
        dot = -dot;
        q2_flipped.x = -q2->x;
        q2_flipped.y = -q2->y;
        q2_flipped.z = -q2->z;
        q2_flipped.w = -q2->w;
    }
    
    if (dot > 0.9995) {
        return dquat_lerp(q1, &q2_flipped, t);
    }
    
    double theta = acos(dot);
    double sin_theta = sin(theta);
    double inv_sin_theta = 1.0 / sin_theta;
    double t1 = sin((1.0 - t) * theta) * inv_sin_theta;
    double t2 = sin(t * theta) * inv_sin_theta;
    
    return (dquat){
        q1->x * t1 + q2_flipped.x * t2,
        q1->y * t1 + q2_flipped.y * t2,
        q1->z * t1 + q2_flipped.z * t2,
        q1->w * t1 + q2_flipped.w * t2
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// creates quaternion from euler angles (radians) - ZYX order (yaw, pitch, roll)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fquat fquat_from_euler(const float3* rad) {
    if (!rad) return fquat_identity();
    
    float cx = cosf(rad->x * 0.5f);
    float cy = cosf(rad->y * 0.5f);
    float cz = cosf(rad->z * 0.5f);
    float sx = sinf(rad->x * 0.5f);
    float sy = sinf(rad->y * 0.5f);
    float sz = sinf(rad->z * 0.5f);
    
    // ZYX order (yaw, pitch, roll)
    return (fquat){
        sx * cy * cz - cx * sy * sz,  // x (pitch)
        cx * sy * cz + sx * cy * sz,  // y (yaw)
        cx * cy * sz - sx * sy * cz,  // z (roll)
        cx * cy * cz + sx * sy * sz   // w
    };
}

VECMATH_FUNC dquat dquat_from_euler(const double3* rad) {
    if (!rad) return dquat_identity();
    
    double cx = cos(rad->x * 0.5);
    double cy = cos(rad->y * 0.5);
    double cz = cos(rad->z * 0.5);
    double sx = sin(rad->x * 0.5);
    double sy = sin(rad->y * 0.5);
    double sz = sin(rad->z * 0.5);
    
    return (dquat){
        sx * cy * cz - cx * sy * sz,
        cx * sy * cz + sx * cy * sz,
        cx * cy * sz - sx * sy * cz,
        cx * cy * cz + sx * sy * sz
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// converts the quaternion into euler angles (roll=x, pitch=y, yaw=z)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float3 fquat_to_euler(const fquat* q) {
    float3 result = float3_zero();
    if (!q) return result;
    
    // roll (x-axis rotation)
    float sinr_cosp = 2.0f * (q->w * q->x + q->y * q->z);
    float cosr_cosp = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);
    result.x = atan2f(sinr_cosp, cosr_cosp);
    
    // pitch (y-axis rotation)
    float sinp = 2.0f * (q->w * q->y - q->z * q->x);
    if (fabsf(sinp) >= 1.0f) result.y = copysignf(VECMATH_FLT_HALF_PI, sinp);else result.y = asinf(sinp);
    
    // yaw (z-axis rotation)
    float siny_cosp = 2.0f * (q->w * q->z + q->x * q->y);
    float cosy_cosp = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
    result.z = atan2f(siny_cosp, cosy_cosp);
    
    return result;
}

VECMATH_FUNC double3 dquat_to_euler(const dquat* q) {
    double3 result = double3_zero();
    if (!q) return result;
    
    double sinr_cosp = 2.0 * (q->w * q->x + q->y * q->z);
    double cosr_cosp = 1.0 - 2.0 * (q->x * q->x + q->y * q->y);
    result.x = atan2(sinr_cosp, cosr_cosp);
    
    double sinp = 2.0 * (q->w * q->y - q->z * q->x);
    if (fabs(sinp) >= 1.0) result.y = copysign(VECMATH_DBL_HALF_PI, sinp);
    else result.y = asin(sinp);
    
    double siny_cosp = 2.0 * (q->w * q->z + q->x * q->y);
    double cosy_cosp = 1.0 - 2.0 * (q->y * q->y + q->z * q->z);
    result.z = atan2(siny_cosp, cosy_cosp);
    
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// converts the quaternion into a matrix
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fmat4 fquat_to_fmat4_rowmajor(const fquat* q) {
    fmat4 result = fmat4_zero();
    if (!q) return fmat4_identity();
    
    float xx = q->x * q->x;
    float yy = q->y * q->y;
    float zz = q->z * q->z;
    float xy = q->x * q->y;
    float xz = q->x * q->z;
    float yz = q->y * q->z;
    float wx = q->w * q->x;
    float wy = q->w * q->y;
    float wz = q->w * q->z;
    
    result.data[0][0] = 1.0f - 2.0f * (yy + zz);
    result.data[0][1] = 2.0f * (xy - wz);
    result.data[0][2] = 2.0f * (xz + wy);
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = 2.0f * (xy + wz);
    result.data[1][1] = 1.0f - 2.0f * (xx + zz);
    result.data[1][2] = 2.0f * (yz - wx);
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = 2.0f * (xz - wy);
    result.data[2][1] = 2.0f * (yz + wx);
    result.data[2][2] = 1.0f - 2.0f * (xx + yy);
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = 0.0f;
    result.data[3][1] = 0.0f;
    result.data[3][2] = 0.0f;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC fmat4 fquat_to_fmat4_colmajor(const fquat* q) {
    fmat4 result = fmat4_zero();
    if (!q) return fmat4_identity();
    
    float xx = q->x * q->x;
    float yy = q->y * q->y;
    float zz = q->z * q->z;
    float xy = q->x * q->y;
    float xz = q->x * q->z;
    float yz = q->y * q->z;
    float wx = q->w * q->x;
    float wy = q->w * q->y;
    float wz = q->w * q->z;
    
    result.data[0][0] = 1.0f - 2.0f * (yy + zz);
    result.data[0][1] = 2.0f * (xy + wz);
    result.data[0][2] = 2.0f * (xz - wy);
    result.data[0][3] = 0.0f;
    
    result.data[1][0] = 2.0f * (xy - wz);
    result.data[1][1] = 1.0f - 2.0f * (xx + zz);
    result.data[1][2] = 2.0f * (yz + wx);
    result.data[1][3] = 0.0f;
    
    result.data[2][0] = 2.0f * (xz + wy);
    result.data[2][1] = 2.0f * (yz - wx);
    result.data[2][2] = 1.0f - 2.0f * (xx + yy);
    result.data[2][3] = 0.0f;
    
    result.data[3][0] = 0.0f;
    result.data[3][1] = 0.0f;
    result.data[3][2] = 0.0f;
    result.data[3][3] = 1.0f;
    
    return result;
}

VECMATH_FUNC dmat4 dquat_to_dmat4_rowmajor(const dquat* q) {
    dmat4 result = dmat4_zero();
    if (!q) return dmat4_identity();
    
    double xx = q->x * q->x;
    double yy = q->y * q->y;
    double zz = q->z * q->z;
    double xy = q->x * q->y;
    double xz = q->x * q->z;
    double yz = q->y * q->z;
    double wx = q->w * q->x;
    double wy = q->w * q->y;
    double wz = q->w * q->z;
    
    result.data[0][0] = 1.0 - 2.0 * (yy + zz);
    result.data[0][1] = 2.0 * (xy - wz);
    result.data[0][2] = 2.0 * (xz + wy);
    result.data[0][3] = 0.0;
    
    result.data[1][0] = 2.0 * (xy + wz);
    result.data[1][1] = 1.0 - 2.0 * (xx + zz);
    result.data[1][2] = 2.0 * (yz - wx);
    result.data[1][3] = 0.0;
    
    result.data[2][0] = 2.0 * (xz - wy);
    result.data[2][1] = 2.0 * (yz + wx);
    result.data[2][2] = 1.0 - 2.0 * (xx + yy);
    result.data[2][3] = 0.0;
    
    result.data[3][0] = 0.0;
    result.data[3][1] = 0.0;
    result.data[3][2] = 0.0;
    result.data[3][3] = 1.0;
    
    return result;
}

VECMATH_FUNC dmat4 dquat_to_dmat4_colmajor(const dquat* q) {
    dmat4 result = dmat4_zero();
    if (!q) return dmat4_identity();
    
    double xx = q->x * q->x;
    double yy = q->y * q->y;
    double zz = q->z * q->z;
    double xy = q->x * q->y;
    double xz = q->x * q->z;
    double yz = q->y * q->z;
    double wx = q->w * q->x;
    double wy = q->w * q->y;
    double wz = q->w * q->z;
    
    result.data[0][0] = 1.0 - 2.0 * (yy + zz);
    result.data[0][1] = 2.0 * (xy + wz);
    result.data[0][2] = 2.0 * (xz - wy);
    result.data[0][3] = 0.0;
    
    result.data[1][0] = 2.0 * (xy - wz);
    result.data[1][1] = 1.0 - 2.0 * (xx + zz);
    result.data[1][2] = 2.0 * (yz + wx);
    result.data[1][3] = 0.0;
    
    result.data[2][0] = 2.0 * (xz + wy);
    result.data[2][1] = 2.0 * (yz - wx);
    result.data[2][2] = 1.0 - 2.0 * (xx + yy);
    result.data[2][3] = 0.0;
    
    result.data[3][0] = 0.0;
    result.data[3][1] = 0.0;
    result.data[3][2] = 0.0;
    result.data[3][3] = 1.0;
    
    return result;
}

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */