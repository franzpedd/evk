#include "vecmath_util.h"
#include <math.h>

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */
