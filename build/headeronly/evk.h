#ifndef EVK_INCLUDED
#define EVK_INCLUDED
#define EVK_HEADER_ONLY

#include "memm.h"
#include "toolbox.h"
#include "vecmath.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project: EVK-definition
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

/// @brief all types of error that a function may return
typedef enum evkResult
{
    evk_Success = 0,
    evk_Failure
} evkResult;

/// @brief all types of severity a log message may have
typedef enum evkSeverity
{
	evk_Trace = 0,
	evk_Todo,
	evk_Info,
	evk_Warn,
	evk_Error,
	evk_Fatal
} evkSeverity;

/// @brief all diretions the camera can be moved towards
typedef enum evkCameraDir
{
	evk_Camera_Direction_Forward,
	evk_Camera_Direction_Backward,
	evk_Camera_Direction_Left,
	evk_Camera_Direction_Right
} evkCameraDir;

/// @brief all supported msaa sample ammout available
typedef enum evkMSAA
{
	evk_Msaa_Off    = 0x00000001,
	evk_Msaa_X2     = 0x00000002,
	evk_Msaa_X4     = 0x00000004,
	evk_Msaa_X8     = 0x00000008,
	evk_Msaa_X16    = 0x00000010,
	evk_Msaa_X32    = 0x00000020,
	evk_Msaa_X64    = 0x00000040
} evkMSAA;

/// @brief all shader types for loading
typedef enum evkShaderType
{
	evk_Shader_Type_Vertex = 0,
	evk_Shader_Type_Fragment,
	evk_Shader_Type_Compute,
	evk_Shader_Type_Geometry,
	evk_Shader_Type_Tess_Ctrl,
	evk_Shader_Type_Tess_Eval
} evkShaderType;

/// @brief all supported vertex components
typedef enum evkVertexComponent
{
	evk_Vertex_Component_Position = 0,
	evk_Vertex_Component_Normal,
	evk_Vertex_Component_UV_0,
	evk_Vertex_Component_Color_0,
	evk_Vertex_Component_Joints_0,
	evk_Vertex_Component_Weights_0,

	evk_Vertex_Component_Max
} evkVertexComponent;

/// @brief all renderphases 
typedef enum evkRenderphaseType
{
	evk_Renderphase_Type_Main,
	evk_Renderphase_Type_Picking,
	evk_Renderphase_Type_UI,
	evk_Renderphase_Type_Viewport
} evkRenderphaseType;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// opaque objects
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief definition of the context structure
typedef struct evkContext evkContext;

/// @brief definition of the vulkan context structure
typedef struct evkVulkanBackend evkVulkanBackend;

/// @brief definition of the camera structure
typedef struct evkCamera evkCamera;

/// @brief definition of the grid structure
typedef struct evkGrid evkGrid;

/// @brief definition of the 2D texture structure
typedef struct evkTexture2D evkTexture2D;

/// @brief definition of the sprite structure
typedef struct evkSprite evkSprite;

/// @brief definition of the billboard structure
typedef struct evkBillboard evkBillboard;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// structures
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds information about a particular vertex
typedef struct evkVertex
{
	align_as(16) float3 position;
	align_as(16) float3 normal;
	align_as(8)  float2 uv_0;
	align_as(16) float4 color_0;
	align_as(16) float4 joints_0;
	align_as(16) float4 weights_0;
} evkVertex;

/// @brief holds information about the push constant sent to gpu
typedef struct evkPushConstant
{
	align_as(4) unsigned int id;
	align_as(16) fmat4 model;
} evkPushConstant;

/// @brief holds information about a camera data sent to to gpu
typedef struct evkCameraUBO
{
	align_as(16) fmat4 view;
	align_as(16) fmat4 viewInverse;
	align_as(16) fmat4 proj;
} evkCameraUBO;

/// @brief holds information about a sprite data sent to gpu
typedef struct evkSpriteUBO
{
	align_as(4) float uv_rotation;	// rotates the uv textures
	align_as(8) float2 uv_offset;	// used to offset the uv textures
	align_as(8) float2 uv_scale;	// used to scale the uv textures
} evkSpriteUBO;

/// @brief holds information about a billboard data sent to gpu
typedef struct evkBillboardUBO
{
	align_as(4) unsigned int lockmode;	// 0 = locks x; 1 = locks y; any other number locks both
	align_as(4) float uv_rotation;		// rotates the uv textures
	align_as(8) float2 uv_offset;		// used to offset the uv textures
	align_as(8) float2 uv_scale;		// used to scale the uv textures
} evkBillboardUBO;

/// @brief holds information about the window the API will be displaying to
typedef struct evkWindow
{
	#ifdef _WIN32
		HWND window;
	#elif defined(__APPLE__)
		CAMetalLayer* layer;
	#elif defined(__ANDROID__)
		ANativeWindow* window;
	#elif defined(EVK_LINUX_USE_XLIB)
		Display* display;
		Window window;
	#elif defined(EVK_LINUX_USE_XCB)
		xcb_connection_t* connection;
		xcb_window_t window;
	#elif defined(__linux__) && !defined(__ANDROID__) && !defined(EVK_LINUX_USE_XLIB) && !defined(EVK_LINUX_USE_XCB)
		wl_display* display;
		wl_surface* surface;
	#endif
} evkWindow;

/// @brief initial arguments necessary to properly initialize the API
typedef struct evkCreateInfo
{
	const char* appName;
	const char* engineName;
	unsigned int engineVersion;
	unsigned int appVersion;
	unsigned int width;
	unsigned int height;
	evkMSAA MSAA;
	bool vsync;
	bool viewport;
	bool validations;
	evkWindow window;
} evkCreateInfo;

/// @brief helpfull struct to hold info about the necessary gpu queues
typedef struct evkQueueFamily
{
	unsigned int graphics;
	unsigned int present;
	unsigned int compute;
	bool graphicsFound;
	bool presentFound;
	bool computeFound;
} evkQueueFamily;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// callbacks
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief definition of the render callback
typedef void (*evkCallback_Render)(evkContext* context, float timestep);

/// @brief definition of the render ui callback
typedef void (*evkCalllback_RenderUI)(evkContext* context, void* rawCmdBuffer);


#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// context
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief initializes the api
EVK_API evkResult evk_init(const evkCreateInfo* ci);

/// @brief shuts down the api, releases all resources 
EVK_API evkResult evk_shutdown();

/// @brief send objects to gpu based on timestep
EVK_API void evk_update(float timestep);

/// @brief renders the frame, starting the render commands and eventually calling back when it's time to render objects
EVK_API void evk_render(float timestep);

/// @brief resizes the renderer
EVK_API void evk_resize(float2 size);

/// @brief resizes the renderer viewport, if it's enabled
EVK_API void evk_resize_viewport(float2 size);

/// @brief minimizes the renderer/stops the rendering process
EVK_API void evk_minimize();

/// @brief restores the renderer to it's previous size before it's minimization/continues the rendering process
EVK_API void evk_restore();

/// @brief returns the id of an object underneath a given xy coordinates
EVK_API unsigned int evk_pick_object(float2 xy);

/// @brief returns the global context, used for external functions
EVK_API evkContext* evk_get_context();

/// @brief returns the main camera, object created to facilitate usage of the api
EVK_API evkCamera* evk_get_main_camera();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// general getters/setters
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief returns if evk was created with vsync enabled
EVK_API bool evk_using_vsync();

/// @brief returns if evk was created with a viewport enabled
EVK_API bool evk_using_viewport();

/// @brief returns if the renderer is currently minimized
EVK_API bool evk_currently_minimized();

/// @brief returns the msaa used at the momment
EVK_API evkMSAA evk_get_msaa();

/// @brief returns the size of the renderer framebuffer
EVK_API float2 evk_get_size();

/// @brief if using viewport, returns it's size
EVK_API float2 evk_get_viewport_size();

/// @brief if using viewport, change it's size 
EVK_API void evk_set_viewport_size(const float2 size);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// callback related
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief sets the user defined pointer address
EVK_API void evk_set_user_pointer(void* pointer);

/// @brief returns the user defined pointer address
EVK_API void* evk_get_user_pointer();

/// @brief sets a function to be called when it's time to draw
EVK_API void evk_set_render_callback(evkCallback_Render callback);

/// @brief returns the function address responsible for issue the rendering of objects
EVK_API evkCallback_Render evk_get_render_callback();

/// @brief sets a function to be called when it's time to draw ui
EVK_API void evk_set_renderui_callback(evkCalllback_RenderUI callback);

/// @brief returns the function address responsible for issue the rendering of ui
EVK_API evkCalllback_RenderUI evk_get_renderui_callback();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// logs and errors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief general logging function
EVK_API void evk_log_message(evkSeverity severity, const char* file, unsigned int line, const char* fmt, ...);

/// @brief log macro 
#ifdef EVK_ENABLE_VALIDATIONS
	#define EVK_LOG(severity, ...) evk_log_message(severity, __FILE__, __LINE__, __VA_ARGS__);
	#define EVK_ASSERT(condition, ...) if (!(condition)) { evk_log_message(evk_Fatal, __FILE__, __LINE__, __VA_ARGS__); }
#else
	#define EVK_LOG(...)
	#define EVK_ASSERT(condition, msg) ((void)0)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// camera related
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates and returns a camera
EVK_API evkCamera* evk_camera_create(float aspectRatio);

/// @brief releases all resources used by the camera
EVK_API void evk_camera_destroy(evkCamera* camera);

/// @brief updates the camera logic for the frame
EVK_API void evk_camera_update(evkCamera* camera, float timestep);

/// @brief sets a new aspect ratio for the camera
EVK_API void evk_camera_set_aspect_ratio(evkCamera* camera, float aspect);

/// @brief returns the camera's current aspect ratio
EVK_API float evk_camera_get_aspect_ratio(evkCamera* camera);

/// @brief returns the camera's current field of view
EVK_API float evk_camera_get_fov(evkCamera* camera);

/// @brief applys a translation to the camera's view, moving the camera
EVK_API void evk_camera_translate(evkCamera* camera, float3 dir);

/// @brief applys a rotation to the camera's view, rotating the camera
EVK_API void evk_camera_rotate(evkCamera* camera, float3 dir);

/// @brief returns the camera's view matrix
EVK_API fmat4 evk_camera_get_view(evkCamera* camera);

/// @brief returns the camera's inverse view matrix
EVK_API fmat4 evk_camera_get_view_inverse(evkCamera* camera);

/// @brief returns the camera's perspective projection matrix
EVK_API fmat4 evk_camera_get_perspective(evkCamera* camera);

/// @brief returns the camera's inverse perspective projection matrix
EVK_API fmat4 evk_camera_get_perspective_inverse(evkCamera* camera);

/// @brief enables/disables the camera movement
EVK_API void evk_camera_set_lock(evkCamera* camera, bool value);

/// @brief returns if camera can currently move
EVK_API bool evk_camera_get_lock(evkCamera* camera);

/// @brief moves/stops moving the camera towards a direction
EVK_API void evk_camera_move(evkCamera* camera, evkCameraDir dir, bool moving);

/// @brief returns if the camera speed modifier is currently applyed
EVK_API bool evk_camera_get_speed_modifier(evkCamera* camera, float* value);

/// @brief sets/unsets the camera's speed modifier
EVK_API void evk_camera_set_speed_modifier(evkCamera* camera, bool status, float value);

/// @brief returns the camera's current 3d position
EVK_API float3 evk_camera_get_position(evkCamera* camera);

/// @brief returns the camera's rotation vector
EVK_API float3 evk_camera_get_rotation(evkCamera* camera);

/// @brief returns the camera's current 3d front position
EVK_API float3 evk_camera_get_front(evkCamera* camera);

#ifdef __cplusplus 
}
#endif


#ifdef EVK_IMPLEMENTATION
#undef EVK_IMPLEMENTATION

#define MEMM_IMPLEMENTATION
#include "memm.h"
#define TOOLBOX_IMPLEMENTATION
#include "toolbox.h"
#define VECMATH_IMPLEMENTATION
#include "vecmath.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define VOLK_IMPLEMENTATION
#include <volk/volk.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project: EVK-implementation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// Stage: vert
const unsigned int billboard_default_vert_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x0000023d, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0008000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x0000012a, 0x00000133, 0x00000140, 
    0x00030047, 0x00000113, 0x00000002, 0x00050048, 0x00000113, 0x00000000, 0x00000023, 0x00000000, 
    0x00040048, 0x00000113, 0x00000001, 0x00000005, 0x00050048, 0x00000113, 0x00000001, 0x00000007, 
    0x00000010, 0x00050048, 0x00000113, 0x00000001, 0x00000023, 0x00000010, 0x00030047, 0x00000119, 
    0x00000002, 0x00040048, 0x00000119, 0x00000000, 0x00000005, 0x00050048, 0x00000119, 0x00000000, 
    0x00000007, 0x00000010, 0x00050048, 0x00000119, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 
    0x00000119, 0x00000001, 0x00000005, 0x00050048, 0x00000119, 0x00000001, 0x00000007, 0x00000010, 
    0x00050048, 0x00000119, 0x00000001, 0x00000023, 0x00000040, 0x00040048, 0x00000119, 0x00000002, 
    0x00000005, 0x00050048, 0x00000119, 0x00000002, 0x00000007, 0x00000010, 0x00050048, 0x00000119, 
    0x00000002, 0x00000023, 0x00000080, 0x00040047, 0x0000011b, 0x00000021, 0x00000000, 0x00040047, 
    0x0000011b, 0x00000022, 0x00000000, 0x00030047, 0x0000011f, 0x00000002, 0x00050048, 0x0000011f, 
    0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000011f, 0x00000001, 0x00000023, 0x00000004, 
    0x00050048, 0x0000011f, 0x00000002, 0x00000023, 0x00000008, 0x00050048, 0x0000011f, 0x00000003, 
    0x00000023, 0x00000010, 0x00040047, 0x00000121, 0x00000021, 0x00000001, 0x00040047, 0x00000121, 
    0x00000022, 0x00000000, 0x00030047, 0x00000128, 0x00000002, 0x00050048, 0x00000128, 0x00000000, 
    0x0000000b, 0x00000000, 0x00050048, 0x00000128, 0x00000001, 0x0000000b, 0x00000001, 0x00050048, 
    0x00000128, 0x00000002, 0x0000000b, 0x00000003, 0x00050048, 0x00000128, 0x00000003, 0x0000000b, 
    0x00000004, 0x00040047, 0x00000133, 0x0000000b, 0x0000002a, 0x00040047, 0x00000140, 0x0000001e, 
    0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 
    0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040018, 0x00000008, 0x00000007, 
    0x00000004, 0x00040015, 0x00000009, 0x00000020, 0x00000000, 0x00040017, 0x00000011, 0x00000006, 
    0x00000003, 0x0004002b, 0x00000009, 0x00000012, 0x00000006, 0x0004001c, 0x00000013, 0x00000011, 
    0x00000012, 0x0004002b, 0x00000006, 0x00000016, 0xbf000000, 0x0004002b, 0x00000006, 0x00000017, 
    0x00000000, 0x0006002c, 0x00000011, 0x00000018, 0x00000016, 0x00000016, 0x00000017, 0x0004002b, 
    0x00000006, 0x00000019, 0x3f000000, 0x0006002c, 0x00000011, 0x0000001a, 0x00000019, 0x00000016, 
    0x00000017, 0x0006002c, 0x00000011, 0x0000001b, 0x00000019, 0x00000019, 0x00000017, 0x0006002c, 
    0x00000011, 0x0000001c, 0x00000016, 0x00000019, 0x00000017, 0x0009002c, 0x00000013, 0x0000001d, 
    0x00000018, 0x0000001a, 0x0000001b, 0x0000001b, 0x0000001c, 0x00000018, 0x00040017, 0x0000001e, 
    0x00000006, 0x00000002, 0x0004001c, 0x0000001f, 0x0000001e, 0x00000012, 0x0004002b, 0x00000006, 
    0x00000022, 0x3f800000, 0x0005002c, 0x0000001e, 0x00000023, 0x00000017, 0x00000022, 0x0005002c, 
    0x0000001e, 0x00000024, 0x00000022, 0x00000022, 0x0005002c, 0x0000001e, 0x00000025, 0x00000022, 
    0x00000017, 0x0005002c, 0x0000001e, 0x00000026, 0x00000017, 0x00000017, 0x0009002c, 0x0000001f, 
    0x00000027, 0x00000023, 0x00000024, 0x00000025, 0x00000025, 0x00000026, 0x00000023, 0x00040020, 
    0x00000028, 0x00000007, 0x00000011, 0x00040015, 0x0000002a, 0x00000020, 0x00000001, 0x0004002b, 
    0x0000002a, 0x0000002b, 0x00000000, 0x0004002b, 0x0000002a, 0x0000002f, 0x00000001, 0x0004002b, 
    0x0000002a, 0x00000033, 0x00000002, 0x0006002c, 0x00000011, 0x0000003d, 0x00000017, 0x00000022, 
    0x00000017, 0x0004002b, 0x00000009, 0x00000046, 0x00000000, 0x00020014, 0x00000047, 0x0004002b, 
    0x00000009, 0x0000004c, 0x00000001, 0x0006002c, 0x00000011, 0x00000055, 0x00000022, 0x00000017, 
    0x00000017, 0x0004001e, 0x00000113, 0x00000009, 0x00000008, 0x00040020, 0x00000114, 0x00000009, 
    0x00000113, 0x0004003b, 0x00000114, 0x00000115, 0x00000009, 0x00040020, 0x00000116, 0x00000009, 
    0x00000008, 0x0005001e, 0x00000119, 0x00000008, 0x00000008, 0x00000008, 0x00040020, 0x0000011a, 
    0x00000002, 0x00000119, 0x0004003b, 0x0000011a, 0x0000011b, 0x00000002, 0x00040020, 0x0000011c, 
    0x00000002, 0x00000008, 0x0006001e, 0x0000011f, 0x00000009, 0x00000006, 0x0000001e, 0x0000001e, 
    0x00040020, 0x00000120, 0x00000002, 0x0000011f, 0x0004003b, 0x00000120, 0x00000121, 0x00000002, 
    0x00040020, 0x00000123, 0x00000002, 0x00000009, 0x0004001c, 0x00000127, 0x00000006, 0x0000004c, 
    0x0006001e, 0x00000128, 0x00000007, 0x00000006, 0x00000127, 0x00000127, 0x00040020, 0x00000129, 
    0x00000003, 0x00000128, 0x0004003b, 0x00000129, 0x0000012a, 0x00000003, 0x00040020, 0x00000132, 
    0x00000001, 0x0000002a, 0x0004003b, 0x00000132, 0x00000133, 0x00000001, 0x00040020, 0x0000013d, 
    0x00000003, 0x00000007, 0x00040020, 0x0000013f, 0x00000003, 0x0000001e, 0x0004003b, 0x0000013f, 
    0x00000140, 0x00000003, 0x00040020, 0x0000022c, 0x00000007, 0x00000013, 0x00040020, 0x0000022d, 
    0x00000007, 0x0000001f, 0x00040020, 0x0000022e, 0x00000007, 0x0000001e, 0x00050036, 0x00000002, 
    0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003b, 0x0000022d, 0x00000021, 
    0x00000007, 0x0004003b, 0x0000022c, 0x00000015, 0x00000007, 0x0003003e, 0x00000015, 0x0000001d, 
    0x0003003e, 0x00000021, 0x00000027, 0x00050041, 0x00000116, 0x00000117, 0x00000115, 0x0000002f, 
    0x0004003d, 0x00000008, 0x00000118, 0x00000117, 0x00050041, 0x0000011c, 0x0000011d, 0x0000011b, 
    0x0000002b, 0x0004003d, 0x00000008, 0x0000011e, 0x0000011d, 0x00050041, 0x00000123, 0x00000124, 
    0x00000121, 0x0000002b, 0x0004003d, 0x00000009, 0x00000125, 0x00000124, 0x000300f7, 0x0000022a, 
    0x00000000, 0x000300fb, 0x00000046, 0x0000015d, 0x000200f8, 0x0000015d, 0x00050051, 0x00000007, 
    0x0000015e, 0x00000118, 0x00000000, 0x0008004f, 0x00000011, 0x0000015f, 0x0000015e, 0x0000015e, 
    0x00000000, 0x00000001, 0x00000002, 0x0006000c, 0x00000006, 0x00000160, 0x00000001, 0x00000042, 
    0x0000015f, 0x00050051, 0x00000007, 0x00000161, 0x00000118, 0x00000001, 0x0008004f, 0x00000011, 
    0x00000162, 0x00000161, 0x00000161, 0x00000000, 0x00000001, 0x00000002, 0x0006000c, 0x00000006, 
    0x00000163, 0x00000001, 0x00000042, 0x00000162, 0x00050051, 0x00000007, 0x00000164, 0x00000118, 
    0x00000002, 0x0008004f, 0x00000011, 0x00000165, 0x00000164, 0x00000164, 0x00000000, 0x00000001, 
    0x00000002, 0x0006000c, 0x00000006, 0x00000166, 0x00000001, 0x00000042, 0x00000165, 0x00050051, 
    0x00000007, 0x00000168, 0x00000118, 0x00000003, 0x00060051, 0x00000006, 0x0000016a, 0x0000011e, 
    0x00000000, 0x00000002, 0x00060051, 0x00000006, 0x0000016b, 0x0000011e, 0x00000001, 0x00000002, 
    0x00060051, 0x00000006, 0x0000016c, 0x0000011e, 0x00000002, 0x00000002, 0x00060050, 0x00000011, 
    0x0000016d, 0x0000016a, 0x0000016b, 0x0000016c, 0x0006000c, 0x00000011, 0x0000016e, 0x00000001, 
    0x00000045, 0x0000016d, 0x000500aa, 0x00000047, 0x00000170, 0x00000125, 0x00000046, 0x000300f7, 
    0x000001ef, 0x00000000, 0x000400fa, 0x00000170, 0x00000171, 0x000001ae, 0x000200f8, 0x00000171, 
    0x00050051, 0x00000006, 0x00000173, 0x0000016e, 0x00000001, 0x00050051, 0x00000006, 0x00000175, 
    0x0000016e, 0x00000002, 0x00060050, 0x00000011, 0x00000176, 0x00000017, 0x00000173, 0x00000175, 
    0x0006000c, 0x00000011, 0x00000177, 0x00000001, 0x00000045, 0x00000176, 0x0007000c, 0x00000011, 
    0x0000017a, 0x00000001, 0x00000044, 0x00000055, 0x00000177, 0x0006000c, 0x00000011, 0x0000017b, 
    0x00000001, 0x00000045, 0x0000017a, 0x0005008e, 0x00000011, 0x0000017f, 0x00000055, 0x00000160, 
    0x00050051, 0x00000006, 0x00000180, 0x0000017f, 0x00000000, 0x00050051, 0x00000006, 0x00000181, 
    0x0000017f, 0x00000001, 0x00050051, 0x00000006, 0x00000182, 0x0000017f, 0x00000002, 0x0005008e, 
    0x00000011, 0x00000187, 0x0000017b, 0x00000163, 0x00050051, 0x00000006, 0x00000188, 0x00000187, 
    0x00000000, 0x00050051, 0x00000006, 0x00000189, 0x00000187, 0x00000001, 0x00050051, 0x00000006, 
    0x0000018a, 0x00000187, 0x00000002, 0x0005008e, 0x00000011, 0x0000018f, 0x00000177, 0x00000166, 
    0x00050051, 0x00000006, 0x00000190, 0x0000018f, 0x00000000, 0x00050051, 0x00000006, 0x00000191, 
    0x0000018f, 0x00000001, 0x00050051, 0x00000006, 0x00000192, 0x0000018f, 0x00000002, 0x00050051, 
    0x00000006, 0x00000195, 0x00000168, 0x00000000, 0x00050051, 0x00000006, 0x00000196, 0x00000168, 
    0x00000001, 0x00050051, 0x00000006, 0x00000197, 0x00000168, 0x00000002, 0x00070050, 0x00000007, 
    0x000001a9, 0x00000180, 0x00000181, 0x00000182, 0x00000017, 0x00070050, 0x00000007, 0x000001aa, 
    0x00000188, 0x00000189, 0x0000018a, 0x00000017, 0x00070050, 0x00000007, 0x000001ab, 0x00000190, 
    0x00000191, 0x00000192, 0x00000017, 0x00070050, 0x00000007, 0x000001ac, 0x00000195, 0x00000196, 
    0x00000197, 0x00000022, 0x00070050, 0x00000008, 0x000001ad, 0x000001a9, 0x000001aa, 0x000001ab, 
    0x000001ac, 0x000200f9, 0x0000022a, 0x000200f8, 0x000001ae, 0x000500aa, 0x00000047, 0x000001b0, 
    0x00000125, 0x0000004c, 0x000300f7, 0x000001ee, 0x00000000, 0x000400fa, 0x000001b0, 0x000001b1, 
    0x000001ee, 0x000200f8, 0x000001b1, 0x00050051, 0x00000006, 0x000001b3, 0x0000016e, 0x00000000, 
    0x00050051, 0x00000006, 0x000001b5, 0x0000016e, 0x00000002, 0x00060050, 0x00000011, 0x000001b6, 
    0x000001b3, 0x00000017, 0x000001b5, 0x0006000c, 0x00000011, 0x000001b7, 0x00000001, 0x00000045, 
    0x000001b6, 0x0007000c, 0x00000011, 0x000001ba, 0x00000001, 0x00000044, 0x0000003d, 0x000001b7, 
    0x0006000c, 0x00000011, 0x000001bb, 0x00000001, 0x00000045, 0x000001ba, 0x0005008e, 0x00000011, 
    0x000001bf, 0x000001bb, 0x00000160, 0x00050051, 0x00000006, 0x000001c0, 0x000001bf, 0x00000000, 
    0x00050051, 0x00000006, 0x000001c1, 0x000001bf, 0x00000001, 0x00050051, 0x00000006, 0x000001c2, 
    0x000001bf, 0x00000002, 0x0005008e, 0x00000011, 0x000001c7, 0x0000003d, 0x00000163, 0x00050051, 
    0x00000006, 0x000001c8, 0x000001c7, 0x00000000, 0x00050051, 0x00000006, 0x000001c9, 0x000001c7, 
    0x00000001, 0x00050051, 0x00000006, 0x000001ca, 0x000001c7, 0x00000002, 0x0005008e, 0x00000011, 
    0x000001cf, 0x000001b7, 0x00000166, 0x00050051, 0x00000006, 0x000001d0, 0x000001cf, 0x00000000, 
    0x00050051, 0x00000006, 0x000001d1, 0x000001cf, 0x00000001, 0x00050051, 0x00000006, 0x000001d2, 
    0x000001cf, 0x00000002, 0x00050051, 0x00000006, 0x000001d5, 0x00000168, 0x00000000, 0x00050051, 
    0x00000006, 0x000001d6, 0x00000168, 0x00000001, 0x00050051, 0x00000006, 0x000001d7, 0x00000168, 
    0x00000002, 0x00070050, 0x00000007, 0x000001e9, 0x000001c0, 0x000001c1, 0x000001c2, 0x00000017, 
    0x00070050, 0x00000007, 0x000001ea, 0x000001c8, 0x000001c9, 0x000001ca, 0x00000017, 0x00070050, 
    0x00000007, 0x000001eb, 0x000001d0, 0x000001d1, 0x000001d2, 0x00000017, 0x00070050, 0x00000007, 
    0x000001ec, 0x000001d5, 0x000001d6, 0x000001d7, 0x00000022, 0x00070050, 0x00000008, 0x000001ed, 
    0x000001e9, 0x000001ea, 0x000001eb, 0x000001ec, 0x000200f9, 0x0000022a, 0x000200f8, 0x000001ee, 
    0x000200f9, 0x000001ef, 0x000200f8, 0x000001ef, 0x0007000c, 0x00000011, 0x000001f2, 0x00000001, 
    0x00000044, 0x0000003d, 0x0000016e, 0x0006000c, 0x00000011, 0x000001f3, 0x00000001, 0x00000045, 
    0x000001f2, 0x0007000c, 0x00000011, 0x000001f6, 0x00000001, 0x00000044, 0x0000016e, 0x000001f3, 
    0x0006000c, 0x00000011, 0x000001f7, 0x00000001, 0x00000045, 0x000001f6, 0x0005008e, 0x00000011, 
    0x000001fb, 0x000001f3, 0x00000160, 0x00050051, 0x00000006, 0x000001fc, 0x000001fb, 0x00000000, 
    0x00050051, 0x00000006, 0x000001fd, 0x000001fb, 0x00000001, 0x00050051, 0x00000006, 0x000001fe, 
    0x000001fb, 0x00000002, 0x0005008e, 0x00000011, 0x00000203, 0x000001f7, 0x00000163, 0x00050051, 
    0x00000006, 0x00000204, 0x00000203, 0x00000000, 0x00050051, 0x00000006, 0x00000205, 0x00000203, 
    0x00000001, 0x00050051, 0x00000006, 0x00000206, 0x00000203, 0x00000002, 0x0005008e, 0x00000011, 
    0x0000020b, 0x0000016e, 0x00000166, 0x00050051, 0x00000006, 0x0000020c, 0x0000020b, 0x00000000, 
    0x00050051, 0x00000006, 0x0000020d, 0x0000020b, 0x00000001, 0x00050051, 0x00000006, 0x0000020e, 
    0x0000020b, 0x00000002, 0x00050051, 0x00000006, 0x00000211, 0x00000168, 0x00000000, 0x00050051, 
    0x00000006, 0x00000212, 0x00000168, 0x00000001, 0x00050051, 0x00000006, 0x00000213, 0x00000168, 
    0x00000002, 0x00070050, 0x00000007, 0x00000225, 0x000001fc, 0x000001fd, 0x000001fe, 0x00000017, 
    0x00070050, 0x00000007, 0x00000226, 0x00000204, 0x00000205, 0x00000206, 0x00000017, 0x00070050, 
    0x00000007, 0x00000227, 0x0000020c, 0x0000020d, 0x0000020e, 0x00000017, 0x00070050, 0x00000007, 
    0x00000228, 0x00000211, 0x00000212, 0x00000213, 0x00000022, 0x00070050, 0x00000008, 0x00000229, 
    0x00000225, 0x00000226, 0x00000227, 0x00000228, 0x000200f9, 0x0000022a, 0x000200f8, 0x0000022a, 
    0x000900f5, 0x00000008, 0x0000023c, 0x000001ad, 0x00000171, 0x000001ed, 0x000001b1, 0x00000229, 
    0x000001ef, 0x00050041, 0x0000011c, 0x0000012b, 0x0000011b, 0x00000033, 0x0004003d, 0x00000008, 
    0x0000012c, 0x0000012b, 0x00050092, 0x00000008, 0x0000012f, 0x0000012c, 0x0000011e, 0x00050092, 
    0x00000008, 0x00000131, 0x0000012f, 0x0000023c, 0x0004003d, 0x0000002a, 0x00000134, 0x00000133, 
    0x00050041, 0x00000028, 0x00000136, 0x00000015, 0x00000134, 0x0004003d, 0x00000011, 0x00000137, 
    0x00000136, 0x00050051, 0x00000006, 0x00000138, 0x00000137, 0x00000000, 0x00050051, 0x00000006, 
    0x00000139, 0x00000137, 0x00000001, 0x00050051, 0x00000006, 0x0000013a, 0x00000137, 0x00000002, 
    0x00070050, 0x00000007, 0x0000013b, 0x00000138, 0x00000139, 0x0000013a, 0x00000022, 0x00050091, 
    0x00000007, 0x0000013c, 0x00000131, 0x0000013b, 0x00050041, 0x0000013d, 0x0000013e, 0x0000012a, 
    0x0000002b, 0x0003003e, 0x0000013e, 0x0000013c, 0x00050041, 0x0000022e, 0x00000143, 0x00000021, 
    0x00000134, 0x0004003d, 0x0000001e, 0x00000144, 0x00000143, 0x0003003e, 0x00000140, 0x00000144, 
    0x000100fd, 0x00010038
};
const unsigned int billboard_default_vert_spv_size = 1130;

// Stage: frag
const unsigned int billboard_default_frag_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000021, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000011, 0x00000020, 0x00030010, 
    0x00000004, 0x00000007, 0x00040047, 0x0000000d, 0x00000021, 0x00000002, 0x00040047, 0x0000000d, 
    0x00000022, 0x00000000, 0x00040047, 0x00000011, 0x0000001e, 0x00000000, 0x00040047, 0x00000020, 
    0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 
    0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00090019, 0x0000000a, 
    0x00000006, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001b, 
    0x0000000b, 0x0000000a, 0x00040020, 0x0000000c, 0x00000000, 0x0000000b, 0x0004003b, 0x0000000c, 
    0x0000000d, 0x00000000, 0x00040017, 0x0000000f, 0x00000006, 0x00000002, 0x00040020, 0x00000010, 
    0x00000001, 0x0000000f, 0x0004003b, 0x00000010, 0x00000011, 0x00000001, 0x0004002b, 0x00000006, 
    0x00000019, 0x00000000, 0x00020014, 0x0000001a, 0x00040020, 0x0000001f, 0x00000003, 0x00000007, 
    0x0004003b, 0x0000001f, 0x00000020, 0x00000003, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 
    0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x0000000b, 0x0000000e, 0x0000000d, 0x0004003d, 
    0x0000000f, 0x00000012, 0x00000011, 0x00050057, 0x00000007, 0x00000013, 0x0000000e, 0x00000012, 
    0x00050051, 0x00000006, 0x00000018, 0x00000013, 0x00000003, 0x000500b4, 0x0000001a, 0x0000001b, 
    0x00000018, 0x00000019, 0x000300f7, 0x0000001d, 0x00000000, 0x000400fa, 0x0000001b, 0x0000001c, 
    0x0000001d, 0x000200f8, 0x0000001c, 0x000100fc, 0x000200f8, 0x0000001d, 0x0003003e, 0x00000020, 
    0x00000013, 0x000100fd, 0x00010038
};
const unsigned int billboard_default_frag_spv_size = 147;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

// Stage: vert
const unsigned int billboard_picking_vert_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000047, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0007000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x00000024, 0x00000033, 0x00030047, 
    0x00000022, 0x00000002, 0x00050048, 0x00000022, 0x00000000, 0x0000000b, 0x00000000, 0x00050048, 
    0x00000022, 0x00000001, 0x0000000b, 0x00000001, 0x00050048, 0x00000022, 0x00000002, 0x0000000b, 
    0x00000003, 0x00050048, 0x00000022, 0x00000003, 0x0000000b, 0x00000004, 0x00030047, 0x00000028, 
    0x00000002, 0x00040048, 0x00000028, 0x00000000, 0x00000005, 0x00050048, 0x00000028, 0x00000000, 
    0x00000007, 0x00000010, 0x00050048, 0x00000028, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 
    0x00000028, 0x00000001, 0x00000005, 0x00050048, 0x00000028, 0x00000001, 0x00000007, 0x00000010, 
    0x00050048, 0x00000028, 0x00000001, 0x00000023, 0x00000040, 0x00040048, 0x00000028, 0x00000002, 
    0x00000005, 0x00050048, 0x00000028, 0x00000002, 0x00000007, 0x00000010, 0x00050048, 0x00000028, 
    0x00000002, 0x00000023, 0x00000080, 0x00040047, 0x0000002a, 0x00000021, 0x00000000, 0x00040047, 
    0x0000002a, 0x00000022, 0x00000000, 0x00040047, 0x00000033, 0x0000000b, 0x0000002a, 0x00020013, 
    0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 
    0x00000007, 0x00000006, 0x00000003, 0x00040015, 0x00000008, 0x00000020, 0x00000000, 0x0004002b, 
    0x00000008, 0x00000009, 0x00000006, 0x0004001c, 0x0000000a, 0x00000007, 0x00000009, 0x0004002b, 
    0x00000006, 0x0000000d, 0xbf000000, 0x0004002b, 0x00000006, 0x0000000e, 0x00000000, 0x0006002c, 
    0x00000007, 0x0000000f, 0x0000000d, 0x0000000d, 0x0000000e, 0x0004002b, 0x00000006, 0x00000010, 
    0x3f000000, 0x0006002c, 0x00000007, 0x00000011, 0x00000010, 0x0000000d, 0x0000000e, 0x0006002c, 
    0x00000007, 0x00000012, 0x00000010, 0x00000010, 0x0000000e, 0x0006002c, 0x00000007, 0x00000013, 
    0x0000000d, 0x00000010, 0x0000000e, 0x0009002c, 0x0000000a, 0x00000014, 0x0000000f, 0x00000011, 
    0x00000012, 0x00000012, 0x00000013, 0x0000000f, 0x0004002b, 0x00000006, 0x00000019, 0x3f800000, 
    0x00040017, 0x0000001f, 0x00000006, 0x00000004, 0x0004002b, 0x00000008, 0x00000020, 0x00000001, 
    0x0004001c, 0x00000021, 0x00000006, 0x00000020, 0x0006001e, 0x00000022, 0x0000001f, 0x00000006, 
    0x00000021, 0x00000021, 0x00040020, 0x00000023, 0x00000003, 0x00000022, 0x0004003b, 0x00000023, 
    0x00000024, 0x00000003, 0x00040015, 0x00000025, 0x00000020, 0x00000001, 0x0004002b, 0x00000025, 
    0x00000026, 0x00000000, 0x00040018, 0x00000027, 0x0000001f, 0x00000004, 0x0005001e, 0x00000028, 
    0x00000027, 0x00000027, 0x00000027, 0x00040020, 0x00000029, 0x00000002, 0x00000028, 0x0004003b, 
    0x00000029, 0x0000002a, 0x00000002, 0x0004002b, 0x00000025, 0x0000002b, 0x00000002, 0x00040020, 
    0x0000002c, 0x00000002, 0x00000027, 0x00040020, 0x00000032, 0x00000001, 0x00000025, 0x0004003b, 
    0x00000032, 0x00000033, 0x00000001, 0x00040020, 0x0000003d, 0x00000003, 0x0000001f, 0x00040020, 
    0x00000045, 0x00000007, 0x0000000a, 0x00040020, 0x00000046, 0x00000007, 0x00000007, 0x00050036, 
    0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003b, 0x00000045, 
    0x0000000c, 0x00000007, 0x0003003e, 0x0000000c, 0x00000014, 0x00050041, 0x0000002c, 0x0000002d, 
    0x0000002a, 0x0000002b, 0x0004003d, 0x00000027, 0x0000002e, 0x0000002d, 0x00050041, 0x0000002c, 
    0x0000002f, 0x0000002a, 0x00000026, 0x0004003d, 0x00000027, 0x00000030, 0x0000002f, 0x00050092, 
    0x00000027, 0x00000031, 0x0000002e, 0x00000030, 0x0004003d, 0x00000025, 0x00000034, 0x00000033, 
    0x00050041, 0x00000046, 0x00000036, 0x0000000c, 0x00000034, 0x0004003d, 0x00000007, 0x00000037, 
    0x00000036, 0x00050051, 0x00000006, 0x00000038, 0x00000037, 0x00000000, 0x00050051, 0x00000006, 
    0x00000039, 0x00000037, 0x00000001, 0x00050051, 0x00000006, 0x0000003a, 0x00000037, 0x00000002, 
    0x00070050, 0x0000001f, 0x0000003b, 0x00000038, 0x00000039, 0x0000003a, 0x00000019, 0x00050091, 
    0x0000001f, 0x0000003c, 0x00000031, 0x0000003b, 0x00050041, 0x0000003d, 0x0000003e, 0x00000024, 
    0x00000026, 0x0003003e, 0x0000003e, 0x0000003c, 0x000100fd, 0x00010038
};
const unsigned int billboard_picking_vert_spv_size = 342;

// Stage: frag
const unsigned int billboard_picking_frag_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000014, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0006000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000008, 0x00030010, 0x00000004, 
    0x00000007, 0x00040047, 0x00000008, 0x0000001e, 0x00000000, 0x00030047, 0x0000000c, 0x00000002, 
    0x00050048, 0x0000000c, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 0x0000000c, 0x00000001, 
    0x00000005, 0x00050048, 0x0000000c, 0x00000001, 0x00000007, 0x00000010, 0x00050048, 0x0000000c, 
    0x00000001, 0x00000023, 0x00000010, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 
    0x00040015, 0x00000006, 0x00000020, 0x00000000, 0x00040020, 0x00000007, 0x00000003, 0x00000006, 
    0x0004003b, 0x00000007, 0x00000008, 0x00000003, 0x00030016, 0x00000009, 0x00000020, 0x00040017, 
    0x0000000a, 0x00000009, 0x00000004, 0x00040018, 0x0000000b, 0x0000000a, 0x00000004, 0x0004001e, 
    0x0000000c, 0x00000006, 0x0000000b, 0x00040020, 0x0000000d, 0x00000009, 0x0000000c, 0x0004003b, 
    0x0000000d, 0x0000000e, 0x00000009, 0x00040015, 0x0000000f, 0x00000020, 0x00000001, 0x0004002b, 
    0x0000000f, 0x00000010, 0x00000000, 0x00040020, 0x00000011, 0x00000009, 0x00000006, 0x00050036, 
    0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x00050041, 0x00000011, 
    0x00000012, 0x0000000e, 0x00000010, 0x0004003d, 0x00000006, 0x00000013, 0x00000012, 0x0003003e, 
    0x00000008, 0x00000013, 0x000100fd, 0x00010038
};
const unsigned int billboard_picking_frag_spv_size = 124;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

// Stage: vert
const unsigned int grid_vert_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000098, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0009000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x0000003a, 0x00000040, 0x00000057, 
    0x00000069, 0x00040047, 0x0000003a, 0x0000000b, 0x0000002a, 0x00040047, 0x00000040, 0x0000001e, 
    0x00000001, 0x00030047, 0x00000041, 0x00000002, 0x00040048, 0x00000041, 0x00000000, 0x00000005, 
    0x00050048, 0x00000041, 0x00000000, 0x00000007, 0x00000010, 0x00050048, 0x00000041, 0x00000000, 
    0x00000023, 0x00000000, 0x00040048, 0x00000041, 0x00000001, 0x00000005, 0x00050048, 0x00000041, 
    0x00000001, 0x00000007, 0x00000010, 0x00050048, 0x00000041, 0x00000001, 0x00000023, 0x00000040, 
    0x00040048, 0x00000041, 0x00000002, 0x00000005, 0x00050048, 0x00000041, 0x00000002, 0x00000007, 
    0x00000010, 0x00050048, 0x00000041, 0x00000002, 0x00000023, 0x00000080, 0x00040047, 0x00000043, 
    0x00000021, 0x00000000, 0x00040047, 0x00000043, 0x00000022, 0x00000000, 0x00040047, 0x00000057, 
    0x0000001e, 0x00000002, 0x00030047, 0x00000067, 0x00000002, 0x00050048, 0x00000067, 0x00000000, 
    0x0000000b, 0x00000000, 0x00050048, 0x00000067, 0x00000001, 0x0000000b, 0x00000001, 0x00050048, 
    0x00000067, 0x00000002, 0x0000000b, 0x00000003, 0x00050048, 0x00000067, 0x00000003, 0x0000000b, 
    0x00000004, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 
    0x00000020, 0x00040017, 0x00000008, 0x00000006, 0x00000004, 0x00040018, 0x00000009, 0x00000008, 
    0x00000004, 0x00040017, 0x0000000b, 0x00000006, 0x00000003, 0x00040015, 0x00000014, 0x00000020, 
    0x00000000, 0x0004002b, 0x00000014, 0x00000015, 0x00000006, 0x0004001c, 0x00000016, 0x0000000b, 
    0x00000015, 0x0004002b, 0x00000006, 0x00000019, 0x3f800000, 0x0004002b, 0x00000006, 0x0000001a, 
    0x00000000, 0x0006002c, 0x0000000b, 0x0000001b, 0x00000019, 0x00000019, 0x0000001a, 0x0004002b, 
    0x00000006, 0x0000001c, 0xbf800000, 0x0006002c, 0x0000000b, 0x0000001d, 0x0000001c, 0x0000001c, 
    0x0000001a, 0x0006002c, 0x0000000b, 0x0000001e, 0x0000001c, 0x00000019, 0x0000001a, 0x0006002c, 
    0x0000000b, 0x0000001f, 0x00000019, 0x0000001c, 0x0000001a, 0x0009002c, 0x00000016, 0x00000020, 
    0x0000001b, 0x0000001d, 0x0000001e, 0x0000001d, 0x0000001b, 0x0000001f, 0x00040020, 0x00000036, 
    0x00000007, 0x0000000b, 0x00040015, 0x00000038, 0x00000020, 0x00000001, 0x00040020, 0x00000039, 
    0x00000001, 0x00000038, 0x0004003b, 0x00000039, 0x0000003a, 0x00000001, 0x00040020, 0x0000003f, 
    0x00000003, 0x0000000b, 0x0004003b, 0x0000003f, 0x00000040, 0x00000003, 0x0005001e, 0x00000041, 
    0x00000009, 0x00000009, 0x00000009, 0x00040020, 0x00000042, 0x00000002, 0x00000041, 0x0004003b, 
    0x00000042, 0x00000043, 0x00000002, 0x0004002b, 0x00000038, 0x00000044, 0x00000000, 0x0004002b, 
    0x00000038, 0x00000045, 0x00000002, 0x0004002b, 0x00000014, 0x0000004b, 0x00000001, 0x00040020, 
    0x00000050, 0x00000002, 0x00000009, 0x0004003b, 0x0000003f, 0x00000057, 0x00000003, 0x0004001c, 
    0x00000066, 0x00000006, 0x0000004b, 0x0006001e, 0x00000067, 0x00000008, 0x00000006, 0x00000066, 
    0x00000066, 0x00040020, 0x00000068, 0x00000003, 0x00000067, 0x0004003b, 0x00000068, 0x00000069, 
    0x00000003, 0x00040020, 0x0000006f, 0x00000003, 0x00000008, 0x00040020, 0x00000097, 0x00000007, 
    0x00000016, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 
    0x0004003b, 0x00000097, 0x00000018, 0x00000007, 0x0003003e, 0x00000018, 0x00000020, 0x0004003d, 
    0x00000038, 0x0000003b, 0x0000003a, 0x00050041, 0x00000036, 0x0000003d, 0x00000018, 0x0000003b, 
    0x0004003d, 0x0000000b, 0x0000003e, 0x0000003d, 0x00050051, 0x00000006, 0x00000049, 0x0000003e, 
    0x00000000, 0x00050051, 0x00000006, 0x0000004d, 0x0000003e, 0x00000001, 0x00050041, 0x00000050, 
    0x00000051, 0x00000043, 0x00000044, 0x0004003d, 0x00000009, 0x00000052, 0x00000051, 0x00050041, 
    0x00000050, 0x00000054, 0x00000043, 0x00000045, 0x0004003d, 0x00000009, 0x00000055, 0x00000054, 
    0x0006000c, 0x00000009, 0x00000075, 0x00000001, 0x00000022, 0x00000052, 0x0006000c, 0x00000009, 
    0x00000077, 0x00000001, 0x00000022, 0x00000055, 0x00050092, 0x00000009, 0x00000078, 0x00000075, 
    0x00000077, 0x00070050, 0x00000008, 0x0000007c, 0x00000049, 0x0000004d, 0x0000001a, 0x00000019, 
    0x00050091, 0x00000008, 0x0000007d, 0x00000078, 0x0000007c, 0x0008004f, 0x0000000b, 0x0000007f, 
    0x0000007d, 0x0000007d, 0x00000000, 0x00000001, 0x00000002, 0x00050051, 0x00000006, 0x00000081, 
    0x0000007d, 0x00000003, 0x00060050, 0x0000000b, 0x00000082, 0x00000081, 0x00000081, 0x00000081, 
    0x00050088, 0x0000000b, 0x00000083, 0x0000007f, 0x00000082, 0x0003003e, 0x00000040, 0x00000083, 
    0x00070050, 0x00000008, 0x0000008f, 0x00000049, 0x0000004d, 0x00000019, 0x00000019, 0x00050091, 
    0x00000008, 0x00000090, 0x00000078, 0x0000008f, 0x0008004f, 0x0000000b, 0x00000092, 0x00000090, 
    0x00000090, 0x00000000, 0x00000001, 0x00000002, 0x00050051, 0x00000006, 0x00000094, 0x00000090, 
    0x00000003, 0x00060050, 0x0000000b, 0x00000095, 0x00000094, 0x00000094, 0x00000094, 0x00050088, 
    0x0000000b, 0x00000096, 0x00000092, 0x00000095, 0x0003003e, 0x00000057, 0x00000096, 0x00050051, 
    0x00000006, 0x0000006d, 0x0000003e, 0x00000002, 0x00070050, 0x00000008, 0x0000006e, 0x00000049, 
    0x0000004d, 0x0000006d, 0x00000019, 0x00050041, 0x0000006f, 0x00000070, 0x00000069, 0x00000044, 
    0x0003003e, 0x00000070, 0x0000006e, 0x000100fd, 0x00010038
};
const unsigned int grid_vert_spv_size = 445;

// Stage: frag
const unsigned int grid_frag_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x000001d0, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0009000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x000000a9, 0x000000ae, 0x000000c4, 
    0x000000e1, 0x00030010, 0x00000004, 0x00000007, 0x00030010, 0x00000004, 0x0000000c, 0x00030047, 
    0x0000006e, 0x00000002, 0x00040048, 0x0000006e, 0x00000000, 0x00000005, 0x00050048, 0x0000006e, 
    0x00000000, 0x00000007, 0x00000010, 0x00050048, 0x0000006e, 0x00000000, 0x00000023, 0x00000000, 
    0x00040048, 0x0000006e, 0x00000001, 0x00000005, 0x00050048, 0x0000006e, 0x00000001, 0x00000007, 
    0x00000010, 0x00050048, 0x0000006e, 0x00000001, 0x00000023, 0x00000040, 0x00040048, 0x0000006e, 
    0x00000002, 0x00000005, 0x00050048, 0x0000006e, 0x00000002, 0x00000007, 0x00000010, 0x00050048, 
    0x0000006e, 0x00000002, 0x00000023, 0x00000080, 0x00040047, 0x00000070, 0x00000021, 0x00000000, 
    0x00040047, 0x00000070, 0x00000022, 0x00000000, 0x00040047, 0x000000a9, 0x0000001e, 0x00000001, 
    0x00040047, 0x000000ae, 0x0000001e, 0x00000002, 0x00040047, 0x000000c4, 0x0000000b, 0x00000016, 
    0x00040047, 0x000000e1, 0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 
    0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000003, 
    0x00040017, 0x0000000a, 0x00000006, 0x00000004, 0x00040017, 0x00000017, 0x00000006, 0x00000002, 
    0x0004002b, 0x00000006, 0x00000023, 0x3f000000, 0x00040015, 0x0000002d, 0x00000020, 0x00000000, 
    0x0004002b, 0x0000002d, 0x00000031, 0x00000001, 0x0004002b, 0x00000006, 0x00000038, 0x3f800000, 
    0x0004002b, 0x00000006, 0x00000040, 0x3e4ccccd, 0x00020014, 0x00000045, 0x0004002b, 0x00000006, 
    0x00000048, 0xbdcccccd, 0x0004002b, 0x00000006, 0x00000050, 0x3dcccccd, 0x00040018, 0x0000006d, 
    0x0000000a, 0x00000004, 0x0005001e, 0x0000006e, 0x0000006d, 0x0000006d, 0x0000006d, 0x00040020, 
    0x0000006f, 0x00000002, 0x0000006e, 0x0004003b, 0x0000006f, 0x00000070, 0x00000002, 0x00040015, 
    0x00000071, 0x00000020, 0x00000001, 0x0004002b, 0x00000071, 0x00000072, 0x00000002, 0x00040020, 
    0x00000073, 0x00000002, 0x0000006d, 0x0004002b, 0x00000071, 0x00000076, 0x00000000, 0x0004002b, 
    0x0000002d, 0x00000082, 0x00000003, 0x0004002b, 0x00000006, 0x0000009c, 0x43800ccd, 0x0004002b, 
    0x00000006, 0x0000009e, 0x437fe666, 0x00040020, 0x000000a8, 0x00000001, 0x00000007, 0x0004003b, 
    0x000000a8, 0x000000a9, 0x00000001, 0x00040020, 0x000000aa, 0x00000001, 0x00000006, 0x0004003b, 
    0x000000a8, 0x000000ae, 0x00000001, 0x0004002b, 0x00000006, 0x000000b6, 0x00000000, 0x00040020, 
    0x000000c3, 0x00000003, 0x00000006, 0x0004003b, 0x000000c3, 0x000000c4, 0x00000003, 0x0004002b, 
    0x00000006, 0x000000d1, 0x41200000, 0x00040020, 0x000000e0, 0x00000003, 0x0000000a, 0x0004003b, 
    0x000000e0, 0x000000e1, 0x00000003, 0x0004002b, 0x00000006, 0x000000e5, 0x3c23d70a, 0x0005002c, 
    0x00000017, 0x000001cf, 0x00000023, 0x00000023, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 
    0x00000003, 0x000200f8, 0x00000005, 0x00050041, 0x000000aa, 0x000000ab, 0x000000a9, 0x00000031, 
    0x0004003d, 0x00000006, 0x000000ac, 0x000000ab, 0x0004007f, 0x00000006, 0x000000ad, 0x000000ac, 
    0x00050041, 0x000000aa, 0x000000af, 0x000000ae, 0x00000031, 0x0004003d, 0x00000006, 0x000000b0, 
    0x000000af, 0x00050083, 0x00000006, 0x000000b3, 0x000000b0, 0x000000ac, 0x00050088, 0x00000006, 
    0x000000b4, 0x000000ad, 0x000000b3, 0x000500bc, 0x00000045, 0x000000b7, 0x000000b4, 0x000000b6, 
    0x000300f7, 0x000000b9, 0x00000000, 0x000400fa, 0x000000b7, 0x000000b8, 0x000000b9, 0x000200f8, 
    0x000000b8, 0x000100fc, 0x000200f8, 0x000000b9, 0x0004003d, 0x00000007, 0x000000bc, 0x000000a9, 
    0x0004003d, 0x00000007, 0x000000be, 0x000000ae, 0x00050083, 0x00000007, 0x000000c0, 0x000000be, 
    0x000000bc, 0x0005008e, 0x00000007, 0x000000c1, 0x000000c0, 0x000000b4, 0x00050081, 0x00000007, 
    0x000000c2, 0x000000bc, 0x000000c1, 0x00050041, 0x00000073, 0x000000ed, 0x00000070, 0x00000072, 
    0x0004003d, 0x0000006d, 0x000000ee, 0x000000ed, 0x00050041, 0x00000073, 0x000000ef, 0x00000070, 
    0x00000076, 0x0004003d, 0x0000006d, 0x000000f0, 0x000000ef, 0x00050092, 0x0000006d, 0x000000f1, 
    0x000000ee, 0x000000f0, 0x00050051, 0x00000006, 0x000000f3, 0x000000c2, 0x00000000, 0x00050051, 
    0x00000006, 0x000000f4, 0x000000c2, 0x00000001, 0x00050051, 0x00000006, 0x000000f5, 0x000000c2, 
    0x00000002, 0x00070050, 0x0000000a, 0x000000f6, 0x000000f3, 0x000000f4, 0x000000f5, 0x00000038, 
    0x00050091, 0x0000000a, 0x000000f7, 0x000000f1, 0x000000f6, 0x00050051, 0x00000006, 0x000000f9, 
    0x000000f7, 0x00000002, 0x00050051, 0x00000006, 0x000000fb, 0x000000f7, 0x00000003, 0x00050088, 
    0x00000006, 0x000000fc, 0x000000f9, 0x000000fb, 0x0003003e, 0x000000c4, 0x000000fc, 0x00050085, 
    0x00000006, 0x00000113, 0x000000fc, 0x0000009e, 0x00050083, 0x00000006, 0x00000114, 0x0000009c, 
    0x00000113, 0x00050088, 0x00000006, 0x00000117, 0x00000040, 0x00000114, 0x00050083, 0x00000006, 
    0x000000ce, 0x00000023, 0x00000117, 0x0007000c, 0x00000006, 0x000000cf, 0x00000001, 0x00000028, 
    0x000000b6, 0x000000ce, 0x0007004f, 0x00000017, 0x00000122, 0x000000c2, 0x000000c2, 0x00000000, 
    0x00000002, 0x0005008e, 0x00000017, 0x00000124, 0x00000122, 0x000000d1, 0x000400d1, 0x00000017, 
    0x00000126, 0x00000124, 0x00050083, 0x00000017, 0x00000129, 0x00000124, 0x000001cf, 0x0006000c, 
    0x00000017, 0x0000012a, 0x00000001, 0x0000000a, 0x00000129, 0x00050083, 0x00000017, 0x0000012c, 
    0x0000012a, 0x000001cf, 0x0006000c, 0x00000017, 0x0000012d, 0x00000001, 0x00000004, 0x0000012c, 
    0x00050088, 0x00000017, 0x0000012f, 0x0000012d, 0x00000126, 0x00050051, 0x00000006, 0x00000131, 
    0x0000012f, 0x00000000, 0x00050051, 0x00000006, 0x00000133, 0x0000012f, 0x00000001, 0x0007000c, 
    0x00000006, 0x00000134, 0x00000001, 0x00000025, 0x00000131, 0x00000133, 0x00050051, 0x00000006, 
    0x00000136, 0x00000126, 0x00000001, 0x0007000c, 0x00000006, 0x00000137, 0x00000001, 0x00000025, 
    0x00000136, 0x00000038, 0x00050051, 0x00000006, 0x00000139, 0x00000126, 0x00000000, 0x0007000c, 
    0x00000006, 0x0000013a, 0x00000001, 0x00000025, 0x00000139, 0x00000038, 0x0007000c, 0x00000006, 
    0x0000013c, 0x00000001, 0x00000025, 0x00000134, 0x00000038, 0x00050083, 0x00000006, 0x0000013d, 
    0x00000038, 0x0000013c, 0x00070050, 0x0000000a, 0x0000013e, 0x00000040, 0x00000040, 0x00000040, 
    0x0000013d, 0x00050085, 0x00000006, 0x00000142, 0x00000048, 0x0000013a, 0x000500ba, 0x00000045, 
    0x00000143, 0x000000f3, 0x00000142, 0x000300f7, 0x0000014a, 0x00000000, 0x000400fa, 0x00000143, 
    0x00000144, 0x0000014a, 0x000200f8, 0x00000144, 0x00050085, 0x00000006, 0x00000148, 0x00000050, 
    0x0000013a, 0x000500b8, 0x00000045, 0x00000149, 0x000000f3, 0x00000148, 0x000200f9, 0x0000014a, 
    0x000200f8, 0x0000014a, 0x000700f5, 0x00000045, 0x0000014b, 0x00000143, 0x000000b9, 0x00000149, 
    0x00000144, 0x000300f7, 0x0000014e, 0x00000000, 0x000400fa, 0x0000014b, 0x0000014c, 0x0000014e, 
    0x000200f8, 0x0000014c, 0x00060052, 0x0000000a, 0x000001b3, 0x00000038, 0x0000013e, 0x00000002, 
    0x000200f9, 0x0000014e, 0x000200f8, 0x0000014e, 0x000700f5, 0x0000000a, 0x000001c9, 0x0000013e, 
    0x0000014a, 0x000001b3, 0x0000014c, 0x00050085, 0x00000006, 0x00000152, 0x00000048, 0x00000137, 
    0x000500ba, 0x00000045, 0x00000153, 0x000000f5, 0x00000152, 0x000300f7, 0x0000015a, 0x00000000, 
    0x000400fa, 0x00000153, 0x00000154, 0x0000015a, 0x000200f8, 0x00000154, 0x00050085, 0x00000006, 
    0x00000158, 0x00000050, 0x00000137, 0x000500b8, 0x00000045, 0x00000159, 0x000000f5, 0x00000158, 
    0x000200f9, 0x0000015a, 0x000200f8, 0x0000015a, 0x000700f5, 0x00000045, 0x0000015b, 0x00000153, 
    0x0000014e, 0x00000159, 0x00000154, 0x000300f7, 0x0000015e, 0x00000000, 0x000400fa, 0x0000015b, 
    0x0000015c, 0x0000015e, 0x000200f8, 0x0000015c, 0x00060052, 0x0000000a, 0x000001b7, 0x00000038, 
    0x000001c9, 0x00000000, 0x000200f9, 0x0000015e, 0x000200f8, 0x0000015e, 0x000700f5, 0x0000000a, 
    0x000001ca, 0x000001c9, 0x0000015a, 0x000001b7, 0x0000015c, 0x0005008e, 0x00000017, 0x0000016c, 
    0x00000122, 0x00000038, 0x000400d1, 0x00000017, 0x0000016e, 0x0000016c, 0x00050083, 0x00000017, 
    0x00000171, 0x0000016c, 0x000001cf, 0x0006000c, 0x00000017, 0x00000172, 0x00000001, 0x0000000a, 
    0x00000171, 0x00050083, 0x00000017, 0x00000174, 0x00000172, 0x000001cf, 0x0006000c, 0x00000017, 
    0x00000175, 0x00000001, 0x00000004, 0x00000174, 0x00050088, 0x00000017, 0x00000177, 0x00000175, 
    0x0000016e, 0x00050051, 0x00000006, 0x00000179, 0x00000177, 0x00000000, 0x00050051, 0x00000006, 
    0x0000017b, 0x00000177, 0x00000001, 0x0007000c, 0x00000006, 0x0000017c, 0x00000001, 0x00000025, 
    0x00000179, 0x0000017b, 0x00050051, 0x00000006, 0x0000017e, 0x0000016e, 0x00000001, 0x0007000c, 
    0x00000006, 0x0000017f, 0x00000001, 0x00000025, 0x0000017e, 0x00000038, 0x00050051, 0x00000006, 
    0x00000181, 0x0000016e, 0x00000000, 0x0007000c, 0x00000006, 0x00000182, 0x00000001, 0x00000025, 
    0x00000181, 0x00000038, 0x0007000c, 0x00000006, 0x00000184, 0x00000001, 0x00000025, 0x0000017c, 
    0x00000038, 0x00050083, 0x00000006, 0x00000185, 0x00000038, 0x00000184, 0x00070050, 0x0000000a, 
    0x00000186, 0x00000040, 0x00000040, 0x00000040, 0x00000185, 0x00050085, 0x00000006, 0x0000018a, 
    0x00000048, 0x00000182, 0x000500ba, 0x00000045, 0x0000018b, 0x000000f3, 0x0000018a, 0x000300f7, 
    0x00000192, 0x00000000, 0x000400fa, 0x0000018b, 0x0000018c, 0x00000192, 0x000200f8, 0x0000018c, 
    0x00050085, 0x00000006, 0x00000190, 0x00000050, 0x00000182, 0x000500b8, 0x00000045, 0x00000191, 
    0x000000f3, 0x00000190, 0x000200f9, 0x00000192, 0x000200f8, 0x00000192, 0x000700f5, 0x00000045, 
    0x00000193, 0x0000018b, 0x0000015e, 0x00000191, 0x0000018c, 0x000300f7, 0x00000196, 0x00000000, 
    0x000400fa, 0x00000193, 0x00000194, 0x00000196, 0x000200f8, 0x00000194, 0x00060052, 0x0000000a, 
    0x000001bf, 0x00000038, 0x00000186, 0x00000002, 0x000200f9, 0x00000196, 0x000200f8, 0x00000196, 
    0x000700f5, 0x0000000a, 0x000001cd, 0x00000186, 0x00000192, 0x000001bf, 0x00000194, 0x00050085, 
    0x00000006, 0x0000019a, 0x00000048, 0x0000017f, 0x000500ba, 0x00000045, 0x0000019b, 0x000000f5, 
    0x0000019a, 0x000300f7, 0x000001a2, 0x00000000, 0x000400fa, 0x0000019b, 0x0000019c, 0x000001a2, 
    0x000200f8, 0x0000019c, 0x00050085, 0x00000006, 0x000001a0, 0x00000050, 0x0000017f, 0x000500b8, 
    0x00000045, 0x000001a1, 0x000000f5, 0x000001a0, 0x000200f9, 0x000001a2, 0x000200f8, 0x000001a2, 
    0x000700f5, 0x00000045, 0x000001a3, 0x0000019b, 0x00000196, 0x000001a1, 0x0000019c, 0x000300f7, 
    0x000001a6, 0x00000000, 0x000400fa, 0x000001a3, 0x000001a4, 0x000001a6, 0x000200f8, 0x000001a4, 
    0x00060052, 0x0000000a, 0x000001c3, 0x00000038, 0x000001cd, 0x00000000, 0x000200f9, 0x000001a6, 
    0x000200f8, 0x000001a6, 0x000700f5, 0x0000000a, 0x000001ce, 0x000001cd, 0x000001a2, 0x000001c3, 
    0x000001a4, 0x00050081, 0x0000000a, 0x000000da, 0x000001ca, 0x000001ce, 0x00050051, 0x00000006, 
    0x000000dd, 0x000000da, 0x00000003, 0x00050085, 0x00000006, 0x000000de, 0x000000dd, 0x000000cf, 
    0x00060052, 0x0000000a, 0x000001c6, 0x000000de, 0x000000da, 0x00000003, 0x0003003e, 0x000000e1, 
    0x000001c6, 0x00050041, 0x000000c3, 0x000000e3, 0x000000e1, 0x00000082, 0x0004003d, 0x00000006, 
    0x000000e4, 0x000000e3, 0x000500bc, 0x00000045, 0x000000e6, 0x000000e4, 0x000000e5, 0x000300f7, 
    0x000000e8, 0x00000000, 0x000400fa, 0x000000e6, 0x000000e7, 0x000000e8, 0x000200f8, 0x000000e7, 
    0x000100fc, 0x000200f8, 0x000000e8, 0x000100fd, 0x00010038
};
const unsigned int grid_frag_spv_size = 933;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

// Stage: vert
const unsigned int sprite_default_vert_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000059, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0008000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x00000023, 0x0000003d, 0x0000004e, 
    0x00040047, 0x00000023, 0x0000000b, 0x0000002a, 0x00030047, 0x0000002c, 0x00000002, 0x00050048, 
    0x0000002c, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 0x0000002c, 0x00000001, 0x00000005, 
    0x00050048, 0x0000002c, 0x00000001, 0x00000007, 0x00000010, 0x00050048, 0x0000002c, 0x00000001, 
    0x00000023, 0x00000010, 0x00030047, 0x0000003b, 0x00000002, 0x00050048, 0x0000003b, 0x00000000, 
    0x0000000b, 0x00000000, 0x00050048, 0x0000003b, 0x00000001, 0x0000000b, 0x00000001, 0x00050048, 
    0x0000003b, 0x00000002, 0x0000000b, 0x00000003, 0x00050048, 0x0000003b, 0x00000003, 0x0000000b, 
    0x00000004, 0x00030047, 0x0000003f, 0x00000002, 0x00040048, 0x0000003f, 0x00000000, 0x00000005, 
    0x00050048, 0x0000003f, 0x00000000, 0x00000007, 0x00000010, 0x00050048, 0x0000003f, 0x00000000, 
    0x00000023, 0x00000000, 0x00040048, 0x0000003f, 0x00000001, 0x00000005, 0x00050048, 0x0000003f, 
    0x00000001, 0x00000007, 0x00000010, 0x00050048, 0x0000003f, 0x00000001, 0x00000023, 0x00000040, 
    0x00040048, 0x0000003f, 0x00000002, 0x00000005, 0x00050048, 0x0000003f, 0x00000002, 0x00000007, 
    0x00000010, 0x00050048, 0x0000003f, 0x00000002, 0x00000023, 0x00000080, 0x00040047, 0x00000041, 
    0x00000021, 0x00000000, 0x00040047, 0x00000041, 0x00000022, 0x00000000, 0x00040047, 0x0000004e, 
    0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 
    0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000003, 0x00040015, 0x00000008, 
    0x00000020, 0x00000000, 0x0004002b, 0x00000008, 0x00000009, 0x00000006, 0x0004001c, 0x0000000a, 
    0x00000007, 0x00000009, 0x0004002b, 0x00000006, 0x0000000d, 0xbf000000, 0x0004002b, 0x00000006, 
    0x0000000e, 0x00000000, 0x0006002c, 0x00000007, 0x0000000f, 0x0000000d, 0x0000000d, 0x0000000e, 
    0x0004002b, 0x00000006, 0x00000010, 0x3f000000, 0x0006002c, 0x00000007, 0x00000011, 0x00000010, 
    0x0000000d, 0x0000000e, 0x0006002c, 0x00000007, 0x00000012, 0x00000010, 0x00000010, 0x0000000e, 
    0x0006002c, 0x00000007, 0x00000013, 0x0000000d, 0x00000010, 0x0000000e, 0x0009002c, 0x0000000a, 
    0x00000014, 0x0000000f, 0x00000011, 0x00000012, 0x00000012, 0x00000013, 0x0000000f, 0x00040017, 
    0x00000015, 0x00000006, 0x00000002, 0x0004001c, 0x00000016, 0x00000015, 0x00000009, 0x0004002b, 
    0x00000006, 0x00000019, 0x3f800000, 0x0005002c, 0x00000015, 0x0000001a, 0x0000000e, 0x00000019, 
    0x0005002c, 0x00000015, 0x0000001b, 0x00000019, 0x00000019, 0x0005002c, 0x00000015, 0x0000001c, 
    0x00000019, 0x0000000e, 0x0005002c, 0x00000015, 0x0000001d, 0x0000000e, 0x0000000e, 0x0009002c, 
    0x00000016, 0x0000001e, 0x0000001a, 0x0000001b, 0x0000001c, 0x0000001c, 0x0000001d, 0x0000001a, 
    0x00040020, 0x0000001f, 0x00000007, 0x00000007, 0x00040015, 0x00000021, 0x00000020, 0x00000001, 
    0x00040020, 0x00000022, 0x00000001, 0x00000021, 0x0004003b, 0x00000022, 0x00000023, 0x00000001, 
    0x00040017, 0x00000028, 0x00000006, 0x00000004, 0x00040018, 0x0000002b, 0x00000028, 0x00000004, 
    0x0004001e, 0x0000002c, 0x00000008, 0x0000002b, 0x00040020, 0x0000002d, 0x00000009, 0x0000002c, 
    0x0004003b, 0x0000002d, 0x0000002e, 0x00000009, 0x0004002b, 0x00000021, 0x0000002f, 0x00000001, 
    0x00040020, 0x00000030, 0x00000009, 0x0000002b, 0x0004002b, 0x00000008, 0x00000039, 0x00000001, 
    0x0004001c, 0x0000003a, 0x00000006, 0x00000039, 0x0006001e, 0x0000003b, 0x00000028, 0x00000006, 
    0x0000003a, 0x0000003a, 0x00040020, 0x0000003c, 0x00000003, 0x0000003b, 0x0004003b, 0x0000003c, 
    0x0000003d, 0x00000003, 0x0004002b, 0x00000021, 0x0000003e, 0x00000000, 0x0005001e, 0x0000003f, 
    0x0000002b, 0x0000002b, 0x0000002b, 0x00040020, 0x00000040, 0x00000002, 0x0000003f, 0x0004003b, 
    0x00000040, 0x00000041, 0x00000002, 0x0004002b, 0x00000021, 0x00000042, 0x00000002, 0x00040020, 
    0x00000043, 0x00000002, 0x0000002b, 0x00040020, 0x0000004b, 0x00000003, 0x00000028, 0x00040020, 
    0x0000004d, 0x00000003, 0x00000015, 0x0004003b, 0x0000004d, 0x0000004e, 0x00000003, 0x00040020, 
    0x00000056, 0x00000007, 0x0000000a, 0x00040020, 0x00000057, 0x00000007, 0x00000016, 0x00040020, 
    0x00000058, 0x00000007, 0x00000015, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 
    0x000200f8, 0x00000005, 0x0004003b, 0x00000057, 0x00000018, 0x00000007, 0x0004003b, 0x00000056, 
    0x0000000c, 0x00000007, 0x0003003e, 0x0000000c, 0x00000014, 0x0003003e, 0x00000018, 0x0000001e, 
    0x0004003d, 0x00000021, 0x00000024, 0x00000023, 0x00050041, 0x0000001f, 0x00000026, 0x0000000c, 
    0x00000024, 0x0004003d, 0x00000007, 0x00000027, 0x00000026, 0x00050041, 0x00000030, 0x00000031, 
    0x0000002e, 0x0000002f, 0x0004003d, 0x0000002b, 0x00000032, 0x00000031, 0x00050051, 0x00000006, 
    0x00000034, 0x00000027, 0x00000000, 0x00050051, 0x00000006, 0x00000035, 0x00000027, 0x00000001, 
    0x00050051, 0x00000006, 0x00000036, 0x00000027, 0x00000002, 0x00070050, 0x00000028, 0x00000037, 
    0x00000034, 0x00000035, 0x00000036, 0x00000019, 0x00050091, 0x00000028, 0x00000038, 0x00000032, 
    0x00000037, 0x00050041, 0x00000043, 0x00000044, 0x00000041, 0x00000042, 0x0004003d, 0x0000002b, 
    0x00000045, 0x00000044, 0x00050041, 0x00000043, 0x00000046, 0x00000041, 0x0000003e, 0x0004003d, 
    0x0000002b, 0x00000047, 0x00000046, 0x00050092, 0x0000002b, 0x00000048, 0x00000045, 0x00000047, 
    0x00050091, 0x00000028, 0x0000004a, 0x00000048, 0x00000038, 0x00050041, 0x0000004b, 0x0000004c, 
    0x0000003d, 0x0000003e, 0x0003003e, 0x0000004c, 0x0000004a, 0x00050041, 0x00000058, 0x00000051, 
    0x00000018, 0x00000024, 0x0004003d, 0x00000015, 0x00000052, 0x00000051, 0x0003003e, 0x0000004e, 
    0x00000052, 0x000100fd, 0x00010038
};
const unsigned int sprite_default_vert_spv_size = 475;

// Stage: frag
const unsigned int sprite_default_frag_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000021, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000011, 0x00000020, 0x00030010, 
    0x00000004, 0x00000007, 0x00040047, 0x0000000d, 0x00000021, 0x00000002, 0x00040047, 0x0000000d, 
    0x00000022, 0x00000000, 0x00040047, 0x00000011, 0x0000001e, 0x00000000, 0x00040047, 0x00000020, 
    0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 
    0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00090019, 0x0000000a, 
    0x00000006, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001b, 
    0x0000000b, 0x0000000a, 0x00040020, 0x0000000c, 0x00000000, 0x0000000b, 0x0004003b, 0x0000000c, 
    0x0000000d, 0x00000000, 0x00040017, 0x0000000f, 0x00000006, 0x00000002, 0x00040020, 0x00000010, 
    0x00000001, 0x0000000f, 0x0004003b, 0x00000010, 0x00000011, 0x00000001, 0x0004002b, 0x00000006, 
    0x00000019, 0x00000000, 0x00020014, 0x0000001a, 0x00040020, 0x0000001f, 0x00000003, 0x00000007, 
    0x0004003b, 0x0000001f, 0x00000020, 0x00000003, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 
    0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x0000000b, 0x0000000e, 0x0000000d, 0x0004003d, 
    0x0000000f, 0x00000012, 0x00000011, 0x00050057, 0x00000007, 0x00000013, 0x0000000e, 0x00000012, 
    0x00050051, 0x00000006, 0x00000018, 0x00000013, 0x00000003, 0x000500b4, 0x0000001a, 0x0000001b, 
    0x00000018, 0x00000019, 0x000300f7, 0x0000001d, 0x00000000, 0x000400fa, 0x0000001b, 0x0000001c, 
    0x0000001d, 0x000200f8, 0x0000001c, 0x000100fc, 0x000200f8, 0x0000001d, 0x0003003e, 0x00000020, 
    0x00000013, 0x000100fd, 0x00010038
};
const unsigned int sprite_default_frag_spv_size = 147;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

// Stage: vert
const unsigned int sprite_picking_vert_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000051, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0007000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x00000023, 0x0000003d, 0x00040047, 
    0x00000023, 0x0000000b, 0x0000002a, 0x00030047, 0x0000002c, 0x00000002, 0x00050048, 0x0000002c, 
    0x00000000, 0x00000023, 0x00000000, 0x00040048, 0x0000002c, 0x00000001, 0x00000005, 0x00050048, 
    0x0000002c, 0x00000001, 0x00000007, 0x00000010, 0x00050048, 0x0000002c, 0x00000001, 0x00000023, 
    0x00000010, 0x00030047, 0x0000003b, 0x00000002, 0x00050048, 0x0000003b, 0x00000000, 0x0000000b, 
    0x00000000, 0x00050048, 0x0000003b, 0x00000001, 0x0000000b, 0x00000001, 0x00050048, 0x0000003b, 
    0x00000002, 0x0000000b, 0x00000003, 0x00050048, 0x0000003b, 0x00000003, 0x0000000b, 0x00000004, 
    0x00030047, 0x0000003f, 0x00000002, 0x00040048, 0x0000003f, 0x00000000, 0x00000005, 0x00050048, 
    0x0000003f, 0x00000000, 0x00000007, 0x00000010, 0x00050048, 0x0000003f, 0x00000000, 0x00000023, 
    0x00000000, 0x00040048, 0x0000003f, 0x00000001, 0x00000005, 0x00050048, 0x0000003f, 0x00000001, 
    0x00000007, 0x00000010, 0x00050048, 0x0000003f, 0x00000001, 0x00000023, 0x00000040, 0x00040048, 
    0x0000003f, 0x00000002, 0x00000005, 0x00050048, 0x0000003f, 0x00000002, 0x00000007, 0x00000010, 
    0x00050048, 0x0000003f, 0x00000002, 0x00000023, 0x00000080, 0x00040047, 0x00000041, 0x00000021, 
    0x00000000, 0x00040047, 0x00000041, 0x00000022, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 
    0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 
    0x00000003, 0x00040015, 0x00000008, 0x00000020, 0x00000000, 0x0004002b, 0x00000008, 0x00000009, 
    0x00000006, 0x0004001c, 0x0000000a, 0x00000007, 0x00000009, 0x0004002b, 0x00000006, 0x0000000d, 
    0xbf000000, 0x0004002b, 0x00000006, 0x0000000e, 0x00000000, 0x0006002c, 0x00000007, 0x0000000f, 
    0x0000000d, 0x0000000d, 0x0000000e, 0x0004002b, 0x00000006, 0x00000010, 0x3f000000, 0x0006002c, 
    0x00000007, 0x00000011, 0x00000010, 0x0000000d, 0x0000000e, 0x0006002c, 0x00000007, 0x00000012, 
    0x00000010, 0x00000010, 0x0000000e, 0x0006002c, 0x00000007, 0x00000013, 0x0000000d, 0x00000010, 
    0x0000000e, 0x0009002c, 0x0000000a, 0x00000014, 0x0000000f, 0x00000011, 0x00000012, 0x00000012, 
    0x00000013, 0x0000000f, 0x0004002b, 0x00000006, 0x00000019, 0x3f800000, 0x00040020, 0x0000001f, 
    0x00000007, 0x00000007, 0x00040015, 0x00000021, 0x00000020, 0x00000001, 0x00040020, 0x00000022, 
    0x00000001, 0x00000021, 0x0004003b, 0x00000022, 0x00000023, 0x00000001, 0x00040017, 0x00000028, 
    0x00000006, 0x00000004, 0x00040018, 0x0000002b, 0x00000028, 0x00000004, 0x0004001e, 0x0000002c, 
    0x00000008, 0x0000002b, 0x00040020, 0x0000002d, 0x00000009, 0x0000002c, 0x0004003b, 0x0000002d, 
    0x0000002e, 0x00000009, 0x0004002b, 0x00000021, 0x0000002f, 0x00000001, 0x00040020, 0x00000030, 
    0x00000009, 0x0000002b, 0x0004002b, 0x00000008, 0x00000039, 0x00000001, 0x0004001c, 0x0000003a, 
    0x00000006, 0x00000039, 0x0006001e, 0x0000003b, 0x00000028, 0x00000006, 0x0000003a, 0x0000003a, 
    0x00040020, 0x0000003c, 0x00000003, 0x0000003b, 0x0004003b, 0x0000003c, 0x0000003d, 0x00000003, 
    0x0004002b, 0x00000021, 0x0000003e, 0x00000000, 0x0005001e, 0x0000003f, 0x0000002b, 0x0000002b, 
    0x0000002b, 0x00040020, 0x00000040, 0x00000002, 0x0000003f, 0x0004003b, 0x00000040, 0x00000041, 
    0x00000002, 0x0004002b, 0x00000021, 0x00000042, 0x00000002, 0x00040020, 0x00000043, 0x00000002, 
    0x0000002b, 0x00040020, 0x0000004b, 0x00000003, 0x00000028, 0x00040020, 0x00000050, 0x00000007, 
    0x0000000a, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 
    0x0004003b, 0x00000050, 0x0000000c, 0x00000007, 0x0003003e, 0x0000000c, 0x00000014, 0x0004003d, 
    0x00000021, 0x00000024, 0x00000023, 0x00050041, 0x0000001f, 0x00000026, 0x0000000c, 0x00000024, 
    0x0004003d, 0x00000007, 0x00000027, 0x00000026, 0x00050041, 0x00000030, 0x00000031, 0x0000002e, 
    0x0000002f, 0x0004003d, 0x0000002b, 0x00000032, 0x00000031, 0x00050051, 0x00000006, 0x00000034, 
    0x00000027, 0x00000000, 0x00050051, 0x00000006, 0x00000035, 0x00000027, 0x00000001, 0x00050051, 
    0x00000006, 0x00000036, 0x00000027, 0x00000002, 0x00070050, 0x00000028, 0x00000037, 0x00000034, 
    0x00000035, 0x00000036, 0x00000019, 0x00050091, 0x00000028, 0x00000038, 0x00000032, 0x00000037, 
    0x00050041, 0x00000043, 0x00000044, 0x00000041, 0x00000042, 0x0004003d, 0x0000002b, 0x00000045, 
    0x00000044, 0x00050041, 0x00000043, 0x00000046, 0x00000041, 0x0000003e, 0x0004003d, 0x0000002b, 
    0x00000047, 0x00000046, 0x00050092, 0x0000002b, 0x00000048, 0x00000045, 0x00000047, 0x00050091, 
    0x00000028, 0x0000004a, 0x00000048, 0x00000038, 0x00050041, 0x0000004b, 0x0000004c, 0x0000003d, 
    0x0000003e, 0x0003003e, 0x0000004c, 0x0000004a, 0x000100fd, 0x00010038
};
const unsigned int sprite_picking_vert_spv_size = 398;

// Stage: frag
const unsigned int sprite_picking_frag_spv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000014, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 
    0x0006000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000008, 0x00030010, 0x00000004, 
    0x00000007, 0x00040047, 0x00000008, 0x0000001e, 0x00000000, 0x00030047, 0x0000000c, 0x00000002, 
    0x00050048, 0x0000000c, 0x00000000, 0x00000023, 0x00000000, 0x00040048, 0x0000000c, 0x00000001, 
    0x00000005, 0x00050048, 0x0000000c, 0x00000001, 0x00000007, 0x00000010, 0x00050048, 0x0000000c, 
    0x00000001, 0x00000023, 0x00000010, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 
    0x00040015, 0x00000006, 0x00000020, 0x00000000, 0x00040020, 0x00000007, 0x00000003, 0x00000006, 
    0x0004003b, 0x00000007, 0x00000008, 0x00000003, 0x00030016, 0x00000009, 0x00000020, 0x00040017, 
    0x0000000a, 0x00000009, 0x00000004, 0x00040018, 0x0000000b, 0x0000000a, 0x00000004, 0x0004001e, 
    0x0000000c, 0x00000006, 0x0000000b, 0x00040020, 0x0000000d, 0x00000009, 0x0000000c, 0x0004003b, 
    0x0000000d, 0x0000000e, 0x00000009, 0x00040015, 0x0000000f, 0x00000020, 0x00000001, 0x0004002b, 
    0x0000000f, 0x00000010, 0x00000000, 0x00040020, 0x00000011, 0x00000009, 0x00000006, 0x00050036, 
    0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x00050041, 0x00000011, 
    0x00000012, 0x0000000e, 0x00000010, 0x0004003d, 0x00000006, 0x00000013, 0x00000012, 0x0003003e, 
    0x00000008, 0x00000013, 0x000100fd, 0x00010038
};
const unsigned int sprite_picking_frag_spv_size = 124;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

struct evkContext
{
    bool hint_viewport;
    bool hint_minimized;
    bool hint_vsync;
    bool hint_resize;

    evkCamera* mainCamera;
    idgen* idgen;
    evkMSAA msaa;

    float2 viewportSize;
    float2 framebufferSize;

    void* userpointer_callback;
    evkCallback_Render render_callback;
    evkCalllback_RenderUI renderui_callback;
};

struct evkCamera
{
    // definitions
    float fov;
    float nearDist;
    float farDist;
    float aspectRatio;
    float movementSpeed;
    float rotationSpeed;
    float modifierSpeed;

    // math
    fmat4 view;
    fmat4 viewInverse;
    fmat4 perspective;
    fmat4 perspectiveInverse;
    float3 rotation;
    float3 position;
    float3 scale;
    float3 viewPosition;
    float3 frontPosition;

    // movement
    bool shouldMove;
    bool modifierPressed;
    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
};

static evkContext* g_EVKContext = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// context
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkResult evk_init(const evkCreateInfo* ci) {
    // general initialization
    memm_init();

    if (g_EVKContext == NULL) {
        g_EVKContext = (evkContext*)m_malloc(sizeof(evkContext));
        if (!g_EVKContext) {
            EVK_LOG(evk_Fatal, "Failed to allocate memory resources for evkContext");
            return evk_Failure;
        }
    }

    g_EVKContext->hint_viewport = ci->viewport;
    g_EVKContext->hint_minimized = false;
    g_EVKContext->hint_vsync = ci->vsync;
    g_EVKContext->hint_resize = false;
    g_EVKContext->msaa = ci->MSAA;
    g_EVKContext->idgen = idgen_create(1);
    g_EVKContext->mainCamera = evk_camera_create((float)(ci->width / ci->height));
    g_EVKContext->framebufferSize.x = (float)ci->width;
    g_EVKContext->framebufferSize.y = (float)ci->height;

    // vulkan initialization
    evkResult res = evk_initialize_backend(ci);

    return evk_Success;
}

EVK_FUNC evkResult evk_shutdown() {
    evk_shutdown_backend();
    idgen_destroy(g_EVKContext->idgen);
    evk_camera_destroy(g_EVKContext->mainCamera);

    m_free(g_EVKContext);
    memm_print_leaks();
    memm_shutdown();

    return evk_Success;
}

EVK_FUNC void evk_update(float timestep) {
    if (g_EVKContext->hint_minimized) return;
    evk_update_backend(timestep);
}

EVK_FUNC void evk_render(float timestep) {
    if (g_EVKContext->hint_minimized) return;
    evk_render_backend(timestep, &g_EVKContext->hint_resize);
}

EVK_FUNC void evk_resize(float2 size) {
    if (g_EVKContext == NULL) {
        return;
    }

    bool is_valid_size = (size.x > VECMATH_FLT_EPSILON) && (size.y > VECMATH_FLT_EPSILON);
    g_EVKContext->hint_minimized = !is_valid_size;

    if (is_valid_size) {
        g_EVKContext->framebufferSize = size;
    }

    g_EVKContext->hint_resize = true;
}

EVK_FUNC void evk_resize_viewport(float2 size) {
    if (!g_EVKContext) return;
    if (!g_EVKContext->hint_viewport) return;
    g_EVKContext->viewportSize = size;
}

EVK_FUNC void evk_minimize() {
    if (g_EVKContext == NULL) {
        return;
    }

    g_EVKContext->hint_minimized = true;
}

EVK_FUNC void evk_restore() {
    if (g_EVKContext == NULL) {
        return;
    }

    g_EVKContext->hint_minimized = false;
}

EVK_FUNC uint32_t evk_pick_object(float2 xy) {
    return evk_pick_object_backend(xy);
}

EVK_FUNC evkContext* evk_get_context() {
    if (!g_EVKContext) return NULL;
    return g_EVKContext;
}

EVK_FUNC evkCamera* evk_get_main_camera() {
    return g_EVKContext != NULL ? g_EVKContext->mainCamera : NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// general getters/setters
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC bool evk_using_vsync() {
    if (!g_EVKContext) return false;
    return g_EVKContext->hint_vsync;
}

EVK_FUNC bool evk_using_viewport() {
    if (!g_EVKContext) return false;
    return g_EVKContext->hint_viewport;
}

EVK_FUNC bool evk_currently_minimized() {
    if (!g_EVKContext) return false;
    return g_EVKContext->hint_minimized;
}

EVK_FUNC evkMSAA evk_get_msaa() {
    if (!g_EVKContext) return evk_Msaa_Off;
    return g_EVKContext->msaa;
}

EVK_FUNC float2 evk_get_size() {
    float2 res = { 0 };
    memset(&res, 0, sizeof(float2));

    if (!g_EVKContext) return res;
    return g_EVKContext->framebufferSize;
}

EVK_FUNC float2 evk_get_viewport_size() {
    float2 res = { 0 };
    memset(&res, 0, sizeof(float2));

    if (!g_EVKContext) return res;

    if (!g_EVKContext->hint_viewport) return res;
    return g_EVKContext->viewportSize;
}

EVK_FUNC void evk_set_viewport_size(const float2 size) {
    if (!g_EVKContext) return;
    if (!g_EVKContext->hint_viewport) return;

    g_EVKContext->viewportSize = size;
    g_EVKContext->hint_resize = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// callback related
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC void evk_set_user_pointer(void* pointer) {
    if (!g_EVKContext) {
        EVK_LOG(evk_Error, "EVK's context is NULL");
        return;
    }

    g_EVKContext->userpointer_callback = pointer;
}

EVK_FUNC void* evk_get_user_pointer() {
    return g_EVKContext == NULL ? NULL : g_EVKContext->userpointer_callback;
}

EVK_FUNC void evk_set_render_callback(evkCallback_Render callback) {
    if (!g_EVKContext) {
        EVK_LOG(evk_Error, "EVK's context is NULL");
        return;
    }

    g_EVKContext->render_callback = callback;
}

EVK_FUNC evkCallback_Render evk_get_render_callback() {
    return g_EVKContext == NULL ? NULL : g_EVKContext->render_callback;
}

EVK_FUNC void evk_set_renderui_callback(evkCalllback_RenderUI callback) {
    if (!g_EVKContext) {
        EVK_LOG(evk_Error, "EVK's context is NULL");
        return;
    }

    g_EVKContext->renderui_callback = callback;
}

EVK_FUNC evkCalllback_RenderUI evk_get_renderui_callback() {
    return g_EVKContext == NULL ? NULL : g_EVKContext->renderui_callback;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// logs and errors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static EVK_FUNC const char* ievk_severity_to_str(evkSeverity severity) {
    switch (severity)
    {
        case evk_Trace: return "TRACE";
        case evk_Todo: return "TODO";
        case evk_Info: return "INFO";
        case evk_Warn: return "WARN";
        case evk_Error: return "ERROR";
        case evk_Fatal: return "FATAL";
    }
    return "UNKNOWN";
}

static EVK_FUNC void ievk_log_format(char* outBuffer, size_t size, const struct tm* localTime, const char* file, int line, evkSeverity severity, const char* buffer) {
    snprintf
    (
        outBuffer,
        size,
        "[%02d/%02d/%04d - %02d:%02d:%02d][%s - %d][%s]: %s",
        localTime->tm_mday,
        localTime->tm_mon + 1,
        localTime->tm_year + 1900,
        localTime->tm_hour,
        localTime->tm_min,
        localTime->tm_sec,
        file,
        line,
        ievk_severity_to_str(severity),
        buffer
    );
}

EVK_FUNC void evk_log_message(evkSeverity severity, const char* file, unsigned int line, const char* fmt, ...) {
    char buffer[EVK_MAX_ERROR_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, EVK_MAX_ERROR_LEN, fmt, args);
    va_end(args);

    char log_message[EVK_MAX_ERROR_LEN + 256];
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);

    ievk_log_format(log_message, sizeof(log_message), local_time, file, line, severity, buffer);

    if (severity == evk_Fatal) {
        #if defined(__ANDROID__)
            __android_log_print(ANDROID_LOG_ERROR, "EVK", "%s", log_message);
        #else
            printf("%s\n", log_message);
        #endif
        abort();
    }

    else
    {
        #if defined(__ANDROID__)
            __android_log_print(ANDROID_LOG_DEBUG, "EVK", "%s", log_message);
        #else
            printf("%s\n", log_message);
        #endif
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// camera related
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static EVK_FUNC void ievk_camera_update_view_matrix(evkCamera* camera) {
    // calculate target point
    float3 target = float3_add(&camera->position, &camera->frontPosition);
    const float3 worldUp = { 0.0f, 1.0f, 0.0f };

    // create the view matrix
    camera->view = fmat4_lookat_vulkan(&camera->position, &target, &worldUp);
    camera->viewPosition = camera->position;
}

EVK_FUNC evkCamera* evk_camera_create(float aspectRatio) {
    evkCamera* camera = (evkCamera*)m_malloc(sizeof(evkCamera));
    EVK_ASSERT(camera != NULL, "Failed to allocate memory for evkCamera");
    memset(camera, 0, sizeof(evkCamera));

    camera->fov = 45.0f;
    camera->nearDist = 0.1f;
    camera->farDist = 256.0f;
    camera->aspectRatio = aspectRatio;
    camera->movementSpeed = 1.0f;
    camera->rotationSpeed = 1.0f;
    camera->modifierSpeed = 2.5f;

    camera->perspective = fmat4_identity();
    camera->perspectiveInverse = fmat4_identity();
    camera->view = fmat4_identity();
    camera->rotation.x = 0.0f; camera->rotation.y = 0.0f; camera->rotation.z = 0.0f;
    camera->position.x = 0.0f; camera->position.y = 1.0f; camera->position.z = 0.0f;
    camera->scale.x = 1.0f; camera->scale.y = 1.0f; camera->scale.z = 1.0f;
    camera->viewPosition.x = 0.0f; camera->viewPosition.y = 0.0f; camera->viewPosition.z = 0.0f;
    camera->frontPosition.x = 1.0f; camera->frontPosition.y = 0.0f; camera->frontPosition.z = 0.0f;

    // calculate initial perspective
    camera->perspective = fmat4_perspective_vulkan(to_fradians(camera->fov), aspectRatio, camera->nearDist, camera->farDist);
    camera->perspectiveInverse = fmat4_inverse(&camera->perspective);
    camera->shouldMove = camera->movingBackward = camera->movingForward = camera->movingLeft = camera->movingRight = false;

    // update initial view
    ievk_camera_update_view_matrix(camera);

    return camera;
}

EVK_FUNC void evk_camera_destroy(evkCamera* camera) {
    if (camera) {
        m_free(camera);
    }
}

EVK_FUNC void evk_camera_update(evkCamera* camera, float timestep) {
    if (!camera->shouldMove) return;

    // calculate front vector
    float yaw = to_fradians(camera->rotation.y);
    float pitch = to_fradians(camera->rotation.x);
    camera->frontPosition.x = f_cos(yaw) * f_cos(pitch);
    camera->frontPosition.y = f_sin(pitch);
    camera->frontPosition.z = f_sin(yaw) * f_cos(pitch);
    camera->frontPosition = float3_normalize(&camera->frontPosition);

    // calculate movement speed and right vector
    float moveSpeed = timestep * camera->movementSpeed;
    if (camera->modifierPressed) {
        moveSpeed *= camera->modifierSpeed;
    }

    const float3 worldUp = { 0.0f, 1.0f, 0.0f };
    float3 right = float3_cross(&worldUp, &camera->frontPosition);
    right = float3_normalize(&right);

    // apply movement
    if (camera->movingForward) {
        float3 movement = float3_scalar(&camera->frontPosition, moveSpeed);
        camera->position = float3_add(&camera->position, &movement);
    }
    if (camera->movingBackward) {
        float3 movement = float3_scalar(&camera->frontPosition, moveSpeed);
        camera->position = float3_sub(&camera->position, &movement);
    }
    if (camera->movingLeft) {
        float3 movement = float3_scalar(&right, moveSpeed);
        camera->position = float3_sub(&camera->position, &movement);
    }
    if (camera->movingRight) {
        float3 movement = float3_scalar(&right, moveSpeed);
        camera->position = float3_add(&camera->position, &movement);
    }

    // update view matrix
    ievk_camera_update_view_matrix(camera);

    camera->viewInverse = fmat4_inverse(&camera->view);
}

EVK_FUNC void evk_camera_set_aspect_ratio(evkCamera* camera, float aspect) {
    camera->perspective = fmat4_perspective_vulkan(to_fradians(camera->fov), aspect, camera->nearDist, camera->farDist);
    camera->perspectiveInverse = fmat4_inverse(&camera->perspective);
    camera->aspectRatio = aspect;
}

EVK_FUNC float evk_camera_get_aspect_ratio(evkCamera* camera) {
    if (!camera) return 1.0f;
    return camera->aspectRatio;
}

EVK_FUNC float evk_camera_get_fov(evkCamera* camera) {
    if (!camera) return 1.0f;
    return camera->fov;
}

EVK_FUNC void evk_camera_translate(evkCamera* camera, float3 dir) {
    camera->position = float3_add(&camera->position, &dir);
    ievk_camera_update_view_matrix(camera);
}

EVK_FUNC void evk_camera_rotate(evkCamera* camera, float3 dir) {
    // avoid scene flip
    if (camera->rotation.x >= 89.0f) camera->rotation.x = 89.0f;
    if (camera->rotation.x <= -89.0f) camera->rotation.x = -89.0f;

    // reset rotation on 360 degrees
    if (camera->rotation.x >= 360.0f) camera->rotation.x = 0.0f;
    if (camera->rotation.x <= -360.0f) camera->rotation.x = 0.0f;
    if (camera->rotation.y >= 360.0f) camera->rotation.y = 0.0f;
    if (camera->rotation.y <= -360.0f) camera->rotation.y = 0.0f;

    // apply rotation speed
    dir.x *= camera->rotationSpeed * 0.5f;
    dir.y *= camera->rotationSpeed * 0.5f;

    camera->rotation = float3_add(&camera->rotation, &dir);
    ievk_camera_update_view_matrix(camera);
}

EVK_FUNC fmat4 evk_camera_get_view(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->view;
}

EVK_FUNC fmat4 evk_camera_get_view_inverse(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->viewInverse;
}

EVK_FUNC fmat4 evk_camera_get_perspective(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->perspective;
}

EVK_FUNC fmat4 evk_camera_get_perspective_inverse(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->perspectiveInverse;
}

EVK_FUNC void evk_camera_set_lock(evkCamera* camera, bool value) {
    if (!camera) return;
    camera->shouldMove = value;
}

EVK_FUNC bool evk_camera_get_lock(evkCamera* camera) {
    if (!camera) return false;
    return camera->shouldMove;
}

EVK_FUNC void evk_camera_move(evkCamera* camera, evkCameraDir dir, bool moving) {
    if (!camera) return;

    switch (dir)
    {
        case evk_Camera_Direction_Forward: { camera->movingForward = moving; break; }
        case evk_Camera_Direction_Backward: { camera->movingBackward = moving; break; }
        case evk_Camera_Direction_Left: { camera->movingLeft = moving; break; }
        case evk_Camera_Direction_Right: { camera->movingRight = moving; break; }
    }
}

EVK_FUNC bool evk_camera_get_speed_modifier(evkCamera* camera, float* value) {
    if (!camera) return false;
    if (value) *(value) = camera->modifierSpeed;
    return camera->modifierPressed;
}

EVK_FUNC void evk_camera_set_speed_modifier(evkCamera* camera, bool status, float value) {
    if (!camera) return;
    camera->modifierPressed = status;
    camera->modifierSpeed = value;
}

EVK_FUNC float3 evk_camera_get_position(evkCamera* camera) {
    float3 res = { 0.0f };
    memset(&res, 0, sizeof(float3));
    if (!camera) return res;
    return camera->position;
}

EVK_FUNC float3 evk_camera_get_rotation(evkCamera* camera) {
    float3 res = { 0.0f };
    memset(&res, 0, sizeof(float3));
    if (!camera) return res;
    return camera->rotation;
}

EVK_FUNC float3 evk_camera_get_front(evkCamera* camera) {
    float3 res = { 0.0f };
    memset(&res, 0, sizeof(float3));
    if (!camera) return res;
    return camera->frontPosition;
}

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

/// @brief holds information about an instance
typedef struct evkInstance
{
    VkInstance instance;
	VkDebugUtilsMessengerEXT debugger;
    VkSurfaceKHR surface;
} evkInstance;

/// @brief holds information about a device
typedef struct evkDevice
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalProps;
    VkPhysicalDeviceMemoryProperties physicaMemProps;
    VkPhysicalDeviceFeatures phyiscalFeatures;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    uint32_t graphicsIndex;
    uint32_t presentIndex;
    uint32_t computeIndex;
} evkDevice;

/// @brief usefull information about a given swapchain, used uppon swapchain creation
typedef struct evkSwapchainDetails
{
    VkExtent2D extent;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* surfaceFormats;
    VkPresentModeKHR* presentModes;
    uint32_t surfaceFormatCount;
    uint32_t presentModeCount;
} evkSwapchainDetails;

/// @brief holds information about a swapchain
typedef struct evkSwapchain
{
    VkSurfaceFormatKHR format;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;
    uint32_t imageCount;
    VkSwapchainKHR swapchain;
    VkImage* images;
    VkImageView* imageViews;
    uint32_t imageIndex;
} evkSwapchain;

/// @brief holds information about the sync system between CPU and GPU
typedef struct evkSync
{
    uint32_t currentFrame;
    VkSemaphore* imageAvailableSemaphores;
    VkSemaphore* finishedRenderingSemaphores;
    VkFence* framesInFlightFences;
    uint32_t objectCount;
} evkSync;

/// @brief holds all vulkan backend structures needed on runtime
struct evkVulkanBackend
{
    evkMSAA msaa;
    evkInstance evkInstance;
    evkDevice evkDevice;
    evkSwapchain evkSwapchain;
    evkSync evkSync;
    
    evkRenderphaseType currentRenderphase;
    evkMainRenderphase evkMainRenderphase;
    evkPickingRenderphase evkPickingRenderphase;
    evkUIRenderphase evkUIRenderphase;
    evkViewportRenderphase evkViewportRenderphase;

    shashtable* buffers;
    shashtable* pipelines;
};

static evkVulkanBackend* g_EVKBackend = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief if validations are enabled, all vulkan messages will be call this function, wich will log the messages into the terminal
static EVK_FUNC VKAPI_ATTR VkBool32 VKAPI_CALL ievk_log_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callback, void* userData) {
    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        EVK_LOG(evk_Error, "%s\n", callback->pMessage);
        return VK_FALSE;
    }

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        EVK_LOG(evk_Warn, "%s\n", callback->pMessage);
        return VK_FALSE;
    }

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        EVK_LOG(evk_Info, "%s\n", callback->pMessage);
        return VK_FALSE;
    }

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        EVK_LOG(evk_Trace, "%s\n", callback->pMessage);
        return VK_FALSE;
    }
    return VK_TRUE;
}

