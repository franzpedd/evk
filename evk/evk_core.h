#ifndef EVK_CORE_INCLUDED
#define EVK_CORE_INCLUDED

#include "evk_defines.h"
#include "evk_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // EVK_CORE_INCLUDED