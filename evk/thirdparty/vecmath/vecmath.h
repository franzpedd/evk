#ifndef VECMATH_INCLUDED
#define VECMATH_INCLUDED

#define VECMATH_REQUESTING_HEADER_ONLY // will make functions static 

// functions definitions

#ifndef VECMATH_TYPES_INCLUDED
#define VECMATH_TYPES_INCLUDED

/// @brief boolean type for checking true/false
typedef signed char vecbool;
#define vec_true 1
#define vec_false 0

/// @brief single precision unions
typedef union float2_t {
    struct { float x, y; } xy;
    struct { float u, v; } uv;
    float data[2];
} float2;

typedef union float3_t {
    struct { float x, y, z; } xyz;
    struct { float r, g, b; } rgb;
    float data[3];
} float3;

typedef union float4_t {
    struct { float x, y, z, w; } xyzw;
    struct { float r, g, b, a; } rgba;
    float data[4];
} float4;

typedef union fmat2_t {
    float data[2][2];
    struct { float2 c0; float2 c1; } column;
    struct { 
        float m00, m01; 
        float m10, m11; 
    } matrix;
} fmat2;

typedef union fmat3_t {
    float data[3][3];
    struct { float3 c0; float3 c1; float3 c2; } column;
    struct { 
        float m00, m01, m02; 
        float m10, m11, m12; 
        float m20, m21, m22; 
    } matrix;
} fmat3;

typedef union fmat4_t {
    float data[4][4];
    struct { float4 c0; float4 c1; float4 c2; float4 c3; } column;
    struct { 
        float m00, m01, m02, m03; 
        float m10, m11, m12, m13; 
        float m20, m21, m22, m23; 
        float m30, m31, m32, m33; 
    } matrix;
} fmat4;

typedef union fquat_t {
	float data[4];
	struct { float x, y, z, w; } vector;
} fquat;

/// @brief double precision unions
typedef union double2_t {
    struct { double x, y; } xy;
    struct { double u, v; } uv;
    double data[2];
} double2;

typedef union double3_t {
    struct { double x, y, z; } xyz;
    struct { double r, g, b; } rgb;
    double data[3];
} double3;

typedef union double4_t {
    struct { double x, y, z, w; } xyzw;
    struct { double r, g, b, a; } rgba;
    double data[4];
} double4;

typedef union dmat2_t {
    double data[2][2];
    struct { double2 c0; double2 c1; } column;
    struct { 
        double m00, m01; 
        double m10, m11; 
    } matrix;
} dmat2;

typedef union dmat3_t {
    double data[3][3];
    struct { double3 c0; double3 c1; double3 c2; } column;
    struct { 
        double m00, m01, m02; 
        double m10, m11, m12; 
        double m20, m21, m22; 
    } matrix;
} dmat3;

typedef union dmat4_t {
    double data[4][4];
    struct { double4 c0; double4 c1; double4 c2; double4 c3; } column;
    struct { 
        double m00, m01, m02, m03; 
        double m10, m11, m12, m13; 
        double m20, m21, m22, m23; 
        double m30, m31, m32, m33; 
    } matrix;
} dmat4;

typedef union dquat_t {
	double data[4];
	struct { double x, y, z, w; } vector;
} dquat;

typedef struct fray_t {
    float3 origin;
    float3 direction;
 } fray;

 typedef struct dray_t {
    double3 origin;
    double3 direction;
 } dray;

#endif // VECMATH_TYPES_INCLUDED
#ifndef VECMATH_DEFINES_INCLUDED
#define VECMATH_DEFINES_INCLUDED

/// @brief used macro definitions
#define VECMATH_EPSILON_FZERO 1e-6f
#define VECMATH_EPSILON_DZERO 1e-12
#define VECMATH_EPSILON_PI 3.14159265358979323846
#define VECMATH_EPSILON_FLT 1.192092896e-07F
#define VECMATH_EPSILON_INT_MIN (-2147483647 - 1)

/// @brief compilation options
#if defined(VECMATH_REQUESTING_HEADER_ONLY)
    #define VECMATH_API static inline // Header-only version - use static inline
    #undef VECMATH_BUILD_SHARED // header-only overrides shared library build