/// @brief the instance creation requires the names of all extensions it'll use, this changes depending on: platform, portability and validation requests, this function returns the list correctly
static EVK_FUNC bool ievk_get_instance_extensions(uint32_t* count, const char** names, bool validations) {
    uint32_t base_count = 2;
    
    #ifdef __APPLE__
    base_count++;
    #endif
    
    uint32_t required_count = base_count + (validations ? 1 : 0);
    
    if (!count) return false;
    if (!names) { *count = required_count; return true; }
    if (*count < required_count) { *count = required_count; return false; }
    
    uint32_t idx = 0;
    names[idx++] = VK_KHR_SURFACE_EXTENSION_NAME;
    
    #ifdef _WIN32
        names[idx++] = "VK_KHR_win32_surface";
    #elif defined(__APPLE__)
        names[idx++] = "VK_EXT_metal_surface";
    #elif defined(__ANDROID__)
        names[idx++] = "VK_KHR_android_surface";
    #elif defined(__linux__) && !defined(__ANDROID__)
        #ifdef EVK_LINUX_USE_XLIB
            names[idx++] = "VK_KHR_xlib_surface";
        #elif defined(EVK_LINUX_USE_XCB)
            names[idx++] = "VK_KHR_xcb_surface";
        #else
            names[idx++] = "VK_KHR_wayland_surface";
        #endif
    #endif
    
    #ifdef __APPLE__
        names[idx++] = "VK_KHR_portability_enumeration";
    #endif
    
    if (validations) {
        names[idx++] = "VK_EXT_debug_utils";
    }
    
    *count = required_count;
    return true;
}

/// @brief the vulkan instance is the begining of all vulkan stuff, it's like the root object between our code and the gpu, this function creates it correctly
static EVK_FUNC evkInstance ievk_instance_create(const char* appName, uint32_t appVersion, const char* engineName, uint32_t engineVersion, bool validations) {
    evkInstance evkInstance;
    memset(&evkInstance, 0, sizeof(evkInstance));

    if(volkInitialize() != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to initialize volk, is vulkan library installed?");
        return evkInstance;
    }

    VkApplicationInfo appInfo;
    memset(&appInfo, 0, sizeof(VkApplicationInfo));
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = engineVersion;
    appInfo.apiVersion = VK_API_VERSION_1_1;
    appInfo.pNext = NULL;

    uint32_t count;
    if (!ievk_get_instance_extensions(&count, NULL, validations)) {
        EVK_LOG(evk_Fatal, "Failed to retrieve initial count of required instance extensions");
    }

    const char** extensions = (const char**)m_malloc(count * sizeof(const char*));
    if (!ievk_get_instance_extensions(&count, extensions, validations)) {
        EVK_LOG(evk_Fatal, "Failed to retrieve further list of required instance extensions");
        m_free(extensions);
    }

    VkInstanceCreateInfo instanceCI;
    memset(&instanceCI, 0, sizeof(VkInstanceCreateInfo));
    instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCI.pApplicationInfo = &appInfo;
    instanceCI.enabledExtensionCount = count;
    instanceCI.ppEnabledExtensionNames = extensions;
    instanceCI.flags = 0;
    instanceCI.pNext = NULL;
    instanceCI.enabledLayerCount = 0;
    instanceCI.ppEnabledLayerNames = NULL;
    
    #ifdef __APPLE__
    instanceCI.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    bool layerFound = false;

    // make validation layer name persistent
    static const char* validationLayer = "VK_LAYER_KHRONOS_validation";
    const char* availableLayers[] = { validationLayer };
    
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsCI;
    memset(&debugUtilsCI, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

    if(validations) {
        // check if validation layer actually exists before enabling
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, NULL);
        VkLayerProperties* layers = (VkLayerProperties*)m_malloc(layerCount * sizeof(VkLayerProperties));
        vkEnumerateInstanceLayerProperties(&layerCount, layers);
        
        for(uint32_t i = 0; i < layerCount; i++) {
            if(strcmp(layers[i].layerName, validationLayer) == 0) {
                layerFound = true;
                break;
            }
        }
        m_free(layers);
        
        if(!layerFound) {
            EVK_LOG(evk_Warn, "Validation layer requested but not available. Disabling validations.");
            validations = false; // disable validations if layer missing
        }
        
        else
        {
            // use the static array directly, not taking address of a pointer variable
            instanceCI.enabledLayerCount = 1;
            instanceCI.ppEnabledLayerNames = availableLayers;

            debugUtilsCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugUtilsCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugUtilsCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugUtilsCI.pfnUserCallback = ievk_log_callback;
            debugUtilsCI.pUserData = NULL;
            instanceCI.pNext = &debugUtilsCI;
        }
    }

    VkResult result = vkCreateInstance(&instanceCI, NULL, &evkInstance.instance);
    if (result != VK_SUCCESS) {
        EVK_LOG(evk_Fatal, "Failed to create vulkan instance. Error: %d", result);
        m_free(extensions);
        return evkInstance;
    }
    
    volkLoadInstance(evkInstance.instance);

    if(validations && layerFound) { // only create debugger if validations are actually enabled
        VkResult debugResult = vkCreateDebugUtilsMessengerEXT(evkInstance.instance, &debugUtilsCI, NULL, &evkInstance.debugger);
        if(debugResult != VK_SUCCESS) {
            EVK_LOG(evk_Warn, "Failed to create vulkan debugger. Error: %d", debugResult);
        }
    }

    m_free(extensions);
    return evkInstance;
}

/// @brief this function releases the used resources on instance creation
static EVK_FUNC void ievk_instance_destroy(evkInstance* evkInstance) {
    EVK_ASSERT(evkInstance != NULL, "Vulkan backend is NULL");

    vkDestroySurfaceKHR(evkInstance->instance, evkInstance->surface, NULL);
    evkInstance->surface = VK_NULL_HANDLE;

    if(evkInstance->debugger != VK_NULL_HANDLE) {
        if (vkDestroyDebugUtilsMessengerEXT) {
            vkDestroyDebugUtilsMessengerEXT(evkInstance->instance, evkInstance->debugger, NULL);
        }
        evkInstance->debugger = VK_NULL_HANDLE;
    }

    vkDestroyInstance(evkInstance->instance, NULL);
    evkInstance->instance = VK_NULL_HANDLE;

    volkFinalize();
}

/// @brief creates the the renderable surface/window and depends on the operating system, pay attention if you're on linux since the auto-detection will default to wayland if not especified
static EVK_FUNC void ievk_surface_create(VkInstance instance, VkSurfaceKHR* surface, void* rawWindow, void* rawDisplay) {
    #ifdef _WIN32
        VkWin32SurfaceCreateInfoKHR createInfo;
        memset(&createInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hinstance = GetModuleHandle(NULL);
        createInfo.hwnd = (HWND)rawWindow;
        if (vkCreateWin32SurfaceKHR) {
            EVK_ASSERT(vkCreateWin32SurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Win32 surface");
        }

        else {
            EVK_ASSERT(true, "Cannot find vkCreateWin32SurfaceKHR");
        }
    #elif defined(__APPLE__)
        VkMetalSurfaceCreateInfoEXT createInfo;
        memset(&createInfo, 0, sizeof(VkMetalSurfaceCreateInfoEXT));
        createInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
        createInfo.pLayer = (CAMetalLayer*)rawWindow;
        if (vkCreateMetalSurfaceEXT) {
            EVK_ASSERT(vkCreateMetalSurfaceEXT(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Metal surface");
        }
        else {
            EVK_ASSERT(true, "Cannot find vkCreateMetalSurfaceEXT");
        }
    #elif defined(__ANDROID__)
        VkAndroidSurfaceCreateInfoKHR createInfo;
        memset(&createInfo, 0, sizeof(VkAndroidSurfaceCreateInfoKHR));
        createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        createInfo.window = (ANativeWindow*)rawWindow;
        if (vkCreateAndroidSurfaceKHR) {
            EVK_ASSERT(vkCreateAndroidSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Android surface");
        }
        else {
            EVK_ASSERT(true, "Cannot find vkCreateAndroidSurfaceKHR");
        }
    #elif defined(__linux__) && !defined(__ANDROID__)
        #ifdef EVK_LINUX_USE_XLIB
            VkXlibSurfaceCreateInfoKHR createInfo;
            memset(&createInfo, 0, sizeof(VkXlibSurfaceCreateInfoKHR));
            createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
            createInfo.dpy = (Display*)rawDisplay;
            createInfo.window = (Window)rawWindow;
            if (vkCreateXlibSurfaceKHR) {
                EVK_ASSERT(vkCreateXlibSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Xlib surface");
            }
            else {
                EVK_ASSERT(true, "Cannot find vkCreateXlibSurfaceKHR");
            }
        #elif defined(EVK_LINUX_USE_XCB)
            VkXcbSurfaceCreateInfoKHR createInfo;
            memset(&createInfo, 0, sizeof(VkXcbSurfaceCreateInfoKHR));
            createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
            createInfo.connection = (xcb_connection_t*)rawDisplay;
            createInfo.window = (xcb_window_t)rawWindow;
            if (vkCreateXcbSurfaceKHR) {
                EVK_ASSERT(vkCreateXcbSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the XCB surface");
            }
            else {
                EVK_ASSERT(true, "Cannot find vkCreateXcbSurfaceKHR");
            }
        #else
            VkWaylandSurfaceCreateInfoKHR createInfo;
            memset(&createInfo, 0, sizeof(VkWaylandSurfaceCreateInfoKHR));
            createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
            createInfo.display = (struct wl_display*)rawDisplay;
            createInfo.surface = (struct wl_surface*)rawWindow;
            if (vkCreateWaylandSurfaceKHR) {
                EVK_ASSERT(vkCreateWaylandSurfaceKHR(instance, &createInfo, NULL, surface) == VK_SUCCESS, "Failed to create the Wayland surface");
            }
            else {
                EVK_ASSERT(true, "Cannot find vkCreateWaylandSurfaceKHR");
            }
        #endif
    #endif
}

/// @brief makes a check if all required extensions are present for that particular physical device
static EVK_FUNC bool ievk_check_device_extension_support(VkPhysicalDevice device, const char** required_extensions, uint32_t extension_count) {
    uint32_t available_extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &available_extension_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties*)m_malloc(available_extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &available_extension_count, available_extensions);

    for (uint32_t i = 0; i < extension_count; i++) {
        int extension_found = 0;
        for (uint32_t j = 0; j < available_extension_count; j++) {
            if (strcmp(required_extensions[i], available_extensions[j].extensionName) == 0) {
                extension_found = 1;
                break;
            }
        }
        if (!extension_found) {
            m_free(available_extensions);
            return 0;
        }
    }

    m_free(available_extensions);
    return 1;
}

/// @brief since one compute may have multiple physical gpus we must check them all to see which is more fit
static EVK_FUNC VkPhysicalDevice ievk_device_choose(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t gpus = 0;
    vkEnumeratePhysicalDevices(instance, &gpus, NULL);

    VkPhysicalDevice* devices = (VkPhysicalDevice*)m_malloc(gpus * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &gpus, devices);

    VkPhysicalDevice choosenOne = VK_NULL_HANDLE;
    const char* requiredExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    const uint32_t requiredExtensionsCount = 1;
    VkDeviceSize bestScore = 0;

    for (uint32_t i = 0; i < gpus; i++) {

        VkPhysicalDeviceProperties device_props;
        VkPhysicalDeviceFeatures device_features;
        VkPhysicalDeviceMemoryProperties mem_props;
        vkGetPhysicalDeviceProperties(devices[i], &device_props);
        vkGetPhysicalDeviceFeatures(devices[i], &device_features);
        vkGetPhysicalDeviceMemoryProperties(devices[i], &mem_props);
        evkQueueFamily indices = evk_device_find_queue_families(devices[i], surface);
        if (!indices.graphicsFound || !indices.presentFound || !indices.computeFound) continue;
        if (!ievk_check_device_extension_support(devices[i], requiredExtensions, requiredExtensionsCount)) continue;

        VkDeviceSize currentScore = 0;
        if (device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) currentScore += 1000;  // discrete gpu
        currentScore += device_props.limits.maxImageDimension2D;                                    // max texture size
        for (uint32_t j = 0; j < mem_props.memoryHeapCount; j++) {                                  // prefer devices with dedicated VRAM
            if (mem_props.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                currentScore += mem_props.memoryHeaps[j].size / (VkDeviceSize)(1024 * 1024);        // mb
            }
        }

        if (currentScore > bestScore) {
            bestScore = currentScore;
            choosenOne = devices[i];
        }
    }

    m_free(devices);
    return choosenOne;
}

/// @brief creates the logical device based on choosen physical device and surface, it'll be logical connection to a specific GPU, used for creatin all vulkan objects from now on
static EVK_FUNC evkDevice ievk_device_create(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice) {
    evkDevice device;
    memset(&device, 0, sizeof(evkDevice));
    device.physicalDevice = physicalDevice;

    #ifdef EVK_ENABLE_VALIDATIONS
        const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
        uint32_t validationLayerCount = 1;
    #else
        const char* validationLayers[] = { "" };
        uint32_t validationLayerCount = 0;
    #endif

    evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
    uint32_t queueFamilyIndices[3] = { 0 };
    uint32_t queueCount = 0;
    float queuePriority = 1.0f;

    if (indices.graphics != -1) queueFamilyIndices[queueCount++] = indices.graphics;
    if (indices.present != -1 && indices.present != indices.graphics)  queueFamilyIndices[queueCount++] = indices.present;
    if (indices.compute != -1 && indices.compute != indices.graphics && indices.compute != indices.present) queueFamilyIndices[queueCount++] = indices.compute;

    VkDeviceQueueCreateInfo* queueCreateInfos = (VkDeviceQueueCreateInfo*)m_malloc(sizeof(VkDeviceQueueCreateInfo) * queueCount);
    for (uint32_t i = 0; i < queueCount; i++) {
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].pNext = NULL;
        queueCreateInfos[i].queueFamilyIndex = queueFamilyIndices[i];
        queueCreateInfos[i].queueCount = 1;
        queueCreateInfos[i].pQueuePriorities = &queuePriority;
        queueCreateInfos[i].flags = 0;
    }

    #if defined(__APPLE__)
        const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME };
        uint32_t extensionCount = 2;
    #else
        const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        uint32_t extensionCount = 1;
    #endif

    VkPhysicalDeviceFeatures deviceFeatures;
    memset(&deviceFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceCI;
    memset(&deviceCI, 0, sizeof(VkDeviceCreateInfo));
    deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCI.pNext = NULL;
    deviceCI.flags = 0;
    deviceCI.queueCreateInfoCount = queueCount;
    deviceCI.pQueueCreateInfos = queueCreateInfos;
    deviceCI.enabledExtensionCount = extensionCount;
    deviceCI.ppEnabledExtensionNames = extensions;
    deviceCI.pEnabledFeatures = &deviceFeatures;
    deviceCI.enabledLayerCount = validationLayerCount;
    deviceCI.ppEnabledLayerNames = validationLayers;
    EVK_ASSERT(vkCreateDevice(physicalDevice, &deviceCI, NULL, &device.device) == VK_SUCCESS, "Failed to create vulkan logical device");
    
    volkLoadDevice(device.device);

    vkGetPhysicalDeviceProperties(physicalDevice, &device.physicalProps);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &device.physicaMemProps);
    vkGetPhysicalDeviceFeatures(physicalDevice, &device.phyiscalFeatures);
    vkGetDeviceQueue(device.device, indices.graphics, 0, &device.graphicsQueue);
    vkGetDeviceQueue(device.device, indices.present, 0, &device.presentQueue);
    vkGetDeviceQueue(device.device, indices.compute, 0, &device.computeQueue);

    device.graphicsIndex = indices.graphics;
    device.presentIndex = indices.present;
    device.computeIndex = indices.compute;

    m_free(queueCreateInfos);

    return device;
}

/// @brief releases all resources used uppon device creation
static EVK_FUNC void ievk_device_destroy(evkDevice* evkDevice) {
    EVK_ASSERT(evkDevice != NULL, "evkDevice is NULL");

    vkDestroyDevice(g_EVKBackend->evkDevice.device, NULL);
    g_EVKBackend->evkDevice.device = VK_NULL_HANDLE;
}

/// @brief queries information for the swapchain, like available surface formats and present mode.
static EVK_FUNC evkSwapchainDetails ievk_swapchain_query_details(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    evkSwapchainDetails details = { 0 };
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &details.surfaceFormatCount, NULL);

    if (details.surfaceFormatCount != 0) {
        details.surfaceFormats = (VkSurfaceFormatKHR*)m_malloc(details.surfaceFormatCount * sizeof(VkSurfaceFormatKHR));

        if (details.surfaceFormats) {
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &details.surfaceFormatCount, details.surfaceFormats);
        }
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &details.presentModeCount, NULL);

    if (details.presentModeCount != 0) {
        details.presentModes = (VkPresentModeKHR*)m_malloc(details.presentModeCount * sizeof(VkPresentModeKHR));
        if (details.presentModes) {
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &details.presentModeCount, details.presentModes);
        }
    }

    return details;
}

/// @brief chooses the surface format most appropriate from a list of available formats
static EVK_FUNC VkSurfaceFormatKHR ievk_swapchain_choose_surface_format(VkSurfaceFormatKHR* formats, uint32_t quantity) {
    for (uint32_t i = 0; i < quantity; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return formats[i];
    }

    return formats[0];
}

/// @brief chooses the appresentation mode for the swapchain
static EVK_FUNC VkPresentModeKHR ievk_swapchain_choose_present_mode(VkPresentModeKHR* modes, uint32_t quantity, bool vsync) {
    if (modes == NULL || quantity == 0 || vsync)  return VK_PRESENT_MODE_FIFO_KHR;

    bool immediateModeAvailable = false;
    for (uint32_t i = 0; i < quantity; i++) {
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)  return VK_PRESENT_MODE_MAILBOX_KHR;
        if (modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) immediateModeAvailable = true;
    }

    if (immediateModeAvailable) return VK_PRESENT_MODE_IMMEDIATE_KHR;
    return VK_PRESENT_MODE_FIFO_KHR;
}

/// @brief adjusts the correct extent for the swapchain
static EVK_FUNC VkExtent2D ievk_swapchain_adjust_extent(const VkSurfaceCapabilitiesKHR* capabilities, uint32_t width, uint32_t height) {
    if (capabilities->currentExtent.width != UINT32_MAX) return capabilities->currentExtent;

    VkExtent2D actualExtent = { width, height };
    actualExtent.width = (uint32_t)VECMATH_CLAMP((const float)actualExtent.width, (const float)capabilities->minImageExtent.width, (const float)capabilities->maxImageExtent.width);
    actualExtent.height = (uint32_t)VECMATH_CLAMP((const float)actualExtent.height, (const float)capabilities->minImageExtent.height, (const float)capabilities->maxImageExtent.height);

    return actualExtent;
}

/// @brief creates the swapchain object
static EVK_FUNC evkSwapchain ievk_swapchain_create(VkSurfaceKHR surface, VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, bool vsync) {
    evkSwapchain swapchain;
    memset(&swapchain, 0, sizeof(evkSwapchain));

    evkSwapchainDetails details = ievk_swapchain_query_details(physicalDevice, surface);
    swapchain.format = ievk_swapchain_choose_surface_format(details.surfaceFormats, details.surfaceFormatCount);
    swapchain.presentMode = ievk_swapchain_choose_present_mode(details.presentModes, details.presentModeCount, vsync);
    swapchain.extent = ievk_swapchain_adjust_extent(&details.capabilities, extent.width, extent.height);

    swapchain.imageCount = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && swapchain.imageCount > details.capabilities.maxImageCount) swapchain.imageCount = details.capabilities.maxImageCount;

    evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
    int32_t queueFamilyIndices[] = { (int32_t)indices.graphics, (int32_t)indices.present, (int32_t)indices.compute };
    VkSwapchainCreateInfoKHR swapchainCI;
    memset(&swapchainCI, 0, sizeof(VkSwapchainCreateInfoKHR));
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.pNext = NULL;
    swapchainCI.flags = 0;
    swapchainCI.surface = surface;
    swapchainCI.minImageCount = swapchain.imageCount;
    swapchainCI.imageFormat = swapchain.format.format;
    swapchainCI.imageColorSpace = swapchain.format.colorSpace;
    swapchainCI.imageExtent = swapchain.extent;
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // allow copying swapchain images
    swapchainCI.preTransform = details.capabilities.currentTransform;

    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    if (!(details.capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)) {
        compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR; // fallback to INHERIT (Android)
    }

    swapchainCI.compositeAlpha = compositeAlpha;
    swapchainCI.presentMode = swapchain.presentMode;
    swapchainCI.clipped = VK_TRUE;

    if (indices.graphics != indices.present) {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2;
        swapchainCI.pQueueFamilyIndices = (uint32_t*)queueFamilyIndices;
    }

    else {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    EVK_ASSERT(vkCreateSwapchainKHR(device, &swapchainCI, NULL, &swapchain.swapchain) == VK_SUCCESS, "Failed to create swapchain");

    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &swapchain.imageCount, NULL);
    swapchain.images = (VkImage*)m_malloc(swapchain.imageCount * sizeof(VkImage));
    swapchain.imageViews = (VkImageView*)m_malloc(sizeof(VkImageView) * swapchain.imageCount);
    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &swapchain.imageCount, swapchain.images);

    for (uint32_t i = 0; i < swapchain.imageCount; i++) {
        evk_device_create_image_view(device, swapchain.images[i], swapchain.format.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, 1, VK_IMAGE_VIEW_TYPE_2D, NULL, &swapchain.imageViews[i]);
    }

    // free details
    m_free(details.presentModes);
    m_free(details.surfaceFormats);

    return swapchain;
}

/// @brief releases all resources used on swapchain creation
static EVK_FUNC void ievk_swapchain_destroy(evkSwapchain* swapchain, VkDevice device) {
    for (uint32_t i = 0; i < swapchain->imageCount; i++) {
        vkDestroyImageView(device, swapchain->imageViews[i], NULL);
    }

    m_free(swapchain->imageViews);
    m_free(swapchain->images);

    vkDestroySwapchainKHR(device, swapchain->swapchain, NULL);
    swapchain->swapchain = VK_NULL_HANDLE;
}

/// @brief creates all syncronization resources for CPU-GPU communication
static EVK_FUNC evkSync ievk_sync_create(VkDevice device, uint32_t objectCount) {
    evkSync sync;
    memset(&sync, 0, sizeof(evkSync));
    sync.objectCount = objectCount;

    VkSemaphoreCreateInfo semaphoreCI;
    memset(&semaphoreCI, 0, sizeof(VkSemaphoreCreateInfo));
    semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCI.pNext = NULL;
    semaphoreCI.flags = 0;

    VkFenceCreateInfo fenceCI;
    memset(&fenceCI, 0, sizeof(VkFenceCreateInfo));
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.pNext = NULL;
    fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    sync.imageAvailableSemaphores = (VkSemaphore*)m_malloc(sizeof(VkSemaphore) * objectCount);
    sync.finishedRenderingSemaphores = (VkSemaphore*)m_malloc(sizeof(VkSemaphore) * objectCount);
    sync.framesInFlightFences = (VkFence*)m_malloc(sizeof(VkFence) * objectCount);

    for (size_t i = 0; i < objectCount; i++) {
        EVK_ASSERT(vkCreateSemaphore(device, &semaphoreCI, NULL, &sync.imageAvailableSemaphores[i]) == VK_SUCCESS, "Failed to create image available semaphore");
        EVK_ASSERT(vkCreateSemaphore(device, &semaphoreCI, NULL, &sync.finishedRenderingSemaphores[i]) == VK_SUCCESS, "Failed to create rendering finished semaphore");
        EVK_ASSERT(vkCreateFence(device, &fenceCI, NULL, &sync.framesInFlightFences[i]) == VK_SUCCESS, "Failed to create syncronizer fence");
    }

    return sync;
}

/// @brief releases all resources used on sync creation
static EVK_FUNC void ievk_sync_destroy(evkSync* sync, VkDevice device)
{
    for (uint32_t i = 0; i < sync->objectCount; i++) {
        if (sync->imageAvailableSemaphores[i]) vkDestroySemaphore(device, sync->imageAvailableSemaphores[i], NULL);
        if (sync->finishedRenderingSemaphores[i]) vkDestroySemaphore(device, sync->finishedRenderingSemaphores[i], NULL);
        if (sync->framesInFlightFences[i]) vkDestroyFence(device, sync->framesInFlightFences[i], NULL);
    }

    m_free(sync->imageAvailableSemaphores);
    m_free(sync->finishedRenderingSemaphores);
    m_free(sync->framesInFlightFences);
}

static EVK_FUNC void ievk_resize(VkExtent2D extent) {
    vkDeviceWaitIdle(g_EVKBackend->evkDevice.device);

    // if you wish to make a vulkan resize, you must first re-invent the universe
    evk_renderphase_ui_destroy(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device);
    if (evk_using_viewport()) {
        evk_renderphase_viewport_destroy(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device);
    }
    evk_renderphase_picking_destroy(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device);
    evk_renderphase_main_destroy(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device);
    
    ievk_swapchain_destroy(&g_EVKBackend->evkSwapchain, g_EVKBackend->evkDevice.device);
    g_EVKBackend->evkSwapchain = ievk_swapchain_create(g_EVKBackend->evkInstance.surface, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, extent, evk_using_vsync());

    // renderphases
    g_EVKBackend->evkMainRenderphase = evk_renderphase_main_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, g_EVKBackend->msaa, false); // false because on this setup it'll never be the final phase
    EVK_ASSERT(evk_renderphase_main_create_framebuffers(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create main render phase frame buffers");

    g_EVKBackend->evkPickingRenderphase = evk_renderphase_picking_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->msaa);
    EVK_ASSERT(evk_renderphase_picking_create_framebuffers(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent) == evk_Success, "Failed to create picking render phase framebuffers");

    g_EVKBackend->evkUIRenderphase = evk_renderphase_ui_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, true); // true because on this setup this is the final phase
    EVK_ASSERT(evk_renderphase_ui_create_framebuffers(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create ui render phase framebuffers");

    if (evk_using_viewport()) {
        g_EVKBackend->evkViewportRenderphase = evk_renderphase_viewport_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, evk_Msaa_Off);
        EVK_ASSERT(evk_renderphase_viewport_create_framebuffers(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create viewport framebuffers");
        evk_camera_set_aspect_ratio(evk_get_main_camera(), (float)(evk_get_viewport_size().x / evk_get_viewport_size().y));
    }

    if (!evk_using_viewport()) {
        evk_camera_set_aspect_ratio(evk_get_main_camera(), (float)(extent.width / extent.height));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vulkan general core
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkResult evk_initialize_backend(const evkCreateInfo* ci) {
    // general initialization
    if (g_EVKBackend == NULL) {
        g_EVKBackend = (evkVulkanBackend*)m_malloc(sizeof(evkVulkanBackend));
        memset(g_EVKBackend, 0, sizeof(evkVulkanBackend));
        if (!g_EVKBackend) {
            EVK_LOG(evk_Fatal, "Failed to allocate memory resources for evkVulkanBackend");
            return evk_Failure;
        }

        g_EVKBackend->buffers = shashtable_init();
        g_EVKBackend->pipelines = shashtable_init();
        g_EVKBackend->msaa = ci->MSAA;
    }

    // instance
    g_EVKBackend->evkInstance = ievk_instance_create(ci->appName, ci->appVersion, ci->engineName, ci->engineVersion, ci->validations);

    // surface
    #ifdef _WIN32
        ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.window, NULL);
    #elif defined(__APPLE__)
        ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.layer, NULL);
    #elif defined(__ANDROID__)
        ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.window, NULL);
    #elif defined(__linux__) && !defined(__ANDROID__)
        #ifdef EVK_LINUX_USE_XLIB
            ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, (unsigned int*)ci->window.window, ci->window.display);
        #elif defined(EVK_LINUX_USE_XCB)
            ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.connection);
        #else
            ievk_surface_create(g_EVKBackend->evkInstance.instance, &g_EVKBackend->evkInstance.surface, ci->window.display);
        #endif
    #endif

    // device
    VkPhysicalDevice physicalDevice = ievk_device_choose(g_EVKBackend->evkInstance.instance, g_EVKBackend->evkInstance.surface);
    g_EVKBackend->evkDevice = ievk_device_create(g_EVKBackend->evkInstance.instance, g_EVKBackend->evkInstance.surface, physicalDevice);

    // swapchain
    VkExtent2D extent = { 0 };
    extent.width = ci->width;
    extent.height = ci->height;
    g_EVKBackend->evkSwapchain = ievk_swapchain_create(g_EVKBackend->evkInstance.surface, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, extent, ci->vsync);
    
    // sync
    g_EVKBackend->evkSync = ievk_sync_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkSwapchain.imageCount);

    // render phases
    g_EVKBackend->evkMainRenderphase = evk_renderphase_main_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, g_EVKBackend->msaa, false); // false because on this setup it'll never be the final phase
    EVK_ASSERT(evk_renderphase_main_create_framebuffers(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create main render phase frame buffers");
    
    g_EVKBackend->evkPickingRenderphase = evk_renderphase_picking_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->msaa);
    EVK_ASSERT(evk_renderphase_picking_create_framebuffers(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent) == evk_Success, "Failed to create picking render phase framebuffers");
    
    g_EVKBackend->evkUIRenderphase = evk_renderphase_ui_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, true); // true because on this setup this is the final phase
    EVK_ASSERT(evk_renderphase_ui_create_framebuffers(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create ui render phase framebuffers");
    
    if (evk_using_viewport()) {
        g_EVKBackend->evkViewportRenderphase = evk_renderphase_viewport_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkInstance.surface, g_EVKBackend->evkSwapchain.format.format, evk_Msaa_Off);
        EVK_ASSERT(evk_renderphase_viewport_create_framebuffers(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, g_EVKBackend->evkSwapchain.imageViews, g_EVKBackend->evkSwapchain.imageCount, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.format.format) == evk_Success, "Failed to create viewport framebuffers");
    }

    // buffers
    evkBuffer* cameraBuffer = evk_buffer_create(g_EVKBackend->evkDevice.device, g_EVKBackend->evkDevice.physicalDevice, sizeof(evkCameraUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, EVK_CONCURRENTLY_RENDERED_FRAMES);
    EVK_ASSERT(shashtable_insert(g_EVKBackend->buffers, "MainCamera", cameraBuffer) == TOOLBOX_SUCCESS, "Failed to insert camera buffer into the buffer library");

    // pipelines
    evkRenderpass* renderpass = evk_using_viewport() ? &g_EVKBackend->evkViewportRenderphase.evkRenderpass : &g_EVKBackend->evkMainRenderphase.evkRenderpass;
    EVK_ASSERT(evk_pipeline_sprite_create(g_EVKBackend->pipelines, renderpass, &g_EVKBackend->evkPickingRenderphase.evkRenderpass, g_EVKBackend->evkDevice.device) == evk_Success, "Failed to create sprite pipelines");
    EVK_ASSERT(evk_pipeline_billboard_create(g_EVKBackend->pipelines, renderpass, &g_EVKBackend->evkPickingRenderphase.evkRenderpass, g_EVKBackend->evkDevice.device) == evk_Success, "Failed to create billboard pipelines");
    EVK_ASSERT(evk_pipeline_grid_create(g_EVKBackend->pipelines, renderpass, g_EVKBackend->evkDevice.device) == evk_Success, "Failed to create grid pipelines");

    return evk_Success;
}

EVK_FUNC void evk_shutdown_backend() {
    evk_buffer_destroy(g_EVKBackend->evkDevice.device, (evkBuffer*)shashtable_lookup(g_EVKBackend->buffers, "MainCamera"));
    shashtable_destroy(g_EVKBackend->buffers);

    evk_pipeline_grid_destroy(g_EVKBackend->pipelines, g_EVKBackend->evkDevice.device);
    evk_pipeline_billboard_destroy(g_EVKBackend->pipelines, g_EVKBackend->evkDevice.device);
    evk_pipeline_sprite_destroy(g_EVKBackend->pipelines, g_EVKBackend->evkDevice.device);
    shashtable_destroy(g_EVKBackend->pipelines);

    if (evk_using_viewport()) {
        evk_renderphase_viewport_destroy(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device);
    }
    
    evk_renderphase_ui_destroy(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device);
    evk_renderphase_picking_destroy(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device);
    evk_renderphase_main_destroy(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device);

    ievk_sync_destroy(&g_EVKBackend->evkSync, g_EVKBackend->evkDevice.device);
    ievk_swapchain_destroy(&g_EVKBackend->evkSwapchain, g_EVKBackend->evkDevice.device);
    ievk_device_destroy(&g_EVKBackend->evkDevice);
    ievk_instance_destroy(&g_EVKBackend->evkInstance);

    m_free(g_EVKBackend);
}

EVK_FUNC void evk_update_backend(float timestep) {
    evkCamera* mainCamera = evk_get_main_camera();
    evk_camera_update(mainCamera, timestep);
}

EVK_FUNC void evk_render_backend(float timestep, bool* mustResize) {
    if (evk_currently_minimized()) return;

    evkCamera* mainCamera = evk_get_main_camera();
    evkCameraUBO mainCameraData = { 0 };
    mainCameraData.view = evk_camera_get_view(mainCamera);
    mainCameraData.viewInverse = evk_camera_get_view_inverse(mainCamera);
    mainCameraData.proj = evk_camera_get_perspective(mainCamera);
    evkBuffer* buffer = (evkBuffer*)shashtable_lookup(evk_get_buffers_library(), "MainCamera");
    evk_buffer_copy(buffer, evk_get_current_frame(), &mainCameraData, sizeof(evkCameraUBO), 0);

    // second phase
    vkWaitForFences(g_EVKBackend->evkDevice.device, 1, & g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame], VK_TRUE, UINT64_MAX);
    VkResult res = vkAcquireNextImageKHR(g_EVKBackend->evkDevice.device, g_EVKBackend->evkSwapchain.swapchain, UINT64_MAX, g_EVKBackend->evkSync.imageAvailableSemaphores[g_EVKBackend->evkSync.currentFrame], VK_NULL_HANDLE, &g_EVKBackend->evkSwapchain.imageIndex);

    if (res == VK_ERROR_OUT_OF_DATE_KHR)
    {
        float2 framebufferSize = evk_get_size();
        VkExtent2D extent = { 0 };
        extent.width = (uint32_t)framebufferSize.x;
        extent.height = (uint32_t)framebufferSize.y;
        ievk_resize(extent);
        *mustResize = false;

        g_EVKBackend->evkSync.currentFrame = (g_EVKBackend->evkSync.currentFrame + 1) % EVK_CONCURRENTLY_RENDERED_FRAMES;
        return;
    }

    EVK_ASSERT(res == VK_SUCCESS || res == VK_SUBOPTIMAL_KHR, "Renderer update was not able to aquire an image from the swapchain");
    vkResetFences(g_EVKBackend->evkDevice.device, 1, &g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame]);

    // render phases
    g_EVKBackend->currentRenderphase = evk_Renderphase_Type_Main;
    evk_renderphase_main_update(&g_EVKBackend->evkMainRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_using_viewport(), evk_get_render_callback());
    
    g_EVKBackend->currentRenderphase = evk_Renderphase_Type_Picking;
    evk_renderphase_picking_update(&g_EVKBackend->evkPickingRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_using_viewport(), evk_get_render_callback());
    
    if (evk_using_viewport()) {
        g_EVKBackend->currentRenderphase = evk_Renderphase_Type_Viewport;
        evk_renderphase_viewport_update(&g_EVKBackend->evkViewportRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_using_viewport(), evk_get_render_callback());
    }

    g_EVKBackend->currentRenderphase = evk_Renderphase_Type_UI;
    evk_renderphase_ui_update(&g_EVKBackend->evkUIRenderphase, g_EVKBackend->evkDevice.device, timestep, g_EVKBackend->evkSync.currentFrame, g_EVKBackend->evkSwapchain.extent, g_EVKBackend->evkSwapchain.imageIndex, evk_get_renderui_callback());

    // submit command buffers
    VkSwapchainKHR swapChains[] = { g_EVKBackend->evkSwapchain.swapchain };
    VkSemaphore waitSemaphores[] = { g_EVKBackend->evkSync.imageAvailableSemaphores[g_EVKBackend->evkSync.currentFrame] };
    VkSemaphore signalSemaphores[] = { g_EVKBackend->evkSync.finishedRenderingSemaphores[g_EVKBackend->evkSwapchain.imageIndex] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (evk_using_viewport()) {
        VkCommandBuffer commandBuffers[4] = { 0 };
        commandBuffers[0] = g_EVKBackend->evkMainRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[1] = g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[2] = g_EVKBackend->evkViewportRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[3] = g_EVKBackend->evkUIRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];

        submitInfo.commandBufferCount = EVK_STATIC_ARRAY_SIZE(commandBuffers);
        submitInfo.pCommandBuffers = commandBuffers;

        VkResult queueSubmit = vkQueueSubmit(g_EVKBackend->evkDevice.graphicsQueue, 1, &submitInfo, g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame]);
        if (queueSubmit != VK_SUCCESS) {
            EVK_ASSERT(1, "Renderer update was not able to submit frame to graphics queue");
        }
    }

    else {
        VkCommandBuffer commandBuffers[3] = { 0 };
        commandBuffers[0] = g_EVKBackend->evkMainRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[1] = g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];
        commandBuffers[2] = g_EVKBackend->evkUIRenderphase.evkRenderpass.cmdBuffers[g_EVKBackend->evkSync.currentFrame];

        submitInfo.commandBufferCount = EVK_STATIC_ARRAY_SIZE(commandBuffers);
        submitInfo.pCommandBuffers = commandBuffers;

        VkResult queueSubmit = vkQueueSubmit(g_EVKBackend->evkDevice.graphicsQueue, 1, &submitInfo, g_EVKBackend->evkSync.framesInFlightFences[g_EVKBackend->evkSync.currentFrame]);
        if (queueSubmit != VK_SUCCESS) {
            EVK_ASSERT(1, "Renderer update was not able to submit frame to graphics queue");
        }
    }

    // present the image
    VkPresentInfoKHR presentInfo;
    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &g_EVKBackend->evkSwapchain.imageIndex;

    res = vkQueuePresentKHR(g_EVKBackend->evkDevice.graphicsQueue, &presentInfo);

    // failed to present the image, must recreate
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || *mustResize == true ) {
        float2 framebufferSize = evk_get_size();
        VkExtent2D extent = { 0 };
        extent.width = (uint32_t)framebufferSize.x;
        extent.height = (uint32_t)framebufferSize.y;
        ievk_resize(extent);
        *mustResize = false;
    }

    else if (res != VK_SUCCESS) {
        EVK_ASSERT(1, "Renderer update was not able to properly present the graphics queue frame");
    }

    // advance to the next frame for the next render call
    g_EVKBackend->evkSync.currentFrame = (g_EVKBackend->evkSync.currentFrame + 1) % EVK_CONCURRENTLY_RENDERED_FRAMES;
}

