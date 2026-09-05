#ifndef EVK_TYPES_INCLUDED
#define EVK_TYPES_INCLUDED

#include "evk_defines.h"
#include "vecmath.h"

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // EVK_TYPES_INCLUDED