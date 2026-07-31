#ifndef VECMATH_TYPES_INCLUDED
#define VECMATH_TYPES_INCLUDED

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // VECMATH_TYPES_INCLUDED