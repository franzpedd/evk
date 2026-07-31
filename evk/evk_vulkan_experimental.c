#include "evk_vulkan_experimental.h"

#include "evk_core.h"
#include "evk_vulkan_core.h"
#include "evk_vulkan_renderphase.h"

/* HEADER_ONLY_ASSEMBLER_START */

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

/* HEADER_ONLY_ASSEMBLER_STOP */