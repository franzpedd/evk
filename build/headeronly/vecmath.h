#ifndef VECMATH_INCLUDED
#define VECMATH_INCLUDED
#define VECMATH_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the building configuration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// build mode selection (only one should be defined)
// VECMATH_HEADER_ONLY  - Header-only (all functions static inline)
// VECMATH_SHARED       - Shared library (DLL/SO)
// VECMATH_STATIC       - Static library (default)

#if defined(VECMATH_SHARED)
    #if defined(_WIN32)
        #if defined(VECMATH_EXPORT) 
            #define VECMATH_API __declspec(dllexport)
        #else
            #define VECMATH_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(VECMATH_EXPORT)
            #define VECMATH_API __attribute__((visibility("default")))
        #else
            #define VECMATH_API
        #endif
    #else
        #define VECMATH_API
    #endif
#else
    #define VECMATH_API
#endif

#if defined(VECMATH_HEADER_ONLY)
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define VECMATH_FUNC static inline
    #else
        #define VECMATH_FUNC static  // fallback for C89
    #endif
    #undef VECMATH_API
    #define VECMATH_API
    
#elif defined(VECMATH_SHARED)
    #define VECMATH_FUNC
#else
    #define VECMATH_FUNC
#endif // VECMATH_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// usefull constants
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VECMATH_FLT_INF             (1.0f/0.0f)
#define VECMATH_FLT_NAN             (0.0f/0.0f)
#define VECMATH_FLT_MIN             1.175494351e-38f
#define VECMATH_FLT_MAX             3.402823466e+38f
#define VECMATH_FLT_PI              3.14159265358979323846f
#define VECMATH_FLT_INV_PI          0.31830988618379067154f
#define VECMATH_FLT_HALF_PI         1.57079632679489661923f
#define VECMATH_FLT_QUARTER_PI      0.78539816339744830962f
#define VECMATH_FLT_TWO_PI          6.28318530717958647692f
#define VECMATH_FLT_EPSILON         1.1920928955078125e-07f

#define VECMATH_DBL_INF             (1.0/0.0)
#define VECMATH_DBL_NAN             (0.0/0.0)
#define VECMATH_DBL_MIN             2.2250738585072014e-308
#define VECMATH_DBL_MAX             1.7976931348623157e+308
#define VECMATH_DBL_PI              3.14159265358979323846
#define VECMATH_DBL_INV_PI          0.31830988618379067154
#define VECMATH_DBL_HALF_PI         1.57079632679489661923
#define VECMATH_DBL_QUARTER_PI      0.78539816339744830962
#define VECMATH_DBL_TWO_PI          6.28318530717958647692
#define VECMATH_DBL_EPSILON         2.2204460492503131e-16


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// boolean implementation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __bool_true_false_are_defined
#ifndef __cplusplus
    #if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
        typedef _Bool bool;
    #else
        typedef unsigned char bool;
    #endif
    
    #define true  1
    #define false 0
#endif // __cplusplus

#define __bool_true_false_are_defined 1
#endif // __bool_true_false_are_defined

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// single-precision types
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct float2 { float x, y; } float2;
typedef struct float3 { float x, y, z; } float3;
typedef struct float4 { float x, y, z, w; } float4;
typedef struct fmat2 { float data[2][2]; } fmat2;
typedef struct fmat3 { float data[3][3]; } fmat3;
typedef struct fmat4 { float data[4][4]; } fmat4;
typedef struct fquat { float x, y, z, w; } fquat;
typedef struct fray { float3 origin, direction; } fray;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// double-precision types
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct double2 { double x, y; } double2;
typedef struct double3 { double x, y, z; } double3;
typedef struct double4 { double x, y, z, w; } double4;
typedef struct dmat2 { double data[2][2]; } dmat2;
typedef struct dmat3 { double data[3][3]; } dmat3;
typedef struct dmat4 { double data[4][4]; } dmat4;
typedef struct dquat { double x, y, z, w; } dquat;
typedef struct dray { double3 origin, direction; } dray;


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


