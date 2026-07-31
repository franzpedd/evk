#include "evk_vulkan_drawable_2d.h"

#include "evk_core.h"
#include "evk_vulkan_core.h"
#include "evk_vulkan_renderphase.h"

#include "vecmath.h"
#include "memm.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */