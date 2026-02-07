#include "evk_vulkan_renderphase.h"

#include "shader/sprite_default_vert_spv.h"
#include "shader/sprite_default_frag_spv.h"
#include "shader/sprite_picking_vert_spv.h"
#include "shader/sprite_picking_frag_spv.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief creates an array of VkVertexInputBindingDescription based on parameters
static VkVertexInputBindingDescription* ievk_pipeline_get_binding_descriptions(bool passingVertexData, uint32_t* bindingCount)
{
    if (!passingVertexData) {
        *bindingCount = 0U;
        return NULL;
    }

    VkVertexInputBindingDescription* bindings = (VkVertexInputBindingDescription*)m_malloc(sizeof(VkVertexInputBindingDescription));
    bindings[0].binding = 0;
    bindings[0].stride = sizeof(evkVertex);
    bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    *bindingCount = 1U;
    return bindings;
}

/// @brief creates an array of VkVertexInputAttributeDescription
static VkVertexInputAttributeDescription* ievk_pipeline_get_attribute_descriptions(evkVertexComponent* vertexComponents, uint32_t componentsCount, uint32_t* attributesCount)
{
	VkVertexInputAttributeDescription* bindings = (VkVertexInputAttributeDescription*)m_malloc(sizeof(VkVertexInputAttributeDescription) * componentsCount);

	for (uint32_t i = 0; i < componentsCount; i++) {
		evkVertexComponent component = vertexComponents[i];
		VkVertexInputAttributeDescription desc = { 0 };
		desc.binding = 0;
		desc.location = (uint32_t)component;

		switch (component)
		{
			case evk_Vertex_Component_Position:
			{
				desc.format = VK_FORMAT_R32G32B32_SFLOAT;
				desc.offset = offsetof(evkVertex, position);
				break;
			}

			case evk_Vertex_Component_Normal:
			{
				desc.format = VK_FORMAT_R32G32B32_SFLOAT;
				desc.offset = offsetof(evkVertex, normal);
				break;
			}

			case evk_Vertex_Component_UV_0:
			{
				desc.format = VK_FORMAT_R32G32_SFLOAT;
				desc.offset = offsetof(evkVertex, uv_0);
				break;
			}

			case evk_Vertex_Component_Color_0:
			{
				desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				desc.offset = offsetof(evkVertex, color_0);
				break;
			}

			case evk_Vertex_Component_Weights_0:
			{
				desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				desc.offset = offsetof(evkVertex, weights_0);
				break;
			}

			case evk_Vertex_Component_Joints_0:
			{
				desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				desc.offset = offsetof(evkVertex, joints_0);
				break;
			}

			default: break;
		}
		bindings[i] = desc;
	}
	*attributesCount = componentsCount;
	return bindings;
}

static VkPipelineVertexInputStateCreateInfo ievk_pipeline_populate_visci(evkPipeline* pipeline, evkVertexComponent* vertexComponents, uint32_t componentsCount)
{
	pipeline->bindingsDescription = ievk_pipeline_get_binding_descriptions(pipeline->passingVertexData, &pipeline->bindingsDescriptionCount);
	pipeline->attributesDescription = ievk_pipeline_get_attribute_descriptions(vertexComponents, componentsCount, &pipeline->attributesDescriptionCount);

	VkPipelineVertexInputStateCreateInfo visci = { 0 };
	visci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	visci.pNext = NULL;
	visci.flags = 0;
	visci.vertexBindingDescriptionCount = pipeline->bindingsDescriptionCount;
	visci.pVertexBindingDescriptions = pipeline->bindingsDescription;
	visci.vertexAttributeDescriptionCount = pipeline->attributesDescriptionCount;
	visci.pVertexAttributeDescriptions = pipeline->attributesDescription;
	return visci;
}

