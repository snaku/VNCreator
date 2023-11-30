#include "Pipeline.h"

Pipeline::Pipeline(VulkanInit& vulkan)
{
	createGfxPipeline(vulkan);
}

void Pipeline::createGfxPipeline(VulkanInit& vulkan)
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

	// fragment shaderstageinfo
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;

	VkPipelineShaderStageCreateInfo shaderStages[2] = { vertShaderStageInfo, fragShaderStageInfo };

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