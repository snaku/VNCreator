#include "SwapChain.h"
#include "VulkanInit.h"

void Swapchain::run(VulkanInit& vulkan)
{
	createSwapChain(vulkan);
	createImageViews(vulkan);
}

VNCreatorApp::swapChainSupport Swapchain::querySwapChainSupport(VkPhysicalDevice device, VulkanInit& vulkan)
{
	VNCreatorApp::swapChainSupport support;
	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkan.getSurface(), &support.capabilities);

	uint32_t count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan.getSurface(), &count, nullptr);

	if (count != 0)
	{
		support.formats.resize(count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan.getSurface(), &count, support.formats.data());
	}

	uint32_t presModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan.getSurface(), &presModeCount, nullptr);

	if (presModeCount != 0)
	{
		support.presModes.resize(presModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan.getSurface(), &presModeCount, support.presModes.data());
	}

	return support;
}

VkSurfaceFormatKHR Swapchain::swapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availFormat)
{
	for (const auto& available : availFormat)
	{
		if (available.format == VK_FORMAT_B8G8R8A8_SRGB && available.colorSpace ==
			VK_COLORSPACE_SRGB_NONLINEAR_KHR)
		{
			return available;
		}
	}

	return availFormat[0];
}

VkPresentModeKHR Swapchain::swapPresMode()
{
	return VK_PRESENT_MODE_FIFO_KHR; // vsync ?
}

VkExtent2D Swapchain::swapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = {LARGEUR, HAUTEUR};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
										capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
										capabilities.maxImageExtent.height);
		return actualExtent;
	}
}

void Swapchain::createSwapChain(VulkanInit& vulkan)
{
	VNCreatorApp::swapChainSupport swapChainSupport = querySwapChainSupport(vulkan.getPhysicalDevice(), vulkan);

	VkSurfaceFormatKHR surfaceFormat = swapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presMode = swapPresMode();
	VkExtent2D extent = swapExtent(swapChainSupport.capabilities);

	// nbr d'images de la swapchain
	uint32_t imgCount = swapChainSupport.capabilities.minImageCount + 1;

	// config de la swapchain
	VkSwapchainCreateInfoKHR swapchainInfo{};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface = vulkan.getSurface();
	swapchainInfo.minImageCount = imgCount;
	swapchainInfo.imageFormat = surfaceFormat.format;
	swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = presMode;
	swapchainInfo.clipped = VK_TRUE; // ???
	swapchainInfo.oldSwapchain = nullptr; // TODO

	VNCreatorApp::queueFamilyIndices familyIndices = vulkan.findQueueFamilies(vulkan.getPhysicalDevice());
	uint32_t FamilyIndices[2] = {familyIndices.gfxFamily.value(), familyIndices.presFamily.value()};

	if (familyIndices.gfxFamily != familyIndices.presFamily)
	{
		// les img peuvent être utilisées par différentes queue families
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.queueFamilyIndexCount = 2;
		swapchainInfo.pQueueFamilyIndices = FamilyIndices;
	}
	else
	{
		// l'inverse de VK_SHARING_MODE_CONCURRENT ?
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	// création de la swapchain
	if (vkCreateSwapchainKHR(vulkan.getDevice(), &swapchainInfo, nullptr, &this->swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("!!!!!SwapChain: vkCreateSwapchainKHR(vulkan.getDevice(), &swapchainInfo, nullptr, &this->swapChain) != VK_SUCCESS");
	}

	vkGetSwapchainImagesKHR(vulkan.getDevice(), this->swapChain, &imgCount, nullptr);
	this->swapChainImg.resize(imgCount);
	vkGetSwapchainImagesKHR(vulkan.getDevice(), this->swapChain, &imgCount, this->swapChainImg.data());
	
	this->swapChainFormat = surfaceFormat.format;
	this->swapChainExtent = extent;
}

void Swapchain::createImageViews(VulkanInit& vulkan)
{
	this->swapChainImg.resize(this->swapChainImg.size());

	for (size_t i = 0; i < this->swapChainImg.size(); i++)
	{
		VkImageViewCreateInfo imgViewInfo{};
		imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imgViewInfo.image = this->swapChainImg[i];
		imgViewInfo.format = this->swapChainFormat;
		imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imgViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imgViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imgViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imgViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imgViewInfo.subresourceRange.baseMipLevel = 0; // ???
		imgViewInfo.subresourceRange.levelCount = 1;
		imgViewInfo.subresourceRange.baseArrayLayer = 0;
		imgViewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(vulkan.getDevice(), &imgViewInfo, nullptr, &this->swapChainImgView[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("!!!!!imageView: vkCreateImageView(vulkan.getDevice(), &imgViewInfo, nullptr, &this->swapChainImg[i]) != VK_SUCCESS");
		}
	}
}

// clean
void Swapchain::clean(VulkanInit& vulkan)
{
	for (auto imgView : this->swapChainImgView)
	{
		vkDestroyImageView(vulkan.getDevice(), imgView, nullptr);
	}
	vkDestroySwapchainKHR(vulkan.getDevice(), this->swapChain, nullptr);
}