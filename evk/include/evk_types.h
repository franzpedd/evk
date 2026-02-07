#ifndef EVK_TYPES_INCLUDED
#define EVK_TYPES_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include "vecmath/vecmath.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief align-as per compiler
#if defined(__cplusplus) && __cplusplus >= 201103L
    #define align_as(X) alignas(X)  // C++11 native
#elif defined(_MSC_VER)
    #define align_as(X) __declspec(align(X))
#elif defined(__GNUC__) || defined(__clang__)
    #define align_as(X) __attribute__((aligned(X)))
#else
    #define align_as(X) _Alignas(X)  // C11 native
#endif

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	evk_Msaa_Off = 0x00000001,
	evk_Msaa_X2 = 0x00000002,
	evk_Msaa_X4 = 0x00000004,
	evk_Msaa_X8 = 0x00000008,
	evk_Msaa_X16 = 0x00000010,
	evk_Msaa_X32 = 0x00000020,
	evk_Msaa_X64 = 0x00000040
} evkMSAA;

/// @brief all supported shader types for loading
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief forward declarations of platform types
#ifdef _WIN32
typedef void* HWND__;
typedef struct HWND__* HWND;
#elif defined(__APPLE__)
typedef struct objc_object CAMetalLayer;
#elif defined(__ANDROID__)
typedef struct ANativeWindow ANativeWindow;
#elif defined(__linux__) && !defined(__ANDROID__)
	#ifdef EVK_LINUX_USE_XLIB
	typedef struct _XDisplay Display;
	typedef unsigned long Window;
	#elif defined(EVK_LINUX_USE_XCB)
	typedef struct xcb_connection_t xcb_connection_t;
	typedef uint32_t xcb_window_t;
#else
typedef struct wl_display wl_display;
typedef struct wl_surface wl_surface;
#endif
#endif

/// @brief definition of the context structure
typedef struct evkContext evkContext;

/// @brief definition of the vulkan context structure
typedef struct evkVulkanBackend evkVulkanBackend;

/// @brief definition of the camera structure
typedef struct evkCamera evkCamera;

/// @brief definition of the 2D texture structure
typedef struct evkTexture2D evkTexture2D;

/// @brief definition of the sprite structure
typedef struct evkSprite evkSprite;

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

/// @brief holds information about the push constant, sent to gpu once unique object
typedef struct evkPushConstant
{
	align_as(4) uint64_t id;
	align_as(16) fmat4 model;
} evkPushConstant;

/// @brief holds information about a camera data, sent to to gpu per camera object
typedef struct evkCameraUBO
{
	align_as(16) fmat4 view;
	align_as(16) fmat4 viewInverse;
	align_as(16) fmat4 proj;
} evkCameraUBO;

/// @brief holds information about a sprite data, sent to gpu per sprite object
typedef struct evkSpriteUBO
{
	align_as(4) float uv_rotation;	// rotates the uv textures
	align_as(8) float2 uv_offset;	// used to offset the uv textures
	align_as(8) float2 uv_scale;	// used to scale the uv textures
} evkSpriteUBO;

/// @brief holds information about the window the API will be displaying to
typedef struct evkWindow
{
	#ifdef _WIN32
	HWND window;
	#elif defined(__APPLE__)
	CAMetalLayer* layer;
	#elif defined(__ANDROID__)
	ANativeWindow* window;
	#elif defined(__linux__) && !defined(__ANDROID__)
		#ifdef EVK_LINUX_USE_XLIB
		Display* display;
		Window window;
		#elif defined(EVK_LINUX_USE_XCB)
		xcb_connection_t* connection;
		xcb_window_t window;
		#else
		wl_display* display;
		wl_surface* surface;
		#endif
	#endif
} evkWindow;

/// @brief initial arguments necessary to properly initialize the API
typedef struct evkCreateInfo
{
	const char* appName;
	const char* engineName;
	uint32_t engineVersion;
	uint32_t appVersion;
	uint32_t width;
	uint32_t height;
	evkMSAA MSAA;
	bool vsync;
	bool viewport;
	evkWindow window;
} evkCreateInfo;

/// @brief helpfull struct to hold info about the necessary gpu queues
typedef struct evkQueueFamily
{
	uint32_t graphics;
	uint32_t present;
	uint32_t compute;
	bool graphicsFound;
	bool presentFound;
	bool computeFound;
} evkQueueFamily;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief definition of the render callback
typedef void (*evkCallback_Render)(evkContext* context, float timestep);

/// @brief definition of the render ui callback
typedef void (*evkCalllback_RenderUI)(evkContext* context, void* rawCmdBuffer);

#endif // EVK_TYPES_INCLUDED