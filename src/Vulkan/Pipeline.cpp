#include "Pipeline.h"
#include "VulkanInit.h"

Pipeline::Pipeline(VulkanInit& vulkan, Swapchain& swapchain)
{
	createGfxPipeline(vulkan, swapchain);
}

void Pipeline::createGfxPipeline(VulkanInit& vulkan, Swapchain& swapchain)
{
	// vertex et fragment shader code
	auto vertShaderCode = readShader("shader/vert.spv");
	auto fragShaderCode = readShader("shader/frag.spv");

	// vertex et fragment shader module
	auto vertShaderModule = createShaderModule(vertShaderCode, vulkan);
	auto fragShaderModule = createShaderModule(fragShaderCode, vulkan);

	// vertex shaderstageinfo
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_ALL;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	// fragment shaderstageinfo
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[2] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain.getSwapchainExtent().width);
	viewport.height = static_cast<float>(swapchain.getSwapchainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{}; 
	scissor.offset = { 0, 0 };
	scissor.extent = swapchain.getSwapchainExtent();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// TODO
	VkPipelineRasterizationStateCreateInfo rasterizer{};


	// destroy
	vkDestroyShaderModule(vulkan.getDevice(), vertShaderModule, nullptr);
	vkDestroyShaderModule(vulkan.getDevice(), fragShaderModule, nullptr);
}

VkShaderModule Pipeline::createShaderModule(const std::vector<char>& code, VulkanInit& vulkan)
{
	// module info
	VkShaderModuleCreateInfo shaderModuleInfo{};
	shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleInfo.codeSize = code.size();
	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	// shader module
	VkShaderModule shaderModule;

	// création du shader module
	if (vkCreateShaderModule(vulkan.getDevice(), &shaderModuleInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("!!!!!SHADER MODULE: vkCreateShaderModule(vulkan.getDevice(), &shaderModuleInfo, nullptr, &shaderModule) != VK_SUCCESS");
	}

	return shaderModule;
}

std::vector<char> Pipeline::readShader(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error(std::string("échec lors de l'ouverture du fichier: "+filename));
	}

	size_t size = (size_t)file.tellg();
	std::vector<char> buffer(size);

	file.seekg(0);
	file.read(buffer.data(), size);

	file.close();

	return buffer;
}