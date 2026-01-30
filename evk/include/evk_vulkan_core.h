#ifndef EVK_VULKAN_CORE_INCLUDED
#define EVK_VULKAN_CORE_INCLUDED

#include "evk_types.h"

#ifdef __cplusplus 
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General core
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief initializes the vulkan backend
evkResult evk_initialize_backend(const evkCreateInfo* ci);

/// @brief shuts down the vulkan backend, releasing all of it's resources
void evk_shutdown_backend();

/// @brief updates the vulkan current frame
void evk_update_backend(float timestep, bool* mustResize);

/// @brief reads the id of the picking renderphase and returns it's number (object id) or 0 if no object was on the coord
uint32_t evk_pick_object_backend(float2 xy);

/// @brief returns the choosen graphics queue uppon device creation
VkQueue evk_get_graphics_queue();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Device
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief based on a surface, finds the queues and it's indices to use on commands submition to the gpu
evkQueueFamily evk_device_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);

/// @brief creates an image on device and binds it
evkResult evk_device_create_image(VkExtent2D size, uint32_t mipLevels, uint32_t arrayLayers, VkDevice device, VkPhysicalDevice physicalDevice, VkImage* image, VkDeviceMemory* memory, VkFormat format, evkMSAA samples, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkImageCreateFlags flags);

/// @brief creates an image view based on various params
evkResult evk_device_create_image_view(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, uint32_t mipLevels, uint32_t layerCount, VkImageViewType viewType, const VkComponentMapping* swizzle, VkImageView* outView);

/// @brief creates an image sampler based on various params
evkResult evk_device_create_image_sampler(VkDevice device, VkPhysicalDevice physicalDevice, VkFilter min, VkFilter mag, VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w, float mipLevels, VkSampler* outSampler);

/// @brief creates an image descriptor set based on various params
evkResult evk_device_create_image_descriptor_set(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkSampler sampler, VkImageView view, VkDescriptorSet* outDescriptor);

/// @brief synchronizes image layout transitions and memory access between pipeline stages
void evk_device_create_image_memory_barrier(VkCommandBuffer cmdBuffer, VkImage image, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange);

// @brief finds the first suitable memory type index based on a type filter and required properties, returns UINT32_MAX on failure
uint32_t evk_device_find_suitable_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

/// @brief retrieves the most appropriate format given a list of formats and requested features
VkFormat evk_device_find_suitable_format(VkPhysicalDevice physicalDevice, const VkFormat* candidates, uint32_t candidatesCount, VkImageTiling tiling, VkFormatFeatureFlags features);

/// @brief retrieves the most appropriate format for a depth buffer
VkFormat evk_device_find_depth_format(VkPhysicalDevice physicalDevice);

/// @brief starts the recording of a command buffer that will be used only once
VkCommandBuffer evk_device_begin_commandbuffer_singletime(VkDevice device, VkCommandPool cmdPool);

/// @brief ends the recording of the single time command buffer and send it to a queue
evkResult evk_device_end_commandbuffer_singletime(VkDevice device, VkCommandPool cmdPool, VkCommandBuffer cmdBuffer, VkQueue queue);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct evkBuffer
{
	VkDeviceSize size;
	VkDeviceSize originalDataSize;
	VkBufferUsageFlags usage;
	VkMemoryPropertyFlags memoryProperties;
	uint32_t frameCount;

	// per-frame resources
	VkBuffer* buffers;
	VkDeviceMemory* memories;
	void** mappedPointers;
	bool* isMapped;
} evkBuffer;

/// @brief creates an evkBuffer, returns NULL on failure with log messages about the error
evkBuffer* evk_buffer_create(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, uint32_t frameCount);

/// @brief releases all resources used by an evkBuffer
void evk_buffer_destroy(VkDevice device, evkBuffer* buffer);

/// @brief maps the buffer memory to host-visible CPU-accessible memory for a specific frame index
evkResult evk_buffer_map(VkDevice device, evkBuffer* buffer, uint32_t frameIndex);

/// @brief unmaps a previously mapped buffer memory for a specific frame index
evkResult evk_buffer_unmap(VkDevice device, evkBuffer* buffer, uint32_t frameIndex);

/// @brief copies data from a CPU-side pointer into a previously mapped region of a GPU buffer for a specific frame
evkResult evk_buffer_copy(evkBuffer* buffer, uint32_t frameIndex, const void* data, VkDeviceSize size, VkDeviceSize offset);

/// @brief ensures that CPU-side writes to a mapped buffer's memory are flushed and made visible to the GPU for non-coherent memory types
evkResult evk_buffer_flush(VkDevice device, evkBuffer* buffer, uint32_t frameIndex, VkDeviceSize size, VkDeviceSize nonCoherentAtomSize, VkDeviceSize offset);

/// @brief records a GPU-side command to copy data from one buffer to another within a command buffer
evkResult evk_buffer_command_copy(VkCommandBuffer commandBuffer, evkBuffer* srcBuffer, uint32_t srcFrameIndex, evkBuffer* dstBuffer, uint32_t dstFrameIndex, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);

#ifdef __cplusplus 
}
#endif

/// @brief prevents circular dependency
#ifdef EVK_VULKAN_CORE_IMPLEMENTATION
#undef EVK_VULKAN_CORE_IMPLEMENTATION
#include "evk_vulkan_core.c"
#endif

#endif // EVK_VULKAN_CORE_INCLUDED