EVK_FUNC uint32_t evk_pick_object_backend(float2 xy) {
    VkResult res;
    uint32_t pixelValue = 0;

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingMemory = VK_NULL_HANDLE;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;

    // create staging buffer (4 bytes for one uint32)
    VkBufferCreateInfo bufferCI;
    memset(&bufferCI, 0, sizeof(VkBufferCreateInfo));
    bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCI.size = sizeof(uint32_t);
    bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    res = vkCreateBuffer(g_EVKBackend->evkDevice.device, &bufferCI, NULL, &stagingBuffer);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create staging buffer for picking");
        return 0;
    }

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(g_EVKBackend->evkDevice.device, stagingBuffer, &memReq);

    VkDeviceSize alignedSize = (memReq.size + 3) & ~3;

    uint32_t memType = evk_device_find_suitable_memory_type(g_EVKBackend->evkDevice.physicalDevice, memReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (memType == UINT32_MAX) {
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "No suitable memory type for picking");
        return 0;
    }

    VkMemoryAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = alignedSize;
    allocInfo.memoryTypeIndex = memType;

    res = vkAllocateMemory(g_EVKBackend->evkDevice.device, &allocInfo, NULL, &stagingMemory);
    if (res != VK_SUCCESS) {
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "Failed to allocate memory for picking");
        return 0;
    }

    res = vkBindBufferMemory(g_EVKBackend->evkDevice.device, stagingBuffer, stagingMemory, 0);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "Failed to bind buffer memory for picking");
        return 0;
    }

    // allocate command buffer
    VkCommandBufferAllocateInfo cmdAlloc;
    memset(&cmdAlloc, 0, sizeof(VkCommandBufferAllocateInfo));
    cmdAlloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAlloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAlloc.commandPool = g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool;
    cmdAlloc.commandBufferCount = 1;

    res = vkAllocateCommandBuffers(g_EVKBackend->evkDevice.device, &cmdAlloc, &cmdBuffer);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        EVK_LOG(evk_Error, "Failed to allocate command buffer for picking");
        return 0;
    }

    // begin command buffer
    VkCommandBufferBeginInfo beginInfo;
    memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    res = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to begin command buffer for picking");
        return 0;
    }

    // convert screen coordinates to framebuffer coordinates
    uint32_t fbX = (uint32_t)(xy.x * 1366 / 1009);  // scale from viewport to framebuffer NOTE: Fix this
    uint32_t fbY = (uint32_t)(xy.y * 768 / 752);

    // copy ONE pixel from the picking image to staging buffer
    VkBufferImageCopy region = { 0 };
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset.x = (int32_t)fbX;
    region.imageOffset.y = (int32_t)fbY;
    region.imageOffset.z = 0;
    region.imageExtent.width = 1;   // ONLY ONE PIXEL
    region.imageExtent.height = 1;  // ONLY ONE PIXEL
    region.imageExtent.depth = 1;
    vkCmdCopyImageToBuffer(cmdBuffer, g_EVKBackend->evkPickingRenderphase.colorImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingBuffer, 1,  &region);

    // end command buffer
    res = vkEndCommandBuffer(cmdBuffer);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to end command buffer for picking");
        return 0;
    }

    // create fence for synchronization
    VkFenceCreateInfo fenceCI;
    memset(&fenceCI, 0, sizeof(VkFenceCreateInfo));
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    res = vkCreateFence(g_EVKBackend->evkDevice.device, &fenceCI, NULL, &fence);
    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to create fence for picking");
        return 0;
    }

    // submit command buffer
    VkSubmitInfo submit;
    memset(&submit, 0, sizeof(VkSubmitInfo));
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmdBuffer;

    res = vkQueueSubmit(g_EVKBackend->evkDevice.graphicsQueue, 1, &submit, fence);
    if (res != VK_SUCCESS) {
        vkDestroyFence(g_EVKBackend->evkDevice.device, fence, NULL);
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to submit picking command buffer");
        return 0;
    }

    // wait for completion
    res = vkWaitForFences(g_EVKBackend->evkDevice.device, 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(g_EVKBackend->evkDevice.device, fence, NULL);

    if (res != VK_SUCCESS) {
        vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
        vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);
        vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
        EVK_LOG(evk_Error, "Failed to wait for picking fence");
        return 0;
    }

    // read the pixel value
    void* data = NULL;
    res = vkMapMemory(g_EVKBackend->evkDevice.device, stagingMemory, 0, sizeof(uint32_t), 0, &data);

    if (res == VK_SUCCESS && data != NULL) {
        pixelValue = *(uint32_t*)data;
        vkUnmapMemory(g_EVKBackend->evkDevice.device, stagingMemory);

        if (pixelValue != 0) {
            EVK_LOG(evk_Info, "Picked entity ID: %u at framebuffer coordinates (%u, %u)", pixelValue, fbX, fbY);
        }
    }
    else {
        EVK_LOG(evk_Error, "Failed to map memory for picking read");
    }

    // cleanup
    vkFreeCommandBuffers(g_EVKBackend->evkDevice.device, g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool, 1, &cmdBuffer);
    vkFreeMemory(g_EVKBackend->evkDevice.device, stagingMemory, NULL);
    vkDestroyBuffer(g_EVKBackend->evkDevice.device, stagingBuffer, NULL);

    return pixelValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vulkan getters/setters
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC VkInstance evk_get_instance() {
    return g_EVKBackend->evkInstance.instance;
}

EVK_FUNC VkPhysicalDevice evk_get_physical_device() {
    return g_EVKBackend->evkDevice.physicalDevice;
}

EVK_FUNC VkPhysicalDeviceProperties evk_get_physical_device_properties() {
    return g_EVKBackend->evkDevice.physicalProps;
}

EVK_FUNC VkPhysicalDeviceFeatures evk_get_physical_device_features() {
    return g_EVKBackend->evkDevice.phyiscalFeatures;
}

EVK_FUNC VkPhysicalDeviceMemoryProperties evk_get_physical_device_memory_properties() {
    return g_EVKBackend->evkDevice.physicaMemProps;
}

EVK_FUNC VkDevice evk_get_device() {
    return g_EVKBackend->evkDevice.device;
}

EVK_FUNC VkQueue evk_get_graphics_queue() {
    return g_EVKBackend->evkDevice.graphicsQueue;
}

EVK_FUNC uint32_t evk_get_graphics_queue_family() {
    return g_EVKBackend->evkDevice.graphicsIndex;
}

EVK_FUNC uint32_t evk_get_swapchain_image_count() {
    return g_EVKBackend->evkSwapchain.imageCount;
}

EVK_FUNC VkRenderPass evk_get_renderpass(evkRenderphaseType type) {
    switch (type)
    {
        case evk_Renderphase_Type_Main: return g_EVKBackend->evkMainRenderphase.evkRenderpass.renderpass;
        case evk_Renderphase_Type_Picking: return g_EVKBackend->evkPickingRenderphase.evkRenderpass.renderpass;
        case evk_Renderphase_Type_UI: return g_EVKBackend->evkUIRenderphase.evkRenderpass.renderpass;
        case evk_Renderphase_Type_Viewport:
        {
            if (!evk_using_viewport()) {
                EVK_LOG(evk_Error, "Requesting viewport renderphase but viewport was not enabled");
                return VK_NULL_HANDLE;
            }

            return g_EVKBackend->evkViewportRenderphase.evkRenderpass.renderpass;
        }
    }
    return VK_NULL_HANDLE;
}

EVK_FUNC VkCommandPool evk_get_command_pool(evkRenderphaseType type) {
    switch (type)
    {
        case evk_Renderphase_Type_Main: return g_EVKBackend->evkMainRenderphase.evkRenderpass.cmdPool;
        case evk_Renderphase_Type_Picking: return g_EVKBackend->evkPickingRenderphase.evkRenderpass.cmdPool;
        case evk_Renderphase_Type_UI: return g_EVKBackend->evkUIRenderphase.evkRenderpass.cmdPool;
        case evk_Renderphase_Type_Viewport:
        {
            if (!evk_using_viewport()) {
                EVK_LOG(evk_Error, "Requesting viewport command pool but viewport was not enabled");
                return VK_NULL_HANDLE;
            }

            return g_EVKBackend->evkViewportRenderphase.evkRenderpass.cmdPool;
        }
    }
    return VK_NULL_HANDLE;
}

EVK_FUNC VkDescriptorSetLayout evk_get_ui_descriptor_set_layout() {
    return g_EVKBackend->evkUIRenderphase.descriptorSetLayout;
}

EVK_FUNC void* evk_get_renderphase(evkRenderphaseType type) {
    switch (type)
    {
        case evk_Renderphase_Type_Main: return &g_EVKBackend->evkMainRenderphase;
        case evk_Renderphase_Type_Picking: return &g_EVKBackend->evkPickingRenderphase;
        case evk_Renderphase_Type_UI: return &g_EVKBackend->evkUIRenderphase;
        case evk_Renderphase_Type_Viewport: return &g_EVKBackend->evkViewportRenderphase;
    }
    EVK_LOG(evk_Fatal, "This renderphase (%d) is undefined", type);
    return NULL;
}

EVK_FUNC shashtable* evk_get_pipelines_library() {
    return g_EVKBackend->pipelines;
}

EVK_FUNC shashtable* evk_get_buffers_library() {
    return g_EVKBackend->buffers;
}

EVK_FUNC uint32_t evk_get_current_frame() {
    return g_EVKBackend->evkSync.currentFrame;
}

EVK_FUNC evkRenderphaseType evk_get_current_renderphase_type() {
    return g_EVKBackend->currentRenderphase;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vulkan device-related 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC void evk_device_wait_idle() {
    vkDeviceWaitIdle(g_EVKBackend->evkDevice.device);
}

EVK_FUNC evkQueueFamily evk_device_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
    evkQueueFamily indices = { 0 };
    indices.graphics = UINT32_MAX;
    indices.present = UINT32_MAX;
    indices.compute = UINT32_MAX;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = (VkQueueFamilyProperties*)m_malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
            indices.graphicsFound = 1;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices.compute = i;
            indices.computeFound = 1;
        }

        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        if (present_support) {
            indices.present = i;
            indices.presentFound = 1;
        }

        if (indices.graphicsFound && indices.presentFound && indices.computeFound) break;
    }

    m_free(queue_families);
    return indices;
}

EVK_FUNC evkResult evk_device_create_image(VkExtent2D size, uint32_t mipLevels, uint32_t arrayLayers, VkDevice device, VkPhysicalDevice physicalDevice, VkImage* image, VkDeviceMemory* memory, VkFormat format, evkMSAA samples, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkImageCreateFlags flags) {
    VkImageCreateInfo imageCI;
    memset(&imageCI, 0, sizeof(VkImageCreateInfo));
    imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCI.pNext = NULL;
    imageCI.flags = flags;
    imageCI.imageType = VK_IMAGE_TYPE_2D;
    imageCI.extent.width = size.width;
    imageCI.extent.height = size.height;
    imageCI.extent.depth = 1;
    imageCI.mipLevels = mipLevels;
    imageCI.arrayLayers = arrayLayers;
    imageCI.format = format;
    imageCI.tiling = tiling;
    imageCI.usage = usage;
    imageCI.samples = (VkSampleCountFlagBits)samples;
    imageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageCI, NULL, image) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create device image, check vulkan validations for a more detailed explanation");
        return evk_Failure;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, *image, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = evk_device_find_suitable_memory_type(physicalDevice, memRequirements.memoryTypeBits, memoryProperties);

    if (vkAllocateMemory(device, &allocInfo, NULL, memory) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate memory for the device image, check vulkan validations for a more detailed explanation");
        return evk_Failure;
    }

    if (vkBindImageMemory(device, *image, *memory, 0) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to bind memory with device image, check vulkan validations for a more detailed explanation");
        return evk_Failure;
    }

    return evk_Success;
}

EVK_FUNC evkResult evk_device_create_image_view(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, uint32_t mipLevels, uint32_t layerCount, VkImageViewType viewType, const VkComponentMapping* swizzle, VkImageView* outView) {
    if (mipLevels == 0 || layerCount == 0) {
        EVK_LOG(evk_Error, "Invalid mipLevels or layerCount (must be >= 1)");
        return evk_Failure;
    }

    VkImageViewCreateInfo imageViewCI;
    memset(&imageViewCI, 0, sizeof(VkImageViewCreateInfo));
    imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCI.image = image;
    imageViewCI.viewType = viewType;
    imageViewCI.format = format;
    imageViewCI.subresourceRange.aspectMask = aspect;
    imageViewCI.subresourceRange.baseMipLevel = 0;
    imageViewCI.subresourceRange.levelCount = mipLevels;
    imageViewCI.subresourceRange.baseArrayLayer = 0;
    imageViewCI.subresourceRange.layerCount = layerCount;
    imageViewCI.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCI.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCI.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCI.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    // using a different swizzling
    if (swizzle) {
        imageViewCI.components = *swizzle;
    }

    if (vkCreateImageView(device, &imageViewCI, NULL, outView) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create image view");
        return evk_Failure;
    }

    return evk_Success;
}

EVK_FUNC evkResult evk_device_create_image_sampler(VkDevice device, VkPhysicalDevice physicalDevice, VkFilter min, VkFilter mag, VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w, float mipLevels, VkSampler* outSampler) {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physicalDevice, &props);

    VkSamplerCreateInfo samplerCI;
    memset(&samplerCI, 0, sizeof(VkSamplerCreateInfo));
    samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCI.magFilter = mag;
    samplerCI.minFilter = min;
    samplerCI.addressModeU = u;
    samplerCI.addressModeV = v;
    samplerCI.addressModeW = w;
    samplerCI.anisotropyEnable = VK_TRUE;
    samplerCI.maxAnisotropy = props.limits.maxSamplerAnisotropy;
    samplerCI.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCI.unnormalizedCoordinates = VK_FALSE;
    samplerCI.compareEnable = VK_FALSE;
    samplerCI.maxLod = mipLevels;
    samplerCI.minLod = 0.0f;
    samplerCI.mipLodBias = 0.0f;
    samplerCI.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(device, &samplerCI, NULL, outSampler) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create image sampler");
        return evk_Failure;
    }

    return evk_Success;
}

EVK_FUNC evkResult evk_device_create_image_descriptor_set(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkSampler sampler, VkImageView view, VkDescriptorSet* outDescriptor) {
    VkDescriptorSetAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    if (vkAllocateDescriptorSets(device, &allocInfo, outDescriptor) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate descriptor set");
        return evk_Failure;
    }

    // update descriptor set
    VkDescriptorImageInfo descImage;
    memset(&descImage, 0, sizeof(VkDescriptorImageInfo));
    descImage.sampler = sampler;
    descImage.imageView = view;
    descImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet writeDesc;
    memset(&writeDesc, 0, sizeof(VkWriteDescriptorSet));
    writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDesc.dstSet = *outDescriptor;
    writeDesc.descriptorCount = 1;
    writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDesc.pImageInfo = &descImage;
    vkUpdateDescriptorSets(device, 1, &writeDesc, 0, NULL);

    return evk_Success;
}

EVK_FUNC void evk_device_create_image_mipmaps(VkDevice device, VkQueue queue, VkCommandBuffer cmdBuffer, int32_t width, int32_t height, int32_t mipLevels, VkImage image) {
    if (mipLevels <= 1) return;

    VkImageMemoryBarrier barrier;
    memset(&barrier, 0, sizeof(VkImageMemoryBarrier));
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = width;
    int32_t mipHeight = height;

    for (int32_t i = 1; i < mipLevels; i++) {

        // transition previous mip level to transfer source
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

        // blit from previous level to current level
        VkImageBlit blit = { 0 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.srcOffsets[1].x = mipWidth;
        blit.srcOffsets[1].y = mipHeight;
        blit.srcOffsets[1].z = 1;
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;
        blit.dstOffsets[1].x = mipWidth > 1 ? mipWidth / 2 : 1;
        blit.dstOffsets[1].y = mipHeight > 1 ? mipHeight / 2 : 1;
        blit.dstOffsets[1].z = 1;
        vkCmdBlitImage(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        // transition previous level to shader read
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    // transition last mip level
    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);
}

EVK_FUNC void evk_device_create_image_memory_barrier(VkCommandBuffer cmdBuffer, VkImage image, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange) {
    VkImageMemoryBarrier imageMemoryBarrier;
    memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = 0;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.srcAccessMask = srcAccessFlags;
    imageMemoryBarrier.dstAccessMask = dstAccessFlags;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier);
}

EVK_FUNC uint32_t evk_device_find_suitable_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    EVK_LOG(evk_Error, "Failed to find suitable memory type");
    return UINT32_MAX;
}

EVK_FUNC VkFormat evk_device_find_suitable_format(VkPhysicalDevice physicalDevice, const VkFormat* candidates, uint32_t candidatesCount, VkImageTiling tiling, VkFormatFeatureFlags features) {
    VkFormat resFormat = VK_FORMAT_UNDEFINED;
    for (uint32_t i = 0; i < candidatesCount; i++) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, candidates[i], &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) resFormat = candidates[i];
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) resFormat = candidates[i];
    }

    EVK_ASSERT(resFormat != VK_FORMAT_UNDEFINED, "Failed to find suitable VkFormat");
    return resFormat;
}

EVK_FUNC VkFormat evk_device_find_depth_format(VkPhysicalDevice physicalDevice) {
    const VkFormat candidates[] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT };
    VkFormat format = evk_device_find_suitable_format(physicalDevice, candidates, 3, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    return format;
}

EVK_FUNC evkResult evk_device_create_buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data) {
    VkBufferCreateInfo bufferCI;
    memset(&bufferCI, 0, sizeof(VkBufferCreateInfo));
    bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCI.size = size;
    bufferCI.usage = usage;
    bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult res = vkCreateBuffer(device, &bufferCI, NULL, buffer);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create buffer on GPU");
        return evk_Failure;
    }

    VkMemoryRequirements memRequirements;
    memset(&memRequirements, 0, sizeof(VkMemoryRequirements));
    vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);

    // allocate memory for the buffer and bind it
    VkMemoryAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = evk_device_find_suitable_memory_type(physicalDevice, memRequirements.memoryTypeBits, properties);

    res = vkAllocateMemory(device, &allocInfo, NULL, memory);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate memory for GPU buffer");
        vkDestroyBuffer(device, *buffer, NULL);
        return evk_Failure;
    }

    res = vkBindBufferMemory(device, *buffer, *memory, 0);
    if (res != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to bind GPU memory with buffer");
        vkDestroyBuffer(device, *buffer, NULL);
        vkFreeMemory(device, *memory, NULL);
        return evk_Failure;
    }

    // map data if passing it
    if (data != NULL) {
        void* mapped;
        res = vkMapMemory(device, *memory, 0, size, 0, &mapped);
        if (res == VK_SUCCESS) {
            memcpy(mapped, data, size);
            vkUnmapMemory(device, *memory);
        }

        else {
            EVK_LOG(evk_Error, "Failed to map memory for data upload (VkResult: %d)", res);
        }
    }
    return evk_Success;
}

EVK_FUNC VkCommandBuffer evk_device_begin_commandbuffer_singletime(VkDevice device, VkCommandPool cmdPool) {
    VkCommandBufferAllocateInfo cmdBufferAllocInfo;
    memset(&cmdBufferAllocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
    cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferAllocInfo.commandPool = cmdPool;
    cmdBufferAllocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, &commandBuffer) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate command buffers for single time use");
    }

    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    memset(&cmdBufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(commandBuffer, &cmdBufferBeginInfo)) {
        EVK_LOG(evk_Error, "Failed to begin single time command buffer");
    }

    return commandBuffer;
}

EVK_FUNC evkResult evk_device_end_commandbuffer_singletime(VkDevice device, VkCommandPool cmdPool, VkCommandBuffer cmdBuffer, VkQueue queue) {
    if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to end command buffer recording");
        return evk_Failure;
    }

    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;

    if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to submit command buffer to queue");
        return evk_Failure;
    }

    if (vkQueueWaitIdle(queue) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to await queue response from sent command buffer");
        return evk_Failure;
    }

    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
    return evk_Success;
}

