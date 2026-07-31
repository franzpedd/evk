#ifndef EVK_DEFINES_INCLUDED
#define EVK_DEFINES_INCLUDED

/* HEADER_ONLY_ASSEMBLER_START */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the building configuration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// build mode selection (only one should be defined)
// EVK_HEADER_ONLY  - Header-only (all functions static inline)
// EVK_SHARED       - Shared library (DLL/SO)
// EVK_STATIC       - Static library (default)

#if defined(EVK_SHARED)
    #if defined(_WIN32)
        #if defined(EVK_EXPORT) 
            #define EVK_API __declspec(dllexport)
        #else
            #define EVK_API __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(EVK_EXPORT)
            #define EVK_API __attribute__((visibility("default")))
        #else
            #define EVK_API
        #endif
    #else
        #define EVK_API
    #endif
#else
    #define EVK_API
#endif

#if defined(EVK_HEADER_ONLY)
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define EVK_FUNC static inline
    #else
        #define EVK_FUNC static  // fallback for C89
    #endif
    #undef EVK_API
    #define EVK_API
    
#elif defined(EVK_SHARED)
    #define EVK_FUNC
#else
    #define EVK_FUNC
#endif // EVK_HEADER_ONLY

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// platform determination
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    
    #ifndef _WINDEF_
        struct HWND__;
        typedef struct HWND__* HWND;
    #endif

#elif defined(__APPLE__)
    #define VK_USE_PLATFORM_METAL_EXT
    #include <Metal/Metal.h>
    
    typedef struct objc_object CAMetalLayer;

#elif defined(__ANDROID__)
    #define VK_USE_PLATFORM_ANDROID_KHR
    #include <android/native_window.h>
    #include <android/log.h>
    
    typedef struct ANativeWindow ANativeWindow;

#elif defined(__linux__) && !defined(__ANDROID__)
    #ifdef EVK_LINUX_USE_XLIB
        #define VK_USE_PLATFORM_XLIB_KHR
        #include <X11/Xlib.h>
        
        #ifndef _XLIB_H_
            typedef struct _XDisplay Display;
            typedef unsigned long Window;
        #endif
        
    #elif defined(EVK_LINUX_USE_XCB)
        #define VK_USE_PLATFORM_XCB_KHR
        #include <xcb/xcb.h>
        
        #ifndef XCB_H
            typedef struct xcb_connection_t xcb_connection_t;
            typedef uint32_t xcb_window_t;
        #endif
        
    #else
        #define VK_USE_PLATFORM_WAYLAND_KHR
        #include <wayland-client.h>
        
        #ifndef WAYLAND_CLIENT_H
            typedef struct wl_display wl_display;
            typedef struct wl_surface wl_surface;
        #endif
    #endif
#endif

/// @brief including this so headeronly generation just reads it instead of forcing it there, also makes other including unncessary
#include <volk/volk.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// compiler align-as definition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus) && __cplusplus >= 201103L
    #define align_as(X) alignas(X)  // C++11 native
#elif defined(_MSC_VER)
    #define align_as(X) __declspec(align(X))
#elif defined(__GNUC__) || defined(__clang__)
    #define align_as(X) __attribute__((aligned(X)))
#else
    #define align_as(X) _Alignas(X)  // C11 native
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// general API definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief max size of characters an error message may have
#define EVK_MAX_ERROR_LEN 1024

/// @brief how many frames are simultaneously rendered
#define EVK_CONCURRENTLY_RENDERED_FRAMES 2

/// @brief how many push constants at max may exist for a given pipeline
#define EVK_PIPELINE_PUSH_CONSTANTS_MAX 8 

/// @brief how many descriptors sets at max a layout binding may have
#define EVK_PIPELINE_DESCRIPTOR_SET_LAYOUT_BINDING_MAX 32 

/// @brief how many shader stages a pipeline may have, since we only support Vertex and Fragment for now, 2
#define EVK_PIPELINE_SHADER_STAGES_COUNT 2

/// @brief macro for getting the size of a static array, DON'T USE ON PTR
#define EVK_STATIC_ARRAY_SIZE(ARR) ((int32_t)(sizeof(ARR) / sizeof(*(ARR))))

/// @brief bit-packed version encoding macro
#define EVK_MAKE_VERSION(variant, major, minor, patch) ((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

/// @brief name of pipelines for easy hashtable lookup
#define EVK_PIPELINE_SPRITE_DEFAULT_NAME "SPRITE:DEFAULT"
#define EVK_PIPELINE_SPRITE_PICKING_NAME "SPRITE:PICKING"
#define EVK_PIPELINE_BILLBOARD_DEFAULT_NAME "BILLBOARD:DEFAULT"
#define EVK_PIPELINE_BILLBOARD_PICKING_NAME "BILLBOARD:PICKING"
#define EVK_PIPELINE_GRID_DEFAULT_NAME "GRID:DEFAULT"

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // EVK_DEFINES_INCLUDED