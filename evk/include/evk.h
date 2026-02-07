#ifndef EVK_INCLUDED
#define EVK_INCLUDED

#include "evk_types.h"

// @HEADER_ONLY_GENERATOR_START

#ifdef __cplusplus 
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Context
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief initializes the api
evkResult evk_init(const evkCreateInfo* ci);

/// @brief shuts down the api, releases all resources 
evkResult evk_shutdown();

/// @brief updates the renderer, starting the render commands and eventually calling back when it's time to render objects
void evk_update(float timestep);

/// @brief returns the id of an object underneath a given xy coordinates
uint32_t evk_pick_object(float2 xy);

/// @brief returns the global context, used for external functions
evkContext* evk_get_context();

/// @brief returns the main camera, object created to facilitate usage of the api
evkCamera* evk_get_main_camera();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getters/Setters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief returns if evk was created with vsync enabled
bool evk_using_vsync();

/// @brief returns if evk was created with a viewport enabled
bool evk_using_viewport();

/// @brief returns the msaa used at the momment
evkMSAA evk_get_msaa();

/// @brief if using viewport, returns it's size
float2 evk_get_viewport_size();

/// @brief if using viewport, set its size
void evk_set_viewport_size(float2 size);

/// @brief returns the size of the framebuffer
float2 evk_get_framebuffer_size();

/// @brief sets a new size for the framebuffer
void evk_set_framebuffer_size(float2 size);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief sets the user defined pointer address
void evk_set_user_pointer(void* pointer);

/// @brief returns the user defined pointer address
void* evk_get_user_pointer();

/// @brief sets a function to be called when it's time to draw
void evk_set_render_callback(evkCallback_Render callback);

/// @brief returns the function address responsible for issue the rendering of objects
evkCallback_Render evk_get_render_callback();

/// @brief sets a function to be called when it's time to draw ui
void evk_set_renderui_callback(evkCalllback_RenderUI callback);

/// @brief returns the function address responsible for issue the rendering of ui
evkCalllback_RenderUI evk_get_renderui_callback();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Log and error
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief general logging function
void evk_log_message(evkSeverity severity, const char* file, int32_t line, const char* fmt, ...);

/// @brief log macro 
#ifdef EVK_ENABLE_VALIDATIONS
	#define EVK_LOG(severity, ...) evk_log_message(severity, __FILE__, __LINE__, __VA_ARGS__);
	#define EVK_ASSERT(condition, ...) if (!(condition)) { evk_log_message(evk_Fatal, __FILE__, __LINE__, __VA_ARGS__); }
#else
	#define EVK_LOG(...)
	#define EVK_ASSERT(condition, msg) ((void)0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Camera
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates and returns a camera
evkCamera* evk_camera_create(float aspectRatio);

/// @brief releases all resources used by the camera
void evk_camera_destroy(evkCamera* camera);

/// @brief updates the camera logic for the frame
void evk_camera_update(evkCamera* camera, float timestep);

/// @brief sets a new aspect ratio for the camera
void evk_camera_set_aspect_ratio(evkCamera* camera, float aspect);

/// @brief returns the camera's current aspect ratio
float evk_camera_get_aspect_ratio(evkCamera* camera);

/// @brief returns the camera's current field of view
float evk_camera_get_fov(evkCamera* camera);

/// @brief applys a translation to the camera's view, moving the camera
void evk_camera_translate(evkCamera* camera, float3 dir);

/// @brief applys a rotation to the camera's view, rotating the camera
void evk_camera_rotate(evkCamera* camera, float3 dir);

/// @brief returns the camera's view matrix
fmat4 evk_camera_get_view(evkCamera* camera);

/// @brief returns the camera's inverse view matrix
fmat4 evk_camera_get_view_inverse(evkCamera* camera);

/// @brief returns the camera's perspective projection matrix
fmat4 evk_camera_get_perspective(evkCamera* camera);

/// @brief returns the camera's inverse perspective projection matrix
fmat4 evk_camera_get_perspective_inverse(evkCamera* camera);

/// @brief enables/disables the camera movement
void evk_camera_set_lock(evkCamera* camera, bool value);

/// @brief returns if camera can currently move
bool evk_camera_get_lock(evkCamera* camera);

/// @brief moves/stops moving the camera towards a direction
void evk_camera_move(evkCamera* camera, evkCameraDir dir, bool moving);

/// @brief returns if the camera speed modifier is currently applyed
bool evk_camera_get_speed_modifier(evkCamera* camera, float* value);

/// @brief sets/unsets the camera's speed modifier
void evk_camera_set_speed_modifier(evkCamera* camera, bool status, float value);

/// @brief returns the camera's current 3d position
float3 evk_camera_get_position(evkCamera* camera);

/// @brief returns the camera's current 3d front position
float3 evk_camera_get_front(evkCamera* camera);

#ifdef __cplusplus 
}
#endif

#endif // EVK_INCLUDED

// @HEADER_ONLY_GENERATOR_END

/// @brief prevents circular dependency
#ifdef EVK_IMPLEMENTATION
#undef EVK_IMPLEMENTATION
#include "evk_impl.h"
#endif