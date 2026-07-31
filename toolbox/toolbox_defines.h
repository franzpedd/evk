#ifndef TOOLBOX_DEFINES_INCLUDED
#define TOOLBOX_DEFINES_INCLUDED

/* HEADER_ONLY_ASSEMBLER_START */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the building configuration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// build mode selection (only one should be defined)
// TOOLBOX_HEADER_ONLY  - Header-only (all functions static inline)
// TOOLBOX_SHARED       - Shared library (DLL/SO)
// TOOLBOX_STATIC       - Static library (default)

#if defined(TOOLBOX_SHARED)
    #if defined(_WIN32)
        #if defined(TOOLBOX_EXPORT) 
            #define TOOLBOX_API __declspec(dllexport)
        #else
            #define TOOLBOX_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(TOOLBOX_EXPORT)
            #define TOOLBOX_API __attribute__((visibility("default")))
        #else
            #define TOOLBOX_API
        #endif
    #else
        #define TOOLBOX_API
    #endif
#else
    #define TOOLBOX_API
#endif

#if defined(TOOLBOX_HEADER_ONLY)
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define TOOLBOX_FUNC static inline
    #else
        #define TOOLBOX_FUNC static  // fallback for C89
    #endif
    #undef TOOLBOX_API
    #define TOOLBOX_API
    
#elif defined(TOOLBOX_SHARED)
    #define TOOLBOX_FUNC
#else
    #define TOOLBOX_FUNC
#endif // TOOLBOX_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// custom option macros
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief defines the size of the static hash table
#ifndef SHASHTABLE_SIZE
    #define SHASHTABLE_SIZE 128
#endif

/// @brief maximum of 16.7 million IDs (2MB memory usage)
#ifndef IDGEN_MAX_SAFE_IDS
    #define IDGEN_MAX_SAFE_IDS 16777216 
#endif

/// @brief maximum size, using 32 bits for backwards compatibility
#ifndef SHASHTABLE_SIZE_MAX
    #define SHASHTABLE_SIZE_MAX 4294967295
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // TOOLBOX_DEFINES_INCLUDED