/// @brief creates and returns a pipeline
static evkResult ievk_pipeline_create(VkDevice device, evkPipelineCreateInfo* ci, evkPipeline* outPipe)
{
	EVK_ASSERT(device != VK_NULL_HANDLE, "device is NULL");
	EVK_ASSERT(ci != NULL, "ci is NULL");
	EVK_ASSERT(outPipe != NULL, "outPipe is NULL");

	outPipe->passingVertexData = ci->passingVertexData;
	outPipe->cache = ci->pipelineCache;
	outPipe->shaderStages[0] = ci->vertexShader.info;
	outPipe->shaderStages[1] = ci->fragmentShader.info;

	// descriptor set
	VkDescriptorSetLayoutCreateInfo descSetLayoutCI = { 0 };
	descSetLayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCI.pNext = NULL;
	descSetLayoutCI.flags = 0;
	descSetLayoutCI.bindingCount = ci->bindingsCount;
	descSetLayoutCI.pBindings = ci->bindings;
	EVK_ASSERT(vkCreateDescriptorSetLayout(device, &descSetLayoutCI, NULL, &outPipe->descriptorSetLayout) == VK_SUCCESS, "Failed to create descriptor set layout");

	// pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCI = { 0 };
	pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCI.pNext = NULL;
	pipelineLayoutCI.flags = 0;
	pipelineLayoutCI.setLayoutCount = 1;
	pipelineLayoutCI.pSetLayouts = &outPipe->descriptorSetLayout;
	pipelineLayoutCI.pushConstantRangeCount = ci->pushConstantsCount;
	pipelineLayoutCI.pPushConstantRanges = ci->pushConstants;
	EVK_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCI, NULL, &outPipe->layout) == VK_SUCCESS, "Failed to create pipeline layout");

	// vertex input state
	outPipe->vertexInputState = ievk_pipeline_populate_visci(outPipe, ci->vertexComponents, ci->vertexComponentsCount);
	// input vertex assembly state
	outPipe->inputVertexAssemblyState = (VkPipelineInputAssemblyStateCreateInfo){ 0 };
	outPipe->inputVertexAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	outPipe->inputVertexAssemblyState.pNext = NULL;
	outPipe->inputVertexAssemblyState.flags = 0;
	outPipe->inputVertexAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	outPipe->inputVertexAssemblyState.primitiveRestartEnable = VK_FALSE;
	// viewport state
	outPipe->viewportState = (VkPipelineViewportStateCreateInfo){ 0 };
	outPipe->viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	outPipe->viewportState.pNext = NULL;
	outPipe->viewportState.flags = 0;
	outPipe->viewportState.viewportCount = 1;
	outPipe->viewportState.pViewports = NULL; // using dynamic viewport
	outPipe->viewportState.scissorCount = 1;
	outPipe->viewportState.pScissors = NULL; // using dynamic scissor
	// rasterization state
	outPipe->rasterizationState = (VkPipelineRasterizationStateCreateInfo){ 0 };
	outPipe->rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	outPipe->rasterizationState.pNext = NULL;
	outPipe->rasterizationState.flags = 0;
	outPipe->rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	outPipe->rasterizationState.cullMode = VK_CULL_MODE_NONE;
	outPipe->rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	outPipe->rasterizationState.depthClampEnable = VK_FALSE;
	outPipe->rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	outPipe->rasterizationState.lineWidth = 1.0f;
	// multisampling state
	outPipe->multisampleState = (VkPipelineMultisampleStateCreateInfo){ 0 };
	outPipe->multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	outPipe->multisampleState.pNext = NULL;
	outPipe->multisampleState.flags = 0;
	outPipe->multisampleState.rasterizationSamples = (VkSampleCountFlagBits)ci->renderpass->msaa;
	outPipe->multisampleState.sampleShadingEnable = VK_FALSE;
	// depth stencil state
	outPipe->depthStencilState = (VkPipelineDepthStencilStateCreateInfo){ 0 };
	outPipe->depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	outPipe->depthStencilState.pNext = NULL;
	outPipe->depthStencilState.flags = 0;
	outPipe->depthStencilState.depthTestEnable = VK_TRUE;
	outPipe->depthStencilState.depthWriteEnable = VK_TRUE;
	outPipe->depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	outPipe->depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
	// color blend attachment
	outPipe->colorBlendAttachmentState = (VkPipelineColorBlendAttachmentState){ 0 };
	outPipe->colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	outPipe->colorBlendAttachmentState.blendEnable = ci->alphaBlending == true ? VK_TRUE : VK_FALSE;
	outPipe->colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	outPipe->colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	outPipe->colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	// color blend state
	outPipe->colorBlendState = (VkPipelineColorBlendStateCreateInfo){ 0 };
	outPipe->colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	outPipe->colorBlendState.pNext = NULL;
	outPipe->colorBlendState.flags = 0;
	outPipe->colorBlendState.attachmentCount = 1;
	outPipe->colorBlendState.pAttachments = &outPipe->colorBlendAttachmentState;
	outPipe->colorBlendState.logicOpEnable = VK_FALSE;
	outPipe->colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	outPipe->colorBlendState.blendConstants[0] = 0.0f;
	outPipe->colorBlendState.blendConstants[1] = 0.0f;
	outPipe->colorBlendState.blendConstants[2] = 0.0f;
	outPipe->colorBlendState.blendConstants[3] = 0.0f;

	return evk_Success;
}

/// @brief releases all resources used by a pipeline
static void ievk_pipeline_destroy(VkDevice device, evkPipeline* pipeline)
{
	if (!pipeline || device == VK_NULL_HANDLE) return;

	vkDeviceWaitIdle(device);
	vkDestroyPipeline(device, pipeline->pipeline, NULL);
	vkDestroyPipelineLayout(device, pipeline->layout, NULL);
	vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, NULL);

	if (pipeline->bindingsDescription != NULL) m_free(pipeline->bindingsDescription);
	if (pipeline->attributesDescription != NULL) m_free(pipeline->attributesDescription);

	// not ideal since shader module was first introduced on shader struct, but it's the same module after-all
	vkDestroyShaderModule(device, pipeline->shaderStages[0].module, NULL);
	vkDestroyShaderModule(device, pipeline->shaderStages[1].module, NULL);

	m_free(pipeline);
}

/// @brief builds a pipeline, must be previously configured as desired
static evkResult ievk_pipeline_build(VkDevice device, evkPipeline* pipeline)
{
	// dynamic state is here because dynamic states must be constant
	VkPipelineDynamicStateCreateInfo dynamicState = { 0 };
	const VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pNext = NULL;
	dynamicState.flags = 0;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkGraphicsPipelineCreateInfo ci = { 0 };
	ci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	ci.pNext = NULL;
	ci.flags = 0;
	ci.stageCount = EVK_PIPELINE_SHADER_STAGES_COUNT;
	ci.pStages = pipeline->shaderStages;
	ci.pVertexInputState = &pipeline->vertexInputState;
	ci.pInputAssemblyState = &pipeline->inputVertexAssemblyState;
	ci.pViewportState = &pipeline->viewportState;
	ci.pRasterizationState = &pipeline->rasterizationState;
	ci.pMultisampleState = &pipeline->multisampleState;
	ci.pDepthStencilState = &pipeline->depthStencilState;
	ci.pColorBlendState = &pipeline->colorBlendState;
	ci.pDynamicState = &dynamicState;
	ci.layout = pipeline->layout;
	ci.renderPass = pipeline->renderpass->renderpass;
	ci.subpass = 0;

	VkResult res = vkCreateGraphicsPipelines(device, pipeline->cache, 1, &ci, NULL, &pipeline->pipeline);
	if (res != VK_SUCCESS) {
		EVK_LOG(evk_Error, "Failed to build the graphics pipeline {%d}", res);
		return evk_Failure;
	}

	return evk_Success;
}

