#ifndef VECMATH_UTIL_INCLUDED
#define VECMATH_UTIL_INCLUDED

#include "vecmath_defines.h"
#include "vecmath_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // VECMATH_UTIL_INCLUDED