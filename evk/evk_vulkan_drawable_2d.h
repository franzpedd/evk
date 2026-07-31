#ifndef EVK_VULKAN_DRAWABLE_2D
#define EVK_VULKAN_DRAWABLE_2D

#include "evk_defines.h"
#include "evk_types.h"

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus 
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stb wrapping
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief load an image from a given filepath
EVK_API unsigned char* evk_stb_load_from_file(const char* filepath, int32_t* width, int32_t* height, int32_t* channels, int32_t desiredChannels);

/// @brief loads an image from memory binary data
EVK_API unsigned char* evk_stb_load_from_memory(const unsigned char* data, size_t length, int32_t* width, int32_t* height, int32_t* channels, int32_t desiredChannels);

/// @brief returns the string message from stb
EVK_API const char* evk_stb_failure_reason();

/// @brief releases a resource used with stb_load
EVK_API void evk_stb_free(void* data);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// texture2d
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates a 2D texture based on disk path
EVK_API evkTexture2D* evk_texture2d_create_from_path(const char* path, bool ui);

/// @brief creates a 2D texture based on buffer data and parameters
EVK_API evkTexture2D* evk_texture2d_create_from_buffer(uint8_t* buffer, size_t bufferLen, uint32_t width, uint32_t height, bool ui);

/// @brief releases all resources used by a texture
EVK_API void evk_texture2d_destroy(evkTexture2D* texture);

/// @brief returns the texture's path
EVK_API const char* evk_texture2d_get_path(evkTexture2D* texture);

/// @brief returns the texture's width
EVK_API uint32_t evk_texture2d_get_width(evkTexture2D* texture);

/// @brief returns the texture's height
EVK_API uint32_t evk_texture2d_get_height(evkTexture2D* texture);

/// @brief returns the texture's levels of resolutions
EVK_API uint32_t evk_texture2d_get_mip_levels(evkTexture2D* texture);

/// @brief returns the texture's vulkan sampler
EVK_API VkSampler evk_texture2d_get_sampler(evkTexture2D* texture);

/// @brief returns the texture's vulkan image view
EVK_API VkImageView evk_texture2d_get_view(evkTexture2D* texture);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// grid
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates and returns the grid
EVK_API evkGrid* evk_grid_create();

/// @brief releases and destroys all resources used by the grid
EVK_API void evk_grid_destroy(evkGrid* grid);

/// @brief updates the UBO of the sprite on gpu, when resent is true also re-upload data to gpu
EVK_API void evk_grid_update(evkGrid* grid, bool resend);

/// @brief renders the grid
EVK_API void evk_grid_render(evkGrid* grid);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sprite
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates and returns a sprite
EVK_API evkSprite* evk_sprite_create_from_path(const char* path, uint32_t id);

/// @brief releases and destroys all resources used by the sprite
EVK_API void evk_sprite_destroy(evkSprite* sprite);

/// @brief updates the UBO of the sprite on gpu, when resent is true also re-upload data to gpu
EVK_API void evk_sprite_update(evkSprite* sprite, bool resend);

/// @brief renders the sprite
EVK_API void evk_sprite_render(evkSprite* sprite);

/// @brief returns the albedo's texture
EVK_API evkTexture2D* evk_sprite_get_albedo(evkSprite* sprite);

/// @brief sets a new albedo for the texture
EVK_API void evk_sprite_set_albedo(evkSprite* sprite, const char* path);

/// @brief returns the sprite's id
EVK_API uint32_t evk_sprite_get_id(evkSprite* sprite);

/// @brief returns the sprite's 3D position
EVK_API float3 evk_sprite_get_translation(evkSprite* sprite);

/// @brief sets the sprite 3D position
EVK_API void evk_sprite_set_translation(evkSprite* sprite, const float3 translation);

/// @brief returns the sprite's 3D rotation angles
EVK_API float3 evk_sprite_get_rotation(evkSprite* sprite);

/// @brief sets the sprite 3D rotation angles
EVK_API void evk_sprite_set_rotation(evkSprite* sprite, const float3 rotation);

/// @brief returns the sprite's 2D scaling factors
EVK_API float2 evk_sprite_get_scale(evkSprite* sprite);

/// @brief sets the sprite 2D scaling factors
EVK_API void evk_sprite_set_scale(evkSprite* sprite, const float2 scale);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// billboard
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates and returns a billboard
EVK_API evkBillboard* evk_billboard_create_from_path(const char* path, uint32_t id);

/// @brief releases and destroys all resources used by the billboard
EVK_API void evk_billboard_destroy(evkBillboard* billboard);

/// @brief updates the UBO of the billboard  on gpu, when resent is true also re-upload data to gpu
EVK_API void evk_billboard_update(evkBillboard* billboard, bool resend);

/// @brief renders the billboard
EVK_API void evk_billboard_render(evkBillboard* billboard);

/// @brief returns the billboard's id
EVK_API uint32_t evk_billboard_get_id(evkBillboard* billboard);

/// @brief returns the billboard's 3D position
EVK_API float3 evk_billboard_get_translation(evkBillboard* billboard);

/// @brief sets the billboard 3D position
EVK_API void evk_billboard_set_translation(evkBillboard* billboard, const float3 translation);

/// @brief returns the billboard's 3D rotation angles
EVK_API float3 evk_billboard_get_rotation(evkBillboard* billboard);

/// @brief sets the billboard 3D rotation angles
EVK_API void evk_billboard_set_rotation(evkBillboard* billboard, const float3 rotation);

/// @brief returns the billboard's 2D scaling factors
EVK_API float2 evk_billboard_get_scale(evkBillboard* billboard);

/// @brief sets the billboard 2D scaling factors
EVK_API void evk_billboard_set_scale(evkBillboard* billboard, const float2 scale);

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */

#endif // EVK_VULKAN_DRAWABLE_2D