/// @brief creates a shader program based on a pre-compiled spirv on disk
static evkShader ievk_pipeline_create_shader(VkDevice device, const char* name, const uint32_t* spirv, size_t spirvSize, evkShaderType type)
{
	evkShader shader = { 0 };
	shader.name = name;
	shader.type = type;
	shader.info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader.info.pNext = NULL;
	shader.info.flags = 0;
	shader.info.pName = "main";
	shader.info.pSpecializationInfo = NULL;
	switch (type)
	{
		case evk_Shader_Type_Vertex: { shader.info.stage = VK_SHADER_STAGE_VERTEX_BIT; break; }
		case evk_Shader_Type_Fragment: { shader.info.stage = VK_SHADER_STAGE_FRAGMENT_BIT; break; }
		case evk_Shader_Type_Compute: { shader.info.stage = VK_SHADER_STAGE_COMPUTE_BIT; break; }
		case evk_Shader_Type_Geometry: { shader.info.stage = VK_SHADER_STAGE_GEOMETRY_BIT; break; }
		case evk_Shader_Type_Tess_Ctrl: { shader.info.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break; }
		case evk_Shader_Type_Tess_Eval: { shader.info.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break; }
		default: { break; }
	}

	VkShaderModuleCreateInfo moduleCI = { 0 };
	moduleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCI.pNext = NULL;
	moduleCI.flags = 0;
	moduleCI.codeSize = spirvSize * sizeof(uint32_t);
	moduleCI.pCode = spirv;
	EVK_ASSERT(vkCreateShaderModule(device, &moduleCI, NULL, &shader.info.module) == VK_SUCCESS, "Failed to create shader module");

	return shader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pipelines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

evkResult evk_pipeline_sprite_create(shashtable* pipelines, evkRenderpass* renderpass, evkRenderpass* pickingRenderpass, VkDevice device)
{
	// default pipeline
	evkPipeline* defaultPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_DEFAULT_NAME);
	if (defaultPipeline != NULL) ievk_pipeline_destroy(device, defaultPipeline);

	evkPipelineCreateInfo ci = { 0 };
	ci.renderpass = renderpass; // this will either be default or viewport renderpass
	ci.vertexShader = ievk_pipeline_create_shader(device, "sprite.vert", sprite_default_vert_spv, sprite_default_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "sprite.frag", sprite_default_frag_spv, sprite_default_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = true;

	// push constant
	ci.pushConstantsCount = 1;
	ci.pushConstants[0].offset = 0;
	ci.pushConstants[0].size = sizeof(evkPushConstant);
	ci.pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	// bindings
	ci.bindingsCount = 3;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;
	// quasprited data
	ci.bindings[1].binding = 1;
	ci.bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[1].descriptorCount = 1;
	ci.bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[1].pImmutableSamplers = NULL;
	// colormap
	ci.bindings[2].binding = 2;
	ci.bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	ci.bindings[2].descriptorCount = 1;
	ci.bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[2].pImmutableSamplers = NULL;

	defaultPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(defaultPipeline != NULL, "Failed to allocate memory for sprite default pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, defaultPipeline) == evk_Success, "Failed to create sprite default pipeline");
	defaultPipeline->renderpass = renderpass;
	defaultPipeline->rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
	EVK_ASSERT(ievk_pipeline_build(device, defaultPipeline) == evk_Success, "Failed to build sprite default pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_SPRITE_DEFAULT_NAME, defaultPipeline) == CTOOLBOX_SUCCESS, "Failed to insert sprite default pipeline into pipeline's library");
	
	// picking pipeline
	evkPipeline* pickingPipeline = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_PICKING_NAME);
	if (pickingPipeline != NULL) ievk_pipeline_destroy(device, pickingPipeline);
	
	ci = (evkPipelineCreateInfo){ 0 };
	ci.renderpass = pickingRenderpass;
	ci.vertexShader = ievk_pipeline_create_shader(device, "sprite.vert", sprite_picking_vert_spv, sprite_picking_vert_spv_size, evk_Shader_Type_Vertex);
	ci.fragmentShader = ievk_pipeline_create_shader(device, "sprite.frag", sprite_picking_frag_spv, sprite_picking_frag_spv_size, evk_Shader_Type_Fragment);
	ci.passingVertexData = false;
	ci.alphaBlending = false;
	
	// push constant
	ci.pushConstantsCount = 1;
	ci.pushConstants[0].offset = 0;
	ci.pushConstants[0].size = sizeof(evkPushConstant);
	ci.pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	
	// bindings
	ci.bindingsCount = 3;
	// camera data
	ci.bindings[0].binding = 0;
	ci.bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[0].descriptorCount = 1;
	ci.bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[0].pImmutableSamplers = NULL;
	// sprite data
	ci.bindings[1].binding = 1;
	ci.bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ci.bindings[1].descriptorCount = 1;
	ci.bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[1].pImmutableSamplers = NULL;
	// colormap
	ci.bindings[2].binding = 2;
	ci.bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	ci.bindings[2].descriptorCount = 1;
	ci.bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ci.bindings[2].pImmutableSamplers = NULL;
	
	pickingPipeline = (evkPipeline*)m_malloc(sizeof(evkPipeline));
	EVK_ASSERT(pickingPipeline != NULL, "Failed to allocate memory for sprite picking pipeline creation");

	EVK_ASSERT(ievk_pipeline_create(device, &ci, pickingPipeline) == evk_Success, "Failed to create sprite picking pipeline");
	pickingPipeline->renderpass = pickingRenderpass;
	pickingPipeline->rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
	pickingPipeline->colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT; // id's are RED channel only
	EVK_ASSERT(ievk_pipeline_build(device, pickingPipeline) == evk_Success, "Failed to build sprite picking pipeline");
	EVK_ASSERT(shashtable_insert(pipelines, EVK_PIPELINE_SPRITE_PICKING_NAME, pickingPipeline) == CTOOLBOX_SUCCESS, "Failed to insert sprite picking pipeline into pipeline's library");

	EVK_ASSERT(evk_Todo, "Create wireframe pipeline");
	return evk_Success;
}

void evk_pipeline_sprite_destroy(shashtable* pipelines, VkDevice device)
{
	evkPipeline* pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_DEFAULT_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);
	pipe = NULL;

	pipe = (evkPipeline*)shashtable_lookup(pipelines, EVK_PIPELINE_SPRITE_PICKING_NAME);
	if (pipe != NULL) ievk_pipeline_destroy(device, pipe);
	pipe = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main renderphase
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

evkMainRenderphase evk_renderphase_main_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, evkMSAA msaa, bool finalPhase)
{
	evkMainRenderphase renderphase = { 0 };
	renderphase.evkRenderpass = (evkRenderpass){ 0 };
	renderphase.evkRenderpass.name = "Main";
	renderphase.evkRenderpass.format = format;
	renderphase.evkRenderpass.msaa = msaa;

	VkAttachmentDescription attachments[3] = {0}; // color, depth, resolve
	attachments[0].format = format;
	attachments[0].samples = (VkSampleCountFlagBits)msaa;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//
	attachments[1].format = evk_device_find_depth_format(physicalDevice);
	attachments[1].samples = (VkSampleCountFlagBits)msaa;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	//
	attachments[2].format = format;
	attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[2].finalLayout = finalPhase == true ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	
	VkAttachmentReference references[3] = { 0 };
	references[0].attachment = 0;
	references[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	references[1].attachment = 1;
	references[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	references[2].attachment = 2;
	references[2].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = { 0 };
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &references[0];
	subpass.pDepthStencilAttachment = &references[1];
	subpass.pResolveAttachments = &references[2];

	VkSubpassDependency dependencies[2] = { 0 };
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	dependencies[0].dependencyFlags = 0;
	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCI = { 0 };
	renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCI.attachmentCount = 3u;
	renderPassCI.pAttachments = attachments;
	renderPassCI.subpassCount = 1;
	renderPassCI.pSubpasses = &subpass;
	renderPassCI.dependencyCount = 2u;
	renderPassCI.pDependencies = dependencies;
	EVK_ASSERT(vkCreateRenderPass(device, &renderPassCI, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create main renderphase renderpass");

	// cmdpool and cmdbuffers
	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);

	VkCommandPoolCreateInfo cmdPoolInfo = { 0 };
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create main renderphase renderpass command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo = { 0 };
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to create main renderphase renderpass command buffers");

    return renderphase;
}

void evk_renderphase_main_destroy(evkMainRenderphase* renderphase, VkDevice device)
{
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroyImage(device, renderphase->colorImage, NULL);
	vkFreeMemory(device, renderphase->colorMem, NULL);
	vkDestroyImageView(device, renderphase->colorView, NULL);
	
	vkDestroyImage(device, renderphase->depthImage, NULL);
	vkFreeMemory(device, renderphase->depthMem, NULL);
	vkDestroyImageView(device, renderphase->depthView, NULL);

	memset(renderphase, 0, sizeof(evkMainRenderphase));
}

evkResult evk_renderphase_main_create_framebuffers(evkMainRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat)
{
	// uppon a resize event, the framebuffers and it's images must be recreated, therefore we must check if they were created already
	if (renderphase->depthView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->depthView, NULL);
	if (renderphase->depthImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->depthImage, NULL);
	if (renderphase->depthMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->depthMem, NULL);
	if (renderphase->colorView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->colorView, NULL);
	if (renderphase->colorImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->colorImage, NULL);
	if (renderphase->colorMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->colorMem, NULL);

	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	VkFormat depthFormat = evk_device_find_depth_format(physicalDevice);

	evkResult res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->colorImage,
		&renderphase->colorMem,
		colorFormat,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->colorImage,
		colorFormat, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->colorView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image view for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->depthImage,
		&renderphase->depthMem,
		depthFormat,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device, 
		renderphase->depthImage,
		depthFormat, 
		VK_IMAGE_ASPECT_DEPTH_BIT, 
		1, 
		1, 
		VK_IMAGE_VIEW_TYPE_2D, 
		NULL, 
		&renderphase->depthView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image view for the main renderphase");
		return evk_Failure;
	}

	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);

	if (!renderphase->evkRenderpass.framebuffers) {
		EVK_LOG(evk_Error, "Failed to allocate memory for main renderphase framebuffers");
		return evk_Failure;
	}

	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		const VkImageView attachments[3] = { renderphase->colorView, renderphase->depthView, views[i] };
		VkFramebufferCreateInfo fbci = { 0 };
		fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbci.renderPass = renderphase->evkRenderpass.renderpass;
		fbci.attachmentCount = 3U;
		fbci.pAttachments = &attachments[0];
		fbci.width = extent.width;
		fbci.height = extent.height;
		fbci.layers = 1;

		if (vkCreateFramebuffer(device, &fbci, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create default renderphase framebuffer");
			return evk_Failure;
		}
	}
	
	return evk_Success;
}

void evk_renderphase_main_update(evkMainRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback)
{
	VkClearValue clearValues[2] = { 0 };
	const uint32_t clearValuesCount = 2;
	clearValues[0].color = (VkClearColorValue){ 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = (VkClearDepthStencilValue){ 1.0f, 0 };
	
	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer frameBuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;
	
	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);
	
	VkCommandBufferBeginInfo cmdBeginInfo = { 0 };
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin default renderphase command buffer");
	
	VkRenderPassBeginInfo renderPassBeginInfo = { 0 };
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = frameBuffer;
	renderPassBeginInfo.renderArea.offset = (VkOffset2D){ 0, 0 };
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = clearValuesCount;
	renderPassBeginInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	
	// set frame commandbuffer viewport
	VkViewport viewport = { 0 };
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
	
	// set frame commandbuffer scissor
	VkRect2D scissor = { 0 };
	scissor.offset = (VkOffset2D){ 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
	
	// not using viewport as the final target, therefore draw the objects
	if (!usingViewport) {
		if (callback != NULL) {
			callback(evk_get_context(), timestep);
		}
	}
	
	vkCmdEndRenderPass(cmdBuffer);
	
	// end command buffer
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to end default renderphase command buffer");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Picking render phase
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

evkPickingRenderphase evk_renderphase_picking_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, evkMSAA msaa)
{
	EVK_LOG(evk_Todo, "Implement MSAA on picking?");

	evkPickingRenderphase renderphase = { 0 };
	renderphase.evkRenderpass = (evkRenderpass){ 0 };
	renderphase.evkRenderpass.msaa = evk_Msaa_Off;
	renderphase.evkRenderpass.name = "Picking";
	renderphase.imageSize = 1 * 8; // (RED Channel) * 8 bits
	renderphase.colorFormat = VK_FORMAT_R32_UINT;
	renderphase.depthFormat = evk_device_find_depth_format(physicalDevice);

	// attachments, color and depth
	VkAttachmentDescription attachments[2] = { 0 };
	attachments[0].format = renderphase.colorFormat;
	attachments[0].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; //VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//
	attachments[1].format = renderphase.depthFormat;
	attachments[1].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference = { 0 };
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = { 0 };
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = { 0 };
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = NULL;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = NULL;
	subpassDescription.pResolveAttachments = NULL;

	VkSubpassDependency dependencies[2] = { 0 };
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	dependencies[0].dependencyFlags = 0;
	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCI = { 0 };
	renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCI.attachmentCount = 2U;
	renderPassCI.pAttachments = attachments;
	renderPassCI.subpassCount = 1;
	renderPassCI.pSubpasses = &subpassDescription;
	renderPassCI.dependencyCount = 2U;
	renderPassCI.pDependencies = dependencies;
	EVK_ASSERT(vkCreateRenderPass(device, &renderPassCI, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create picking renderphase renderpass");

	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
	VkCommandPoolCreateInfo cmdPoolInfo = { 0 };
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create picking renderphase command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo = { 0 };
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to allocate picking renderphase command buffers");

	return renderphase;
}

void evk_renderphase_picking_destroy(evkPickingRenderphase* renderphase, VkDevice device)
{
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroyImage(device, renderphase->colorImage, NULL);
	vkFreeMemory(device, renderphase->colorMem, NULL);
	vkDestroyImageView(device, renderphase->colorView, NULL);

	vkDestroyImage(device, renderphase->depthImage, NULL);
	vkFreeMemory(device, renderphase->depthMem, NULL);
	vkDestroyImageView(device, renderphase->depthView, NULL);

	memset(renderphase, 0, sizeof(evkPickingRenderphase));
}

evkResult evk_renderphase_picking_create_framebuffers(evkPickingRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent)
{
	// uppon a resize event, the framebuffers and it's images must be recreated, therefore we must check if they were created already
	if (renderphase->depthView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->depthView, NULL);
	if (renderphase->depthImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->depthImage, NULL);
	if (renderphase->depthMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->depthMem, NULL);
	if (renderphase->colorView != VK_NULL_HANDLE) vkDestroyImageView(device, renderphase->colorView, NULL);
	if (renderphase->colorImage != VK_NULL_HANDLE) vkDestroyImage(device, renderphase->colorImage, NULL);
	if (renderphase->colorMem != VK_NULL_HANDLE) vkFreeMemory(device, renderphase->colorMem, NULL);

	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	VkFormat depthFormat = evk_device_find_depth_format(physicalDevice);

	evkResult res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->colorImage,
		&renderphase->colorMem,
		renderphase->colorFormat,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->colorImage,
		renderphase->colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->colorView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create color image view for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->depthImage,
		&renderphase->depthMem,
		depthFormat,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image for the main renderphase");
		return evk_Failure;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->depthImage,
		depthFormat,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->depthView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create depth image view for the main renderphase");
		return evk_Failure;
	}

	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);

	if (!renderphase->evkRenderpass.framebuffers) {
		EVK_LOG(evk_Error, "Failed to allocate memory for main renderphase framebuffers");
		return evk_Failure;
	}

	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		const VkImageView attachments[2] = { renderphase->colorView, renderphase->depthView };
		VkFramebufferCreateInfo fbci = { 0 };
		fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbci.renderPass = renderphase->evkRenderpass.renderpass;
		fbci.attachmentCount = 2U;
		fbci.pAttachments = &attachments[0];
		fbci.width = extent.width;
		fbci.height = extent.height;
		fbci.layers = 1;

		if (vkCreateFramebuffer(device, &fbci, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create default renderphase framebuffer");
			return evk_Failure;
		}
	}

	return evk_Success;
}

void evk_renderphase_picking_update(evkPickingRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback)
{
	VkClearValue clearValues[2] = { 0 };
	const uint32_t clearValuesCount = 2;
	clearValues[0].color = (VkClearColorValue){ 0.0f,  0.0f,  0.0f, 1.0f };
	clearValues[1].depthStencil = (VkClearDepthStencilValue){ 1.0f,0 };

	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer frameBuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;

	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

	VkCommandBufferBeginInfo cmdBeginInfo = { 0 };
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to beging picking renderphase command buffer");

	VkRenderPassBeginInfo renderPassBeginInfo = { 0 };
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = frameBuffer;
	renderPassBeginInfo.renderArea.offset = (VkOffset2D){ 0, 0 };
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = clearValuesCount;
	renderPassBeginInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// set frame commandbuffer viewport
	VkViewport viewport = { 0 };
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

	VkRect2D scissor = { 0 };
	scissor.offset = (VkOffset2D){ 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

	if (callback != NULL) {
		callback(evk_get_context(), timestep);
	}

	// end render pass
	vkCmdEndRenderPass(cmdBuffer);

	// end command buffer
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to finish picking renderphase command buffer");
}

evkUIRenderphase evk_renderphase_ui_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, bool finalPhase)
{
	evkUIRenderphase renderphase = { 0 };
	renderphase.evkRenderpass.name = "UI";
	renderphase.evkRenderpass.format = format;

	VkAttachmentDescription attachment = { 0 };
	attachment.format = format;
	attachment.samples = (VkSampleCountFlagBits)evk_Msaa_Off;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachment.finalLayout = finalPhase == 1 ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachment = { 0 };
	colorAttachment.attachment = 0;
	colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = { 0 };
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachment;

	VkSubpassDependency dependency = { 0 };
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo info = { 0 };
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &attachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;
	EVK_ASSERT(vkCreateRenderPass(device, &info, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create ui render phase renderpass");

	// command pool and buffers
	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
	VkCommandPoolCreateInfo cmdPoolInfo = { 0 };
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create ui render phase command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo = { 0 };
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to allocate ui render phase command buffers");

	// descriptor pool and descriptor set layout for UI image of things
	VkDescriptorSetLayoutBinding binding[1] = { 0 };
	binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding[0].descriptorCount = 1;
	binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo descInfo = { 0 };
	descInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descInfo.bindingCount = 1;
	descInfo.pBindings = binding;
	EVK_ASSERT(vkCreateDescriptorSetLayout(device, &descInfo, NULL, &renderphase.descriptorSetLayout) == VK_SUCCESS, "Failed to create the ui render phase descriptor set layout");

	VkDescriptorPoolSize poolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo poolCI = { 0 };
	poolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCI.pNext = NULL;
	poolCI.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolCI.maxSets = 1000 * 11U;
	poolCI.poolSizeCount = 11U;
	poolCI.pPoolSizes = poolSizes;
	EVK_ASSERT(vkCreateDescriptorPool(device, &poolCI, NULL, &renderphase.descriptorPool) == VK_SUCCESS, "Failed to create the ui render phase descriptor pool");

	return renderphase;
}

void evk_renderphase_ui_destroy(evkUIRenderphase* renderphase, VkDevice device)
{
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroyDescriptorSetLayout(device, renderphase->descriptorSetLayout, NULL);
	vkDestroyDescriptorPool(device, renderphase->descriptorPool, NULL);

	memset(renderphase, 0, sizeof(evkUIRenderphase));
}

evkResult evk_renderphase_ui_create_framebuffers(evkUIRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat)
{
	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);
	if (renderphase->evkRenderpass.framebuffers == NULL) {
		EVK_LOG(evk_Error, "Failed to allocate memory for the ui renderphase framebuffers");
		return evk_Failure;
	}
	
	for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		const VkImageView attachments[] = { views[i] };

		VkFramebufferCreateInfo framebufferCI = { 0 };
		framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCI.renderPass = renderphase->evkRenderpass.renderpass;
		framebufferCI.attachmentCount = 1U;
		framebufferCI.pAttachments = attachments;
		framebufferCI.width = extent.width;
		framebufferCI.height = extent.height;
		framebufferCI.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferCI, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create ui render phase framebuffer");
			return evk_Failure;
		}
	}

	return evk_Success;
}

void evk_renderphase_ui_update(evkUIRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, evkCalllback_RenderUI callback)
{
	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer frameBuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;

	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

	VkCommandBufferBeginInfo cmdBeginInfo = { 0 };
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin ui renderphase command buffer");

	VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 1.0f };

	VkRenderPassBeginInfo renderPassBeginInfo = { 0 };
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = frameBuffer;
	renderPassBeginInfo.renderArea.offset = (VkOffset2D){ 0, 0 };
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	if (callback != NULL) {
		callback(evk_get_context(), cmdBuffer);
	}

	vkCmdEndRenderPass(cmdBuffer);
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to end ui renderphase command buffer");
}

evkViewportRenderphase evk_renderphase_viewport_create(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkFormat format, evkMSAA msaa)
{
	evkViewportRenderphase renderphase = { 0 };
	renderphase.evkRenderpass.name = "Viewport";
	renderphase.evkRenderpass.format = format;
	renderphase.evkRenderpass.msaa = msaa;

	// attachments, subpass
	const uint32_t attachmentsSize = 2U;
	VkAttachmentDescription attachments[2] = { 0 };
	attachments[0].format = renderphase.evkRenderpass.format;
	attachments[0].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	attachments[1].format = evk_device_find_depth_format(physicalDevice);
	attachments[1].samples = (VkSampleCountFlagBits)renderphase.evkRenderpass.msaa;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference = { 0 };
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = { 0 };
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = { 0 };
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = NULL;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = NULL;
	subpassDescription.pResolveAttachments = NULL;

	const uint32_t dependenciesSize = 2U;
	VkSubpassDependency dependencies[2] = { 0 };
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	dependencies[0].dependencyFlags = 0;
	dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = 0;
	dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCI = { 0 };
	renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCI.attachmentCount = attachmentsSize;
	renderPassCI.pAttachments = attachments;
	renderPassCI.subpassCount = 1;
	renderPassCI.pSubpasses = &subpassDescription;
	renderPassCI.dependencyCount = dependenciesSize;
	renderPassCI.pDependencies = dependencies;
	EVK_ASSERT(vkCreateRenderPass(device, &renderPassCI, NULL, &renderphase.evkRenderpass.renderpass) == VK_SUCCESS, "Failed to create viewport render phase renderpass");
	
	// command pool and buffers
	evkQueueFamily indices = evk_device_find_queue_families(physicalDevice, surface);
	VkCommandPoolCreateInfo cmdPoolInfo = { 0 };
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = indices.graphics;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	EVK_ASSERT(vkCreateCommandPool(device, &cmdPoolInfo, NULL, &renderphase.evkRenderpass.cmdPool) == VK_SUCCESS, "Failed to create viewport renderphase command pool");

	VkCommandBufferAllocateInfo cmdBufferAllocInfo = { 0 };
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.commandPool = renderphase.evkRenderpass.cmdPool;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandBufferCount = EVK_CONCURRENTLY_RENDERED_FRAMES;
	EVK_ASSERT(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, renderphase.evkRenderpass.cmdBuffers) == VK_SUCCESS, "Failed to create viewport renderphase command pool");

	return renderphase;
}