EVK_FUNC int32_t evk_device_calculate_image_mipmap(uint32_t width, uint32_t height, bool uiImage) {
    if (uiImage || evk_get_msaa() != evk_Msaa_Off) return 1; // UI textures or MSAA textures cannot have mipmaps
    return (int32_t)f_floor(f_log2(VECMATH_MAX((float)width, (float)height))) + 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vulkan buffer-related 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkBuffer* evk_buffer_create(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, uint32_t frameCount) {
    if (size == 0 || frameCount == 0) {
        EVK_LOG(evk_Error, "Invalid buffer size or frame count");
        return NULL;
    }

    evkBuffer* buffer = (evkBuffer*)m_malloc(sizeof(evkBuffer));
    if (!buffer) {
        EVK_LOG(evk_Error, "Failed to allocate buffer structure");
        return NULL;
    }

    buffer->size = size;
    buffer->usage = usage;
    buffer->memoryProperties = memoryProperties;
    buffer->frameCount = frameCount;

    // allocate arrays for per-frame resources
    buffer->buffers = (VkBuffer*)m_malloc(sizeof(VkBuffer) * frameCount);
    buffer->memories = (VkDeviceMemory*)m_malloc(sizeof(VkDeviceMemory) * frameCount);
    buffer->mappedPointers = (void**)m_malloc(sizeof(void*) * frameCount);
    buffer->isMapped = (bool*)m_malloc(sizeof(bool) * frameCount);

    if (!buffer->buffers || !buffer->memories || !buffer->mappedPointers || !buffer->isMapped) {
        EVK_LOG(evk_Error, "Failed to allocate buffer arrays");
        evk_buffer_destroy(device, buffer);
        return NULL;
    }

    memset(buffer->mappedPointers, 0, sizeof(void*) * frameCount);
    memset(buffer->isMapped, 0, sizeof(bool) * frameCount);

    // create each buffer
    for (uint32_t i = 0; i < frameCount; i++)
    {
        VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkResult result = vkCreateBuffer(device, &bufferInfo, NULL, &buffer->buffers[i]);
        if (result != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to create buffer %u: %d", i, result);
            evk_buffer_destroy(device, buffer);
            return NULL;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer->buffers[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo;
        memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = evk_device_find_suitable_memory_type(physicalDevice, memRequirements.memoryTypeBits, memoryProperties);

        result = vkAllocateMemory(device, &allocInfo, NULL, &buffer->memories[i]);
        if (result != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to allocate buffer memory %u: %d", i, result);
            evk_buffer_destroy(device, buffer);
            return NULL;
        }

        result = vkBindBufferMemory(device, buffer->buffers[i], buffer->memories[i], 0);
        if (result != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to bind buffer memory %u: %d", i, result);
            evk_buffer_destroy(device, buffer);
            return NULL;
        }

        // auto-map if host visible
        if (memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            evkResult evkResult = evk_buffer_map(device, buffer, i);
            if (evkResult != evk_Success) {
                EVK_LOG(evk_Warn, "Failed to auto-map buffer %u: %d", i, result);
            }
        }
    }

    return buffer;
}

EVK_FUNC void evk_buffer_destroy(VkDevice device, evkBuffer* buffer) {
    if (!buffer) return;

    if (buffer->buffers) {
        for (uint32_t i = 0; i < buffer->frameCount; i++) {
            if (buffer->buffers[i] != VK_NULL_HANDLE) {
                // unmap if mapped
                if (buffer->isMapped[i]) {
                    evk_buffer_unmap(device, buffer, i);
                }

                vkDestroyBuffer(device, buffer->buffers[i], NULL);
                buffer->buffers[i] = VK_NULL_HANDLE;
            }
        }
        m_free(buffer->buffers);
    }

    if (buffer->memories) {
        for (uint32_t i = 0; i < buffer->frameCount; i++) {
            if (buffer->memories[i] != VK_NULL_HANDLE) {
                vkFreeMemory(device, buffer->memories[i], NULL);
                buffer->memories[i] = VK_NULL_HANDLE;
            }
        }
        m_free(buffer->memories);
    }

    if (buffer->mappedPointers) m_free(buffer->mappedPointers);
    if (buffer->isMapped) m_free(buffer->isMapped);

    m_free(buffer);
}

EVK_FUNC evkResult evk_buffer_map(VkDevice device, evkBuffer* buffer, uint32_t frameIndex) {
    if (!buffer || frameIndex >= buffer->frameCount) return evk_Failure;
    if (buffer->isMapped[frameIndex]) return evk_Success; // already mapped

    if (!(buffer->memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
        EVK_LOG(evk_Error, "Cannot map non-host-visible buffer");
        return evk_Failure;
    }

    VkResult result = vkMapMemory(device, buffer->memories[frameIndex], 0, buffer->size, 0, &buffer->mappedPointers[frameIndex]);
    if (result == VK_SUCCESS) buffer->isMapped[frameIndex] = true;
    else EVK_LOG(evk_Error, "Failed to map buffer: %d", result);

    return evk_Success;
}

EVK_FUNC evkResult evk_buffer_unmap(VkDevice device, evkBuffer* buffer, uint32_t frameIndex) {
    if (!buffer || frameIndex >= buffer->frameCount) return evk_Failure;
    if (!buffer->isMapped[frameIndex]) return evk_Success; // not mapped

    vkUnmapMemory(device, buffer->memories[frameIndex]);
    buffer->mappedPointers[frameIndex] = NULL;
    buffer->isMapped[frameIndex] = false;

    return evk_Success;
}

EVK_FUNC evkResult evk_buffer_copy(evkBuffer* buffer, uint32_t frameIndex, const void* data, VkDeviceSize size, VkDeviceSize offset) {
    if (!buffer || !data || size == 0) return evk_Failure;

    if (frameIndex >= buffer->frameCount) {
        EVK_LOG(evk_Error, "Frame index %u out of bounds", frameIndex);
        return evk_Failure;
    }

    if (offset + size > buffer->size) {
        EVK_LOG(evk_Error, "Copy exceeds buffer size");
        return evk_Failure;
    }

    if (!buffer->isMapped[frameIndex]) {
        EVK_LOG(evk_Error, "Buffer not mapped at frame %u", frameIndex);
        return evk_Failure;
    }

    memcpy((char*)buffer->mappedPointers[frameIndex] + offset, data, size);
    return evk_Success;
}

EVK_FUNC evkResult evk_buffer_flush(VkDevice device, evkBuffer* buffer, uint32_t frameIndex, VkDeviceSize size, VkDeviceSize nonCoherentAtomSize, VkDeviceSize offset) {
    if (!buffer || frameIndex >= buffer->frameCount) return evk_Failure;

    if (!(buffer->memoryProperties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
    {
        VkMappedMemoryRange memoryRange = { VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE };
        memoryRange.memory = buffer->memories[frameIndex];

        VkDeviceSize atomSize = nonCoherentAtomSize;
        VkDeviceSize alignedOffset = offset & ~(atomSize - 1);
        VkDeviceSize end = offset + size;
        VkDeviceSize alignedEnd = (end + atomSize - 1) & ~(atomSize - 1);
        VkDeviceSize alignedSize = alignedEnd - alignedOffset;

        // clamp to buffer size
        if (alignedOffset + alignedSize > buffer->size) {
            alignedSize = buffer->size - alignedOffset;
        }

        memoryRange.offset = alignedOffset;
        memoryRange.size = alignedSize;

        if (vkFlushMappedMemoryRanges(device, 1, &memoryRange) != VK_SUCCESS) {
            return evk_Failure;
        }
    }

    return evk_Success;
}

EVK_FUNC evkResult evk_buffer_command_copy(VkCommandBuffer commandBuffer, evkBuffer* srcBuffer, uint32_t srcFrameIndex, evkBuffer* dstBuffer, uint32_t dstFrameIndex, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
    if (!srcBuffer || !dstBuffer || srcFrameIndex >= srcBuffer->frameCount || dstFrameIndex >= dstBuffer->frameCount) return evk_Failure;

    VkBufferCopy copyRegion = { 0 };
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;
    copyRegion.size = (size == VK_WHOLE_SIZE) ? srcBuffer->size - srcOffset : size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer->buffers[srcFrameIndex], dstBuffer->buffers[dstFrameIndex], 1, &copyRegion);

    return evk_Success;
}

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stb wrapping
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC unsigned char* evk_stb_load_from_file(const char* filepath, int32_t* width, int32_t* height, int32_t* channels, int32_t desiredChannels) {
    return stbi_load(filepath, width, height, channels, desiredChannels);
}

EVK_FUNC unsigned char* evk_stb_load_from_memory(const unsigned char* data, size_t length, int32_t* width, int32_t* height, int32_t* channels, int32_t desiredChannels) {
    return stbi_load_from_memory(data, (int32_t)length, width, height, channels, desiredChannels);
}

EVK_FUNC const char* evk_stb_failure_reason() {
    return stbi_failure_reason();
}

EVK_FUNC void evk_stb_free(void* data) {
    stbi_image_free(data);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// texture2d
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct evkTexture2D
{
    VkImage image;
    VkDeviceMemory mem;
    VkSampler sampler;
    VkImageView view;
    int32_t width;
    int32_t height;
    int32_t mipLevel;
    const char* path;
};

EVK_FUNC evkTexture2D* evk_texture2d_create_from_path(const char* path, bool ui) {
    if (path == NULL) return NULL;

    evkTexture2D* texture = (evkTexture2D*)m_malloc(sizeof(evkTexture2D));
    if (!texture) return NULL;

    memset(texture, 0, sizeof(evkTexture2D));
    texture->path = path;

    uint8_t* pixels = NULL;
    VkDevice device = evk_get_device();
    VkPhysicalDevice physicalDevice = evk_get_physical_device();
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory stagingMem = VK_NULL_HANDLE;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    evkResult result = evk_Success;
    bool success = false;

    do {
        int32_t channels = 0;
        const int32_t desiredChannels = 4;

        pixels = evk_stb_load_from_file(path, &texture->width, &texture->height, &channels, desiredChannels);
        
        if (!pixels) {
            EVK_LOG(evk_Error, "Failed to load texture %s", path);
            result = evk_Failure;
            break;
        }

        texture->mipLevel = evk_device_calculate_image_mipmap(texture->width, texture->height, ui);
        VkDeviceSize imageSize = (VkDeviceSize)(texture->width * texture->height * desiredChannels);
        result = evk_device_create_buffer
        (
            device,
            physicalDevice,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            imageSize,
            &staging, 
            &stagingMem, 
            NULL
        );
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create staging buffer for: %s", path);
            break;
        }

        void* data;
        if (vkMapMemory(device, stagingMem, 0, imageSize, 0, &data) != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to map staging memory for: %s", path);
            result = evk_Failure;
            break;
        }

        memcpy(data, pixels, imageSize);
        vkUnmapMemory(device, stagingMem);

        const VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
        const VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        evkRenderphaseType renderphaseType = evk_using_viewport() ? evk_Renderphase_Type_Viewport : evk_Renderphase_Type_Main;
        VkCommandPool cmdPool = evk_get_command_pool(renderphaseType);
        VkQueue graphicsQueue = evk_get_graphics_queue();

        VkExtent2D extent = { 0 };
        extent.width = (uint32_t)texture->width;
        extent.height = (uint32_t)texture->height;

        result = evk_device_create_image
        (
            extent,
            texture->mipLevel,
            1,
            device,
            physicalDevice,
            &texture->image,
            &texture->mem,
            format,
            (evkMSAA)texture->mipLevel,
            VK_IMAGE_TILING_OPTIMAL,
            usage,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            0
        );
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create vulkan image for %s", path);
            break;
        }

        cmdBuffer = evk_device_begin_commandbuffer_singletime(device, cmdPool);
        if (!cmdBuffer) {
            EVK_LOG(evk_Error, "Failed to begin command buffer for: %s", path);
            result = evk_Failure;
            break;
        }

        // transition to TRANSFER_DST
        VkImageMemoryBarrier barrier;
        memset(&barrier, 0, sizeof(VkImageMemoryBarrier));
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = texture->image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = texture->mipLevel;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

        // copy buffer to image
        VkBufferImageCopy region;
        memset(&region, 0, sizeof(VkBufferImageCopy));
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset.x = 0;
        region.imageOffset.y = 0;
        region.imageOffset.z = 0;
        region.imageExtent.width = texture->width;
        region.imageExtent.height = texture->height;
        region.imageExtent.depth = 1;
        vkCmdCopyBufferToImage(cmdBuffer, staging, texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        // generate mipmaps if needed
        if (texture->mipLevel > 1) {
            evk_device_create_image_mipmaps(device, graphicsQueue, cmdBuffer, texture->width, texture->height, texture->mipLevel, texture->image);
        }
        else {
            // transition to SHADER_READ_ONLY
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);
        }

        result = evk_device_end_commandbuffer_singletime(device, cmdPool, cmdBuffer, graphicsQueue);
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to submit command buffer for: %s", path);
            break;
        }

        // create image view and sampler
        result = evk_device_create_image_view(device, texture->image, format, VK_IMAGE_ASPECT_COLOR_BIT, texture->mipLevel, 1, VK_IMAGE_VIEW_TYPE_2D, NULL, &texture->view);
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create image view for: %s", path);
            break;
        }

        result = evk_device_create_image_sampler(device, physicalDevice, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, (float)texture->mipLevel, &texture->sampler);
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create sampler for: %s", path);
            break;
        }

        success = true;
    } while (0);

    // cleanup
    if (!success) {
        if (texture) {
            if (texture->image != VK_NULL_HANDLE || texture->mem != VK_NULL_HANDLE) {
                evk_texture2d_destroy(texture);
            }
            m_free(texture);
            texture = NULL;
        }
    }

    if (staging != VK_NULL_HANDLE) vkDestroyBuffer(device, staging, NULL);
    if (stagingMem != VK_NULL_HANDLE) vkFreeMemory(device, stagingMem, NULL);
    if (pixels) evk_stb_free(pixels);

    return texture;
}

EVK_FUNC evkTexture2D* evk_texture2d_create_from_buffer(uint8_t* buffer, size_t bufferLen, uint32_t width, uint32_t height, bool ui) {
    if (!buffer || width == 0 || height == 0) return NULL;

    evkTexture2D* texture = (evkTexture2D*)m_malloc(sizeof(evkTexture2D));
    if (!texture) return NULL;

    memset(texture, 0, sizeof(evkTexture2D));
    texture->width = width;
    texture->height = height;
    texture->path = NULL;  // no path for buffer textures
    texture->mipLevel = evk_device_calculate_image_mipmap(texture->width, texture->height, ui);

    VkDevice device = evk_get_device();
    VkPhysicalDevice physicalDevice = evk_get_physical_device();
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory stagingMem = VK_NULL_HANDLE;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    evkResult result = evk_Success;
    bool success = false;

    do
    {
        // validate buffer size (assuming RGBA format)
        VkDeviceSize expectedSize = (VkDeviceSize)(width * height * 4);
        if (bufferLen != expectedSize) {
            EVK_LOG(evk_Error, "Buffer size mismatch: expected %zu, got %zu", expectedSize, bufferLen);
            result = evk_Failure;
            break;
        }

        VkDeviceSize imageSize = expectedSize;
        result = evk_device_create_buffer
        (
            device,
            physicalDevice,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            imageSize,
            &staging,
            &stagingMem,
            NULL
        );
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create staging buffer for texture from buffer");
            break;
        }

        void* data;
        if (vkMapMemory(device, stagingMem, 0, imageSize, 0, &data) != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to map staging memory for texture from buffer");
            result = evk_Failure;
            break;
        }
        memcpy(data, buffer, imageSize);
        vkUnmapMemory(device, stagingMem);

        const VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
        const VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        evkRenderphaseType renderphaseType = evk_using_viewport() ? evk_Renderphase_Type_Viewport : evk_Renderphase_Type_Main;
        evkMSAA msaa = ui ? evk_Msaa_Off : evk_get_msaa();
        VkCommandPool cmdPool = evk_get_command_pool(renderphaseType);
        VkQueue graphicsQueue = evk_get_graphics_queue();

        VkExtent2D extent = { 0 };
        extent.width = (uint32_t)texture->width;
        extent.height = (uint32_t)texture->height;

        result = evk_device_create_image
        (
            extent,
            texture->mipLevel,
            1,
            device,
            physicalDevice,
            &texture->image,
            &texture->mem,
            format,
            msaa,
            VK_IMAGE_TILING_OPTIMAL,
            usage,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            0
        );
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create Vulkan image for texture from buffer");
            break;
        }

        cmdBuffer = evk_device_begin_commandbuffer_singletime(device, cmdPool);
        if (!cmdBuffer) {
            EVK_LOG(evk_Error, "Failed to begin command buffer for texture from buffer");
            result = evk_Failure;
            break;
        }

        // transition to TRANSFER_DST_OPTIMAL
        VkImageMemoryBarrier barrier;
        memset(&barrier, 0, sizeof(VkImageMemoryBarrier));
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = texture->image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = texture->mipLevel;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

        // copy buffer to image
        VkBufferImageCopy region = { 0 };
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset.x = 0;
        region.imageOffset.y = 0;
        region.imageOffset.z = 0;
        region.imageExtent.width = width;
        region.imageExtent.height = height;
        region.imageExtent.depth = 1;
        vkCmdCopyBufferToImage(cmdBuffer, staging, texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        // generate mipmaps
        if (texture->mipLevel > 1) {
            evk_device_create_image_mipmaps(device, graphicsQueue, cmdBuffer, width, height, texture->mipLevel, texture->image);
        }
        else {
            // transition to SHADER_READ_ONLY_OPTIMAL
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);
        }

        result = evk_device_end_commandbuffer_singletime(device, cmdPool, cmdBuffer, graphicsQueue);
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to submit command buffer for texture from buffer");
            break;
        }

        // create image view and sampler
        result = evk_device_create_image_view(device, texture->image, format, VK_IMAGE_ASPECT_COLOR_BIT, texture->mipLevel, 1, VK_IMAGE_VIEW_TYPE_2D, NULL, &texture->view);
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create image view for texture from buffer");
            break;
        }

        result = evk_device_create_image_sampler(device, physicalDevice, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, (float)texture->mipLevel, &texture->sampler);
        if (result != evk_Success) {
            EVK_LOG(evk_Error, "Failed to create sampler for texture from buffer");
            break;
        }

        success = true;
    } while (0);

    // cleanup
    if (!success) {
        if (texture) {
            if (texture->image != VK_NULL_HANDLE || texture->mem != VK_NULL_HANDLE) {
                evk_texture2d_destroy(texture);
            }
            m_free(texture);
            texture = NULL;
        }
    }

    if (staging != VK_NULL_HANDLE) vkDestroyBuffer(device, staging, NULL);
    if (stagingMem != VK_NULL_HANDLE) vkFreeMemory(device, stagingMem, NULL);

    return texture;
}

EVK_FUNC void evk_texture2d_destroy(evkTexture2D* texture) {
    EVK_ASSERT(texture != NULL, "Vulkan Texture is NULL");
    VkDevice device = evk_get_device();

    if (texture->sampler != VK_NULL_HANDLE) vkDestroySampler(device, texture->sampler, NULL);
    if (texture->view != VK_NULL_HANDLE) vkDestroyImageView(device, texture->view, NULL);
    if (texture->image != VK_NULL_HANDLE) vkDestroyImage(device, texture->image, NULL);
    if (texture->mem != VK_NULL_HANDLE) vkFreeMemory(device, texture->mem, NULL);

    m_free(texture);
}

EVK_FUNC const char* evk_texture2d_get_path(evkTexture2D* texture) {
    if (texture) {
        if (texture->path == NULL) {
            EVK_LOG(evk_Warn, "Texture has a NULL path, probably created as a buffer and not diskpath");
        }
        return texture->path;
    }

    return NULL;
}

EVK_FUNC uint32_t evk_texture2d_get_width(evkTexture2D* texture) {
    return texture ? texture->width : 0;
}

EVK_FUNC uint32_t evk_texture2d_get_height(evkTexture2D* texture) {
    return texture ? texture->height : 0;
}

EVK_FUNC uint32_t evk_texture2d_get_mip_levels(evkTexture2D* texture) {
    return texture ? texture->mipLevel : 0;
}

EVK_FUNC VkSampler evk_texture2d_get_sampler(evkTexture2D* texture) {
    return texture ? texture->sampler : VK_NULL_HANDLE;
}

EVK_FUNC VkImageView evk_texture2d_get_view(evkTexture2D* texture) {
    return texture ? texture->view : VK_NULL_HANDLE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// grid
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct evkGrid
{
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[EVK_CONCURRENTLY_RENDERED_FRAMES];
};

static void ievk_grid_refresh_data(evkGrid* grid) {
    if (!grid) return;

    VkDevice device = evk_get_device();

    for (size_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++)
    {
        evkBuffer* cameraBuffer = (evkBuffer*)shashtable_lookup(evk_get_buffers_library(), "MainCamera");
        VkDescriptorBufferInfo camInfo = { 0 };
        camInfo.buffer = cameraBuffer->buffers[i];
        camInfo.offset = 0;
        camInfo.range = sizeof(evkCameraUBO);

        VkWriteDescriptorSet desc;
        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = grid->descriptorSets[i];
        desc.dstBinding = 0;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc.descriptorCount = 1;
        desc.pBufferInfo = &camInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);
    }
}

EVK_FUNC evkGrid* evk_grid_create() {
    evkGrid* grid = (evkGrid*)m_malloc(sizeof(evkGrid));
    if (!grid) return NULL;

    memset(grid, 0, sizeof(evkGrid));

    VkDevice device = evk_get_device();
    evkPipeline* pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_GRID_DEFAULT_NAME);
    bool success = false;

    do
    {
        VkDescriptorPoolSize poolSize;
        memset(&poolSize, 0, sizeof(VkDescriptorPoolSize));
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;

        VkDescriptorPoolCreateInfo descPoolCI;
        memset(&descPoolCI, 0, sizeof(VkDescriptorPoolCreateInfo));
        descPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descPoolCI.poolSizeCount = 1;
        descPoolCI.pPoolSizes = &poolSize;
        descPoolCI.maxSets = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

        if (vkCreateDescriptorPool(device, &descPoolCI, NULL, &grid->descriptorPool) != VK_SUCCESS)
        {
            EVK_LOG(evk_Error, "Failed to create descriptor pool for grid");
            break;
        }

        VkDescriptorSetLayout layouts[EVK_CONCURRENTLY_RENDERED_FRAMES];
        for (int i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            layouts[i] = pipeline->descriptorSetLayout;
        }

        VkDescriptorSetAllocateInfo descSetAllocInfo;
        memset(&descSetAllocInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
        descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descSetAllocInfo.descriptorPool = grid->descriptorPool;
        descSetAllocInfo.descriptorSetCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        descSetAllocInfo.pSetLayouts = layouts;
        if (vkAllocateDescriptorSets(device, &descSetAllocInfo, grid->descriptorSets) != VK_SUCCESS)
        {
            EVK_LOG(evk_Error, "Failed to allocate descriptor sets ford grid");
            break;
        }

        ievk_grid_refresh_data(grid);
        success = true;
    } while (0);

    // cleanup
    if (!success) {
        evk_grid_destroy(grid);
        m_free(grid);
        return NULL;
    }

    return grid;
}

EVK_FUNC void evk_grid_destroy(evkGrid* grid) {
    if (!grid) return;

    VkDevice device = evk_get_device();
    vkDeviceWaitIdle(device);
    vkDestroyDescriptorPool(device, grid->descriptorPool, NULL);

    m_free(grid);
}

EVK_FUNC void evk_grid_update(evkGrid* grid, bool resend) {
    if (!grid) return;

    // really nothing much right now as grid is not yet customizeble
    if (resend) {
        ievk_grid_refresh_data(grid);
    }
}

EVK_FUNC void evk_grid_render(evkGrid* grid) {
    if (!grid) return;

    const VkDeviceSize offsets[] = { 0 };
    evkPipeline* pipeline = NULL;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    uint32_t currentFrame = evk_get_current_frame();
    evkRenderphaseType stage = evk_get_current_renderphase_type();

    switch (stage)
    {
        case evk_Renderphase_Type_Main:
        {
            evkMainRenderphase* renderphase = (evkMainRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_GRID_DEFAULT_NAME);
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        case evk_Renderphase_Type_Viewport:
        {
            evkViewportRenderphase* renderphase = (evkViewportRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_GRID_DEFAULT_NAME); // viewport uses the same pipe as the default one
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        default:
        {
            return;
        }
    }

    pipelineLayout = pipeline->layout;

    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &grid->descriptorSets[currentFrame], 0, NULL);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
    vkCmdDraw(cmdBuffer, 6, 1, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sprite
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct evkSprite
{
    uint32_t id;
    float3 translation;
    float3 rotation;
    float2 scale;
    fmat4 modelMatrix;
    evkSpriteUBO ubo;
    evkBuffer* buffer;
    evkTexture2D* albedo;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[EVK_CONCURRENTLY_RENDERED_FRAMES];
};

static void ievk_sprite_refresh_modelmatrix(evkSprite* sprite) {
    if (!sprite) return;

    float3 rot_sprite = { to_fradians(sprite->rotation.x), to_fradians(sprite->rotation.y), to_fradians(sprite->rotation.z) };
    fquat quaternion = fquat_from_euler(&rot_sprite);
    fmat4 rot_matrix = fquat_to_fmat4_rowmajor(&quaternion);

    fmat4 model_matrix = fmat4_identity();
    model_matrix.data[0][0] = rot_matrix.data[0][0] * sprite->scale.x;
    model_matrix.data[0][1] = rot_matrix.data[0][1] * sprite->scale.x;
    model_matrix.data[0][2] = rot_matrix.data[0][2] * sprite->scale.x;

    model_matrix.data[1][0] = rot_matrix.data[1][0] * sprite->scale.y;
    model_matrix.data[1][1] = rot_matrix.data[1][1] * sprite->scale.y;
    model_matrix.data[1][2] = rot_matrix.data[1][2] * sprite->scale.y;

    model_matrix.data[3][0] = sprite->translation.x;
    model_matrix.data[3][1] = sprite->translation.y;
    model_matrix.data[3][2] = sprite->translation.z;

    sprite->modelMatrix = model_matrix;
}

static void ievk_sprite_refresh_data(evkSprite* sprite) {
    VkDevice device = evk_get_device();

    for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++)
    {
        evkBuffer* cameraBuffer = (evkBuffer*)shashtable_lookup(evk_get_buffers_library(), "MainCamera");
        VkDescriptorBufferInfo camInfo;
        memset(&camInfo, 0, sizeof(VkDescriptorBufferInfo));
        camInfo.buffer = cameraBuffer->buffers[i];
        camInfo.offset = 0;
        camInfo.range = sizeof(evkCameraUBO);

        VkWriteDescriptorSet desc;
        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = sprite->descriptorSets[i];
        desc.dstBinding = 0;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc.descriptorCount = 1;
        desc.pBufferInfo = &camInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);

        // 1: sprite data
        VkDescriptorBufferInfo spriteInfo;
        memset(&spriteInfo, 0, sizeof(VkDescriptorBufferInfo));
        spriteInfo.buffer = sprite->buffer->buffers[i];
        spriteInfo.offset = 0;
        spriteInfo.range = sizeof(evkSpriteUBO);

        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = sprite->descriptorSets[i];
        desc.dstBinding = 1;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc.descriptorCount = 1;
        desc.pBufferInfo = &spriteInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);

        // 2: albedo texture
        VkDescriptorImageInfo albedoInfo;
        memset(&albedoInfo, 0, sizeof(VkDescriptorImageInfo));
        albedoInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        albedoInfo.imageView = sprite->albedo->view;
        albedoInfo.sampler = sprite->albedo->sampler;

        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = sprite->descriptorSets[i];
        desc.dstBinding = 2;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        desc.descriptorCount = 1;
        desc.pImageInfo = &albedoInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);
    }
    evk_sprite_update(sprite, false);
}

EVK_FUNC evkSprite* evk_sprite_create_from_path(const char* path, uint32_t id) {
    if (path == NULL) {
        EVK_LOG(evk_Error, "Sprite path is NULL");
        return NULL;
    }

    evkSprite* sprite = (evkSprite*)m_malloc(sizeof(evkSprite));
    if (sprite == NULL) {
        EVK_LOG(evk_Error, "Out of memory to allocate sprite %s", path);
        return NULL;
    }

    memset(sprite, 0, sizeof(evkSprite));

    evkResult result = evk_Success;
    evkBuffer* staging = NULL;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkDevice device = evk_get_device();
    VkPhysicalDevice physicalDevice = evk_get_physical_device();
    VkPhysicalDeviceProperties properties = evk_get_physical_device_properties();
    bool success = false;

    do
    {
        EVK_LOG(evk_Todo, "Trace registered ids internally to inform when multiple identical ids were registered");
        sprite->id = id;
        // these are hardcoded so on a scenario where the user doesnt have the camera setup, it'll appear in front of it
        sprite->translation.x = 2.0f; 
        sprite->translation.y = 1.0f; 
        sprite->translation.z = 0.0f;
        sprite->rotation.x = 270.0f;
        sprite->rotation.y = 0.0f;
        sprite->rotation.z = 0.0f;
        sprite->scale.x = 1.0f;
        sprite->scale.y = 1.0f;
        sprite->albedo = evk_texture2d_create_from_path(path, false);
        if (!sprite->albedo) {
            EVK_LOG(evk_Error, "Failed to load albedo texture for sprite: %s because: %s", path, evk_stb_failure_reason());
            break;
        }

        VkDeviceSize atomSize = properties.limits.nonCoherentAtomSize;
        VkDeviceSize uniformAlign = properties.limits.minUniformBufferOffsetAlignment;
        VkDeviceSize requiredAlignment = (atomSize > uniformAlign) ? atomSize : uniformAlign;
        VkDeviceSize perFrameAlignedSize = (sizeof(evkSpriteUBO) + requiredAlignment - 1) & ~(requiredAlignment - 1);
        VkDeviceSize totalBufferSize = perFrameAlignedSize * EVK_CONCURRENTLY_RENDERED_FRAMES;

        // create uniform buffer (non-coherent for cached performance)
        sprite->buffer = evk_buffer_create(device, physicalDevice, totalBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, EVK_CONCURRENTLY_RENDERED_FRAMES);
        sprite->buffer->originalDataSize = sizeof(evkSpriteUBO);
        sprite->buffer->alignedPerFrameSize = perFrameAlignedSize;

        staging = evk_buffer_create(device, physicalDevice, sizeof(evkSpriteUBO), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1);

        // copy initial UBO data through staging
        if (evk_buffer_map(device, staging, 0) != evk_Success) {
            EVK_LOG(evk_Error, "Failed to map staging buffer");
            break;
        }

        if (evk_buffer_copy(staging, 0, &sprite->ubo, sizeof(evkSpriteUBO), 0) != evk_Success) {
            EVK_LOG(evk_Error, "Failed to copy to staging buffer");
            evk_buffer_unmap(device, staging, 0);
            break;
        }

        evk_buffer_unmap(device, staging, 0);

        // copy from staging to each frame's uniform buffer
        evkRenderphaseType renderPhaseType = evk_using_viewport() ? evk_Renderphase_Type_Viewport : evk_Renderphase_Type_Main;
        VkCommandPool cmdPool = evk_get_command_pool(renderPhaseType);

        cmdBuffer = evk_device_begin_commandbuffer_singletime(device, cmdPool);

        for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            VkDeviceSize dstOffset = i * perFrameAlignedSize;
            evk_buffer_command_copy(cmdBuffer, staging, 0, sprite->buffer, i, sizeof(evkSpriteUBO), 0, dstOffset);
        }

        evk_device_end_commandbuffer_singletime(device, cmdPool, cmdBuffer, evk_get_graphics_queue());

        // get pipeline, create descriptor pool and allocate descriptor sets
        evkPipeline* pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_SPRITE_DEFAULT_NAME);

        if (!pipeline) {
            EVK_LOG(evk_Error, "Failed to find sprite pipeline");
            break;
        }
       
        VkDescriptorPoolSize poolSizes[3];
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[1].descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[2].descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;

        VkDescriptorPoolCreateInfo descriptorPoolCI;
        memset(&descriptorPoolCI, 0, sizeof(VkDescriptorPoolCreateInfo));
        descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCI.poolSizeCount = (uint32_t)EVK_STATIC_ARRAY_SIZE(poolSizes);
        descriptorPoolCI.pPoolSizes = poolSizes;
        descriptorPoolCI.maxSets = EVK_CONCURRENTLY_RENDERED_FRAMES;

        if (vkCreateDescriptorPool(device, &descriptorPoolCI, NULL, &sprite->descriptorPool) != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to create descriptor pool for sprite: %s", path);
            break;
        }

        VkDescriptorSetLayout layouts[EVK_CONCURRENTLY_RENDERED_FRAMES];

        for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            layouts[i] = pipeline->descriptorSetLayout;
        }

        VkDescriptorSetAllocateInfo descSetAllocInfo;
        memset(&descSetAllocInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
        descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descSetAllocInfo.descriptorPool = sprite->descriptorPool;
        descSetAllocInfo.descriptorSetCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        descSetAllocInfo.pSetLayouts = layouts;

        if (vkAllocateDescriptorSets(device, &descSetAllocInfo, sprite->descriptorSets) != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to allocate descriptor sets for sprite");
            break;
        }

        ievk_sprite_refresh_data(sprite);
        ievk_sprite_refresh_modelmatrix(sprite);

        success = true;
    } while (0);

    // cleanup
    if (staging) {
        evk_buffer_destroy(device, staging);
    }

    if (!success) {
        evk_sprite_destroy(sprite);
        m_free(sprite);
        return NULL;
    }

    return sprite;
}

EVK_FUNC void evk_sprite_destroy(evkSprite* sprite) {
    if (!sprite) return;

    VkDevice device = evk_get_device();
    vkDeviceWaitIdle(device);

    if (sprite->descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device, sprite->descriptorPool, NULL);
    }

    if (sprite->buffer) {
        evk_buffer_destroy(device, sprite->buffer);
    }

    if (sprite->albedo) {
        evk_texture2d_destroy(sprite->albedo);
    }

    m_free(sprite);
}

EVK_FUNC void evk_sprite_update(evkSprite* sprite, bool resend) {
    if (!sprite) return;

    VkDevice device = evk_get_device();

    if (sprite->buffer) {
        if (resend) {
            ievk_sprite_refresh_data(sprite);
        }

        VkPhysicalDeviceProperties properties = evk_get_physical_device_properties();

        for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            evk_buffer_map(device, sprite->buffer, i);
            evk_buffer_copy(sprite->buffer, i, &sprite->ubo, sizeof(evkSpriteUBO), 0);
            evk_buffer_flush(device, sprite->buffer, i, sizeof(evkSpriteUBO), properties.limits.nonCoherentAtomSize, 0);
            evk_buffer_unmap(device, sprite->buffer, i);
        }
    }
}

