#ifndef VECMATH_DEFINES_INCLUDED
#define VECMATH_DEFINES_INCLUDED

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // VECMATH_DEFINES_INCLUDED