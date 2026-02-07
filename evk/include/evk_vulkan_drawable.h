#ifndef EVK_VULKAN_DRAWABLE
#define EVK_VULKAN_DRAWABLE

#include "volk/volk.h"
#include "evk_types.h"

#ifdef __cplusplus 
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Texture
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates a 2D texture based on disk path
evkTexture2D* evk_texture2d_create_from_path(const char* path, bool ui);

/// @brief creates a 2D texture based on buffer data and parameters
evkTexture2D* evk_texture2d_create_from_buffer(uint8_t* buffer, size_t bufferLen, uint32_t width, uint32_t height, bool ui);

/// @brief releases all resources used by a texture
void evk_texture2d_destroy(evkTexture2D* texture);

/// @brief returns the texture's path
const char* evk_texture2d_get_path(evkTexture2D* texture);

/// @brief returns the texture's width
uint32_t evk_texture2d_get_width(evkTexture2D* texture);

/// @brief returns the texture's height
uint32_t evk_texture2d_get_height(evkTexture2D* texture);

/// @brief returns the texture's levels of resolutions
uint32_t evk_texture2d_get_mip_levels(evkTexture2D* texture);

/// @brief returns the texture's vulkan sampler
VkSampler evk_texture2d_get_sampler(evkTexture2D* texture);

/// @brief returns the texture's vulkan descriptor set (normally used on showing the image into the ui, like texture browser)
VkDescriptorSet evk_texture2d_get_descriptor_set(evkTexture2D* texture);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sprite
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates and returns a sprite
evkSprite* evk_sprite_create_from_path(const char* path, uint32_t id);

/// @brief releases and destroys all resources used by the sprite
void evk_sprite_destroy(evkSprite* sprite);

/// @brief updates the UBO of the sprite on gpu, when resent is true also re-upload data to gpu
void evk_sprite_update(evkSprite* sprite, bool resend);

/// @brief renders the sprite
void evk_sprite_render(evkSprite* sprite, fmat4* modelMatrix);

/// @brief returns the sprite's id
uint32_t evk_sprite_get_id(evkSprite* sprite);

#ifdef __cplusplus 
}
#endif

#endif // EVK_VULKAN_DRAWABLE

#ifdef EVK_VULKAN_DRAWABLE_IMPLEMENTATION
#undef EVK_VULKAN_DRAWABLE_IMPLEMENTATION
#include "evk_vulkan_drawable_impl.h"
#endif // EVK_VULKAN_DRAWABLE_IMPLEMENTATION