void evk_renderphase_viewport_destroy(evkViewportRenderphase* renderphase, VkDevice device)
{
	vkDeviceWaitIdle(device);

	// renderpass
	if (renderphase->evkRenderpass.renderpass != VK_NULL_HANDLE) {
		vkDestroyRenderPass(device, renderphase->evkRenderpass.renderpass, NULL);
	}

	if (renderphase->evkRenderpass.cmdBuffers) {
		vkFreeCommandBuffers(device, renderphase->evkRenderpass.cmdPool, EVK_CONCURRENTLY_RENDERED_FRAMES, renderphase->evkRenderpass.cmdBuffers);
	}

	if (renderphase->evkRenderpass.cmdPool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(device, renderphase->evkRenderpass.cmdPool, NULL);
	}

	for (unsigned int i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
		vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
	}

	m_free(renderphase->evkRenderpass.framebuffers);
	memset(&renderphase->evkRenderpass, 0, sizeof(evkRenderpass));

	// general
	vkDestroySampler(device, renderphase->sampler, NULL);
	vkDestroyDescriptorPool(device, renderphase->descriptorPool, NULL);
	vkDestroyDescriptorSetLayout(device, renderphase->descriptorSetLayout, NULL);

	vkDestroyImageView(device, renderphase->depthView, NULL);
	vkDestroyImage(device, renderphase->depthImage, NULL);
	vkFreeMemory(device, renderphase->depthMem, NULL);

	vkDestroyImageView(device, renderphase->colorView, NULL);
	vkDestroyImage(device, renderphase->colorImage, NULL);
	vkFreeMemory(device, renderphase->colorMem, NULL);

	memset(renderphase, 0, sizeof(evkUIRenderphase));
}

