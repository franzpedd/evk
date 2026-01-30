#ifndef EVK_VULKAN_RENDERPHASE_INCLUDED
#define EVK_VULKAN_RENDERPHASE_INCLUDED

#include "volk/volk.h"
#include "evk_types.h"

#ifdef __cplusplus 
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pipelines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds information about the lifecycle of a render operation
typedef struct evkRenderpass
{
	const char* name;
	evkMSAA msaa;
	VkFormat format;
	VkCommandPool cmdPool;
	VkCommandBuffer cmdBuffers[EVK_CONCURRENTLY_RENDERED_FRAMES];
	VkFramebuffer* framebuffers;
	uint32_t framebufferCount;
	VkRenderPass renderpass;
} evkRenderpass;

/// @brief holds information about a shader program
typedef struct evkShader
{
	const char* name;
	evkShaderType type;
	VkShaderModule module;
	VkPipelineShaderStageCreateInfo info;
} evkShader;

/// @brief holds all information related with a vulkan pipeline creation setup
typedef struct evkPipelineCreateInfo
{
	evkRenderpass* renderpass;
	VkPipelineCache pipelineCache;
	evkShader vertexShader;
	evkShader fragmentShader;
	bool passingVertexData;
	bool alphaBlending;
	VkDescriptorSetLayoutBinding bindings[EVK_PIPELINE_DESCRIPTOR_SET_LAYOUT_BINDING_MAX];
	uint32_t bindingsCount;
	VkPushConstantRange pushConstants[EVK_PIPELINE_PUSH_CONSTANTS_MAX];
	uint32_t pushConstantsCount;
	evkVertexComponent vertexComponents[evk_Vertex_Component_Max];
	uint32_t vertexComponentsCount;
} evkPipelineCreateInfo;

/// @brief holds all information about a pipeline
typedef struct evkPipeline
{
	evkRenderpass* renderpass;
	bool passingVertexData;
	bool alphaBlending;
	VkPipelineCache cache;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout layout;
	VkPipeline pipeline;
	VkVertexInputBindingDescription* bindingsDescription;
	VkVertexInputAttributeDescription* attributesDescription;
	uint32_t bindingsDescriptionCount;
	uint32_t attributesDescriptionCount;

	// auto-populated, can be modified before building pipeline
	VkPipelineShaderStageCreateInfo shaderStages[EVK_PIPELINE_SHADER_STAGES_COUNT];
	VkPipelineVertexInputStateCreateInfo vertexInputState;
	VkPipelineInputAssemblyStateCreateInfo inputVertexAssemblyState;
	VkPipelineViewportStateCreateInfo viewportState;
	VkPipelineRasterizationStateCreateInfo rasterizationState;
	VkPipelineMultisampleStateCreateInfo multisampleState;
	VkPipelineDepthStencilStateCreateInfo depthStencilState;
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
	VkPipelineColorBlendStateCreateInfo colorBlendState;
} evkPipeline;

/// @brief name of pipelines for easy hashtable lookup
#define EVK_PIPELINE_QUAD_DEFAULT_NAME "QUAD:DEFAULT"
#define EVK_PIPELINE_QUAD_PICKING_NAME "QUAD:PICKING"

/// @brief creates the quad pipeline
evkResult evk_pipeline_quad_create(shashtable* pipelines, evkRenderpass* renderpass, evkRenderpass* pickingRenderpass, VkDevice device);

/// @brief releases all resources used in the quad pipeline
void evk_pipeline_quad_destroy(shashtable* pipelines, VkDevice device);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main render phase
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds information about the main render phase
typedef struct evkMainRenderphase
{
	evkRenderpass evkRenderpass;
	evkPipeline evkPipeline;

	VkDeviceSize imageSize;
	VkImage colorImage;
	VkImage depthImage;
	VkDeviceMemory colorMem;
	VkDeviceMemory depthMem;
	VkImageView colorView;
	VkImageView depthView;
	VkFormat colorFormat;
	VkFormat depthFormat;
} evkMainRenderphase;

/// @brief creates the main renderphase
evkMainRenderphase evk_renderphase_main_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, evkMSAA msaa, bool finalPhase);

/// @brief releases all resources used by the main renderphase
void evk_renderphase_main_destroy(evkMainRenderphase* renderphase, VkDevice device);

/// @brief creates the renderphase framebuffers
evkResult evk_renderphase_main_create_framebuffers(evkMainRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat);

/// @brief updates the renderphase
void evk_renderphase_main_update(evkMainRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Picking render phase
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds the resources used on the render phase
typedef struct evkPickingRenderphase
{
	evkRenderpass evkRenderpass;
	evkPipeline evkPipeline;

	VkDeviceSize imageSize;
	VkImage colorImage;
	VkImage depthImage;
	VkDeviceMemory colorMem;
	VkDeviceMemory depthMem;
	VkImageView colorView;
	VkImageView depthView;
	VkFormat colorFormat;
	VkFormat depthFormat;
} evkPickingRenderphase;

/// @brief creates the picking render phase
evkPickingRenderphase evk_renderphase_picking_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, evkMSAA msaa);

/// @brief releases all resources used by the picking renderphase
void evk_renderphase_picking_destroy(evkPickingRenderphase* renderphase, VkDevice device);

/// @brief creates the renderphase framebuffers
evkResult evk_renderphase_picking_create_framebuffers(evkPickingRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent);

/// @brief updates the renderphase
void evk_renderphase_picking_update(evkPickingRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI render phase
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds the resources used on the render phase
typedef struct evkUIRenderphase
{
	evkRenderpass evkRenderpass;

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
} evkUIRenderphase;

/// @brief creates the picking render phase
evkUIRenderphase evk_renderphase_ui_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, bool finalPhase);

/// @brief releases all resources used by the renderphase
void evk_renderphase_ui_destroy(evkUIRenderphase* renderphase, VkDevice device);

/// @brief creates the renderphase framebuffers
evkResult evk_renderphase_ui_create_framebuffers(evkUIRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat);

/// @brief updates the renderphase
void evk_renderphase_ui_update(evkUIRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, evkCalllback_RenderUI callback);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Viewport render phase
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief holds the resources used on the render phase
typedef struct evkViewportRenderphase
{
	evkRenderpass evkRenderpass;

	VkImage colorImage;
	VkDeviceMemory colorMem;
	VkImageView colorView;
	VkImage depthImage;
	VkDeviceMemory depthMem;
	VkImageView depthView;
	VkSampler sampler;
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSet;
} evkViewportRenderphase;

/// @brief creates the picking render phase
evkViewportRenderphase evk_renderphase_viewport_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, evkMSAA msaa);

/// @brief releases all resources used by the renderphase
void evk_renderphase_viewport_destroy(evkViewportRenderphase* renderphase, VkDevice device);

/// @brief creates the renderphase framebuffers
evkResult evk_renderphase_viewport_create_framebuffers(evkViewportRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat);

/// @brief updates the renderphase
void evk_renderphase_viewport_update(evkViewportRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback);

#ifdef __cplusplus 
}
#endif

/// @brief prevents circular dependency
#ifdef EVK_VULKAN_RENDERPHASE_IMPLEMENTATION
#undef EVK_VULKAN_RENDERPHASE_IMPLEMENTATION
#include "evk_vulkan_renderphase.c"
#endif

#endif // EVK_VULKAN_RENDERPHASE_INCLUDED