EVK_FUNC void evk_sprite_render(evkSprite* sprite) {
    if (!sprite) return;

    const VkDeviceSize offsets[] = { 0 };
    evkPipeline* pipeline = NULL;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    uint32_t currentFrame = evk_get_current_frame();
    evkRenderphaseType stage = evk_get_current_renderphase_type();

    switch (stage)
    {
        case evk_Renderphase_Type_Main:
        {
            evkMainRenderphase* renderphase = (evkMainRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_SPRITE_DEFAULT_NAME);
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        case evk_Renderphase_Type_Picking:
        {
            evkPickingRenderphase* renderphase = (evkPickingRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_SPRITE_PICKING_NAME);
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        case evk_Renderphase_Type_Viewport:
        {
            evkViewportRenderphase* renderphase = (evkViewportRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_SPRITE_DEFAULT_NAME); // viewport uses the same pipe as the default one
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        default:
        {
            return;
        }
    }

    pipelineLayout = pipeline->layout;

    evkPushConstant constants = { 0 };
    constants.id = sprite->id;
    constants.model = sprite->modelMatrix;
    vkCmdPushConstants(cmdBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(evkPushConstant), &constants);

    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &sprite->descriptorSets[currentFrame], 0, NULL);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
    vkCmdDraw(cmdBuffer, 6, 1, 0, 0);
}

EVK_FUNC evkTexture2D* evk_sprite_get_albedo(evkSprite* sprite) {
    if (!sprite) return NULL;
    return sprite->albedo;
}

EVK_FUNC void evk_sprite_set_albedo(evkSprite* sprite, const char* path) {
    if (sprite == NULL) {
        EVK_LOG(evk_Error, "Sprite is NULL");
        return;
    }

    if (path == NULL) {
        EVK_LOG(evk_Error, "Sprite path is NULL");
        return;
    }

    evkTexture2D* newTexture = evk_texture2d_create_from_path(path, false);
    if (!newTexture) {
        EVK_LOG(evk_Error, "Failed to load albedo texture for sprite: %s because: %s", path, evk_stb_failure_reason());
        return;
    }

    if (sprite->albedo) {
        evk_texture2d_destroy(sprite->albedo);
    }

    sprite->albedo = newTexture;

    ievk_sprite_refresh_data(sprite);
    ievk_sprite_refresh_modelmatrix(sprite);
}

EVK_FUNC uint32_t evk_sprite_get_id(evkSprite* sprite) {
    return sprite != NULL ? sprite->id : 0;
}

EVK_FUNC float3 evk_sprite_get_translation(evkSprite* sprite)
{
    float3 res = { 0 };
    res.x = 0.0f;
    res.y = 0.0f;
    res.z = 0.0f;
    return sprite == NULL ? res : sprite->translation;
}

EVK_FUNC void evk_sprite_set_translation(evkSprite* sprite, const float3 translation) {
    if (sprite != NULL) {
        sprite->translation = translation;
        ievk_sprite_refresh_modelmatrix(sprite);
    }
}

EVK_FUNC float3 evk_sprite_get_rotation(evkSprite* sprite) {
    float3 res = { 0 };
    res.x = 0.0f;
    res.y = 0.0f;
    res.z = 0.0f;
    return sprite == NULL ? res : sprite->rotation;
}

EVK_FUNC void evk_sprite_set_rotation(evkSprite* sprite, const float3 rotation) {
    if (sprite != NULL) {
        sprite->rotation = rotation;
        ievk_sprite_refresh_modelmatrix(sprite);
    }
}

EVK_FUNC float2 evk_sprite_get_scale(evkSprite* sprite) {
    float2 res = { 0 };
    res.x = 0.0f;
    res.y = 0.0f;
    return sprite == NULL ? res : sprite->scale;
}

EVK_FUNC void evk_sprite_set_scale(evkSprite* sprite, const float2 scale) {
    if (sprite != NULL) {
        sprite->scale = scale;
        ievk_sprite_refresh_modelmatrix(sprite);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// billboard
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct evkBillboard
{
    uint32_t id;
    float3 translation;
    float3 rotation;
    float2 scale;
    fmat4 modelMatrix;
    evkSpriteUBO ubo;
    evkBuffer* buffer;
    evkTexture2D* albedo;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[EVK_CONCURRENTLY_RENDERED_FRAMES];
};

static EVK_FUNC void ievk_billboard_refresh_modelmatrix(evkBillboard* billboard) {
    if (!billboard) return;

    float3 rot_sprite = { to_fradians(billboard->rotation.x), to_fradians(billboard->rotation.y), to_fradians(billboard->rotation.z) };
    fquat quaternion = fquat_from_euler(&rot_sprite);
    fmat4 rot_matrix = fquat_to_fmat4_rowmajor(&quaternion);

    fmat4 model_matrix = fmat4_identity();
    model_matrix.data[0][0] = rot_matrix.data[0][0] * billboard->scale.x;
    model_matrix.data[0][1] = rot_matrix.data[0][1] * billboard->scale.x;
    model_matrix.data[0][2] = rot_matrix.data[0][2] * billboard->scale.x;

    model_matrix.data[1][0] = rot_matrix.data[1][0] * billboard->scale.y;
    model_matrix.data[1][1] = rot_matrix.data[1][1] * billboard->scale.y;
    model_matrix.data[1][2] = rot_matrix.data[1][2] * billboard->scale.y;

    model_matrix.data[3][0] = billboard->translation.x;
    model_matrix.data[3][1] = billboard->translation.y;
    model_matrix.data[3][2] = billboard->translation.z;

    billboard->modelMatrix = model_matrix;
}

static EVK_FUNC void ievk_billboard_refresh_data(evkBillboard* billboard)
{
    VkDevice device = evk_get_device();

    for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++)
    {
        evkBuffer* cameraBuffer = (evkBuffer*)shashtable_lookup(evk_get_buffers_library(), "MainCamera");
        VkDescriptorBufferInfo camInfo;
        memset(&camInfo, 0, sizeof(VkDescriptorBufferInfo));
        camInfo.buffer = cameraBuffer->buffers[i];
        camInfo.offset = 0;
        camInfo.range = sizeof(evkCameraUBO);

        VkWriteDescriptorSet desc;
        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = billboard->descriptorSets[i];
        desc.dstBinding = 0;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc.descriptorCount = 1;
        desc.pBufferInfo = &camInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);

        // 1: sprite data
        VkDescriptorBufferInfo billboardInfo;
        memset(&billboardInfo, 0, sizeof(VkDescriptorBufferInfo));
        billboardInfo.buffer = billboard->buffer->buffers[i];
        billboardInfo.offset = 0;
        billboardInfo.range = sizeof(evkBillboardUBO);

        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = billboard->descriptorSets[i];
        desc.dstBinding = 1;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc.descriptorCount = 1;
        desc.pBufferInfo = &billboardInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);

        // 2: albedo texture
        VkDescriptorImageInfo albedoInfo;
        memset(&albedoInfo, 0, sizeof(VkDescriptorImageInfo));
        albedoInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        albedoInfo.imageView = billboard->albedo->view;
        albedoInfo.sampler = billboard->albedo->sampler;

        memset(&desc, 0, sizeof(VkWriteDescriptorSet));
        desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc.dstSet = billboard->descriptorSets[i];
        desc.dstBinding = 2;
        desc.dstArrayElement = 0;
        desc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        desc.descriptorCount = 1;
        desc.pImageInfo = &albedoInfo;
        vkUpdateDescriptorSets(device, 1, &desc, 0, NULL);
    }
    evk_billboard_update(billboard, false);
}

EVK_FUNC evkBillboard* evk_billboard_create_from_path(const char* path, uint32_t id) {
    if (path == NULL) {
        EVK_LOG(evk_Error, "Sprite path is NULL");
        return NULL;
    }

    evkBillboard* billboard = (evkBillboard*)m_malloc(sizeof(evkBillboard));
    if (billboard == NULL) {
        EVK_LOG(evk_Error, "Out of memory to allocate billboard %s", path);
        return NULL;
    }

    memset(billboard, 0, sizeof(evkBillboard));
    evkResult result = evk_Success;
    evkBuffer* staging = NULL;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkDevice device = evk_get_device();
    VkPhysicalDevice physicalDevice = evk_get_physical_device();
    VkPhysicalDeviceProperties properties = evk_get_physical_device_properties();
    bool success = false;

    do
    {
        EVK_LOG(evk_Todo, "Trace registered ids internally to inform when multiple identical ids were registered");
        billboard->id = id;
        // these are hardcoded so on a scenario where the user doesnt have the camera setup, it'll appear in front of it
        billboard->translation.x = 2.0f;
        billboard->translation.y = 1.0f;
        billboard->translation.z = 0.0f;
        billboard->rotation.x = 270.0f;
        billboard->rotation.y = 0.0f;
        billboard->rotation.z = 0.0f;
        billboard->scale.x = 1.0f;
        billboard->scale.y = 1.0f;
        billboard->albedo = evk_texture2d_create_from_path(path, false);
        if (!billboard->albedo) {
            EVK_LOG(evk_Error, "Failed to load albedo texture for billboard: %s because: %s", path, evk_stb_failure_reason());
            break;
        }

        VkDeviceSize atomSize = properties.limits.nonCoherentAtomSize;
        VkDeviceSize uniformAlign = properties.limits.minUniformBufferOffsetAlignment;
        VkDeviceSize requiredAlignment = (atomSize > uniformAlign) ? atomSize : uniformAlign;
        VkDeviceSize perFrameAlignedSize = (sizeof(evkBillboardUBO) + requiredAlignment - 1) & ~(requiredAlignment - 1);
        VkDeviceSize totalBufferSize = perFrameAlignedSize * EVK_CONCURRENTLY_RENDERED_FRAMES;

        // create uniform buffer (non-coherent for cached performance)
        billboard->buffer = evk_buffer_create(device, physicalDevice, totalBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, EVK_CONCURRENTLY_RENDERED_FRAMES);
        billboard->buffer->originalDataSize = sizeof(evkBillboardUBO);
        billboard->buffer->alignedPerFrameSize = perFrameAlignedSize;
        staging = evk_buffer_create(device, physicalDevice, sizeof(evkBillboardUBO), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1);

        // copy initial UBO data through staging
        if (evk_buffer_map(device, staging, 0) != evk_Success) {
            EVK_LOG(evk_Error, "Failed to map staging buffer");
            break;
        }

        if (evk_buffer_copy(staging, 0, &billboard->ubo, sizeof(evkBillboardUBO), 0) != evk_Success) {
            EVK_LOG(evk_Error, "Failed to copy to staging buffer");
            evk_buffer_unmap(device, staging, 0);
            break;
        }

        evk_buffer_unmap(device, staging, 0);

        // copy from staging to each frame's uniform buffer
        evkRenderphaseType renderPhaseType = evk_using_viewport() ? evk_Renderphase_Type_Viewport : evk_Renderphase_Type_Main;
        VkCommandPool cmdPool = evk_get_command_pool(renderPhaseType);

        cmdBuffer = evk_device_begin_commandbuffer_singletime(device, cmdPool);

        for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            VkDeviceSize dstOffset = i * perFrameAlignedSize;
            evk_buffer_command_copy(cmdBuffer, staging, 0, billboard->buffer, i, sizeof(evkBillboardUBO), 0, dstOffset);
        }

        evk_device_end_commandbuffer_singletime(device, cmdPool, cmdBuffer, evk_get_graphics_queue());

        // get pipeline, create descriptor pool and allocate descriptor sets
        evkPipeline* pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_SPRITE_DEFAULT_NAME);

        if (!pipeline) {
            EVK_LOG(evk_Error, "Failed to find sprite pipeline");
            break;
        }

        VkDescriptorPoolSize poolSizes[3];
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[1].descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[2].descriptorCount = EVK_CONCURRENTLY_RENDERED_FRAMES;

        VkDescriptorPoolCreateInfo descriptorPoolCI;
        memset(&descriptorPoolCI, 0, sizeof(VkDescriptorPoolCreateInfo));
        descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCI.poolSizeCount = (uint32_t)EVK_STATIC_ARRAY_SIZE(poolSizes);
        descriptorPoolCI.pPoolSizes = poolSizes;
        descriptorPoolCI.maxSets = EVK_CONCURRENTLY_RENDERED_FRAMES;

        if (vkCreateDescriptorPool(device, &descriptorPoolCI, NULL, &billboard->descriptorPool) != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to create descriptor pool for billboard: %s", path);
            break;
        }

        VkDescriptorSetLayout layouts[EVK_CONCURRENTLY_RENDERED_FRAMES];

        for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            layouts[i] = pipeline->descriptorSetLayout;
        }

        VkDescriptorSetAllocateInfo descSetAllocInfo;
        memset(&descSetAllocInfo, 0, sizeof(VkDescriptorSetAllocateInfo));
        descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descSetAllocInfo.descriptorPool = billboard->descriptorPool;
        descSetAllocInfo.descriptorSetCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
        descSetAllocInfo.pSetLayouts = layouts;

        if (vkAllocateDescriptorSets(device, &descSetAllocInfo, billboard->descriptorSets) != VK_SUCCESS) {
            EVK_LOG(evk_Error, "Failed to allocate descriptor sets for billboard");
            break;
        }

        ievk_billboard_refresh_data(billboard);
        ievk_billboard_refresh_modelmatrix(billboard);

        success = true;
    } while (0);

    // cleanup
    if (staging) {
        evk_buffer_destroy(device, staging);
    }

    if (!success) {
        evk_billboard_destroy(billboard);
        m_free(billboard);
        return NULL;
    }

    return billboard;
}

EVK_FUNC void evk_billboard_destroy(evkBillboard* billboard) {
    if (!billboard) return;

    VkDevice device = evk_get_device();
    vkDeviceWaitIdle(device);

    if (billboard->descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device, billboard->descriptorPool, NULL);
    }

    if (billboard->buffer) {
        evk_buffer_destroy(device, billboard->buffer);
    }

    if (billboard->albedo) {
        evk_texture2d_destroy(billboard->albedo);
    }

    m_free(billboard);
}

EVK_FUNC void evk_billboard_update(evkBillboard* billboard, bool resend) {
    if (!billboard) return;

    VkDevice device = evk_get_device();

    if (billboard->buffer) {
        if (resend) {
            ievk_billboard_refresh_data(billboard);
        }

        VkPhysicalDeviceProperties properties = evk_get_physical_device_properties();

        for (uint32_t i = 0; i < EVK_CONCURRENTLY_RENDERED_FRAMES; i++) {
            evk_buffer_map(device, billboard->buffer, i);
            evk_buffer_copy(billboard->buffer, i, &billboard->ubo, sizeof(evkBillboardUBO), 0);
            evk_buffer_flush(device, billboard->buffer, i, sizeof(evkBillboardUBO), properties.limits.nonCoherentAtomSize, 0);
            evk_buffer_unmap(device, billboard->buffer, i);
        }
    }
}

EVK_FUNC void evk_billboard_render(evkBillboard* billboard) {
    const VkDeviceSize offsets[] = { 0 };
    evkPipeline* pipeline = NULL;
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    uint32_t currentFrame = evk_get_current_frame();
    evkRenderphaseType stage = evk_get_current_renderphase_type();

    switch (stage)
    {
        case evk_Renderphase_Type_Main:
        {
            evkMainRenderphase* renderphase = (evkMainRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_BILLBOARD_DEFAULT_NAME);
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        case evk_Renderphase_Type_Viewport:
        {
            evkViewportRenderphase* renderphase = (evkViewportRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_BILLBOARD_DEFAULT_NAME); // viewport uses the same pipe as the default one
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
        }

        case evk_Renderphase_Type_Picking:
        {
            evkPickingRenderphase* renderphase = (evkPickingRenderphase*)evk_get_renderphase(stage);
            pipeline = (evkPipeline*)shashtable_lookup(evk_get_pipelines_library(), EVK_PIPELINE_BILLBOARD_PICKING_NAME);
            cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
            break;
        }

        default:
        {
            return;
        }
    }

    pipelineLayout = pipeline->layout;

    evkPushConstant constants = { 0 };
    constants.id = billboard->id;
    constants.model = billboard->modelMatrix;
    vkCmdPushConstants(cmdBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(evkPushConstant), &constants);

    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &billboard->descriptorSets[currentFrame], 0, NULL);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
    vkCmdDraw(cmdBuffer, 6, 1, 0, 0);
}

EVK_FUNC uint32_t evk_billboard_get_id(evkBillboard* billboard) {
    return billboard != NULL ? billboard->id : 0;
}

EVK_FUNC float3 evk_billboard_get_translation(evkBillboard* billboard) {
    float3 res = { 0 };
    res.x = 0.0f;
    res.y = 0.0f;
    res.z = 0.0f;
    return billboard == NULL ? res : billboard->translation;
}

EVK_FUNC void evk_billboard_set_translation(evkBillboard* billboard, const float3 translation) {
    if (billboard != NULL) {
        billboard->translation = translation;
        ievk_billboard_refresh_modelmatrix(billboard);
    }
}

EVK_FUNC float3 evk_billboard_get_rotation(evkBillboard* billboard) {
    float3 res = { 0 };
    res.x = 0.0f;
    res.y = 0.0f;
    res.z = 0.0f;
    return billboard == NULL ? res : billboard->rotation;
}

EVK_FUNC void evk_billboard_set_rotation(evkBillboard* billboard, const float3 rotation) {
    if (billboard != NULL) {
        billboard->rotation = rotation;
        ievk_billboard_refresh_modelmatrix(billboard);
    }
}

EVK_FUNC float2 evk_billboard_get_scale(evkBillboard* billboard) {
    float2 res = { 0 };
    res.x = 0.0f;
    res.y = 0.0f;
    return billboard == NULL ? res : billboard->scale;
}

EVK_FUNC void evk_billboard_set_scale(evkBillboard* billboard, const float2 scale)
{
    if (billboard != NULL) {
        billboard->scale = scale;
        ievk_billboard_refresh_modelmatrix(billboard);
    }
}

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

EVK_FUNC void evk_dump_picking_framebuffer() {
    VkDevice device = evk_get_device();
    VkPhysicalDevice physicalDevice = evk_get_physical_device();
    VkQueue queue = evk_get_graphics_queue();
    
    EVK_LOG(evk_Info, "Try getting size from swapchain instead of evk_get_size?");
    uint32_t width = (uint32_t)evk_get_size().x;
    uint32_t height = (uint32_t)evk_get_size().y;
    
    // validate we got reasonable values
    if (width >= 0 || width > 16384 || height >= 0 || height > 16384) {
        EVK_LOG(evk_Error, "Probably invalid framebuffer size: %dx%d", width, height);
        return;
    }
    
    uint32_t pixelCount = width * height;
    VkDeviceSize bufferSize = (VkDeviceSize)pixelCount * sizeof(uint32_t);
    
    EVK_LOG(evk_Info, "Dumping picking framebuffer: %dx%d (%u bytes)", width, height, (uint32_t)bufferSize);
    
    // create staging buffer
    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingMemory = VK_NULL_HANDLE;
    
    VkBufferCreateInfo bufferCI;
    memset(&bufferCI, 0, sizeof(VkBufferCreateInfo));
    bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCI.size = bufferSize;
    bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(device, &bufferCI, NULL, &stagingBuffer) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to create staging buffer");
        return;
    }
    
    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(device, stagingBuffer, &memReq);
    
    uint32_t memType = evk_device_find_suitable_memory_type(physicalDevice, memReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    
    if (memType == UINT32_MAX) {
        EVK_LOG(evk_Error, "No suitable memory type");
        vkDestroyBuffer(device, stagingBuffer, NULL);
        return;
    }
    
    VkMemoryAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = memType;
    
    if (vkAllocateMemory(device, &allocInfo, NULL, &stagingMemory) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate staging memory");
        vkDestroyBuffer(device, stagingBuffer, NULL);
        return;
    }
    
    vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0);
    
    evkPickingRenderphase* phase = (evkPickingRenderphase*)evk_get_renderphase(evk_Renderphase_Type_Picking);
    if (!phase || phase->colorImage == VK_NULL_HANDLE) {
        EVK_LOG(evk_Error, "Invalid picking renderphase");
        vkFreeMemory(device, stagingMemory, NULL);
        vkDestroyBuffer(device, stagingBuffer, NULL);
        return;
    }
    
    // create command buffer
    VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
    VkCommandBufferAllocateInfo cmdAlloc;
    memset(&cmdAlloc, 0, sizeof(VkCommandBufferAllocateInfo));
    cmdAlloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAlloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAlloc.commandPool = phase->evkRenderpass.cmdPool;
    cmdAlloc.commandBufferCount = 1;
    
    if (vkAllocateCommandBuffers(device, &cmdAlloc, &cmdBuffer) != VK_SUCCESS) {
        EVK_LOG(evk_Error, "Failed to allocate command buffer");
        vkFreeMemory(device, stagingMemory, NULL);
        vkDestroyBuffer(device, stagingBuffer, NULL);
        return;
    }
    
    // begin command buffer
    VkCommandBufferBeginInfo beginInfo;
    memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmdBuffer, &beginInfo);
    
    // transition image to TRANSFER_SRC_OPTIMAL
    VkImageMemoryBarrier barrier;
    memset(&barrier, 0, sizeof(VkImageMemoryBarrier));
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = phase->colorImage;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);
    
    // copy entire image to buffer
    VkBufferImageCopy region;
    memset(&region, 0, sizeof(VkBufferImageCopy));
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset.x = 0;
    region.imageOffset.y = 0;
    region.imageOffset.z = 0;
    region.imageExtent.width = width;
    region.imageExtent.height = height;
    region.imageExtent.depth = 1;
    vkCmdCopyImageToBuffer(cmdBuffer, phase->colorImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingBuffer, 1, &region);
    
    // transition back to COLOR_ATTACHMENT_OPTIMAL
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);
    
    vkEndCommandBuffer(cmdBuffer);
    
    // submit and wait
    VkFence fence = VK_NULL_HANDLE;
    VkFenceCreateInfo fenceCI;
    memset(&fenceCI, 0, sizeof(VkFenceCreateInfo));
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    vkCreateFence(device, &fenceCI, NULL, &fence);
    
    VkSubmitInfo submit;
    memset(&submit, 0, sizeof(VkSubmitInfo));
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmdBuffer;
    vkQueueSubmit(queue, 1, &submit, fence);
    vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
    
    // map and read data
    void* data = NULL;
    vkMapMemory(device, stagingMemory, 0, bufferSize, 0, &data);
    uint32_t* pixels = (uint32_t*)data;
    
    // analyze results
    uint32_t uniqueIds[1024];
    uint32_t idCounts[1024];
    uint32_t numUnique = 0;
    
    EVK_LOG(evk_Info, "=== PICKING FRAMEBUFFER DUMP ===");
    EVK_LOG(evk_Info, "Resolution: %dx%d", width, height);
    
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t id = pixels[y * width + x];
            if (id != 0) {
                // check if ID already in list
                uint32_t found = 0;
                for (uint32_t i = 0; i < numUnique; i++) {
                    if (uniqueIds[i] == id) {
                        idCounts[i]++;
                        found = 1;
                        break;
                    }
                }
                if (!found && numUnique < 1024) {
                    uniqueIds[numUnique] = id;
                    idCounts[numUnique] = 1;
                    numUnique++;
                }
            }
        }
    }
    
    if (numUnique == 0) {
        EVK_LOG(evk_Warn, "NO ENTITIES FOUND IN PICKING BUFFER - All pixels are 0!");
        EVK_LOG(evk_Warn, "Possible issues:");
        EVK_LOG(evk_Warn, "  - Entities not being rendered to picking pass");
        EVK_LOG(evk_Warn, "  - Shader not writing ID correctly");
        EVK_LOG(evk_Warn, "  - Pipeline using wrong color attachment format");
        EVK_LOG(evk_Warn, "  - Viewport/scissor not covering entities");
        EVK_LOG(evk_Warn, "  - No draw calls submitted to picking command buffer");
    }
    
    else {
        EVK_LOG(evk_Info, "Found %u unique entity IDs:", numUnique);
        for (uint32_t i = 0; i < numUnique; i++) {
            EVK_LOG(evk_Info, "  Entity ID %u appears %u times", uniqueIds[i], idCounts[i]);
        }
        
        // find and print center pixel value
        uint32_t centerX = width / 2;
        uint32_t centerY = height / 2;
        uint32_t centerId = pixels[centerY * width + centerX];
        EVK_LOG(evk_Info, "Center pixel (%u, %u) ID: %u", centerX, centerY, centerId);
        
        // find first non-zero pixel position
        for (uint32_t y = 0; y < height && y < 10; y++) {
            for (uint32_t x = 0; x < width && x < 10; x++) {
                uint32_t id = pixels[y * width + x];
                if (id != 0) {
                    EVK_LOG(evk_Info, "First non-zero pixel at (%u, %u) has ID %u", x, y, id);
                    y = height; // break outer loop
                    break;
                }
            }
        }
    }
    
    vkUnmapMemory(device, stagingMemory);
    vkDestroyFence(device, fence, NULL);
    vkFreeCommandBuffers(device, phase->evkRenderpass.cmdPool, 1, &cmdBuffer);
    vkFreeMemory(device, stagingMemory, NULL);
    vkDestroyBuffer(device, stagingBuffer, NULL);
}

EVK_FUNC void evk_debug_pick_coordinates(float2 mousePos) {
    float2 winSize = evk_get_size();
    if (evk_using_viewport()) {
        winSize = evk_get_viewport_size();
    }
    
    uint32_t fbX = (uint32_t)(mousePos.x * evk_get_size().x / winSize.x);
    uint32_t fbY = (uint32_t)(mousePos.y * evk_get_size().y  / winSize.y);
    
    EVK_LOG(evk_Info, "Mouse: (%.0f, %.0f)", mousePos.x, mousePos.y);
    EVK_LOG(evk_Info, "Viewport size: (%.0f, %.0f)", winSize.x, winSize.y);
    EVK_LOG(evk_Info, "Framebuffer size: (%.0f, %.0f)", evk_get_size().x, evk_get_size().y);
    EVK_LOG(evk_Info, "Converted FB coords: (%u, %u)", fbX, fbY);
    
    // test the converted coordinates
    uint32_t id = evk_pick_object_backend(mousePos);
    EVK_LOG(evk_Info, "Pick result: %u", id);
}

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates an array of VkVertexInputBindingDescription based on parameters
static EVK_FUNC VkVertexInputBindingDescription* ievk_pipeline_get_binding_descriptions(bool passingVertexData, uint32_t* bindingCount) {
    if (!passingVertexData) {
        *bindingCount = 0U;
        return NULL;
    }

    VkVertexInputBindingDescription* bindings = (VkVertexInputBindingDescription*)m_malloc(sizeof(VkVertexInputBindingDescription));
    bindings[0].binding = 0;
    bindings[0].stride = sizeof(evkVertex);
    bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    *bindingCount = 1U;
    return bindings;
}

/// @brief creates an array of VkVertexInputAttributeDescription
static EVK_FUNC VkVertexInputAttributeDescription* ievk_pipeline_get_attribute_descriptions(evkVertexComponent* vertexComponents, uint32_t componentsCount, uint32_t* attributesCount) {
	VkVertexInputAttributeDescription* bindings = (VkVertexInputAttributeDescription*)m_malloc(sizeof(VkVertexInputAttributeDescription) * componentsCount);

	for (uint32_t i = 0; i < componentsCount; i++) {
		evkVertexComponent component = vertexComponents[i];
		VkVertexInputAttributeDescription desc = { 0 };
		desc.binding = 0;
		desc.location = (uint32_t)component;

		switch (component)
		{
			case evk_Vertex_Component_Position:
			{
				desc.format = VK_FORMAT_R32G32B32_SFLOAT;
				desc.offset = offsetof(evkVertex, position);
				break;
			}

			case evk_Vertex_Component_Normal:
			{
				desc.format = VK_FORMAT_R32G32B32_SFLOAT;
				desc.offset = offsetof(evkVertex, normal);
				break;
			}

			case evk_Vertex_Component_UV_0:
			{
				desc.format = VK_FORMAT_R32G32_SFLOAT;
				desc.offset = offsetof(evkVertex, uv_0);
				break;
			}

			case evk_Vertex_Component_Color_0:
			{
				desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				desc.offset = offsetof(evkVertex, color_0);
				break;
			}

			case evk_Vertex_Component_Weights_0:
			{
				desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				desc.offset = offsetof(evkVertex, weights_0);
				break;
			}

			case evk_Vertex_Component_Joints_0:
			{
				desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				desc.offset = offsetof(evkVertex, joints_0);
				break;
			}

			default: break;
		}
		bindings[i] = desc;
	}
	*attributesCount = componentsCount;
	return bindings;
}

/// @brief populates the vertex input state pipeline structure
static EVK_FUNC VkPipelineVertexInputStateCreateInfo ievk_pipeline_populate_visci(evkPipeline* pipeline, evkVertexComponent* vertexComponents, uint32_t componentsCount) {
	pipeline->bindingsDescription = ievk_pipeline_get_binding_descriptions(pipeline->passingVertexData, &pipeline->bindingsDescriptionCount);
	pipeline->attributesDescription = ievk_pipeline_get_attribute_descriptions(vertexComponents, componentsCount, &pipeline->attributesDescriptionCount);

	VkPipelineVertexInputStateCreateInfo visci;
	memset(&visci, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
	visci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	visci.pNext = NULL;
	visci.flags = 0;
	visci.vertexBindingDescriptionCount = pipeline->bindingsDescriptionCount;
	visci.pVertexBindingDescriptions = pipeline->bindingsDescription;
	visci.vertexAttributeDescriptionCount = pipeline->attributesDescriptionCount;
	visci.pVertexAttributeDescriptions = pipeline->attributesDescription;
	return visci;
}

/// @brief creates and returns a pipeline
static EVK_FUNC evkResult ievk_pipeline_create(VkDevice device, evkPipelineCreateInfo* ci, evkPipeline* outPipe) {
	EVK_ASSERT(device != VK_NULL_HANDLE, "device is NULL");
	EVK_ASSERT(ci != NULL, "ci is NULL");
	EVK_ASSERT(outPipe != NULL, "outPipe is NULL");

	outPipe->passingVertexData = ci->passingVertexData;
	outPipe->cache = ci->pipelineCache;
	outPipe->shaderStages[0] = ci->vertexShader.info;
	outPipe->shaderStages[1] = ci->fragmentShader.info;

	// descriptor set
	VkDescriptorSetLayoutCreateInfo descSetLayoutCI;
    memset(&descSetLayoutCI, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
	descSetLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCI.pNext = NULL;
	descSetLayoutCI.flags = 0;
	descSetLayoutCI.bindingCount = ci->bindingsCount;
	descSetLayoutCI.pBindings = ci->bindings;
	EVK_ASSERT(vkCreateDescriptorSetLayout(device, &descSetLayoutCI, NULL, &outPipe->descriptorSetLayout) == VK_SUCCESS, "Failed to create descriptor set layout");

	// pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCI;
    memset(&pipelineLayoutCI, 0, sizeof(VkPipelineLayoutCreateInfo));
	pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCI.pNext = NULL;
	pipelineLayoutCI.flags = 0;
	pipelineLayoutCI.setLayoutCount = 1;
	pipelineLayoutCI.pSetLayouts = &outPipe->descriptorSetLayout;
	pipelineLayoutCI.pushConstantRangeCount = ci->pushConstantsCount;
	pipelineLayoutCI.pPushConstantRanges = ci->pushConstants;
	EVK_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCI, NULL, &outPipe->layout) == VK_SUCCESS, "Failed to create pipeline layout");

	// vertex input state
	outPipe->vertexInputState = ievk_pipeline_populate_visci(outPipe, ci->vertexComponents, ci->vertexComponentsCount);
	// input vertex assembly state
	memset(&outPipe->inputVertexAssemblyState, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
	outPipe->inputVertexAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	outPipe->inputVertexAssemblyState.pNext = NULL;
	outPipe->inputVertexAssemblyState.flags = 0;
	outPipe->inputVertexAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	outPipe->inputVertexAssemblyState.primitiveRestartEnable = VK_FALSE;
	// viewport state
	memset(&outPipe->viewportState, 0, sizeof(VkPipelineViewportStateCreateInfo));
	outPipe->viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	outPipe->viewportState.pNext = NULL;
	outPipe->viewportState.flags = 0;
	outPipe->viewportState.viewportCount = 1;
	outPipe->viewportState.pViewports = NULL; // using dynamic viewport
	outPipe->viewportState.scissorCount = 1;
	outPipe->viewportState.pScissors = NULL; // using dynamic scissor
	// rasterization state
	memset(&outPipe->rasterizationState, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
	outPipe->rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	outPipe->rasterizationState.pNext = NULL;
	outPipe->rasterizationState.flags = 0;
	outPipe->rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	outPipe->rasterizationState.cullMode = VK_CULL_MODE_NONE;
	outPipe->rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	outPipe->rasterizationState.depthClampEnable = VK_FALSE;
	outPipe->rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	outPipe->rasterizationState.lineWidth = 1.0f;
	// multisampling state
	memset(&outPipe->multisampleState, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
	outPipe->multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	outPipe->multisampleState.pNext = NULL;
	outPipe->multisampleState.flags = 0;
	outPipe->multisampleState.rasterizationSamples = (VkSampleCountFlagBits)ci->renderpass->msaa;
	outPipe->multisampleState.sampleShadingEnable = VK_FALSE;
	// depth stencil state
	memset(&outPipe->depthStencilState, 0, sizeof(VkPipelineDepthStencilStateCreateInfo));
	outPipe->depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	outPipe->depthStencilState.pNext = NULL;
	outPipe->depthStencilState.flags = 0;
	outPipe->depthStencilState.depthTestEnable = VK_TRUE;
	outPipe->depthStencilState.depthWriteEnable = VK_TRUE;
	outPipe->depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	outPipe->depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
	// color blend attachment
	memset(&outPipe->colorBlendAttachmentState, 0, sizeof(VkPipelineColorBlendAttachmentState));
	outPipe->colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	outPipe->colorBlendAttachmentState.blendEnable = ci->alphaBlending == true ? VK_TRUE : VK_FALSE;
	outPipe->colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	outPipe->colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	// color blend state
	memset(&outPipe->colorBlendState, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
	outPipe->colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	outPipe->colorBlendState.pNext = NULL;
	outPipe->colorBlendState.flags = 0;
	outPipe->colorBlendState.attachmentCount = 1;
	outPipe->colorBlendState.pAttachments = &outPipe->colorBlendAttachmentState;
	outPipe->colorBlendState.logicOpEnable = VK_FALSE;
	outPipe->colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	outPipe->colorBlendState.blendConstants[0] = 0.0f;
	outPipe->colorBlendState.blendConstants[1] = 0.0f;
	outPipe->colorBlendState.blendConstants[2] = 0.0f;
	outPipe->colorBlendState.blendConstants[3] = 0.0f;

	return evk_Success;
}

/// @brief releases all resources used by a pipeline
static EVK_FUNC void ievk_pipeline_destroy(VkDevice device, evkPipeline* pipeline) {
	if (!pipeline || device == VK_NULL_HANDLE) return;

	vkDeviceWaitIdle(device);
	vkDestroyPipeline(device, pipeline->pipeline, NULL);
	vkDestroyPipelineLayout(device, pipeline->layout, NULL);
	vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, NULL);

	if (pipeline->bindingsDescription != NULL) m_free(pipeline->bindingsDescription);
	if (pipeline->attributesDescription != NULL) m_free(pipeline->attributesDescription);

	// not ideal since shader module was first introduced on shader struct, but it's the same module after-all
	vkDestroyShaderModule(device, pipeline->shaderStages[0].module, NULL);
	vkDestroyShaderModule(device, pipeline->shaderStages[1].module, NULL);

	m_free(pipeline);
}

/// @brief builds a pipeline, must be previously configured as desired
static EVK_FUNC evkResult ievk_pipeline_build(VkDevice device, evkPipeline* pipeline) {
	// dynamic state is here because dynamic states must be constant
	VkPipelineDynamicStateCreateInfo dynamicState;
    memset(&dynamicState, 0, sizeof(VkPipelineDynamicStateCreateInfo));
	const VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pNext = NULL;
	dynamicState.flags = 0;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkGraphicsPipelineCreateInfo ci;
    memset(&ci, 0, sizeof(VkGraphicsPipelineCreateInfo));
	ci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	ci.pNext = NULL;
	ci.flags = 0;
	ci.stageCount = EVK_PIPELINE_SHADER_STAGES_COUNT;
	ci.pStages = pipeline->shaderStages;
	ci.pVertexInputState = &pipeline->vertexInputState;
	ci.pInputAssemblyState = &pipeline->inputVertexAssemblyState;
	ci.pViewportState = &pipeline->viewportState;
	ci.pRasterizationState = &pipeline->rasterizationState;
	ci.pMultisampleState = &pipeline->multisampleState;
	ci.pDepthStencilState = &pipeline->depthStencilState;
	ci.pColorBlendState = &pipeline->colorBlendState;
	ci.pDynamicState = &dynamicState;
	ci.layout = pipeline->layout;
	ci.renderPass = pipeline->renderpass->renderpass;
	ci.subpass = 0;

	VkResult res = vkCreateGraphicsPipelines(device, pipeline->cache, 1, &ci, NULL, &pipeline->pipeline);
	if (res != VK_SUCCESS) {
		EVK_LOG(evk_Error, "Failed to build the graphics pipeline {%d}", res);
		return evk_Failure;
	}

	return evk_Success;
}

/// @brief creates a shader program based on a pre-compiled spirv on disk
static EVK_FUNC evkShader ievk_pipeline_create_shader(VkDevice device, const char* name, const uint32_t* spirv, size_t spirvSize, evkShaderType type) {
	evkShader shader = { 0 };
	shader.name = name;
	shader.type = type;
	shader.info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader.info.pNext = NULL;
	shader.info.flags = 0;
	shader.info.pName = "main";
	shader.info.pSpecializationInfo = NULL;
	switch (type)
	{
		case evk_Shader_Type_Vertex: { shader.info.stage = VK_SHADER_STAGE_VERTEX_BIT; break; }
		case evk_Shader_Type_Fragment: { shader.info.stage = VK_SHADER_STAGE_FRAGMENT_BIT; break; }
		case evk_Shader_Type_Compute: { shader.info.stage = VK_SHADER_STAGE_COMPUTE_BIT; break; }
		case evk_Shader_Type_Geometry: { shader.info.stage = VK_SHADER_STAGE_GEOMETRY_BIT; break; }
		case evk_Shader_Type_Tess_Ctrl: { shader.info.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break; }
		case evk_Shader_Type_Tess_Eval: { shader.info.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break; }
		default: { break; }
	}

	VkShaderModuleCreateInfo moduleCI;
    memset(&moduleCI, 0, sizeof(VkShaderModuleCreateInfo));
	moduleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCI.pNext = NULL;
	moduleCI.flags = 0;
	moduleCI.codeSize = spirvSize * sizeof(uint32_t);
	moduleCI.pCode = spirv;
	EVK_ASSERT(vkCreateShaderModule(device, &moduleCI, NULL, &shader.info.module) == VK_SUCCESS, "Failed to create shader module");

	return shader;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pipelines
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkResult evk_pipeline_sprite_create(shashtable* pipelines, evkRenderpass* renderpass, evkRenderpass* pickingRenderpass, VkDevice device) {
	// default pipeline
	evkPipeline* defaultPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_DEFAULT_NAME);
	if (defaultPipeline != NULL) ievk_pipeline_destroy(device, defaultPipeline);

	evkPipelineCreateInfo ci = { 0 };
	ci.renderpass = renderpass; // this will either be default or viewport renderpass
	ci.vertexShader = ievk_pipeline_create_shader(device, "sprite:default.vert", sprite_default_vert_spv, sprite_default_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "sprite:default.frag", sprite_default_frag_spv, sprite_default_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = true;

	// push constant
	ci.pushConstantsCount = 1;
	ci.pushConstants[0].offset = 0;
	ci.pushConstants[0].size = sizeof(evkPushConstant);
	ci.pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	// bindings
	ci.bindingsCount = 3;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;
	// sprite data
	ci.bindings[1].binding = 1;
	ci.bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[1].descriptorCount = 1;
	ci.bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[1].pImmutableSamplers = NULL;
	// colormap
	ci.bindings[2].binding = 2;
	ci.bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	ci.bindings[2].descriptorCount = 1;
	ci.bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[2].pImmutableSamplers = NULL;

	defaultPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(defaultPipeline != NULL, "Failed to allocate memory for sprite default pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, defaultPipeline) == evk_Success, "Failed to create sprite default pipeline");
	defaultPipeline->renderpass = renderpass;
	defaultPipeline->rasterizationState.cullMode = VK_CULL_MODE_NONE;
	EVK_ASSERT(ievk_pipeline_build(device, defaultPipeline) == evk_Success, "Failed to build sprite default pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_SPRITE_DEFAULT_NAME, defaultPipeline) == TOOLBOX_SUCCESS, "Failed to insert sprite default pipeline into pipeline's library");
	
	// picking pipeline
	evkPipeline* pickingPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_PICKING_NAME);
	if (pickingPipeline != NULL) ievk_pipeline_destroy(device, pickingPipeline);
	
	memset(&ci, 0, sizeof(evkPipelineCreateInfo));
	ci.renderpass = pickingRenderpass;
	ci.vertexShader = ievk_pipeline_create_shader(device, "sprite:picking.vert", sprite_picking_vert_spv, sprite_picking_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "sprite:picking.frag", sprite_picking_frag_spv, sprite_picking_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = false;
	
	// push constant
	ci.pushConstantsCount = 1;
	ci.pushConstants[0].offset = 0;
	ci.pushConstants[0].size = sizeof(evkPushConstant);
	ci.pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	
	// bindings
	ci.bindingsCount = 3;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;
	// sprite data
	ci.bindings[1].binding = 1;
	ci.bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[1].descriptorCount = 1;
	ci.bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[1].pImmutableSamplers = NULL;
	// colormap
	ci.bindings[2].binding = 2;
	ci.bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	ci.bindings[2].descriptorCount = 1;
	ci.bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[2].pImmutableSamplers = NULL;
	
	pickingPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(pickingPipeline != NULL, "Failed to allocate memory for sprite picking pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, pickingPipeline) == evk_Success, "Failed to create sprite picking pipeline");
	pickingPipeline->renderpass = pickingRenderpass;
	pickingPipeline->rasterizationState.cullMode = VK_CULL_MODE_NONE;
	pickingPipeline->colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT; // id's are RED channel only
	pickingPipeline->depthStencilState.depthTestEnable = VK_FALSE;   // Disable depth testing
	pickingPipeline->depthStencilState.depthWriteEnable = VK_FALSE;  // Don't write to depth buffer
	EVK_ASSERT(ievk_pipeline_build(device, pickingPipeline) == evk_Success, "Failed to build sprite picking pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_SPRITE_PICKING_NAME, pickingPipeline) == TOOLBOX_SUCCESS, "Failed to insert sprite picking pipeline into pipeline's library");

	EVK_ASSERT(evk_Todo, "Create wireframe pipeline");
	return evk_Success;
}

EVK_FUNC void evk_pipeline_sprite_destroy(shashtable* pipelines, VkDevice device) {
	evkPipeline* pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_DEFAULT_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);

	pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_PICKING_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);
}

EVK_FUNC evkResult evk_pipeline_billboard_create(shashtable* pipelines, evkRenderpass* renderpass, evkRenderpass* pickingRenderpass, VkDevice device) {
	// default pipeline
	evkPipeline* defaultPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_BILLBOARD_DEFAULT_NAME);
	if (defaultPipeline != NULL) ievk_pipeline_destroy(device, defaultPipeline);

	evkPipelineCreateInfo ci = { 0 };
	ci.renderpass = renderpass; // this will either be default or viewport renderpass
	ci.vertexShader = ievk_pipeline_create_shader(device, "billboard:default.vert", billboard_default_vert_spv, billboard_default_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "billboard:default.frag", billboard_default_frag_spv, billboard_default_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = true;

	// push constant
	ci.pushConstantsCount = 1;
	ci.pushConstants[0].offset = 0;
	ci.pushConstants[0].size = sizeof(evkPushConstant);
	ci.pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	// bindings
	ci.bindingsCount = 3;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;
	// billboard data
	ci.bindings[1].binding = 1;
	ci.bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[1].descriptorCount = 1;
	ci.bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[1].pImmutableSamplers = NULL;
	// colormap
	ci.bindings[2].binding = 2;
	ci.bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	ci.bindings[2].descriptorCount = 1;
	ci.bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[2].pImmutableSamplers = NULL;

	defaultPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(defaultPipeline != NULL, "Failed to allocate memory for billboard default pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, defaultPipeline) == evk_Success, "Failed to create billboard default pipeline");
	defaultPipeline->renderpass = renderpass;
	defaultPipeline->rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
	EVK_ASSERT(ievk_pipeline_build(device, defaultPipeline) == evk_Success, "Failed to build billboard default pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_BILLBOARD_DEFAULT_NAME, defaultPipeline) == TOOLBOX_SUCCESS, "Failed to insert billboard default pipeline into pipeline's library");

	// picking pipeline
	evkPipeline* pickingPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_BILLBOARD_PICKING_NAME);
	if (pickingPipeline != NULL) ievk_pipeline_destroy(device, pickingPipeline);

	memset(&ci, 0, sizeof(evkPipelineCreateInfo));
	ci.renderpass = pickingRenderpass;
	ci.vertexShader = ievk_pipeline_create_shader(device, "billboard:picking.vert", billboard_picking_vert_spv, billboard_picking_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "billboard:picking.frag", billboard_picking_frag_spv, billboard_picking_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = false;

	// push constant
	ci.pushConstantsCount = 1;
	ci.pushConstants[0].offset = 0;
	ci.pushConstants[0].size = sizeof(evkPushConstant);
	ci.pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	// bindings
	ci.bindingsCount = 3;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;
	// sprite data
	ci.bindings[1].binding = 1;
	ci.bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[1].descriptorCount = 1;
	ci.bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[1].pImmutableSamplers = NULL;
	// colormap
	ci.bindings[2].binding = 2;
	ci.bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	ci.bindings[2].descriptorCount = 1;
	ci.bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[2].pImmutableSamplers = NULL;

	pickingPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(pickingPipeline != NULL, "Failed to allocate memory for billboard picking pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, pickingPipeline) == evk_Success, "Failed to create billboard picking pipeline");
	pickingPipeline->renderpass = pickingRenderpass;
	pickingPipeline->rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
	pickingPipeline->colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT; // id's are RED channel only
	EVK_ASSERT(ievk_pipeline_build(device, pickingPipeline) == evk_Success, "Failed to build billboard picking pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_BILLBOARD_PICKING_NAME, pickingPipeline) == TOOLBOX_SUCCESS, "Failed to insert billboard picking pipeline into pipeline's library");

	EVK_ASSERT(evk_Todo, "Create wireframe pipeline");
	return evk_Success;
}

EVK_FUNC void evk_pipeline_billboard_destroy(shashtable* pipelines, VkDevice device) {
	evkPipeline* pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_BILLBOARD_DEFAULT_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);

	pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_BILLBOARD_PICKING_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);
}

EVK_FUNC evkResult evk_pipeline_grid_create(shashtable* pipelines, evkRenderpass* renderpass, VkDevice device) {
	// default pipeline
	evkPipeline* defaultPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_GRID_DEFAULT_NAME);
	if (defaultPipeline != NULL) ievk_pipeline_destroy(device, defaultPipeline);

	evkPipelineCreateInfo ci = { 0 };
	ci.renderpass = renderpass; // this will either be default or viewport renderpass
	ci.vertexShader = ievk_pipeline_create_shader(device, "grid:default.vert", grid_vert_spv, grid_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "grid:default.frag", grid_frag_spv, grid_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = false;

	// push constant
	ci.pushConstantsCount = 0;

	// bindings
	ci.bindingsCount = 1;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;

	defaultPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(defaultPipeline != NULL, "Failed to allocate memory for grid default pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, defaultPipeline) == evk_Success, "Failed to create grid default pipeline");
	defaultPipeline->renderpass = renderpass;
	defaultPipeline->rasterizationState.cullMode = VK_CULL_MODE_NONE;
	EVK_ASSERT(ievk_pipeline_build(device, defaultPipeline) == evk_Success, "Failed to build grid default pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_GRID_DEFAULT_NAME, defaultPipeline) == TOOLBOX_SUCCESS, "Failed to insert grid default pipeline into pipeline's library");

	// grid is not "pickable" nor has a wireframe pipeline
	return evk_Success;
}

EVK_FUNC void evk_pipeline_grid_destroy(shashtable* pipelines, VkDevice device) {
	evkPipeline* pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_GRID_DEFAULT_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main render-phase
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkMainRenderphase evk_renderphase_main_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, evkMSAA msaa, bool finalPhase) {
	evkMainRenderphase renderphase = { 0 };
	memset(&renderphase.evkRenderpass, 0, sizeof(evkRenderpass));
	renderphase.evkRenderpass.name = "Main";
	renderphase.evkRenderpass.format = format;
	renderphase.evkRenderpass.msaa = msaa;

	VkAttachmentDescription attachments[3] = {0}; // color, depth, resolve
	attachments[0].format = format;
	attachments[0].samples = (VkSampleCountFlagBits)msaa;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//
	attachments[1].format = evk_device_find_depth_format(physicalDevice);
	attachments[1].samples = (VkSampleCountFlagBits)msaa;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	//
	attachments[2].format = format;
	attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[2].finalLayout = finalPhase == true ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	
	VkAttachmentReference references[3] = { 0 };
	references[0].attachment = 0;
	references[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	references[1].attachment = 1;
	references[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	references[2].attachment = 2;
	references[2].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = { 0 };
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &references[0];
	subpass.pDepthStencilAttachment = &references[1];
	subpass.pResolveAttachments = &references[2];

	VkSubpassDependency dependencies[2] = { 0 };
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	dependencies[0].dependencyFlags = 0;
	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCI;
    memset(&renderPassCI, 0, sizeof(VkRenderPassCreateInfo));
	renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCI.attachmentCount = 3u;
	renderPassCI.pAttachments = attachments;
	renderPassCI.subpassCount = 1;
	renderPassCI.pSubpasses = &subpass;
	renderPassCI.dependencyCount = 2u;
	renderPassCI.pDependencies = dependencies;
	EVK_ASSERT(vkCreateRenderPass(device, &renderPassCI, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create main renderphase renderpass");

	// cmdpool and cmdbuffers
	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);

	VkCommandPoolCreateInfo cmdPoolInfo;
    memset(&cmdPoolInfo, 0, sizeof(VkCommandPoolCreateInfo));
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create main renderphase renderpass command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo;
    memset(&cmdBufferAllocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to create main renderphase renderpass command buffers");

    return renderphase;
}

EVK_FUNC void evk_renderphase_main_destroy(evkMainRenderphase* renderphase, VkDevice device) {
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers[0]) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroyImage(device, renderphase->colorImage, NULL);
	vkFreeMemory(device, renderphase->colorMem, NULL);
	vkDestroyImageView(device, renderphase->colorView, NULL);
	
	vkDestroyImage(device, renderphase->depthImage, NULL);
	vkFreeMemory(device, renderphase->depthMem, NULL);
	vkDestroyImageView(device, renderphase->depthView, NULL);

	memset(renderphase, 0, sizeof(evkMainRenderphase));
}

EVK_FUNC evkResult evk_renderphase_main_create_framebuffers(evkMainRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat) {
	// uppon a resize event, the framebuffers and it's images must be recreated, therefore we must check if they were created already
	if (renderphase->depthView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->depthView, NULL);
	if (renderphase->depthImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->depthImage, NULL);
	if (renderphase->depthMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->depthMem, NULL);
	if (renderphase->colorView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->colorView, NULL);
	if (renderphase->colorImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->colorImage, NULL);
	if (renderphase->colorMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->colorMem, NULL);

	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	VkFormat depthFormat = evk_device_find_depth_format(physicalDevice);

	evkResult res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->colorImage,
		&renderphase->colorMem,
		colorFormat,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->colorImage,
		colorFormat, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->colorView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image view for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->depthImage,
		&renderphase->depthMem,
		depthFormat,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device, 
		renderphase->depthImage,
		depthFormat, 
		VK_IMAGE_ASPECT_DEPTH_BIT, 
		1, 
		1, 
		VK_IMAGE_VIEW_TYPE_2D, 
		NULL, 
		&renderphase->depthView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image view for the main renderphase");
		return evk_Failure;
	}

	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);

	if (!renderphase->evkRenderpass.framebuffers) {
		EVK_LOG(evk_Error, "Failed to allocate memory for main renderphase framebuffers");
		return evk_Failure;
	}

	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		const VkImageView attachments[3] = { renderphase->colorView, renderphase->depthView, views[i] };
		VkFramebufferCreateInfo fbci ;
        memset(&fbci, 0, sizeof(VkFramebufferCreateInfo));
		fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbci.renderPass = renderphase->evkRenderpass.renderpass;
		fbci.attachmentCount = 3U;
		fbci.pAttachments = &attachments[0];
		fbci.width = extent.width;
		fbci.height = extent.height;
		fbci.layers = 1;

		if (vkCreateFramebuffer(device, &fbci, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create default renderphase framebuffer");
			return evk_Failure;
		}
	}
	
	return evk_Success;
}

EVK_FUNC void evk_renderphase_main_update(evkMainRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback) {
	VkClearValue clearValues[2] = { 0 };
	const uint32_t clearValuesCount = 2;
	clearValues[0].color.float32[0] = 0.0f;
	clearValues[0].color.float32[1] = 0.0f;
	clearValues[0].color.float32[2] = 0.0f;
	clearValues[0].color.float32[3] = 1.0f;
	clearValues[1].depthStencil.depth = 1.0f;
	clearValues[1].depthStencil.stencil = 0;
	
	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer frameBuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;
	
	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);
	
	VkCommandBufferBeginInfo cmdBeginInfo;
    memset(&cmdBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin default renderphase command buffer");
	
	VkRenderPassBeginInfo renderPassBeginInfo;
    memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = frameBuffer;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = clearValuesCount;
	renderPassBeginInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	
	// set frame commandbuffer viewport
	VkViewport viewport = { 0 };
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
	
	// set frame commandbuffer scissor
	VkRect2D scissor = { 0 };
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = extent;
	vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
	
	// not using viewport as the final target, therefore draw the objects
	if (!usingViewport) {
		if (callback != NULL) {
			callback(evk_get_context(), timestep);
		}
	}
	
	vkCmdEndRenderPass(cmdBuffer);
	
	// end command buffer
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to end default renderphase command buffer");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// picking render-phase
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkPickingRenderphase evk_renderphase_picking_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, evkMSAA msaa) {
	EVK_LOG(evk_Todo, "Implement MSAA on picking?");

	evkPickingRenderphase renderphase;
    memset(&renderphase, 0, sizeof(evkPickingRenderphase));
    memset(&renderphase.evkRenderpass, 0, sizeof(evkRenderpass));
    renderphase.evkRenderpass.msaa = evk_Msaa_Off;
    renderphase.evkRenderpass.name = "Picking";
    renderphase.imageSize = 1 * 8; // (RED Channel) * 8 bits
    renderphase.colorFormat = VK_FORMAT_R32_UINT;
    renderphase.depthFormat = evk_device_find_depth_format(physicalDevice);

	// attachments, color and depth
	VkAttachmentDescription attachments[2];
    memset(attachments, 0, sizeof(VkAttachmentDescription) * 2);
	attachments[0].format = renderphase.colorFormat;
	attachments[0].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	//
	attachments[1].format = renderphase.depthFormat;
    attachments[1].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference;
    memset(&colorReference, 0, sizeof(VkAttachmentReference));
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference;
    memset(&depthReference, 0, sizeof(VkAttachmentReference));
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;
    memset(&subpassDescription, 0, sizeof(VkSubpassDescription));
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = NULL;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = NULL;
    subpassDescription.pResolveAttachments = NULL;

	VkSubpassDependency dependencies[3];
    memset(dependencies, 0, sizeof(VkSubpassDependency) * 3);
	// external to renderpass (color)
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = 0;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = 0;
	// external to renderpass (depth)
	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].dstSubpass = 0;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[1].srcAccessMask = 0;
    dependencies[1].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dependencyFlags = 0;
	// renderpass to external (transfer read)
	dependencies[2].srcSubpass = 0;
    dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[2].dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[2].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    dependencies[2].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCI;
    memset(&renderPassCI, 0, sizeof(VkRenderPassCreateInfo));
	renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCI.attachmentCount = 2U;
	renderPassCI.pAttachments = attachments;
	renderPassCI.subpassCount = 1;
	renderPassCI.pSubpasses = &subpassDescription;
	renderPassCI.dependencyCount = 3U;
	renderPassCI.pDependencies = dependencies;
	EVK_ASSERT(vkCreateRenderPass(device, &renderPassCI, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create picking renderphase renderpass");

	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
	VkCommandPoolCreateInfo cmdPoolInfo;
    memset(&cmdPoolInfo, 0, sizeof(VkCommandPoolCreateInfo));
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create picking renderphase command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo;
    memset(&cmdBufferAllocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to allocate picking renderphase command buffers");

	return renderphase;
}

EVK_FUNC void evk_renderphase_picking_destroy(evkPickingRenderphase* renderphase, VkDevice device) {
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers[0]) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroyImage(device, renderphase->colorImage, NULL);
	vkFreeMemory(device, renderphase->colorMem, NULL);
	vkDestroyImageView(device, renderphase->colorView, NULL);

	vkDestroyImage(device, renderphase->depthImage, NULL);
	vkFreeMemory(device, renderphase->depthMem, NULL);
	vkDestroyImageView(device, renderphase->depthView, NULL);

	memset(renderphase, 0, sizeof(evkPickingRenderphase));
}

EVK_FUNC evkResult evk_renderphase_picking_create_framebuffers(evkPickingRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent) {
	// uppon a resize event, the framebuffers and it's images must be recreated, therefore we must check if they were created already
	if (renderphase->depthView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->depthView, NULL);
	if (renderphase->depthImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->depthImage, NULL);
	if (renderphase->depthMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->depthMem, NULL);
	if (renderphase->colorView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->colorView, NULL);
	if (renderphase->colorImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->colorImage, NULL);
	if (renderphase->colorMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->colorMem, NULL);

	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	VkFormat depthFormat = evk_device_find_depth_format(physicalDevice);

	evkResult res = evk_device_create_image
    (
        extent,
        1,
        1,
        device,
        physicalDevice,
        &renderphase->colorImage,
        &renderphase->colorMem,
        renderphase->colorFormat,
        evk_Msaa_Off,	// no msaa on picking
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        0
    );

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->colorImage,
		renderphase->colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->colorView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image view for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->depthImage,
		&renderphase->depthMem,
		depthFormat,
		evk_Msaa_Off,	// no msaa on picking
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->depthImage,
		depthFormat,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->depthView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image view for the main renderphase");
		return evk_Failure;
	}

	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);

	if (!renderphase->evkRenderpass.framebuffers) {
		EVK_LOG(evk_Error, "Failed to allocate memory for main renderphase framebuffers");
		return evk_Failure;
	}

	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		const VkImageView attachments[2] = { renderphase->colorView, renderphase->depthView };
		VkFramebufferCreateInfo fbci;
        memset(&fbci, 0, sizeof(VkFramebufferCreateInfo));
		fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbci.renderPass = renderphase->evkRenderpass.renderpass;
		fbci.attachmentCount = 2U;
		fbci.pAttachments = &attachments[0];
		fbci.width = extent.width;
		fbci.height = extent.height;
		fbci.layers = 1;

		if (vkCreateFramebuffer(device, &fbci, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create default renderphase framebuffer");
			return evk_Failure;
		}
	}

	return evk_Success;
}

EVK_FUNC void evk_renderphase_picking_update(evkPickingRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback) {
    VkClearValue clearValues[2];
    memset(clearValues, 0, sizeof(VkClearValue) * 2);
    const uint32_t clearValuesCount = 2;
    clearValues[0].color.uint32[0] = 0;
    clearValues[0].color.uint32[1] = 0;
    clearValues[0].color.uint32[2] = 0;
    clearValues[0].color.uint32[3] = 1;
    clearValues[1].depthStencil.depth = 1.0f;
    clearValues[1].depthStencil.stencil = 0;

    VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
    VkFramebuffer frameBuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
    VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;

    vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

    VkCommandBufferBeginInfo cmdBeginInfo;
    memset(&cmdBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext = NULL;
    cmdBeginInfo.flags = 0;
    EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin picking renderphase command buffer");

    // begin renderpass
    VkRenderPassBeginInfo renderPassBeginInfo;
    memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = frameBuffer;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent = extent;
    renderPassBeginInfo.clearValueCount = clearValuesCount;
    renderPassBeginInfo.pClearValues = clearValues;
    vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    // set frame commandbuffer viewport
    VkViewport viewport = { 0 };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    VkRect2D scissor = { 0 };
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = extent;
    vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

    if (callback != NULL) {
        callback(evk_get_context(), timestep);
    }

    // end render pass
    vkCmdEndRenderPass(cmdBuffer);

    // end command buffer
    EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to finish picking renderphase command buffer");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ui render-phase
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkUIRenderphase evk_renderphase_ui_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, bool finalPhase) {
	evkUIRenderphase renderphase;
    memset(&renderphase, 0, sizeof(evkUIRenderphase));
	renderphase.evkRenderpass.name = "UI";
	renderphase.evkRenderpass.format = format;
	renderphase.evkRenderpass.msaa = evk_Msaa_Off;

	VkAttachmentDescription attachment;
    memset(&attachment, 0, sizeof(VkAttachmentDescription));
	attachment.format = format;
	attachment.samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachment.finalLayout = finalPhase == 1 ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachment;
    memset(&colorAttachment, 0, sizeof(VkAttachmentReference));
	colorAttachment.attachment = 0;
	colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachment;

	VkSubpassDependency dependency;
    memset(&dependency, 0, sizeof(VkSubpassDependency));
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo info;
    memset(&info, 0, sizeof(VkRenderPassCreateInfo));
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &attachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;
	EVK_ASSERT(vkCreateRenderPass(device, &info, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create ui render phase renderpass");

	// command pool and buffers
	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
	VkCommandPoolCreateInfo cmdPoolInfo;
    memset(&cmdPoolInfo, 0, sizeof(VkCommandPoolCreateInfo));
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create ui render phase command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo;
    memset(&cmdBufferAllocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to allocate ui render phase command buffers");

	// descriptor pool and descriptor set layout for UI image of things
	VkDescriptorSetLayoutBinding binding[1];
    memset(binding, 0, sizeof(VkDescriptorSetLayoutBinding));
	binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding[0].descriptorCount = 1;
	binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo descInfo;
    memset(&descInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
	descInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descInfo.bindingCount = 1;
	descInfo.pBindings = binding;
	EVK_ASSERT(vkCreateDescriptorSetLayout(device, &descInfo, NULL, &renderphase.descriptorSetLayout) == VK_SUCCESS, "Failed to create the ui render phase descriptor set layout");

	return renderphase;
}

EVK_FUNC void evk_renderphase_ui_destroy(evkUIRenderphase* renderphase, VkDevice device) {
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers[0]) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroyDescriptorSetLayout(device, renderphase->descriptorSetLayout, NULL);

	memset(renderphase, 0, sizeof(evkUIRenderphase));
}

EVK_FUNC evkResult evk_renderphase_ui_create_framebuffers(evkUIRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat) {
	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);
	if (renderphase->evkRenderpass.framebuffers == NULL) {
		EVK_LOG(evk_Error, "Failed to allocate memory for the ui renderphase framebuffers");
		return evk_Failure;
	}
	
	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		const VkImageView attachments[] = { views[i] };

		VkFramebufferCreateInfo framebufferCI;
        memset(&framebufferCI, 0, sizeof(VkFramebufferCreateInfo));
		framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCI.renderPass = renderphase->evkRenderpass.renderpass;
		framebufferCI.attachmentCount = 1U;
		framebufferCI.pAttachments = attachments;
		framebufferCI.width = extent.width;
		framebufferCI.height = extent.height;
		framebufferCI.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferCI, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create ui render phase framebuffer");
			return evk_Failure;
		}
	}

	return evk_Success;
}

EVK_FUNC void evk_renderphase_ui_update(evkUIRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, evkCalllback_RenderUI callback) {
	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer frameBuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;

	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

	VkCommandBufferBeginInfo cmdBeginInfo;
    memset(&cmdBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin ui renderphase command buffer");

	VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 1.0f };

	VkRenderPassBeginInfo renderPassBeginInfo;
    memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = frameBuffer;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	if (callback != NULL) {
		callback(evk_get_context(), cmdBuffer);
	}

	vkCmdEndRenderPass(cmdBuffer);
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to end ui renderphase command buffer");
}

EVK_FUNC evkViewportRenderphase evk_renderphase_viewport_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, evkMSAA msaa) {
	evkViewportRenderphase renderphase;
    memset(&renderphase, 0, sizeof(evkViewportRenderphase));
	renderphase.evkRenderpass.name = "Viewport";
	renderphase.evkRenderpass.format = format;
	renderphase.evkRenderpass.msaa = msaa;

	// attachments, subpass
	const uint32_t attachmentsSize = 2U;
	VkAttachmentDescription attachments[2] = { 0 };
	attachments[0].format = renderphase.evkRenderpass.format;
	attachments[0].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	attachments[1].format = evk_device_find_depth_format(physicalDevice);
	attachments[1].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference;
    memset(&colorReference, 0, sizeof(VkAttachmentReference));
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference;
    memset(&depthReference, 0, sizeof(VkAttachmentReference));
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;
    memset(&subpassDescription, 0, sizeof(VkSubpassDescription));
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = NULL;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = NULL;
	subpassDescription.pResolveAttachments = NULL;

	const uint32_t dependenciesSize = 2U;
	VkSubpassDependency dependencies[2] = { 0 };
    memset(dependencies, 0, sizeof(VkSubpassDependency) * dependenciesSize);
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	dependencies[0].dependencyFlags = 0;
	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCI;
    memset(&renderPassCI, 0, sizeof(VkRenderPassCreateInfo));
	renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCI.attachmentCount = attachmentsSize;
	renderPassCI.pAttachments = attachments;
	renderPassCI.subpassCount = 1;
	renderPassCI.pSubpasses = &subpassDescription;
	renderPassCI.dependencyCount = dependenciesSize;
	renderPassCI.pDependencies = dependencies;
	EVK_ASSERT(vkCreateRenderPass(device, &renderPassCI, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create viewport render phase renderpass");
	
	// command pool and buffers
	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
	VkCommandPoolCreateInfo cmdPoolInfo;
    memset(&cmdPoolInfo, 0, sizeof(VkCommandPoolCreateInfo));
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create viewport renderphase command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo;
    memset(&cmdBufferAllocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to create viewport renderphase command pool");

	return renderphase;
}

EVK_FUNC void evk_renderphase_viewport_destroy(evkViewportRenderphase* renderphase, VkDevice device) {
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers[0]) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroySampler(device, renderphase->sampler, NULL);
	vkDestroyDescriptorPool(device, renderphase->descriptorPool, NULL);
	vkDestroyDescriptorSetLayout(device, renderphase->descriptorSetLayout, NULL);

	vkDestroyImageView(device, renderphase->depthView, NULL);
	vkDestroyImage(device, renderphase->depthImage, NULL);
	vkFreeMemory(device, renderphase->depthMem, NULL);

	vkDestroyImageView(device, renderphase->colorView, NULL);
	vkDestroyImage(device, renderphase->colorImage, NULL);
	vkFreeMemory(device, renderphase->colorMem, NULL);

	memset(renderphase, 0, sizeof(evkUIRenderphase));
}

EVK_FUNC evkResult evk_renderphase_viewport_create_framebuffers(evkViewportRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat) {
	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	if (renderphase->descriptorPool != VK_NULL_HANDLE) {
		vkDestroyDescriptorPool(device, renderphase->descriptorPool, NULL);
	}

	if (renderphase->descriptorSet != VK_NULL_HANDLE) {
		vkDestroyDescriptorSetLayout(device, renderphase->descriptorSetLayout, NULL);
	}

	// descriptor pool
	VkDescriptorPoolSize poolSizes[] = { { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, EVK_CONCURRENTLY_RENDERED_FRAMES } };
	VkDescriptorPoolCreateInfo poolCI;
	memset(&poolCI, 0, sizeof(VkDescriptorPoolCreateInfo));
	poolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCI.pNext = NULL;
	poolCI.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolCI.maxSets = (uint32_t)(2 * EVK_STATIC_ARRAY_SIZE(poolSizes));
	poolCI.poolSizeCount = (uint32_t)EVK_STATIC_ARRAY_SIZE(poolSizes);
	poolCI.pPoolSizes = poolSizes;

	if (vkCreateDescriptorPool(device, &poolCI, NULL, &renderphase->descriptorPool) != VK_SUCCESS) {
		EVK_LOG(evk_Error, "Failed to create viewport render phase descriptor pool");
		return evk_Failure;
	}

	// descriptor set layout
	VkDescriptorSetLayoutBinding binding[1];
	memset(binding, 0, sizeof(VkDescriptorSetLayoutBinding) * 1);
	binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding[0].descriptorCount = 1;
	binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo info;
	memset(&info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.bindingCount = 1;
	info.pBindings = binding;
	if (vkCreateDescriptorSetLayout(device, &info, NULL, &renderphase->descriptorSetLayout) != VK_SUCCESS) {
		EVK_LOG(evk_Error, "Failed to create viewport render phase descriptor set layout");
		return evk_Failure;
	}

	// sampler
	evkResult res = evk_device_create_image_sampler
	(
		device,
		physicalDevice,
		VK_FILTER_LINEAR,
		VK_FILTER_LINEAR,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		1.0f,
		&renderphase->sampler
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create viewport render phase sampler");
		return res;
	}

	// color image
	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->colorImage,
		&renderphase->colorMem,
		renderphase->evkRenderpass.format,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport render phase color image");
		return res;
	}

	res = evk_device_create_image_view
	(
		device, 
		renderphase->colorImage,
		renderphase->evkRenderpass.format,
		VK_IMAGE_ASPECT_COLOR_BIT, 
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->colorView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport render phase color image view");
		return res;
	}

	// depth buffer
	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->depthImage,
		&renderphase->depthMem,
		evk_device_find_depth_format(physicalDevice),
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport renderphase depth image");
		return res;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->depthImage,
		evk_device_find_depth_format(physicalDevice),
		VK_IMAGE_ASPECT_DEPTH_BIT, 
		1, 
		1, 
		VK_IMAGE_VIEW_TYPE_2D,
		NULL, 
		&renderphase->depthView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport renderphase depth image view");
		return res;
	}

	// command buffer
	VkCommandBuffer command = evk_device_begin_commandbuffer_singletime(device, renderphase->evkRenderpass.cmdPool);

	VkImageSubresourceRange subresourceRange = { 0 };
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	evk_device_create_image_memory_barrier
	(
		command,
		renderphase->colorImage,
		VK_ACCESS_TRANSFER_READ_BIT,
		VK_ACCESS_MEMORY_READ_BIT,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		subresourceRange
	);

	res = evk_device_end_commandbuffer_singletime(device, renderphase->evkRenderpass.cmdPool, command, evk_get_graphics_queue());

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport renderphase depth image view");
		return res;
	}

	res = evk_device_create_image_descriptor_set
	(
		device,
		renderphase->descriptorPool,
		renderphase->descriptorSetLayout,
		renderphase->sampler,
		renderphase->colorView,
		&renderphase->descriptorSet
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport image descriptor set");
		return res;
	}

	// framebuffer
	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);
	EVK_ASSERT(renderphase->evkRenderpass.framebuffers != NULL, "Failed to allocate memory for the viewport renderphass framebuffers");

	for (size_t i = 0; i < viewsCount; i++) {
		const VkImageView attachments[2] = { renderphase->colorView, renderphase->depthView };

		VkFramebufferCreateInfo framebufferCI;
        memset(&framebufferCI, 0, sizeof(VkFramebufferCreateInfo));
		framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCI.renderPass = renderphase->evkRenderpass.renderpass;
		framebufferCI.attachmentCount = 2U;
		framebufferCI.pAttachments = attachments;
		framebufferCI.width = extent.width;
		framebufferCI.height = extent.height;
		framebufferCI.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferCI, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create viewport renderphase framebuffer");
			return res;
		}
	}

	return evk_Success;
}

EVK_FUNC void evk_renderphase_viewport_update(evkViewportRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback) {
	VkClearValue clearValues[2] = { 0 };
    memset(clearValues, 0, sizeof(VkClearValue) * 2);
	clearValues[0].color.float32[0] = 0.0f;
	clearValues[0].color.float32[1] = 0.0f;
	clearValues[0].color.float32[2] = 0.0f;
	clearValues[0].color.float32[3] = 1.0f;
	clearValues[1].depthStencil.depth = 1.0f;
	clearValues[1].depthStencil.stencil = 0;

	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer framebuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;

	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

	VkCommandBufferBeginInfo cmdBeginInfo;
    memset(&cmdBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin viewport render phase command buffer");

	VkRenderPassBeginInfo renderPassBeginInfo;
    memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = framebuffer;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = 2U;
	renderPassBeginInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport;
    memset(&viewport, 0, sizeof(VkViewport));
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

	VkRect2D scissor;
    memset(&scissor, 0, sizeof(VkRect2D));
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = extent;
	vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

	if (callback != NULL) {
		callback(evk_get_context(), timestep);
	}

	vkCmdEndRenderPass(cmdBuffer);
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to end viewport render phase command buffer");
}

#ifdef __cplusplus 
}
#endif

#endif // EVK_IMPLEMENTATION
#endif // EVK_INCLUDED