evkResult evk_renderphase_viewport_create_framebuffers(evkViewportRenderphase* renderphase, VkDevice device, VkPhysicalDevice physicalDevice, VkImageView* views, uint32_t viewsCount, VkExtent2D extent, VkFormat colorFormat)
{
	if (renderphase->evkRenderpass.framebuffers != NULL) {
		for (uint32_t i = 0; i < renderphase->evkRenderpass.framebufferCount; i++) {
			vkDestroyFramebuffer(device, renderphase->evkRenderpass.framebuffers[i], NULL);
		}
		m_free(renderphase->evkRenderpass.framebuffers);
	}

	if (renderphase->descriptorPool != VK_NULL_HANDLE) {
		vkDestroyDescriptorPool(device, renderphase->descriptorPool, NULL);
	}

	if (renderphase->descriptorSet != VK_NULL_HANDLE) {
		vkDestroyDescriptorSetLayout(device, renderphase->descriptorSetLayout, NULL);
	}
	
	// descriptor pool
	VkDescriptorPoolSize poolSizes[] = { { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, EVK_CONCURRENTLY_RENDERED_FRAMES } };
	VkDescriptorPoolCreateInfo poolCI = { 0 };
	poolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCI.pNext = NULL;
	poolCI.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolCI.maxSets = (uint32_t)(2 * EVK_STATIC_ARRAY_SIZE(poolSizes));
	poolCI.poolSizeCount = (uint32_t)EVK_STATIC_ARRAY_SIZE(poolSizes);
	poolCI.pPoolSizes = poolSizes;

	if (vkCreateDescriptorPool(device, &poolCI, NULL, &renderphase->descriptorPool) != VK_SUCCESS) {
		EVK_LOG(evk_Error, "Failed to create viewport render phase descriptor pool");
		return evk_Failure;
	}

	// descriptor set layout
	VkDescriptorSetLayoutBinding binding[1] = { 0 };
	binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding[0].descriptorCount = 1;
	binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo info = { 0 };
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.bindingCount = 1;
	info.pBindings = binding;
	if (vkCreateDescriptorSetLayout(device, &info, NULL, &renderphase->descriptorSetLayout) != VK_SUCCESS) {
		EVK_LOG(evk_Error, "Failed to create viewport render phase descriptor set layout");
		return evk_Failure;
	}

	// sampler
	evkResult res = evk_device_create_image_sampler
	(
		device,
		physicalDevice,
		VK_FILTER_LINEAR,
		VK_FILTER_LINEAR,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		1.0f,
		&renderphase->sampler
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create viewport render phase sampler");
		return res;
	}

	// color image
	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->colorImage,
		&renderphase->colorMem,
		renderphase->evkRenderpass.format,
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport render phase color image");
		return res;
	}

	res = evk_device_create_image_view
	(
		device, 
		renderphase->colorImage,
		renderphase->evkRenderpass.format,
		VK_IMAGE_ASPECT_COLOR_BIT, 
		1,
		1,
		VK_IMAGE_VIEW_TYPE_2D,
		NULL,
		&renderphase->colorView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport render phase color image view");
		return res;
	}

	// depth buffer
	res = evk_device_create_image
	(
		extent,
		1,
		1,
		device,
		physicalDevice,
		&renderphase->depthImage,
		&renderphase->depthMem,
		evk_device_find_depth_format(physicalDevice),
		renderphase->evkRenderpass.msaa,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport renderphase depth image");
		return res;
	}

	res = evk_device_create_image_view
	(
		device,
		renderphase->depthImage,
		evk_device_find_depth_format(physicalDevice),
		VK_IMAGE_ASPECT_DEPTH_BIT, 
		1, 
		1, 
		VK_IMAGE_VIEW_TYPE_2D,
		NULL, 
		&renderphase->depthView
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport renderphase depth image view");
		return res;
	}

	// command buffer
	VkCommandBuffer command = evk_device_begin_commandbuffer_singletime(device, renderphase->evkRenderpass.cmdPool);

	VkImageSubresourceRange subresourceRange = { 0 };
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	evk_device_create_image_memory_barrier
	(
		command,
		renderphase->colorImage,
		VK_ACCESS_TRANSFER_READ_BIT,
		VK_ACCESS_MEMORY_READ_BIT,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		subresourceRange
	);

	res = evk_device_end_commandbuffer_singletime(device, renderphase->evkRenderpass.cmdPool, command, evk_get_graphics_queue());

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport renderphase depth image view");
		return res;
	}

	res = evk_device_create_image_descriptor_set
	(
		device,
		renderphase->descriptorPool,
		renderphase->descriptorSetLayout,
		renderphase->sampler,
		renderphase->colorView,
		&renderphase->descriptorSet
	);

	if (res != evk_Success) {
		EVK_LOG(evk_Error, "Failed to create the viewport image descriptor set");
		return res;
	}

	// framebuffer
	renderphase->evkRenderpass.framebufferCount = viewsCount;
	renderphase->evkRenderpass.framebuffers = (VkFramebuffer*)m_malloc(sizeof(VkFramebuffer) * viewsCount);
	EVK_ASSERT(renderphase->evkRenderpass.framebuffers != NULL, "Failed to allocate memory for the viewport renderphass framebuffers");

	for (size_t i = 0; i < viewsCount; i++) {
		const VkImageView attachments[2] = { renderphase->colorView, renderphase->depthView };

		VkFramebufferCreateInfo framebufferCI = { 0 };
		framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCI.renderPass = renderphase->evkRenderpass.renderpass;
		framebufferCI.attachmentCount = 2U;
		framebufferCI.pAttachments = attachments;
		framebufferCI.width = extent.width;
		framebufferCI.height = extent.height;
		framebufferCI.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferCI, NULL, &renderphase->evkRenderpass.framebuffers[i]) != VK_SUCCESS) {
			EVK_LOG(evk_Error, "Failed to create viewport renderphase framebuffer");
			return res;
		}
	}

	return evk_Success;
}

