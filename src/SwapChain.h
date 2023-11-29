#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <limits>
#include <vector>

class VulkanInit;

namespace VNCreatorApp
{
	struct swapChainSupport
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presModes;
	};
}

class Swapchain 
{
public:

	void createSwapChain(VulkanInit& vulkan);
	void createImageViews(VulkanInit& vulkan);
	void run(VulkanInit& vulkan);
	void clean(VulkanInit& vulkan);

	VkSurfaceFormatKHR swapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availFormat);
	VkPresentModeKHR swapPresMode();
	VkExtent2D swapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VNCreatorApp::swapChainSupport querySwapChainSupport(VkPhysicalDevice device, VulkanInit& vulkan);
private:
	const uint32_t LARGEUR = 800;
	const uint32_t HAUTEUR = 600;

	VkSwapchainKHR swapChain;
	std::vector<VkImageView> swapChainImgView;
	std::vector<VkImage> swapChainImg;
	VkFormat swapChainFormat;
	VkExtent2D swapChainExtent;
};