#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// identity
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_identity();
VECMATH_API fmat3 fmat3_identity();
VECMATH_API fmat4 fmat4_identity();
VECMATH_API dmat2 dmat2_identity();
VECMATH_API dmat3 dmat3_identity();
VECMATH_API dmat4 dmat4_identity();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat*vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float2 fmat2_mul_float2(const fmat2* m, const float2* v);
VECMATH_API float3 fmat3_mul_float3(const fmat3* m, const float3* v);
VECMATH_API float4 fmat4_mul_float4(const fmat4* m, const float4* v);
VECMATH_API double2 dmat2_mul_double2(const dmat2* m, const double2* v);
VECMATH_API double3 dmat3_mul_double3(const dmat3* m, const double3* v);
VECMATH_API double4 dmat4_mul_double4(const dmat4* m, const double4* v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// changes colums with lines
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_transpose(const fmat2* m);
VECMATH_API fmat3 fmat3_transpose(const fmat3* m); 
VECMATH_API fmat4 fmat4_transpose(const fmat4* m);
VECMATH_API dmat2 dmat2_transpose(const dmat2* m);
VECMATH_API dmat3 dmat3_transpose(const dmat3* m);
VECMATH_API dmat4 dmat4_transpose(const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the determinant
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float fmat2_determinant(const fmat2* m);
VECMATH_API float fmat3_determinant(const fmat3* m);
VECMATH_API float fmat4_determinant(const fmat4* m);
VECMATH_API double dmat2_determinant(const dmat2* m);
VECMATH_API double dmat3_determinant(const dmat3* m);
VECMATH_API double dmat4_determinant(const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the inverse matrix
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat2 fmat2_inverse(const fmat2* m);
VECMATH_API fmat3 fmat3_inverse(const fmat3* m);
VECMATH_API fmat4 fmat4_inverse(const fmat4* m);
VECMATH_API dmat2 dmat2_inverse(const dmat2* m);
VECMATH_API dmat3 dmat3_inverse(const dmat3* m);
VECMATH_API dmat4 dmat4_inverse(const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat translation decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fmat4_get_translation_rowmajor(const fmat4* m);
VECMATH_API float3 fmat4_get_translation_colmajor(const fmat4* m);
VECMATH_API double3 dmat4_get_translation_rowmajor(const dmat4* m);
VECMATH_API double3 dmat4_get_translation_colmajor(const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat scale decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fmat4_get_scale_rowmajor(const fmat4* m);
VECMATH_API float3 fmat4_get_scale_colmajor(const fmat4* m);
VECMATH_API double3 dmat4_get_scale_rowmajor(const dmat4* m);
VECMATH_API double3 dmat4_get_scale_colmajor(const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat rotation decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fmat4_get_rotation_rowmajor(const fmat4* m);
VECMATH_API float3 fmat4_get_rotation_colmajor(const fmat4* m);
VECMATH_API double3 dmat4_get_rotation_rowmajor(const dmat4* m);
VECMATH_API double3 dmat4_get_rotation_colmajor(const dmat4* m);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat translation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_translate_rowmajor(const fmat4* m, const float3* dir);
VECMATH_API fmat4 fmat4_translate_colmajor(const fmat4* m, const float3* dir);
VECMATH_API dmat4 dmat4_translate_rowmajor(const dmat4* m, const double3* dir);
VECMATH_API dmat4 dmat4_translate_colmajor(const dmat4* m, const double3* dir);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat scale
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_scale_rowmajor(const fmat4* m, const float3* dim);
VECMATH_API fmat4 fmat4_scale_colmajor(const fmat4* m, const float3* dim);
VECMATH_API dmat4 dmat4_scale_rowmajor(const dmat4* m, const double3* dim);
VECMATH_API dmat4 dmat4_scale_colmajor(const dmat4* m, const double3* dim);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat rotation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_rotate_colmajor(const fmat4* m, float angle, const float3* axis);
VECMATH_API fmat4 fmat4_rotate_rowmajor(const fmat4* m, float angle, const float3* axis);
VECMATH_API dmat4 dmat4_rotate_colmajor(const dmat4* m, double angle, const double3* axis);
VECMATH_API dmat4 dmat4_rotate_rowmajor(const dmat4* m, double angle, const double3* axis);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mat decomposition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief fully decomposes the matrix into it's components
VECMATH_API void fmat4_decompose_rowmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale);
VECMATH_API void fmat4_decompose_colmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale);
VECMATH_API void dmat4_decompose_rowmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale);
VECMATH_API void dmat4_decompose_colmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// look at projection
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_lookat_vulkan(const float3* eye, const float3* target, const float3* up);
VECMATH_API fmat4 fmat4_lookat_directx(const float3* eye, const float3* target, const float3* up);
VECMATH_API fmat4 fmat4_lookat_opengl(const float3* eye, const float3* target, const float3* up);
VECMATH_API dmat4 dmat4_lookat_vulkan(const double3* eye, const double3* target, const double3* up);
VECMATH_API dmat4 dmat4_lookat_directx(const double3* eye, const double3* target, const double3* up);
VECMATH_API dmat4 dmat4_lookat_opengl(const double3* eye, const double3* target, const double3* up);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// perspective projection 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_perspective_vulkan(float fov_rad, float aspect, float nearVal, float farVal);
VECMATH_API fmat4 fmat4_perspective_directx(float fov_rad, float aspect, float nearVal, float farVal);
VECMATH_API fmat4 fmat4_perspective_opengl(float fov_rad, float aspect, float nearVal, float farVal);
VECMATH_API dmat4 dmat4_perspective_vulkan(double fov_rad, double aspect, double nearVal, double farVal);
VECMATH_API dmat4 dmat4_perspective_directx(double fov_rad, double aspect, double nearVal, double farVal);
VECMATH_API dmat4 dmat4_perspective_opengl(double fov_rad, double aspect, double nearVal, double farVal);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// orthographic projection 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fmat4_orthographic_vulkan(float left, float right, float bottom, float top, float near, float far);
VECMATH_API fmat4 fmat4_orthographic_directx(float left, float right, float bottom, float top, float near, float far);
VECMATH_API fmat4 fmat4_orthographic_opengl(float left, float right, float bottom, float top, float near, float far);
VECMATH_API dmat4 dmat4_orthographic_vulkan(double left, double right, double bottom, double top, double near, double far);
VECMATH_API dmat4 dmat4_orthographic_directx(double left, double right, double bottom, double top, double near, double far);
VECMATH_API dmat4 dmat4_orthographic_opengl(double left, double right, double bottom, double top, double near, double far);

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// identity
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_identity(void);
VECMATH_API dquat dquat_identity(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// length
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float fquat_length(const fquat* q);
VECMATH_API double dquat_length(const dquat* q);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// quaternion conjugate (inverse for unit quaternions)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_conjugate(const fquat* q);
VECMATH_API dquat dquat_conjugate(const dquat* q);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// normalizes the quaternion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_normalize(const fquat* q);
VECMATH_API dquat dquat_normalize(const dquat* q);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns the dot product of two quaternions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float fquat_dot(const fquat* q1, const fquat* q2);
VECMATH_API double dquat_dot(const dquat* q1, const dquat* q2);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  performs linear interpolation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_lerp(const fquat* q1, const fquat* q2, float t);
VECMATH_API dquat dquat_lerp(const dquat* q1, const dquat* q2, double t);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// performs spherical interpolation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_slerp(const fquat* q1, const fquat* q2, float t);
VECMATH_API dquat dquat_slerp(const dquat* q1, const dquat* q2, double t);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// creates quaternion from euler angles (radians) - ZYX order (yaw, pitch, roll)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fquat fquat_from_euler(const float3* rad);
VECMATH_API dquat dquat_from_euler(const double3* rad);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// converts the quaternion into euler angles (roll=x, pitch=y, yaw=z)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fquat_to_euler(const fquat* q);
VECMATH_API double3 dquat_to_euler(const dquat* q);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// converts the quaternion into a matrix
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fmat4 fquat_to_fmat4_rowmajor(const fquat* q);
VECMATH_API fmat4 fquat_to_fmat4_colmajor(const fquat* q);
VECMATH_API dmat4 dquat_to_dmat4_rowmajor(const dquat* q);
VECMATH_API dmat4 dquat_to_dmat4_colmajor(const dquat* q);

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ray from screen point (vulkan only for now)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API fray fray_from_screen_point_vulkan(const float2* screenPos, const float2* windowSize, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// screen to world point (vulkan only for now)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float3 fray_screen_to_world_point_vulkan(const float2* screenPos, const float2* windowSize, float distance, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// angle utilities
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float to_fradians(float degrees);
VECMATH_API float to_fdegrees(float radians);
VECMATH_API float f_cos(float degree);
VECMATH_API float f_sin(float degree);
VECMATH_API float f_tan(float degree);

VECMATH_API double to_dradians(double degrees);
VECMATH_API double to_ddegrees(double radians);
VECMATH_API double d_cos(double degree);
VECMATH_API double d_sin(double degree);
VECMATH_API double d_tan(double degree);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic utilities
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_API float f_power(float b, int e);
VECMATH_API double d_power(double b, int e);

VECMATH_API float f_log10(const float x);
VECMATH_API double d_log10(const double x);

VECMATH_API float f_log2(const float x);
VECMATH_API double d_log2(const double x);

VECMATH_API float f_logn(const float x);
VECMATH_API double d_logn(const double x);

VECMATH_API float f_floor(const float x);
VECMATH_API double d_floor(const double x);

VECMATH_API float f_ceil(const float x);
VECMATH_API double d_ceil(const double x);

VECMATH_API float f_frac(const float x);
VECMATH_API double d_frac(const double x);

VECMATH_API float f_round(const float x);
VECMATH_API double d_round(const double x);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clamp utilities
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  arguments must not have side effects, so be carefull
#define VECMATH_MIN(a, b)  ((a) < (b) ? (a) : (b))
#define VECMATH_MAX(a, b)  ((a) > (b) ? (a) : (b))
#define VECMATH_CLAMP(x, min, max) (VECMATH_MIN(VECMATH_MAX((x), (min)), (max)))

#ifdef __cplusplus 
}
#endif


#ifdef VECMATH_IMPLEMENTATION
#undef VECMATH_IMPLEMENTATION

#include <math.h>
#include <string.h>

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


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ray from screen point (vulkan only for now)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fray fray_from_screen_point_vulkan(const float2* screenPos, const float2* windowSize, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp) {
    fray result = fray_zero();
    if (!screenPos || !windowSize || !cameraPos || !cameraFront || !cameraUp) return result;
    
    float ndcX = (2.0f * screenPos->x) / windowSize->x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos->y) / windowSize->y;

    float3 cameraRight = float3_cross(cameraUp, cameraFront);
    cameraRight = float3_normalize(&cameraRight);

    float3 up = float3_cross(cameraFront, &cameraRight);
    float3 normalizedUp = float3_normalize(&up);

    float halfHeight = tanf(fov * 0.5f);
    float halfWidth = halfHeight * aspectRatio;

    float3 direction = {
        cameraFront->x + cameraRight.x * (ndcX * halfWidth) + normalizedUp.x * (ndcY * halfHeight),
        cameraFront->y + cameraRight.y * (ndcX * halfWidth) + normalizedUp.y * (ndcY * halfHeight),
        cameraFront->z + cameraRight.z * (ndcX * halfWidth) + normalizedUp.z * (ndcY * halfHeight)
    };

    result.origin = *cameraPos;
    result.direction = float3_normalize(&direction);

    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// screen to world point (vulkan only for now)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float3 fray_screen_to_world_point_vulkan(const float2* screenPos, const float2* windowSize, float distance, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp) {
    float3 result = float3_zero();
    if (!screenPos || !windowSize || !cameraPos || !cameraFront || !cameraUp) return result;

    float ndcX = (2.0f * screenPos->x) / windowSize->x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos->y) / windowSize->y;

    float3 cameraRight = float3_cross(cameraUp, cameraFront);
    cameraRight = float3_normalize(&cameraRight);

    float3 up = float3_cross(cameraFront, &cameraRight);
    float3 normalizedUp = float3_normalize(&up);

    float halfHeight = distance * tanf(fov * 0.5f);
    float halfWidth = halfHeight * aspectRatio;

    float3 worldPoint = {
        cameraPos->x + cameraFront->x * distance + cameraRight.x * (ndcX * halfWidth) + normalizedUp.x * (ndcY * halfHeight),
        cameraPos->y + cameraFront->y * distance + cameraRight.y * (ndcX * halfWidth) + normalizedUp.y * (ndcY * halfHeight),
        cameraPos->z + cameraFront->z * distance + cameraRight.z * (ndcX * halfWidth) + normalizedUp.z * (ndcY * halfHeight)
    };

    return worldPoint;
}

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

#define VECMATH_FLT_DEG_TO_RAD 0.01745329251994329577f
#define VECMATH_FLT_RAD_TO_DEG 57.29577951308232087684f
#define VECMATH_DBL_DEG_TO_RAD 0.01745329251994329577
#define VECMATH_DBL_RAD_TO_DEG 57.29577951308232087684

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// angle utilities
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float to_fradians(float degrees) {
    return degrees * VECMATH_FLT_DEG_TO_RAD;
}

VECMATH_FUNC float to_fdegrees(float radians) {
    return radians * VECMATH_FLT_RAD_TO_DEG;
}

VECMATH_FUNC float f_cos(float angle_radians) {
    return cosf(angle_radians);
}

VECMATH_FUNC float f_sin(float angle_radians) {
    return sinf(angle_radians);
}

VECMATH_FUNC float f_tan(float angle_radians) {
    return tanf(angle_radians);
}

VECMATH_FUNC double to_dradians(double degrees) {
    return degrees * VECMATH_DBL_DEG_TO_RAD;
}

VECMATH_FUNC double to_ddegrees(double radians) {
    return radians * VECMATH_DBL_RAD_TO_DEG;
}

VECMATH_FUNC double d_cos(double angle_radians) {
    return cos(angle_radians);
}

VECMATH_FUNC double d_sin(double angle_radians) {
    return sin(angle_radians);
}

VECMATH_FUNC double d_tan(double angle_radians) {
    return tan(angle_radians);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic utilities
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float f_power(float b, int e) 
{
    // edge cases
    if (e < 0) return 1.0f / f_power(b, -e);
    if (e == 0) return 1.0f;
    if (b == 0.0f) return 0.0f;
    if (b == 1.0f) return 1.0f;
    
    // exponentiation by squaring (O(log n))
    float result = 1.0f;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

VECMATH_FUNC double d_power(double b, int e)
{
    // edge cases
    if (e < 0) return 1.0 / d_power(b, -e);
    if (e == 0) return 1.0;
    if (b == 0.0) return 0.0;
    if (b == 1.0) return 1.0;
    
    // exponentiation by squaring (O(log n))
    double result = 1.0;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

VECMATH_FUNC float f_log10(const float x) {
    return log10f(x);
}

VECMATH_FUNC double d_log10(const double x) {
    return log10(x);
}

VECMATH_FUNC float f_log2(const float x) {
    return log2f(x);
}

VECMATH_FUNC double d_log2(const double x) {
    return log2(x);
}

VECMATH_FUNC float f_logn(const float x) {
    return logf(x);
}

VECMATH_FUNC double d_logn(const double x) {
    return log(x);
}

VECMATH_FUNC float f_floor(const float x) {
    return floorf(x);
}

VECMATH_FUNC double d_floor(const double x) {
    return floor(x);
}

VECMATH_FUNC float f_ceil(const float x) {
    return ceilf(x);
}

VECMATH_FUNC double d_ceil(const double x) {
    return ceil(x);
}

VECMATH_FUNC float f_frac(const float x) {
    return x - f_floor(x);
}

VECMATH_FUNC double d_frac(const double x) {
    return x - d_floor(x);
}

VECMATH_FUNC float f_round(const float x) {
    return roundf(x);
}

VECMATH_FUNC double d_round(const double x) {
    return round(x);
}

#ifdef __cplusplus 
}
#endif

#endif // VECMATH_IMPLEMENTATION
#endif // VECMATH_INCLUDED