void evk_renderphase_viewport_update(evkViewportRenderphase* renderphase, VkDevice device, float timestep, uint32_t currentFrame, VkExtent2D extent, uint32_t swapchainImageIndex, bool usingViewport, evkCallback_Render callback)
{
	VkClearValue clearValues[2] = { 0 };
	clearValues[0].color = (VkClearColorValue){ 0.0f,  0.0f,  0.0f, 1.0f };
	clearValues[1].depthStencil = (VkClearDepthStencilValue){ 1.0f,  0 };

	VkCommandBuffer cmdBuffer = renderphase->evkRenderpass.cmdBuffers[currentFrame];
	VkFramebuffer framebuffer = renderphase->evkRenderpass.framebuffers[swapchainImageIndex];
	VkRenderPass renderPass = renderphase->evkRenderpass.renderpass;

	vkResetCommandBuffer(cmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

	VkCommandBufferBeginInfo cmdBeginInfo = { 0 };
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = NULL;
	cmdBeginInfo.flags = 0;
	EVK_ASSERT(vkBeginCommandBuffer(cmdBuffer, &cmdBeginInfo) == VK_SUCCESS, "Failed to begin viewport render phase command buffer");

	VkRenderPassBeginInfo renderPassBeginInfo = { 0 };
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = framebuffer;
	renderPassBeginInfo.renderArea.offset = (VkOffset2D){ 0, 0 };
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.clearValueCount = 2U;
	renderPassBeginInfo.pClearValues = clearValues;
	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport = { 0 };
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

	VkRect2D scissor = { 0 };
	scissor.offset = (VkOffset2D){ 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

	if (callback != NULL) {
		callback(evk_get_context(), timestep);
	}

	vkCmdEndRenderPass(cmdBuffer);
	EVK_ASSERT(vkEndCommandBuffer(cmdBuffer) == VK_SUCCESS, "Failed to end viewport render phase command buffer");
}