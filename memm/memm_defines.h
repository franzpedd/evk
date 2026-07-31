#ifndef MEMM_DEFINES_INCLUDED
#define MEMM_DEFINES_INCLUDED

/* HEADER_ONLY_ASSEMBLER_START */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the building configuration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// build mode selection (only one should be defined)
// MEMM_HEADER_ONLY  - Header-only (all functions static inline)
// MEMM_SHARED       - Shared library (DLL/SO)
// MEMM_STATIC       - Static library (default)

#if defined(MEMM_SHARED)
    #if defined(_WIN32)
        #if defined(MEMM_EXPORT) 
            #define MEMM_API __declspec(dllexport)
        #else
            #define MEMM_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(MEMM_EXPORT)
            #define MEMM_API __attribute__((visibility("default")))
        #else
            #define MEMM_API
        #endif
    #else
        #define MEMM_API
    #endif
#else
    #define MEMM_API
#endif

#if defined(MEMM_HEADER_ONLY)
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define MEMM_FUNC static inline
    #else
        #define MEMM_FUNC static  // fallback for C89
    #endif
    #undef MEMM_API
    #define MEMM_API
    
#elif defined(MEMM_SHARED)
    #define MEMM_FUNC
#else
    #define MEMM_FUNC
#endif // MEMM_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// costumizable constants and validation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief sets how many allocations the system can hold information about
#ifndef MEMM_HASH_TABLE_SIZE
    #define MEMM_HASH_TABLE_SIZE 2048
#endif

/// @brief sets how many bytes the helper-macros string have at max
#ifndef MEMM_MAX_STRING_LENGTH
    #define MEMM_MAX_STRING_LENGTH 2048
#endif

/// @brief compile-time validation that size is power of 2
#if (MEMM_HASH_TABLE_SIZE & (MEMM_HASH_TABLE_SIZE - 1)) != 0
    #error "MEMM_HASH_TABLE_SIZE must be a power of 2 for hashing efficiency"
#endif

/// @brief enable/disable logging for memory error messages
#ifndef MEMM_DISABLE_LOGGING
    #define MEMM_ENABLE_LOGGING 1
#endif

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // MEMM_DEFINES_INCLUDED