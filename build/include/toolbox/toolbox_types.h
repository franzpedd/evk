#ifndef TOOLBOX_TYPES_INCLUDED
#define TOOLBOX_TYPES_INCLUDED

#include "toolbox_defines.h"

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
// enumerations
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief various types of erros that may happen when using the library
typedef enum toolbox_result
{
    TOOLBOX_SUCCESS = 0,
    TOOLBOX_ERROR_INVALID_PARAM,
    TOOLBOX_ERROR_MEMORY_ALLOC,
    TOOLBOX_ERROR_OUT_OF_BOUNDS,
    TOOLBOX_ERROR_EMPTY,
    TOOLBOX_ERROR_NOT_FOUND,
    TOOLBOX_ERROR_FULL
} toolbox_result;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// custom allocation functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief callback for custom allocation memories
typedef void* (*malloc_func)(size_t size);
typedef void* (*calloc_func)(size_t num, size_t size);
typedef void (*free_func)(void* ptr);
typedef void* (*realloc_func)(void* ptr, size_t newSize);

/// @brief bundle-up the memory functions into one structure
typedef struct toolbox_memfuncs
{
    malloc_func malloc_fn;
    calloc_func calloc_fn;
    free_func free_fn;
    realloc_func realloc_fn;
} toolbox_memfuncs;

/// @brief export memory functions
TOOLBOX_API extern const toolbox_memfuncs TOOLBOX_DEFAULT_MEMFUNCS;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// opaque objects
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief opaque dynamic array structure
typedef struct darray darray;

/// @brief opaque id generator structure
typedef struct idgen idgen;

/// @brief opaque structure for the hash entry
typedef struct shash shash;

/// @brief opaque structure for the hash table
typedef struct shashtable shashtable;

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_TYPES_INCLUDED