#elif defined(VECMATH_BUILD_SHARED) // Shared library build
    #if defined(_WIN32) || defined(_WIN64)
        #if defined(VECMATH_EXPORTS)
            #define VECMATH_API __declspec(dllexport)
        #else
            #define VECMATH_API __declspec(dllimport)
        #endif
    #elif defined(__linux__) && !defined(__ANDROID__)
        #define VECMATH_API __attribute__((visibility("default")))
    #else
        #define VECMATH_API
    #endif
#else
    #define VECMATH_API // static library
#endif

#endif // VECMATH_DEFINES_INCLUDED
#ifdef __cplusplus 
extern "C" {
#endif

/// @brief performs a+b
VECMATH_API float2 float2_add(const float2* a, const float2* b);
VECMATH_API float3 float3_add(const float3* a, const float3* b);
VECMATH_API float4 float4_add(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_add(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_add(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_add(const fmat4* a, const fmat4* b);
//
VECMATH_API double2 double2_add(const double2* a, const double2* b);
VECMATH_API double3 double3_add(const double3* a, const double3* b);
VECMATH_API double4 double4_add(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_add(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_add(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_add(const dmat4* a, const dmat4* b);

/// @brief performs a-b
VECMATH_API float2 float2_sub(const float2* a, const float2* b);
VECMATH_API float3 float3_sub(const float3* a, const float3* b);
VECMATH_API float4 float4_sub(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_sub(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_sub(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_sub(const fmat4* a, const fmat4* b);
//
VECMATH_API double2 double2_sub(const double2* a, const double2* b);
VECMATH_API double3 double3_sub(const double3* a, const double3* b);
VECMATH_API double4 double4_sub(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_sub(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_sub(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_sub(const dmat4* a, const dmat4* b);

/// @brief performs a * b
VECMATH_API float2 float2_mul(const float2* a, const float2* b);
VECMATH_API float3 float3_mul(const float3* a, const float3* b);
VECMATH_API float4 float4_mul(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_mul(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_mul(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_mul(const fmat4* a, const fmat4* b);
//
VECMATH_API double2 double2_mul(const double2* a, const double2* b);
VECMATH_API double3 double3_mul(const double3* a, const double3* b);
VECMATH_API double4 double4_mul(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_mul(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_mul(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_mul(const dmat4* a, const dmat4* b);

/// @brief performs a / b (let IEEE754 handle division by 0)
VECMATH_API float2 float2_div(const float2* a, const float2* b);
VECMATH_API float3 float3_div(const float3* a, const float3* b);
VECMATH_API float4 float4_div(const float4* a, const float4* b);
VECMATH_API fmat2 fmat2_div(const fmat2* a, const fmat2* b);
VECMATH_API fmat3 fmat3_div(const fmat3* a, const fmat3* b);
VECMATH_API fmat4 fmat4_div(const fmat4* a, const fmat4* b);
//
VECMATH_API double2 double2_div(const double2* a, const double2* b);
VECMATH_API double3 double3_div(const double3* a, const double3* b);
VECMATH_API double4 double4_div(const double4* a, const double4* b);
VECMATH_API dmat2 dmat2_div(const dmat2* a, const dmat2* b);
VECMATH_API dmat3 dmat3_div(const dmat3* a, const dmat3* b);
VECMATH_API dmat4 dmat4_div(const dmat4* a, const dmat4* b);

/// @brief checks if are exactly equals in value, this compare memory and is fast
VECMATH_API vecbool float2_equals(const float2* a, const float2* b);
VECMATH_API vecbool float3_equals(const float3* a, const float3* b);
VECMATH_API vecbool float4_equals(const float4* a, const float4* b);
VECMATH_API vecbool fmat2_equals(const fmat2* a, const fmat2* b);
VECMATH_API vecbool fmat3_equals(const fmat3* a, const fmat3* b);
VECMATH_API vecbool fmat4_equals(const fmat4* a, const fmat4* b);
//
VECMATH_API vecbool double2_equals(const double2* a, const double2* b);
VECMATH_API vecbool double3_equals(const double3* a, const double3* b);
VECMATH_API vecbool double4_equals(const double4* a, const double4* b);
VECMATH_API vecbool dmat2_equals(const dmat2* a, const dmat2* b);
VECMATH_API vecbool dmat3_equals(const dmat3* a, const dmat3* b);
VECMATH_API vecbool dmat4_equals(const dmat4* a, const dmat4* b);

/// @brief checks if almos-to-exactly equals in value, this compare value and is slow
VECMATH_API vecbool float2_aprox_equals(const float2* a, const float2* b);
VECMATH_API vecbool float3_aprox_equals(const float3* a, const float3* b);
VECMATH_API vecbool float4_aprox_equals(const float4* a, const float4* b);
VECMATH_API vecbool fmat2_aprox_equals(const fmat2* a, const fmat2* b);
VECMATH_API vecbool fmat3_aprox_equals(const fmat3* a, const fmat3* b);
VECMATH_API vecbool fmat4_aprox_equals(const fmat4* a, const fmat4* b);
//
VECMATH_API vecbool double2_aprox_equals(const double2* a, const double2* b);
VECMATH_API vecbool double3_aprox_equals(const double3* a, const double3* b);
VECMATH_API vecbool double4_aprox_equals(const double4* a, const double4* b);
VECMATH_API vecbool dmat2_aprox_equals(const dmat2* a, const dmat2* b);
VECMATH_API vecbool dmat3_aprox_equals(const dmat3* a, const dmat3* b);
VECMATH_API vecbool dmat4_aprox_equals(const dmat4* a, const dmat4* b);

#ifdef __cplusplus 
extern "C" {
#endif

/// @brief calculates the multiplication of a matrix
VECMATH_API float4 float4_mul_fmat4(const float4* v, const fmat4* m);
VECMATH_API double4 double4_mul_fmat4(const double4* v, const dmat4* m);

/// @ performs v * scalar whe n v is constant
VECMATH_API float2 float2_scalar(const float2* v, const float value);
VECMATH_API float3 float3_scalar(const float3* v, const float value);
VECMATH_API float4 float4_scalar(const float4* v, const float value);
VECMATH_API double2 double2_scalar(const double2* v, const float value);
VECMATH_API double3 double3_scalar(const double3* v, const float value);
VECMATH_API double4 double4_scalar(const double4* v, const float value);

/// @brief returns the length of the vector, (slower since performs sqrt)
VECMATH_API float float2_length(const float2* v);
VECMATH_API float float3_length(const float3* v);
VECMATH_API float float4_length(const float4* v);
VECMATH_API double double2_length(const double2* v);
VECMATH_API double double3_length(const double3* v);
VECMATH_API double double4_length(const double4* v);

/// @brief returns the length squared of the vector, (faster since doesn't call sqrt)
VECMATH_API float float2_length_sqrt(const float2* v);
VECMATH_API float float3_length_sqrt(const float3* v);
VECMATH_API float float4_length_sqrt(const float4* v);
VECMATH_API double double2_length_sqrt(const double2* v);
VECMATH_API double double3_length_sqrt(const double3* v);
VECMATH_API double double4_length_sqrt(const double4* v);

/// @brief normalizes the vectors, storing into result
VECMATH_API float2 float2_normalize(const float2* v);
VECMATH_API float3 float3_normalize(const float3* v);
VECMATH_API float4 float4_normalize(const float4* v);
VECMATH_API double2 double2_normalize(const double2* v);
VECMATH_API double3 double3_normalize(const double3* v);
VECMATH_API double4 double4_normalize(const double4* v);

/// @brief calculates the dot product
VECMATH_API float float2_dot(const float2* a, const float2* b);
VECMATH_API float float3_dot(const float3* a, const float3* b);
VECMATH_API float float4_dot(const float4* a, const float4* b);
VECMATH_API double double2_dot(const double2* a, const double2* b);
VECMATH_API double double3_dot(const double3* a, const double3* b);
VECMATH_API double double4_dot(const double4* a, const double4* b);

/// @brief calculates the cross product
VECMATH_API float float2_cross(const float2* a, const float2* b);
VECMATH_API float3 float3_cross(const float3* a, const float3* b);
VECMATH_API double double2_cross(const double2* a, const double2* b);
VECMATH_API double3 double3_cross(const double3* a, const double3* b);

/// @brief scales the vector v with scalar, storing into result
VECMATH_API float2 float2_scale(const float2* v, float scalar);
VECMATH_API float3 float3_scale(const float3* v, float scalar);
VECMATH_API float4 float4_scale(const float4* v, float scalar);
VECMATH_API double2 double2_scale(const double2* v, double scalar);
VECMATH_API double3 double3_scale(const double3* v, double scalar);
VECMATH_API double4 double4_scale(const double4* v, double scalar);

/// @brief performs linear interpolation between a and b, storing into result
VECMATH_API float2 float2_lerp(const float2* a, const float2* b, float t);
VECMATH_API float3 float3_lerp(const float3* a, const float3* b, float t);
VECMATH_API float4 float4_lerp(const float4* a, const float4* b, float t);
VECMATH_API double2 double2_lerp(const double2* a, const double2* b, double t);
VECMATH_API double3 double3_lerp(const double3* a, const double3* b, double t);
VECMATH_API double4 double4_lerp(const double4* a, const double4* b, double t);

/// @brief calculates the distance of two vectors, (slower since performs sqrt)
VECMATH_API float float2_distance(const float2* a, const float2* b);
VECMATH_API float float3_distance(const float3* a, const float3* b);
VECMATH_API float float4_distance(const float4* a, const float4* b);
VECMATH_API double double2_distance(const double2* a, const double2* b);
VECMATH_API double double3_distance(const double3* a, const double3* b);
VECMATH_API double double4_distance(const double4* a, const double4* b);

/// @brief calculates the distance squared of two vectors, (faster since doesn't call sqrt)
VECMATH_API float float2_distance_sqrt(const float2* a, const float2* b);
VECMATH_API float float3_distance_sqrt(const float3* a, const float3* b);
VECMATH_API float float4_distance_sqrt(const float4* a, const float4* b);
VECMATH_API double double2_distance_sqrt(const double2* a, const double2* b);
VECMATH_API double double3_distance_sqrt(const double3* a, const double3* b);
VECMATH_API double double4_distance_sqrt(const double4* a, const double4* b);

/// @brief calculates the reflection of a vector given it's normal, storing into result
VECMATH_API float2 float2_reflect(const float2* v, const float2* normal);
VECMATH_API float3 float3_reflect(const float3* v, const float3* normal);
VECMATH_API float4 float4_reflect(const float4* v, const float4* normal);
VECMATH_API double2 double2_reflect(const double2* v, const double2* normal);
VECMATH_API double3 double3_reflect(const double3* v, const double3* normal);
VECMATH_API double4 double4_reflect(const double4* v, const double4* normal);

/// @brief calculates the projection of two vectors, storing into result
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

/// @brief returns the identity matrix
VECMATH_API fmat2 fmat2_identity();
VECMATH_API fmat3 fmat3_identity();
VECMATH_API fmat4 fmat4_identity();
VECMATH_API dmat2 dmat2_identity();
VECMATH_API dmat3 dmat3_identity();
VECMATH_API dmat4 dmat4_identity();

/// @brief multiplies a matrix with a vector
VECMATH_API fmat2 fmat2_mul_float2(const fmat2* m, const float2* v);
VECMATH_API fmat3 fmat3_mul_float3(const fmat3* m, const float3* v);
VECMATH_API fmat4 fmat4_mul_float4(const fmat4* m, const float4* v);
VECMATH_API dmat2 dmat2_mul_double2(const dmat2* m, const double2* v);
VECMATH_API dmat3 dmat3_mul_double3(const dmat3* m, const double3* v);
VECMATH_API dmat4 dmat4_mul_double4(const dmat4* m, const double4* v);

/// @brief transposes the matrix/ flip matrix by changing row and columns
VECMATH_API fmat2 fmat2_transpose(const fmat2* m);
VECMATH_API fmat3 fmat3_transpose(const fmat3* m); 
VECMATH_API fmat4 fmat4_transpose(const fmat4* m);
VECMATH_API dmat2 dmat2_transpose(const dmat2* m);
VECMATH_API dmat3 dmat3_transpose(const dmat3* m);
VECMATH_API dmat4 dmat4_transpose(const dmat4* m);

/// @brief calculates the determinant of a matrix
VECMATH_API float fmat2_determinant(const fmat2* m);
VECMATH_API float fmat3_determinant(const fmat3* m);
VECMATH_API float fmat4_determinant(const fmat4* m);
VECMATH_API double dmat2_determinant(const dmat2* m);
VECMATH_API double dmat3_determinant(const dmat3* m);
VECMATH_API double dmat4_determinant(const dmat4* m);

/// @brief calculates the matrix inverse
VECMATH_API fmat2 fmat2_inverse(const fmat2* m);
VECMATH_API fmat3 fmat3_inverse(const fmat3* m);
VECMATH_API fmat4 fmat4_inverse(const fmat4* m);
VECMATH_API dmat2 dmat2_inverse(const dmat2* m);
VECMATH_API dmat3 dmat3_inverse(const dmat3* m);
VECMATH_API dmat4 dmat4_inverse(const dmat4* m);

/// @brief matrix decomposition to retrieve translation
VECMATH_API float3 fmat4_get_translation_rowmajor(const fmat4* m);
VECMATH_API float3 fmat4_get_translation_colmajor(const fmat4* m);
VECMATH_API double3 dmat4_get_translation_rowmajor(const dmat4* m);
VECMATH_API double3 dmat4_get_translation_colmajor(const dmat4* m);

/// @brief matrix decomposition to retrieve scale
VECMATH_API float3 fmat4_get_scale_rowmajor(const fmat4* m);
VECMATH_API float3 fmat4_get_scale_colmajor(const fmat4* m);
VECMATH_API double3 dmat4_get_scale_rowmajor(const dmat4* m);
VECMATH_API double3 dmat4_get_scale_colmajor(const dmat4* m);

/// @brief matrix decomposition to retrieve rotation (uses euler angles)
VECMATH_API float3 fmat4_get_rotation_rowmajor(const fmat4* m);
VECMATH_API float3 fmat4_get_rotation_colmajor(const fmat4* m);
VECMATH_API double3 dmat4_get_rotation_rowmajor(const dmat4* m);
VECMATH_API double3 dmat4_get_rotation_colmajor(const dmat4* m);

/// @brief fully decomposes the matrix into it's components
VECMATH_API void fmat4_decompose_rowmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale);
VECMATH_API void fmat4_decompose_colmajor(const fmat4* m, float3* translation, float3* rotation, float3* scale);
VECMATH_API void dmat4_decompose_rowmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale);
VECMATH_API void dmat4_decompose_colmajor(const dmat4* m, double3* translation, double3* rotation, double3* scale);

/// @brief matrix translation
VECMATH_API fmat4 fmat4_translate_rowmajor(const fmat4* m, const float3* dir);
VECMATH_API fmat4 fmat4_translate_colmajor(const fmat4* m, const float3* dir);
VECMATH_API dmat4 dmat4_translate_rowmajor(const dmat4* m, const double3* dir);
VECMATH_API dmat4 dmat4_translate_colmajor(const dmat4* m, const double3* dir);

/// @brief matrix rotating
VECMATH_API fmat4 fmat4_rotate_colmajor(const fmat4* m, float angle, const float3* axis);
VECMATH_API fmat4 fmat4_rotate_rowmajor(const fmat4* m, float angle, const float3* axis);
VECMATH_API dmat4 dmat4_rotate_colmajor(const dmat4* m, double angle, const double3* axis);
VECMATH_API dmat4 dmat4_rotate_rowmajor(const dmat4 *m, double angle, const double3* axis);

/// @brief matrix scaleing
VECMATH_API fmat4 fmat4_scale_rowmajor(const fmat4* m, const float3* dim);
VECMATH_API fmat4 fmat4_scale_colmajor(const fmat4* m, const float3* dim);
VECMATH_API dmat4 dmat4_scale_rowmajor(const dmat4* m, const double3* dim);
VECMATH_API dmat4 dmat4_scale_colmajor(const dmat4* m, const double3* dim);

/// @brief lookat taking into account multiple rendering api coordinates
VECMATH_API fmat4 fmat4_lookat_agnostic(const float3* eye, const float3* target, const float3* up);
VECMATH_API fmat4 fmat4_lookat_vulkan(const float3* eye, const float3* target, const float3* up);
VECMATH_API fmat4 fmat4_lookat_directx(const float3* eye, const float3* target, const float3* up);
VECMATH_API fmat4 fmat4_lookat_opengl(const float3* eye, const float3* target, const float3* up);
VECMATH_API dmat4 dmat4_lookat_agnostic(const double3* eye, const double3* target, const double3* up);
VECMATH_API dmat4 dmat4_lookat_vulkan(const double3* eye, const double3* target, const double3* up);
VECMATH_API dmat4 dmat4_lookat_directx(const double3* eye, const double3* target, const double3* up);
VECMATH_API dmat4 dmat4_lookat_opengl(const double3* eye, const double3* target, const double3* up);

/// @brief perspective projection taking into account multiple rendering api coordinates
VECMATH_API fmat4 fmat4_perspective_vulkan(float fov_rad, float aspect, float nearVal, float farVal);
VECMATH_API fmat4 fmat4_perspective_directx(float fov_rad, float aspect, float nearVal, float farVal);
VECMATH_API fmat4 fmat4_perspective_opengl(float fov_rad, float aspect, float nearVal, float farVal);
VECMATH_API dmat4 dmat4_perspective_vulkan(double fov_rad, double aspect, double nearVal, double farVal);
VECMATH_API dmat4 dmat4_perspective_directx(double fov_rad, double aspect, double nearVal, double farVal);
VECMATH_API dmat4 dmat4_perspective_opengl(double fov_rad, double aspect, double nearVal, double farVal);

/// @brief orthographic projection taking into account multiple rendering api coordinates
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

/// @brief returns identity quaternion
VECMATH_API fquat fquat_identity();
VECMATH_API dquat dquat_identity();

/// @brief multiply two quaternions
VECMATH_API fquat fquat_mul(const fquat* q1, const fquat* q2);
VECMATH_API dquat dquat_mul(const dquat* q1, const dquat* q2);

/// @brief returns the quaternion length
VECMATH_API float fquat_length(const fquat* q);
VECMATH_API double dquat_length(const dquat* q);

/// @brief quaternion conjugate (inverse for unit quaternions)
VECMATH_API fquat fquat_conjugate(const fquat* q);
VECMATH_API dquat dquat_conjugate(const dquat* q);

/// @brief normalizes the quaternion
VECMATH_API fquat fquat_normalize(const fquat* q);
VECMATH_API dquat dquat_normalize(const dquat* q);

/// @brief returns the dot product of two quaternions
VECMATH_API float fquat_dot(const fquat* q1, const fquat* q2);
VECMATH_API double dquat_dot(const dquat* q1, const dquat* q2);

/// @brief performs linear interpolation
VECMATH_API fquat fquat_lerp(const fquat* q1, const fquat* q2, float t);
VECMATH_API dquat dquat_lerp(const dquat* q1, const dquat* q2, double t);

/// @brief performs spherical interpolation
VECMATH_API fquat fquat_slerp(const fquat* q1, const fquat* q2, float t);
VECMATH_API dquat dquat_slerp(const dquat* q1, const dquat* q2, double t);

// create quaternion from axis-angle representation
VECMATH_API fquat fquat_from_euler(const float3* rad);
VECMATH_API dquat dquat_from_euler(const double3* d);

// converts a quaternion into euler angles (row=x, pitch=y, yaw=z)
VECMATH_API float3 fquat_to_euler(const fquat* q);
VECMATH_API double3 dquat_to_euler(const dquat* q);

/// @brief converts a quaternion to a matrix
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

/// @brief angle utilities
VECMATH_API float to_fradians(float degrees);
VECMATH_API float to_fdegrees(float radians);
VECMATH_API float f_cos(float degree);
VECMATH_API float f_sin(float degree);
VECMATH_API float f_tan(float degree);
//
VECMATH_API double to_dradians(double degrees);
VECMATH_API double to_ddegrees(double radians);
VECMATH_API double d_cos(double degree);
VECMATH_API double d_sin(double degree);
VECMATH_API double d_tan(double degree);

/// @brief clamp utilities
VECMATH_API float f_max(const float x, const float y);
VECMATH_API float f_min(const float x, const float y);
VECMATH_API float f_clamp(const float x, const float upper, const float lower);
//
VECMATH_API double d_max(const double x, const double y);
VECMATH_API double d_min(const double x, const double y);
VECMATH_API double d_clamp(const double x, const double upper, const double lower);

/// @brief calculates b^e smartly (OlogN)
VECMATH_API float f_power(float b, int e);
VECMATH_API double d_power(double b, int e);

/// @brief calculates the log2
VECMATH_API float f_log2(const float x);
VECMATH_API double d_log2(const double x);

/// @brief non-float double funcs
VECMATH_API int i_floor(const double x);

#ifdef __cplusplus 
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// @brief casts a ray from screen coordinate 2d point (usually mouse coordinates)
VECMATH_API fray fray_from_screen_point_vulkan(const float2* screenPos, const float2* windowSize, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp);

/// @brief returns the 3D point at a distance from a screen position (usually mouse coordinates)
VECMATH_API float3 fray_screen_to_world_point_vulkan(const float2* screenPos, const float2* windowSize, float distance, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp);

#ifdef __cplusplus
}
#endif

#endif // VECMATH_INCLUDED

/// @brief prevents circular dependency
#ifdef VECMATH_IMPLEMENTATION
#undef VECMATH_IMPLEMENTATION
#include "vecmath.c"
#endif
